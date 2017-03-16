//Boost Includes
#include <asio.hpp>

//Rapidjson Includes
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

//Standard Includes
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>


class client{
    public:
      void recieve(){
        asio::io_service io_service;
        asio::ip::tcp::endpoint endpoint(asio::ip::tcp::endpoint(asio::ip::tcp::v4(),5012));
        asio::ip::tcp::acceptor acceptor(io_service,endpoint);
        asio::ip::tcp::socket socket(io_service);
        acceptor.accept(socket);
        std::array<char,256> recv_buf;
        asio::error_code error;
        size_t len = socket.read_some(asio::buffer(recv_buf),error);
        std::string delimeter = "}";
        std::string firstString = recv_buf.data();
        finalString = firstString.substr(0,firstString.find(delimeter)+1);
        std::ofstream ofs;
        ofs.open("Users.txt",std::ofstream::out|std::ofstream::trunc);
        std::ofstream output("Users.txt",std::ofstream::out);
        ofs << finalString;
        std::cout<<finalString<<std::endl;
      }
    private:
      std::string finalString;
};
int main(){
    client room;
    room.recieve();
    return 0;
}
