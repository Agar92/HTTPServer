#include <iostream>
#include <vector>
#include <thread>
#include <memory>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

using namespace std::literals::chrono_literals;
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

//Task Execution with Asio:
//https://hub.packtpub.com/task-execution-asio/
int main(int argc, char* argv[])
{
    boost::asio::io_context io_context;
    std::unique_ptr<boost::asio::io_context::work> work(new boost::asio::io_context::work(io_context));
    std::cout<<"Before constructor:"<<std::endl;
    std::string cmd;
    /*
    while(std::cin >> cmd)
    {
        std::cout<<"cmd="<<cmd<<std::endl;
        HTTPGetRequest req(
            "127.0.0.1",
            cmd.c_str(),
            OnDataReceived,
            OnRequestCompleted);
        req.sendRequest();
        g_data.clear();
    }
    */
    std::thread th([&io_context](){
        io_context.run();
    });
    std::this_thread::sleep_for(100ms);
    while(std::cin >> cmd)
    {
        std::string copy_cmd=cmd;
        boost::to_upper(copy_cmd);
        if(copy_cmd=="EXIT" || copy_cmd=="QUIT" || copy_cmd=="X" || copy_cmd=="Q") break;
        std::cout<<"cmd="<<cmd<<std::endl;
        io_context.post([&io_context,&cmd]()
            {
                HTTPGetRequest req(
                    /*io_context,*/
                    "127.0.0.1",
                    cmd.c_str(),
                    OnDataReceived,
                    OnRequestCompleted);
                req.sendRequest();
            });
        g_data.clear();
    }
    std::cout<<"END OF WHILE LOOP"<<std::endl;
    work.reset();// destroy work object: signals end of work
    std::cout<<"AFTER work.reset();"<<std::endl;
    io_context.stop();
    th.join();
    std::cout<<"END OF PROGRAM. GOOD BYE!"<<std::endl;
    return 0;
}