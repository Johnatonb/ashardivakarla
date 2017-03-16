//Boost Includes
#include <asio.hpp>

//Rapidjson Includes
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

//Standard Includes
#include <iostream>
#include <thread>
#include <mutex>

class server{
     public:
        std::string User;
        void createAccountString(std::string Name,std::string Password){
            std::string nameInternal = Name;
            std::string passwordInternal = Password;
            rapidjson::StringBuffer account;
            rapidjson::Writer<rapidjson::StringBuffer> writer(account);
            writer.StartObject();
            writer.Key("Purpose");
            writer.String("Account");
            writer.Key("Name");
            writer.String(nameInternal.c_str());
            writer.Key("Password");
            writer.String(passwordInternal.c_str());
            writer.EndObject();
            User = account.GetString();
        }
        void initSocket(){
          asio::io_service io_service;
          asio::ip::tcp::resolver resolver(io_service);
          asio::ip::tcp::resolver::query query("127.0.0.1","5012");
          asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
          asio::ip::tcp::socket socket(io_service);
          asio::connect(socket,endpoint_iterator);
          asio::write(socket,asio::buffer(User));
        }
    private:
};

int main(){
    server room;
    room.createAccountString("John","Brubaker");
    room.initSocket();
}
