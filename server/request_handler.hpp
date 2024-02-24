/*!
  \file  request_handler.hpp
  \brief helper class for building an answer to an HTTP request from a client
*/
#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include <string>

namespace http
{
namespace server
{

struct reply;
struct request;

//! The common handler for all incoming requests.
class request_handler
{
  public:
  //! Constructor
  request_handler(const request_handler&)            = delete;
  //! Assignment operator
  request_handler& operator=(const request_handler&) = delete;

  //! Constructor with a directory containing files to be served sepcified.
  explicit request_handler(const std::string& doc_root);

  //! Handle a request and produce a reply.
  /*! 
   * The reply is being built on the fly.
   */
  void handle_request(const request& req, reply& rep);

  private:
  //! The directory containing the files to be served.
  std::string doc_root_;

  //! Perform URL-decoding on a string. Returns false if the encoding was invalid.
  /*!
   * Checks if a given string is a valid URL or not.
   * return bool value indicating whether a given string is a valid URL or not. 
   */
  static bool url_decode(const std::string& in, std::string& out);
};

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HANDLER_HPP