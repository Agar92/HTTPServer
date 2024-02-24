/*!
  \file  request.hpp
  \brief definition of the structure of an HTTP request
*/
#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "header.hpp"
#include <string>
#include <vector>

namespace http
{
namespace server
{

//! structure for a client HTTP request to a server
struct request
{
  std::string         method;/*!< HTTP method (by default, GET) */
  std::string         uri;/*!< URI of the resource requested by the client from a server */
  int                 http_version_major;/*!< major version of the HTTP protocol */
  int                 http_version_minor;/*!< minor version of the HTTP protocol */
  std::vector<header> headers;/*!< HTTP request headers */
  std::string temp;/*!< unnecessary string for storing some temporary data, used for debugging */
};

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_HPP