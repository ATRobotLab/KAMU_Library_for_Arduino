#ifndef __KAMU_H__
#define __KAMU_H__
#include <Arduino.h>
#define CONNECTED 1
#define DISCONNECTED 0
#define CONNECTION_UART 0
#define CONNECTION_BT 1
#define RUNNING 1
#define STOPPED 0
#define MOTORS_KAMU 18 // 까무의 기본 모터수는 18개 입니다.
#define TIMEOUT -1     // 블루투스 통신의 응답 대기시간입니다. 기본값은 '제한없음'입니다.
                       // 통신이 원할하지 않을 경우 플레이할 동작 시간보다 크게 설정해 대기시간을 설정 해 주세요.
typedef struct
{
    byte slot;
    String name;
    byte framelength;
} HEADER;
typedef struct
{
    byte loop;
    byte loop_start;
    byte loop_end;
    byte loop_count;
    byte detect;
} HEADER_EXTENDED;
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
    void waitUntilStopped(unsigned long timeout);
    void waitUntilReceived();
    int angleCorrection(int8_t angle);
    void saveProcess(const HEADER *header, const uint16_t *time_arr, const int8_t **angledata_arr, String header_add);

public:
    void init(unsigned long baud);
    void run();
    void setConnectionType(uint8_t connectiontype);
    void playBasicMotion(int motionnum);
    void playCustomMotion(int motionnum);
    void rotateMotor(int motornum, int angle);
    void rotateMotor(int motornum, int angle, int time);
    void playTempMotion(uint8_t framelength, const uint16_t *time_arr, const int8_t **angledata_arr);
    void saveTempMotion(const HEADER *header, const uint16_t *time_arr, const int8_t **angledata_arr);
    void saveTempMotion(const HEADER *header, const HEADER_EXTENDED *header_extended, const uint16_t *time_arr, const int8_t **angledata_arr);
};

#endif