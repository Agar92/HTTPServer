/*!
  \file  mime_types.hpp
  \brief definition of all types of files according to MIME (Multipurpose Internet Mail Extensions) protocol
*/
#ifndef HTTP_MIME_TYPES_HPP
#define HTTP_MIME_TYPES_HPP

#include <string>

namespace http
{
namespace server
{
namespace mime_types
{

//! Convert a file extension into a MIME type to be used in an HTTP request header
/*!
 * The conversion is as follows:<BR>
 * gif -> image/gif<BR>
 * htm -> text/html<BR>
 * html-> text/html<BR>
 * jpg -> image/jpeg<BR>
 * png -> image/png
 */
std::string extension_to_type(const std::string& extension);

} // namespace mime_types
} // namespace server
} // namespace http

#endif // HTTP_MIME_TYPES_HPP