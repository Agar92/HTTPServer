/*!
  \file  HTTPGetRequest.hpp
  \brief Full description of the client HTTP request
*/
#pragma once

#include <string>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

/*! \brief typedef for a pointer to a function that must be called on recieving the data from the server */
typedef void (*HTTPRequestDataReceived)(char*, size_t);
/*! \brief typedef for a pointer to a function that must be called at the completion of the asynchronous Boost Asio read operation */
typedef void (*HTTPRequestComplete)();

class HTTPGetRequest
{
  public:
  //! HTTP client request class constructor
  /*! Constructor parameters:
   *  \param[in] io_context   context necessary for Boost Asio event loop to work
   *  \param[in] host   IP address of the server
   *  \param[in] relativeURL   URI of the resource on the server requested by the client
   *  \param[in] receivedCB   pointer to a function that must be called on recieving the data from the server
   *  \param[in] completeCB   pointer to a function that must be called at the completion of the asynchronous Boost Asio read operation
   */
  HTTPGetRequest(boost::asio::io_context& io_context, std::string host,
                 std::string relativeURL, HTTPRequestDataReceived receivedCB,
                 HTTPRequestComplete completeCB);
  //! HTTP client request class destructor
  ~HTTPGetRequest();

  public:
  //! function that send HTTP request from client to server 
  void sendRequest();

  private:
  boost::asio::io_context& m_io_service;/*!< reference to boost::asio::io_context object */
  HTTPRequestDataReceived  m_receivedCB;/*!< pointer to a function that must be called on recieving the data from the server */
  HTTPRequestComplete      m_completeCB;/*!< pointer to a function that must be called at the completion of the asynchronous Boost Asio read operation */

  std::string m_host;/*!< IP address of the server */
  std::string m_relativeURL;/*!< the URI of the resource requested on the server */

  tcp::socket   m_socket;/*!< TCP socket */
  tcp::resolver m_resolver;/*< resolver that returns the endpoint that allows to connect to the server */

  boost::asio::streambuf m_request;/*!< Boost Asio streambuf for the request */
  boost::asio::streambuf m_response;/*!< Boost Asio streambuf for the response */

  std::string file_contents;/*!< here the contents of the file transferred from the server are temporarily stored */
  size_t      file_size = 0;/*!< the size of the file transferred from the server */

  //! Helper function for void sendRequest();
  /*!
    The function sendRequest() is designed to read only the header of the HTTP response from the server.
    The contents of the response (the image file or the text for a text file or an html text for an html file) should
    be read by this function, porion by portion. 
  */
  void ReadData();
  //! extension of the file requested in m_relativeURL
  std::string requested_file_extension = "";
};