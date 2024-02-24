#include "connection.hpp"
#include "connection_manager.hpp"
#include "request_handler.hpp"
#include <iostream>
#include <utility>
#include <vector>

namespace http
{
namespace server
{

connection::connection(boost::asio::ip::tcp::socket socket,
                       connection_manager& manager, request_handler& handler)
    : socket_(std::move(socket)), connection_manager_(manager),
      request_handler_(handler)
{
}

connection::~connection() {}

void connection::start()
{
  do_read();
}

void connection::stop()
{
  socket_.close();
}

void connection::do_read()
{
  auto self(shared_from_this());
  socket_.async_read_some(
      boost::asio::buffer(buffer_),
      [this, self](boost::system::error_code ec, std::size_t bytes_transferred)
      {
        if (!ec)
        {
          std::cout << "Read the request received:" << std::endl;
          for (int i = 0; i < bytes_transferred; ++i) std::cout << buffer_[i];
          std::cout << std::endl;
          request_parser::result_type result;
          std::tie(result, std::ignore) = request_parser_.parse(
              request_, buffer_.data(), buffer_.data() + bytes_transferred);

          if (result == request_parser::good)
          {
            if (reply_.content.length())
              std::cout << "reply_.content=" << reply_.content << std::endl;
            request_handler_.handle_request(request_, reply_);
            do_write();
          }
          else if (result == request_parser::bad)
          {
            reply_ = reply::stock_reply(reply::bad_request);
            do_write();
          }
          else if (result == request_parser::shutdown)
          {
            // Initiate graceful connection closure.
            // server::do_await_stop() is waiting for it:
            std::raise(SIGINT); // or std::raise(SIGINT);
          }
          else { do_read(); }
        }
        else if (ec != boost::asio::error::operation_aborted)
        {
          connection_manager_.stop(shared_from_this());
        }
      });
}

void connection::do_write()
{
  auto self(shared_from_this());
  /*
  std::cout << "connection::do_write(): reply_.to_buffers().size()="
            << reply_.to_buffers().size() << std::endl;
  std::cout << " reply_.content=" << reply_.content
            << " reply_.headers[0].name=" << reply_.headers[0].name
            << " reply_.headers[0].value=" << reply_.headers[0].value
            << " reply_.headers[1].name=" << reply_.headers[1].name
            << " reply_.headers[1].value=" << reply_.headers[1].value
            << std::endl;
  std::cout << "boost::asio::async_write(socket_, reply_.to_buffers()"
            << std::endl;
  for (int i = 0; i < reply_.to_buffers().size(); ++i)
  {
    std::cout << boost::asio::buffer_cast<const char*>(
                     reply_.to_buffers().at(i))
              << std::endl;
  }
  */
  boost::asio::async_write(
      socket_, reply_.to_buffers(),
      [this, self](boost::system::error_code ec, std::size_t)
      {
        if (!ec)
        {
          do_read();
          /*
          // Initiate graceful connection closure.
          boost::system::error_code ignored_ec;
          socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,
            ignored_ec);
          */
        }

        if (ec != boost::asio::error::operation_aborted)
        {
          connection_manager_.stop(shared_from_this());
        }
      });
}

} // namespace server
} // namespace http