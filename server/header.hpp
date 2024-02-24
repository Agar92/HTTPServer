/*!
  \file  header.hpp
  \brief definition of the structure of an HTTP request header
*/
#ifndef HTTP_HEADER_HPP
#define HTTP_HEADER_HPP

#include <string>

namespace http
{
namespace server
{

//! structure for a client HTTP request to a server header
struct header
{
  std::string name;/*!< name of the field */
  std::string value;/*!< value of the field */
};

} // namespace server
} // namespace http

#endif // HTTP_HEADER_HPP