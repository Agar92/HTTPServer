/*!
  \file  main.cpp
  \brief main program file for the server
*/
#include "server.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <string>

/*!
 * @brief server program entry point
 * @details Execution of the program starts here
 * @param[in] argc Number of arguments
 * @param[in] argv List of arguments
 * @return Program exit status
 */
int main(int argc, char* argv[])
{
  try
  {
    //Initialize the server.
    http::server::server s("127.0.0.1", "10000",
                           "."); /// argv[1], argv[2], argv[3]);
    //Run the server until stopped.
    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }
  return 0;
}