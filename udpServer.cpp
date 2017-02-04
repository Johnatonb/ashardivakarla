#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <thread>
#include <chrono>
#include <iostream>

using boost::asio::ip::udp;
using namespace std;
using namespace rapidjson;

class udp_server{
    public:
        udp_server():
            socket_(io_service,udp::endpoint(udp::v4(),25566)){
                start_recieve();
        }
        int setPressure(double paramPressure){pressure = paramPressure;}
        int setHighGear(bool paramHighGear){highGear = paramHighGear;}
        int setBottomIntake(bool paramBottomIntake){bottomIntake = paramBottomIntake;}
        int setStream(bool paramStream){stream = paramStream;}
        int setCrosshairOffset(double paramCrosshairOffset){crosshairOffset = paramCrosshairOffset;}
        int setTurretAngle(double paramTurretAngle){turretAngle = paramTurretAngle;}
        int setRPM(int paramRPM){RPM = paramRPM;}
        int setTopIntake(bool paramTopIntake){topIntake = paramTopIntake;}
        int setLeftRPM(int paramLeftRPM){leftRPM = paramLeftRPM;}
        int setRightRPM(int paramRightRPM){rightRPM = paramRightRPM;}
        int setHoldsGear(int paramHoldsGear){holdsGear = paramHoldsGear;}
        int setMode(int paramMode){mode = paramMode;}
        int setPowered(int paramPowered){powered = paramPowered;}
        void createJson(){
           StringBuffer message;
           Writer<StringBuffer> writer(message);
           writer.StartObject();
           writer.Key("pressure");
           writer.Double(pressure);
           writer.Key("highGear");
           writer.Bool(highGear);
           writer.Key("bottomIntake");
           writer.Bool(bottomIntake);
           writer.Key("stream");
           writer.Bool(stream);
           writer.Key("crosshairOffset");
           writer.Double(crosshairOffset);
           writer.Key("turretAngle");
           writer.Double(turretAngle);
           writer.Key("RPM");
           writer.Int(RPM);
           writer.Key("topIntake");
           writer.Bool(topIntake);
           writer.Key("leftRPM");
           writer.Int(leftRPM);
           writer.Key("rightRPM");
           writer.Int(rightRPM);
           writer.Key("holdsGear");
           writer.Bool(holdsGear);
           writer.Key("mode");
           writer.Int(mode);
           writer.Key("powered");
           writer.Bool(powered);
           writer.EndObject();
           //cout<<message.GetString()<<endl;
           finalString = message.GetString();
           cout<<finalString<<endl;
           }
        void serverInit(){
            thread serverThread(boost::bind(&boost::asio::io_service::run,&io_service));
            this_thread::sleep_for(chrono::seconds(1000));
        }
        void serverEnd(){
            serverThread.join();
        }
    private:
        boost::asio::io_service io_service;
        thread serverThread;
        double pressure;
        bool highGear;
        bool bottomIntake;
        bool stream;
        double crosshairOffset;
        double turretAngle;
        int RPM;
        bool topIntake;
        int leftRPM;
        int rightRPM;
        bool holdsGear;
        int mode;
        bool powered;
        string finalString;
        rapidjson::Document document;
        void start_recieve(){
            socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_),remote_endpoint,
            boost::bind(&udp_server::handle_recieve,this,
            boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
        }
        void handle_recieve(const boost::system::error_code& error,size_t){
            if(!error || error == boost::asio::error::message_size){
                boost::shared_ptr<string> message(new string (finalString));
                socket_.async_send_to(
                boost::asio::buffer(*message),remote_endpoint,
                boost::bind(&udp_server::handle_send,this,message,
                boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
                start_recieve();
            }
        }
        void handle_send(boost::shared_ptr<string>,
            const boost::system::error_code&, size_t){
        }

        udp::socket socket_;
        udp::endpoint remote_endpoint;
        array<char,1>recv_buffer_;
};
int main(){
    udp_server server;
    server.setPressure(1.2);
    server.setHighGear(false);
    server.setBottomIntake(true);
    server.setStream(false);
    server.setCrosshairOffset(3.4);
    server.setTurretAngle(5.6);
    server.setRPM(78);
    server.setTopIntake(true);
    server.setLeftRPM(90);
    server.setRightRPM(12);
    server.setHoldsGear(34);
    server.setMode(1);
    server.setPowered(1);
    server.createJson();
    server.serverInit();
    server.serverEnd();
}