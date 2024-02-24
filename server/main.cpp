#include "server.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
  try
  {
    // Initialise the server.
    http::server::server s("127.0.0.1", "10000",
                           "."); /// argv[1], argv[2], argv[3]);
    // Run the server until stopped.
    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }
  return 0;
}