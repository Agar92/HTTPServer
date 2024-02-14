#include "HTTPGetRequest.hpp"

#include <iostream>
#include <fstream>
#include <string>

HTTPGetRequest::HTTPGetRequest(boost::asio::io_context& io_context, std::string host, std::string clipURL, HTTPRequestDataReceived receivedCB, HTTPRequestComplete completeCB) :
 m_io_service(io_context),
 m_host(host),
 m_relativeURL(clipURL),
 m_socket(m_io_service),
 m_resolver(m_io_service),
 m_receivedCB(receivedCB),
 m_completeCB(completeCB)
{
    std::cout<<"HTTPGetRequest::HTTPGetRequest(...)"<<std::endl;
    std::cout<<"m_relativeURL="<<m_relativeURL<<std::endl;
    //
    // Determine the file extension.
    std::size_t last_slash_pos = m_relativeURL.find_last_of("/");
    std::size_t last_dot_pos = m_relativeURL.find_last_of(".");
    if (last_slash_pos != std::string::npos &&
        last_dot_pos != std::string::npos   &&
        last_dot_pos > last_slash_pos)
        requested_file_extension = m_relativeURL.substr(last_dot_pos + 1);
    else requested_file_extension=".dat";
    std::cout<<"requested_file_extension="<<requested_file_extension<<std::endl;
}

HTTPGetRequest::~HTTPGetRequest(){}

// host should be in format such as "www.google.co.nz"
// url should be in format such as "/index.html"
void HTTPGetRequest::sendRequest()
{
    std::cout<<"HTTPGetRequest::sendRequest(...)"<<std::endl;
    tcp::resolver::query query(m_host, "10000");//"http");

    m_resolver.async_resolve(query, 
        [this](const boost::system::error_code& ec, tcp::resolver::iterator endpoint_iterator)
    {
        boost::asio::async_connect(m_socket, endpoint_iterator,
            [this](boost::system::error_code ec, tcp::resolver::iterator)
        {
            if (!ec)
            {
             std::ostream request_stream(&m_request);
             request_stream << "GET " << m_relativeURL << " HTTP/1.1\r\n";
             request_stream << "Host: " << m_host << "\r\n";
             request_stream << "Accept: */*\r\n";
             request_stream << "Connection: close\r\n\r\n";

             boost::asio::async_write(m_socket, m_request,
              [this](boost::system::error_code ec, std::size_t /*length*/)
             {
              boost::asio::async_read_until(m_socket, m_response, "\r\n\r\n",
               [this](boost::system::error_code ec, std::size_t length)
               {
                ReadData();
               });
             });
            }
        });
    });
    m_io_service.run();
}

void HTTPGetRequest::ReadData()
{
    std::cout<<"HTTPGetRequest::ReadData(...)"<<std::endl;
    boost::asio::async_read(m_socket, m_response, boost::asio::transfer_at_least(1),
        [this](boost::system::error_code ec, std::size_t)
    {
        static int THIS_FUNCTION_CALLS_COUNTER=0;
        THIS_FUNCTION_CALLS_COUNTER++;
        /*
        std::string s( (std::istreambuf_iterator<char>(&m_response)), std::istreambuf_iterator<char>() );
        size_t index=s.find("\r\n\r\n");
        std::cout<<"index="<<index<<std::endl;
        for(int i=index+4; i<s.length(); ++i) std::cout<<s[i];
        std::cout<<std::endl;
        const size_t shift=index+4;
        */
        size_t size = m_response.size();
        std::cout<<"APPROACHING WRITE received.dat size="<<size
                 <<" THIS_FUNCTION_CALLS_COUNTER="<<THIS_FUNCTION_CALLS_COUNTER
                 <<std::endl;
        if (size > 0)
        {
            std::cout<<"BEGIN WRITE received."<<requested_file_extension<<std::endl;
            std::unique_ptr<char> buf(new char[size]);
            m_response.sgetn(buf.get(), size);
            size_t shift=0;
            if(THIS_FUNCTION_CALLS_COUNTER == 1)
            {
                std::string s( buf.get(), size );
                size_t index=s.find("\r\n\r\n");
                std::cout<<"START CHECK THE REPLY"<<std::endl;
                ///std::cout<<"index="<<index<<std::endl;
                for(int i=0; i<s.length(); ++i) std::cout<<s[i];
                std::cout<<std::endl;
                std::cout<<"FINISH CHECK THE REPLY"<<std::endl;
                shift=index+4;
            }
            file_contents.append( buf.get() + shift, size - shift);
            m_response.consume(size);
            file_size += size - shift;
            m_receivedCB(buf.get(), size);
        }
        std::ofstream out("received." + requested_file_extension, std::ios::out | std::ios::binary);
        out.write( file_contents.data(), file_size);
        out.close();
        std::cout<<"END WRITE received."<<requested_file_extension<<std::endl;
        if (ec != boost::asio::error::eof)
        {
            ReadData();
            return;
        }
        file_contents.clear();
        file_size=0;
        m_socket.close();
        m_completeCB();
    });
}