#pragma once

#include <string>

#include <boost/asio.hpp>
using boost::asio::ip::tcp;

typedef void(*HTTPRequestDataReceived)(char*, size_t);
typedef void(*HTTPRequestComplete)();

class HTTPGetRequest
{
public:
 HTTPGetRequest(
  boost::asio::io_context& io_context,
  std::string host, 
  std::string relativeURL, 
  HTTPRequestDataReceived receivedCB,
  HTTPRequestComplete completeCB);
 
 ~HTTPGetRequest();

public:
 void sendRequest();

private:
 boost::asio::io_context & m_io_service;
 HTTPRequestDataReceived m_receivedCB;
 HTTPRequestComplete m_completeCB;

 std::string m_host;
 std::string m_relativeURL;

 tcp::socket m_socket;
 tcp::resolver m_resolver;

 boost::asio::streambuf m_request;
 boost::asio::streambuf m_response;


 std::string file_contents;
 size_t file_size=0;

 void ReadData();
 //extension of the file requested in m_relativeURL:
 std::string requested_file_extension="";
};