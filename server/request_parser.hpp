/*!
  \file  request_parser.hpp
  \brief helper class for parsing HTTP requests from a client
*/
#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <tuple>

#include "request.hpp"

namespace http
{
namespace server
{

struct request;

//! Class for a parser for incoming requests.
class request_parser
{
  public:
  //! Constructor
  request_parser();

  //! Reset to initial parser state.
  void reset();

  //! Result of parse.
  enum result_type
  {
    good,         /*!< good (proper) parsing operation */
    bad,          /*!< can not parse */
    indeterminate,/*!< parsing operation in process (has not finished yet) */
    shutdown      /*!< shutdown the server */
  };

  //! function to parse the incoming HTTP request from a client
  /*! 
   * Parse some data. The enum return value is good when a complete request has
   * been parsed, bad if the data is invalid, indeterminate when more data is
   * required or shutdown. The InputIterator return value indicates how much of the input
   * has been consumed.<BR>
   * Parsing parameters:
   * \param[in] req   request being parsed
   * \param[in] begin pointer to the 1st byte of the string being parsed
   * \param[in] end   pointer to the byte after the last byte of the string being parsed
   * \return pair<parsing result type,iterator to the end of parsing>
   */
  template <typename InputIterator>
  std::tuple<result_type, InputIterator>
  parse(request& req, InputIterator begin, InputIterator end)
  {
    while (begin != end)
    {
      result_type result = consume(req, *begin++);
      std::string URI    = req.uri;
      boost::to_upper(URI);
      if (URI == "SERVER SHUTDOWN" || URI == "SERVER EXIT" ||
          URI == "SERVER STOP" || URI == "SERVER FINISH")
        result = shutdown;
      if (result == good || result == bad || result == shutdown)
        return std::make_tuple(result, begin);
    }
    return std::make_tuple(indeterminate, begin);
  }

  private:
  //! Handle the next character of input.
  result_type consume(request& req, char input);

  //! Check if a byte is an HTTP character.
  static bool is_char(int c);

  //! Check if a byte is an HTTP control character.
  static bool is_ctl(int c);

  //! Check if a byte is defined as an HTTP tspecial character.
  static bool is_tspecial(int c);

  //! Check if a byte is a digit.
  static bool is_digit(int c);

  //! The current state of the parser (what part of an HTTP request it is parsing at the moment)
  enum state
  {
    method_start,
    method,
    uri,
    http_version_h,
    http_version_t_1,
    http_version_t_2,
    http_version_p,
    http_version_slash,
    http_version_major_start,
    http_version_major,
    http_version_minor_start,
    http_version_minor,
    expecting_newline_1,
    header_line_start,
    header_lws,
    header_name,
    space_before_header_value,
    header_value,
    expecting_newline_2,
    expecting_newline_3
  } state_;
};

} // namespace server
} // namespace http

#endif // HTTP_REQUEST_PARSER_HPP