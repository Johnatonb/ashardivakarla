#pragma once
#ifndef UDP_SERVER_H
#define UDP_SERVER_H

class udp_server{
    public:
        udp_server();
        int setPressure();
        int setHighGear();
        int setBottomIntake();
        int setStream();
        int setCrosshairOffset();
        int setTurretAngle();
        int setRPM();
        int setTopIntake();
        int setLeftRPM();
        int setRightRPM();
        int setHoldsGear();
        int setMode();
        int setPowered();
        void createJson();
        void serverInit();
        void serverEnd();
        void start_recieve();
        void handle_recieve();
        void handle_send();
}

#endif


