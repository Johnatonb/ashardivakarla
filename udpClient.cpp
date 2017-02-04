#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <rapidjson/document.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;
using namespace std;

class udp_client{
    public:
    /*udp_client(){
        SetSettings();
    }*/
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
    int Recieve(string ip,int waitTime,int waitTime2){
        thread requesting(&udp_client::sendRequest,this,ip,waitTime2);
        cout<<"thread"<<endl;
        thread recieving(&udp_client::RecieveThread,this,ip);
        cout<<"created"<<endl;
        this_thread::sleep_for(chrono::milliseconds(waitTime));
        recieving.join();
        cout<<"join 1"<<endl;
        requesting.join();
        cout<<"join 2"<<endl;
    }
    int sendRequest(string ip,int waitTime2){
        for(int wait=0;wait<waitTime2;wait++){
            boost::asio::io_service io_service;
            udp::resolver resolver(io_service);
            udp::resolver::query query(ip,"25565");
            udp::endpoint reciever_endpoint = *resolver.resolve(query);
            udp::socket socket(io_service);
            socket.open(udp::v4());
            array<char,1> send_buf = {0};
            socket.send_to(boost::asio::buffer(send_buf),reciever_endpoint);
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }
    void RecieveThread(string ip){
        boost::asio::io_service io_service;
        udp::resolver resolver(io_service);
        udp::resolver::query query(ip,"25565");
        udp::endpoint reciever_endpoint = *resolver.resolve(query);
        udp::socket socket(io_service);
        socket.open(udp::v4());
        array<char,1> send_buf = {0};
        socket.send_to(boost::asio::buffer(send_buf),reciever_endpoint);
        if(socket.is_open()==false){
            RecieveThread("127.0.0.1");
            cout<<"Hey your socket isn't open!"<<endl;
        }else{
        cout<<"Hey your socket is open!"<<endl;
        array<char,256> recv_buf;
        udp::endpoint sender_endpoint;
        size_t len = socket.async_receive_from(boost::asio::buffer(recv_buf),sender_endpoint,boost::bind(&udp_client::handler,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
        cout<<"Recieves from server"<<endl;
        string delimeter = "}";
        string firstString = recv_buf.data();
        finalString = firstString.substr(0,firstString.find(delimeter)+1);
        document.Parse(finalString.c_str());
        if(document.IsObject()){
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
               /*double pressure, bool highGear, bool bottomIntake,
                bool stream, double crosshairOffset, double turretAngle,
                int RPM, bool topIntake, int leftRPM, int rightRPM, bool holdsGear, int mode, bool powered*/
                }
            }
        }
    }}
    void handler(const boost::system::error_code& error,size_t bytes_transferred);
    int getPressure(){return pressure;}
    int getHighGear(){return highGear;}
    int getBottomIntake(){return bottomIntake;}
    int getStream(){return stream;}
    int getCrosshairOffset(){return crosshairOffset;}
    int getTurretAngle(){return turretAngle;}
    int getRPM(){return RPM;}
    int getTopIntake(){return topIntake;}
    int getLeftRPM(){return leftRPM;}
    int getRightRPM(){return rightRPM;}
    int getHoldsGear(){return holdsGear;}
    int getMode(){return mode;}
    int getPowered(){return powered;}
    ~udp_client(){
        cout<<"it runs"<<endl;
        ofs.open("Settings.txt",ofstream::out|ofstream::trunc);
        ofstream output("Settings.txt",ofstream::out);
        output<<finalString;
    }
};
int main(){
    udp_client client;
    while(true){client.RecieveThread("127.0.0.1");}
    cout<<"Hi"<<endl;
}


