#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <iostream>
#include <array>
#include <stdio.h>
#include <ctype.h>

using boost::asio::ip::tcp;
using namespace std;

class networking{
    public:
    int sendData(int x,int distance){
        try{
            boost::asio::io_service io_service;
            tcp::endpoint endpoint(tcp::endpoint(tcp::v4(),5012));
            tcp::acceptor acceptor(io_service,endpoint);
            std::string messageX = to_string(x);
            std::string messageDistance = to_string(distance);
            tcp::socket socket(io_service);
            acceptor.accept(socket);
            boost::asio::write(socket,boost::asio::buffer(messageX));
            boost::asio::write(socket,boost::asio::buffer(messageDistance));
            }
        catch(exception& e){
            cerr<<e.what()<<endl;
        }
        return 0;
    }
    int Client(){
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query("127.0.0.1","5012");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::socket socket(io_service);
        boost::asio::connect(socket,endpoint_iterator);
        for(;;){
            std::array<char,256> bufX;
            boost::system::error_code error;
            size_t len = socket.read_some(boost::asio::buffer(bufX),error);
            std::string stringX = (begin(bufX),end(bufX));
            int intX = stoi(stringX);
            cout<<"This is int X "<<intX<<endl;
            if(error == boost::asio::error::eof){
                break;
            }
            cout.write(bufX.data(),len)<<endl;
        }for(;;){
            std::array<char,256> bufDistance;
            boost::system::error_code error;
            size_t len = socket.read_some(boost::asio::buffer(bufDistance),error);
            if(error == boost::asio::error::eof){
                break;
            }
            cout.write(bufDistance.data(),len)<<endl;
        }

        return 0;
        }
};
int main(){
    networking network;
    network.Client();
}
