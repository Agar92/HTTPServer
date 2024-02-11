#include <iostream>
#include <vector>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

using boost::asio::ip::tcp;

#include "HTTPGetRequest.hpp"

std::vector<char> g_data;

void OnDataReceived(char* data, size_t dataLen)
{
 // store data in vector for sake of demo...

 unsigned int oldSize = g_data.size();
 g_data.resize(oldSize + dataLen);
 memcpy(&g_data[oldSize], data, dataLen);
}

void OnRequestCompleted()
{
 // print contents of data we received back...
 g_data.push_back('\0');
 printf(&g_data[0]);
 
}

int main(int argc, char* argv[])
{
    //boost::asio::io_service io_service;

    std::cout<<"Before constructor:"<<std::endl;
    std::string cmd;
    while(std::cin >> cmd)
    {
        std::cout<<"cmd="<<cmd<<std::endl;
        HTTPGetRequest req(
            /*io_service,*/
            "127.0.0.1",
            cmd.c_str(),
            OnDataReceived,
            OnRequestCompleted);
        req.sendRequest();
        g_data.clear();
    }
    //io_service.run();
    return 0;
}