#ifndef __KAMU_H__
#define __KAMU_H__
#include <Arduino.h>
#define CONNECTED 1
#define DISCONNECTED 0
#define CONNECTION_UART 0
#define CONNECTION_BT 1
#define RUNNING 1
#define STOPPED 0

class TempMotion
{
private:
    struct motionBuffer
    {
        int8_t angle[18];
        uint16_t time;
    };
    motionBuffer motionbuffer[20];

public:
    int8_t getAngle(uint8_t frame, uint8_t motornum);
    uint16_t getTime(uint8_t frame);
    void setMotorDegree(uint8_t framenum, uint8_t motornum, int8_t angle);
    void setFrameTime(uint8_t framenum, uint16_t time);
};

class KAMU
{
private:
    uint8_t ConnectionType = 0;
    bool ConnectionStatus = false;
    void usbConfig();
    void flushBuffer();
    void startEntryMod();
    void writeMsg(String msg);
    String readMsg();
    bool checkStatus();
    bool status = STOPPED;
    void waitUntilStopped();

public:
    void init(unsigned long baud);
    void run();
    void setConnectionType(uint8_t connectiontype);
    void playBasicMotion(int motionnum);
    void playCustomMotion(int motionnum);
    void rotateMotor(int motornum, int angle);
    void rotateMotor(int motornum, int angle, int time);
    // void setTempMotion_MotorDegree(int framenum, int motornum, int angle);
    // void setTempMotion_FrameTime(int framenum, int time);
    void playTempMotion(TempMotion motion, int startFrame, int endFrame);
};

#endif