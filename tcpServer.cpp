#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <fstream>
#include <iostream>
using boost::asio::ip::tcp;
using namespace std;

class networking{
    public:
    int sendData(float x,float distance){
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
    int Client(int argc){
        if(argc!=2){
            cerr<<"Usage: client <host>"<<endl;
        }
        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query("127.0.0.1","5012");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::socket socket(io_service);
        boost::asio::connect(socket,endpoint_iterator);
        float floatX;
        for(;;){
            std::array<char,256> bufX;
            boost::system::error_code error;
            size_t len = socket.read_some(boost::asio::buffer(bufX),error);
            string recievedX(begin(bufX),end(bufX));
            float floatX = ::atof(recievedX.c_str());
            //cout<<floatX<<endl;
            if(error == boost::asio::error::eof){
                break;
            }else if(error){
                throw boost::system::system_error(error);
            }
        }
        float floatDistance;
        for(;;){
            std::array<char,256> bufDistance;
            boost::system::error_code error;
            size_t len = socket.read_some(boost::asio::buffer(bufDistance),error);
            string recievedDistance(begin(bufDistance),end(bufDistance));
            cout<<floatDistance<<endl;
            float floatDistance = ::atof(recievedDistance.c_str());
            if(error == boost::asio::error::eof){
                break;
            }else if(error){
                throw boost::system::system_error(error);
            }
        }
        return 0;
        }

};
int main(){
    networking network;
    network.sendData(3.3,3.335);
    return 0;
}
