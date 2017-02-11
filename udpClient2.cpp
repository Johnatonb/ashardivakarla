#include <asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

using asio::ip::udp;
using namespace std;

class udp_server{
    public:
        udp_server(asio::io_service& io_service):
            socket_(io_service,udp::endpoint(udp::v4(),25565)){
                start_send();
                start_recieve();
        }
        void clientInit(){
            thread clientThread(boost::bind(&asio::io_service::run,&io_service));
        }
        void clientEnd(){
            clientThread.join();
        }
        std::ofstream ofs;
        std::ifstream ifs;
        rapidjson::Document document;
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
        thread recieving;
        void SetSettings(){
            ifs.open("Settings.txt");
            string settingsJson;
            if(ifs.is_open()){
                while(!ifs.eof()){
                    ifs>>settingsJson;
                    cout<<settingsJson.c_str()<<endl;
                    document.Parse(settingsJson.c_str());
                        if(document.IsObject()){
                            cout<<"isobject"<<endl;
                            if(document.HasMember("pressure")&&document.HasMember("highGear")&&document.HasMember("bottomIntake")&&
                                document.HasMember("stream")&&document.HasMember("crosshairOffset")&&document.HasMember("turretAngle")&&
                                document.HasMember("RPM")&&document.HasMember("topIntake")&&document.HasMember("leftRPM")&&document.HasMember("rightRPM")&&
                                document.HasMember("holdsGear")&&document.HasMember("mode")&&document.HasMember("powered")){
                                if(document["pressure"].IsDouble()&&document["highGear"].IsBool()&&document["bottomIntake"].IsBool()&&
                                    document["stream"].IsBool()&&document["crosshairOffset"].IsDouble()&&document["turretAngle"].IsDouble()&&
                                    document["RPM"].IsInt()&&document["topIntake"].IsBool()&&document["leftRPM"].IsInt()&&document["rightRPM"].IsInt()&&
                                    document["holdsGear"].IsBool()&&document["mode"].IsInt()&&document["powered"].IsBool()){
                                        pressure=document["pressure"].GetDouble();
                                        highGear=document["highGear"].GetBool();
                                        bottomIntake=document["bottomIntake"].GetBool();
                                        stream=document["stream"].GetBool();
                                        crosshairOffset=document["crosshairOffset"].GetDouble();
                                        turretAngle=document["turretAngle"].GetDouble();
                                        RPM=document["RPM"].GetInt();
                                        topIntake=document["topIntake"].GetBool();
                                        leftRPM=document["leftRPM"].GetInt();
                                        rightRPM=document["rightRPM"].GetInt();
                                        holdsGear=document["holdsGear"].GetBool();
                                        mode=document["mode"].GetInt();
                                        powered=document["powered"].GetBool();
                                        cout<<pressure<<" "<<highGear<<" "<<bottomIntake<<" "<<stream<<" "<<crosshairOffset<<" "<<turretAngle<<" "
                                        <<RPM<<" "<<topIntake<<" "<<leftRPM<<" "<<rightRPM<<" "<<holdsGear<<" "<<mode<<" "<<powered<<endl;
                            }
                        }
                    }
                }
            }
        }
        asio::io_service io_service;
    private:
        void start_recieve(){
            socket_.async_receive_from(
            asio::buffer(recv_buf),remote_endpoint,
            boost::bind(&udp_server::handle_recieve,this,
            asio::placeholders::error,asio::placeholders::bytes_transferred));
            string delimeter = "}";
            string firstString = recv_buf.data();
            finalString = firstString.substr(0,firstString.find(delimeter)+1);
            document.Parse(finalString.c_str());
            if(document.IsObject()){
                cout<<"Its a json"<<endl;
                if(document.HasMember("pressure")&&document.HasMember("highGear")&&document.HasMember("bottomIntake")&&
                document.HasMember("stream")&&document.HasMember("crosshairOffset")&&document.HasMember("turretAngle")&&
                document.HasMember("RPM")&&document.HasMember("topIntake")&&document.HasMember("leftRPM")&&document.HasMember("rightRPM")&&
                document.HasMember("holdsGear")&&document.HasMember("mode")&&document.HasMember("powered")){
                    if(document["pressure"].IsDouble()&&document["highGear"].IsBool()&&document["bottomIntake"].IsBool()&&
                        document["stream"].IsBool()&&document["crosshairOffset"].IsDouble()&&document["turretAngle"].IsDouble()&&
                        document["RPM"].IsInt()&&document["topIntake"].IsBool()&&document["leftRPM"].IsInt()&&document["rightRPM"].IsInt()&&
                        document["holdsGear"].IsBool()&&document["mode"].IsInt()&&document["powered"].IsBool()){
                            cout<<"I can read it"<<endl;
                            pressure=document["pressure"].GetDouble();
                            highGear=document["highGear"].GetBool();
                            bottomIntake=document["bottomIntake"].GetBool();
                            stream=document["stream"].GetBool();
                            crosshairOffset=document["crosshairOffset"].GetDouble();
                            turretAngle=document["turretAngle"].GetDouble();
                            RPM=document["RPM"].GetInt();
                            topIntake=document["topIntake"].GetBool();
                            leftRPM=document["leftRPM"].GetInt();
                            rightRPM=document["rightRPM"].GetInt();
                            holdsGear=document["holdsGear"].GetBool();
                            mode=document["mode"].GetInt();
                            powered=document["powered"].GetBool();
                            cout<<pressure<<" "<<highGear<<" "<<bottomIntake<<" "<<stream<<" "<<crosshairOffset<<" "<<turretAngle<<" "
                            <<RPM<<" "<<topIntake<<" "<<leftRPM<<" "<<rightRPM<<" "<<holdsGear<<" "<<mode<<" "<<powered<<endl;
                    /*double pressure, bool highGear, bool bottomIntake,
                    bool stream, double crosshairOffset, double turretAngle,
                    int RPM, bool topIntake, int leftRPM, int rightRPM, bool holdsGear, int mode, bool powered*/
                    }
                }
            }
        }
        void handle_recieve(const asio::error_code& error,size_t){
            start_recieve();
        }
        void start_send(){
                udp::resolver resolver(io_service);
                udp::resolver::query query("127.0.0.1","25566");
                udp::endpoint reciever_endpoint = *resolver.resolve(query);
                udp::socket socket(io_service);
                socket.open(udp::v4());
                array<char,1> send_buf = {0};
                udp::endpoint sender_endpoint;
                boost::shared_ptr<string> message(new string ("0"));
                socket_.async_send_to(
                asio::buffer("0"),reciever_endpoint,
                boost::bind(&udp_server::handle_send,this,
                asio::placeholders::error,asio::placeholders::bytes_transferred));
        }
        void handle_send(const asio::error_code& error,size_t){
            start_send();
        }
        udp::socket socket_;
        udp::endpoint remote_endpoint;
        array<char,256>recv_buf;
        thread clientThread;
};
int main(){
    std::thread async = std::thread([](){
        asio::io_service io_service;
        udp_server udp_Server(io_service);
        io_service.run();
    });
    while(true){}
    return 0;
}
