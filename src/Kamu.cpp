#include "Kamu.h"
#include "CP210x.h"
class CP210xAsync : public CP210xAsyncOper
{
public:
    uint8_t OnInit(CP210x *pftdi);
};

uint8_t CP210xAsync::OnInit(CP210x *cp210x)
{
    uint8_t rcode = 0;
    rcode = cp210x->IFCEnable();

    if (rcode)
    {
        return rcode;
    }

    rcode = cp210x->SetBaudRate(115200);

    if (rcode)
    {
        return rcode;
    }

    rcode = cp210x->SetDataBits(8);
    if (rcode)
    {
        return rcode;
    }

    rcode = cp210x->SetStopBits(CP210X_STOP_BITS_1);
    if (rcode)
    {
        return rcode;
    }

    rcode = cp210x->SetParity(CP210X_PARITY_NONE);
    if (rcode)
    {
        return rcode;
    }

    rcode = cp210x->SetFlowControl(CP210X_FLOW_CONTROL_OFF);
    if (rcode)
    {
        return rcode;
    }

    return rcode;
}

USB Usb;
// USBHub Hub(&Usb);
CP210xAsync cp210xAsync;
CP210x cp210x(&Usb, &cp210xAsync);
int status_old = USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE;
/*
        연결 방식을 선택합니다.
        CONNECTION_BT => 블루투스로 연결
        CONNECTION_UART => USB 케이블 직접 연결
*/
void KAMU::setConnectionType(uint8_t connectiontype)
{
    ConnectionType = connectiontype;
}
void KAMU::usbConfig()
{
    static int trycnt;
    while (true)
    {
        Usb.Task();
        switch (Usb.getUsbTaskState())
        {
        case USB_STATE_RUNNING:
        {
            if (trycnt > 14)
            {
                Serial.println();
                Serial.println("Connection Failure!");
                Serial.println("Please check the connection and try again.");
                return;
            }
            switch (ConnectionType)
            {
            case CONNECTION_BT: // 블루투스로 연결시 엔트리모드 진입할것
            {
                if (!trycnt)
                {
                    Serial.print("Preparing to connect Bluetooth...");
                }
                else
                {
                    Serial.print(".");
                }
                String rcvMsg = readMsg();
                int length = rcvMsg.length();
                if (length > 14)
                {
                    String compare = rcvMsg.substring(length - 14, length);
                    if (compare == "\r\nConnected!\r\n")
                    {
                        Serial.println("Connection Succeed");
                        ConnectionStatus = CONNECTED;
                        trycnt = 0;
                        writeMsg("^et");
                        delay(500);
                        flushBuffer();
                        return;
                    }
                }

                delay(500);
            }
            break;
            case CONNECTION_UART:
            {
                uint8_t rcvcnt = 5;
                if (!trycnt)
                {
                    Serial.print("Preparing to connect UART...");
                }
                else
                {
                    Serial.print(".");
                }
                writeMsg("$hp");
                delay(400);
                while (rcvcnt)
                {
                    String rcvMsg = readMsg();
                    int length = rcvMsg.length();
                    if (length >= 8)
                    {
                        String compare = rcvMsg.substring(length - 8, length);
                        if (compare == "Finish\r\n")
                        {
                            Serial.println("Connection Succeed");
                            ConnectionStatus = CONNECTED;
                            trycnt = 0;
                            delay(500);
                            return;
                        }
                    }
                    rcvcnt--;
                    delay(10);
                }
                delay(1000);
            }
            break;

            default:
                break;
            }
            trycnt++;
        }
        break;
        case USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE:
            if (trycnt)
            {
                Serial.println("\nUSB Device Disconnected!");
                Serial.print("Waiting for connection...");
            }
            trycnt = 0;
            Serial.print(".");
            delay(500);
            break;
        }
    }
}
/*
        라이브러리 설정을 초기화 하는 함수 입니다.
        원하는 Serial 통신 baud rate 를 설정 해 주세요.
*/
void KAMU::init(unsigned long baud)
{
    Serial.begin(baud);
    Serial.print("Waiting for connection...");
    int connection = Usb.Init();
    if (connection == -1)
    {
        Serial.println("OSC Failed");
        return;
    }
    usbConfig();
    while (ConnectionStatus == DISCONNECTED)
    {
        delay(10);
    };
}
void KAMU::writeMsg(String msg)
{
    uint8_t rcode;
    int strlength = msg.length();
    char strbuf[strlength + 1];
    msg.toCharArray(strbuf, strlength + 1);
    rcode = cp210x.SndData(strlength, (uint8_t *)strbuf);
    if (rcode)
        ErrorMessage<uint8_t>(PSTR("SndData"), rcode);
}
String KAMU::readMsg()
{
    uint8_t buf[64];
    for (uint8_t i = 0; i < 64; i++)
        buf[i] = 0;
    uint8_t rcode;
    uint16_t rcvd = 64;
    rcode = cp210x.RcvData(&rcvd, buf);
    if (rcode && rcode != hrNAK)
        ErrorMessage<uint8_t>(PSTR("Ret"), rcode);
    String result = "";
    if (rcvd > 2)
    {
        for (uint16_t i = 0; i < rcvd; i++)
        {
            result += char(buf[i]);
        }
    }
    return result;
}
void KAMU::flushBuffer()
{

    Usb.Task();
    uint8_t buf[64];
    for (uint8_t i = 0; i < 64; i++)
        buf[i] = 0;
    uint8_t rcode;
    uint16_t rcvd = 64;
    rcode = cp210x.RcvData(&rcvd, buf);
    if (rcode && rcode != hrNAK)
        ErrorMessage<uint8_t>(PSTR("Ret"), rcode);
}
/*
        Arduino 와 까무의 연결 상태를 체크하는 함수 입니다.
        원활한 동작을 위해 주기적으로 실행하는 것을 권장합니다.
*/
void KAMU::run()
{
    Usb.Task();
    delay(10);
    if (Usb.getUsbTaskState() != status_old && Usb.getUsbTaskState() == USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE)
    {
        Serial.println("Disconnected!!");
        Serial.print("waiting for usb connection...");
        ConnectionStatus = DISCONNECTED;
        usbConfig();
        while (ConnectionStatus == DISCONNECTED)
        {
            delay(10);
        };
    }
    status_old = Usb.getUsbTaskState();
    if (ConnectionType == CONNECTION_BT && !checkStatus())
    {
        Serial.println("Bluetooth Disconnected!!");
        Serial.print("waiting for usb connection...");
        ConnectionStatus = DISCONNECTED;
        usbConfig();
        while (ConnectionStatus == DISCONNECTED)
        {
            delay(10);
        };
    }
}

void KAMU::waitUntilStopped()
{
    switch (ConnectionType)
    {
    case CONNECTION_UART:
        while (true)
        {
            Usb.Task();
            if (Usb.getUsbTaskState() == USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE)
                return;
            String rcvMsg = readMsg();
            int length = rcvMsg.length();
            if (length >= 8)
            {
                String compare = rcvMsg.substring(length - 8, length);
                Serial.println(compare);
                if (compare == "Finish\r\n")
                {
                    Serial.println("Finish");
                    return;
                }
            }
        }
        break;
    case CONNECTION_BT:
    {
        uint8_t proc = 0;
        while (true)
        {
            if (!checkStatus())
                return;
            switch (proc)
            {
            case 0:
                if (status == RUNNING)
                {
                    proc++;
                }
                break;

            case 1:
                if (status == STOPPED)
                {
                    return;
                }
                break;
            }
        }
    }
    break;
    }
}
bool KAMU::checkStatus()
{
    unsigned long time = millis();
    int errcnt = 0;
    while (errcnt < 100)
    {
        if (millis() - time > 1)
        {
            time = millis();
            String rcvMsg = readMsg();
            int rcvcnt = 0;
            while (rcvMsg)
            {
                uint8_t length = rcvMsg.length();
                int pos = rcvMsg.indexOf("\n");
                if (pos > 0)
                {
                    String dat = rcvMsg.substring(0, pos);
                    String remain = rcvMsg.substring(pos + 1, length);
                    rcvMsg = remain;
                    rcvcnt++;
                    if (dat == "{entry:true}")
                    {
                        status = RUNNING;
                    }
                    else if (dat == "{entry:false}")
                    {
                        status = STOPPED;
                    }
                }
                else
                {
                    break;
                }
            }
            errcnt = rcvcnt ? 0 : errcnt + 1;
            if (!errcnt)
                return true;
        }
    }
    return false;
}
/*기본 동작 (motionnum)번 동작을 수행합니다.
        0 <= motionnum <= 89*/
void KAMU::playBasicMotion(int motionnum)
{
    if (ConnectionStatus)
    {
        if (motionnum > 89)
        {
            Serial.println("The size of the motion number must be less than 90.");
            return;
        }
        else if (motionnum < 0)
        {
            Serial.println("motion number must be 0 or more.");
            return;
        }
        char hex[3];
        sprintf(hex, "%02x", motionnum);
        String cmd = "$pm" + String(hex);
        flushBuffer();
        writeMsg(cmd);
        waitUntilStopped();
    }
}
/*
        커스텀 동작 (motionnum)번 동작을 수행합니다.
        0 <= motionnum <= 89
*/
void KAMU::playCustomMotion(int motionnum)
{
    if (ConnectionStatus)
    {
        if (motionnum > 89)
        {
            Serial.println("The size of the motion number must be less than 90.");
            return;
        }
        else if (motionnum < 0)
        {
            Serial.println("motion number must be 0 or more.");
            return;
        }
        char hex[3];
        sprintf(hex, "%02x", motionnum + 90);
        String cmd = "$pm" + String(hex);
        flushBuffer();
        writeMsg(cmd);
        waitUntilStopped();
    }
}
/*
        (motornum)번 모터를 (angle)도 만큼 회전합니다.
        회전시간은 약 250ms 입니다.
        0 <= motornum <= 17,  -90 <= angle <= 90
*/
void KAMU::rotateMotor(int motornum, int angle)
{
    if (ConnectionStatus)
    {
        if (motornum > 17 || motornum < 0)
        {
            Serial.println("motornum variable must be in 0 <= motornum <= 17");
            return;
        }
        if (angle < -90)
        {
            Serial.println("The angle variable cannot be less than -90.");
            angle = -90;
        }
        else if (angle > 90)
        {
            Serial.println("The angle variable cannot be greater than 90.");
            angle = 90;
        }
        char hex1[3];
        char hex2[4];
        sprintf(hex1, "%02x", motornum);
        if (angle * 10 < 0)
        {
            sprintf(hex2, "%03x", 4096 + (angle * 10));
        }
        else
            sprintf(hex2, "%03x", angle * 10);
        String cmd = "^an" + String(hex1) + String(hex2);
        flushBuffer();
        Serial.println(cmd);
        writeMsg(cmd);
        waitUntilStopped();
    }
}
/*
        (motornum)번 모터를 (angle)도 만큼 (time)밀리초 동안 회전합니다.
        0 <= motionnum <= 17,  -90 <= angle <= 90,  time >= 0
*/
void KAMU::rotateMotor(int motornum, int angle, int time)
{
    if (ConnectionStatus)
    {
        if (motornum > 17 || motornum < 0)
        {
            Serial.println("motornum variable must be in 0 <= motornum <= 17");
            return;
        }
        if (time < 0)
        {
            Serial.println("The time variable must be greater than or equal to 0.");
            return;
        }
        if (angle < -90)
        {
            Serial.println("The angle variable cannot be less than -90.");

            angle = -90;
        }
        else if (angle > 90)
        {
            Serial.println("The angle variable cannot be greater than 90.");
            angle = 90;
        }
        char hex1[3];
        char hex2[4];
        char hex3[5];
        sprintf(hex1, "%02x", motornum);
        if (angle * 10 < 0)
        {
            sprintf(hex2, "%03x", 4096 + (angle * 10));
        }
        else
            sprintf(hex2, "%03x", angle * 10);
        sprintf(hex3, "%04x", time);
        String cmd = "^ad" + String(hex1) + String(hex2) + String(hex3);
        flushBuffer();
        writeMsg(cmd);
        waitUntilStopped();
    }
}
/*
    임시동작의 (startFrame)프레임부터 (endFrame)프레임 까지 동작을 실행 합니다.
*/
void KAMU::playTempMotion(TempMotion motion, int startFrame, int endFrame)
{
    if (startFrame < 0)
    {
        return;
    }
    if (endFrame > 20)
    {
        return;
    }
    if (startFrame > endFrame)
    {
        return;
    }
    int cnt;
    for (int f = startFrame; f <= endFrame; f++)
    {
        String cmd = "*mf";
        char hex_frame[3];
        sprintf(hex_frame, "%02x", f - startFrame);
        cmd += String(hex_frame);
        for (int i = 0; i < 18; i++)
        {
            char hex_angle[5];
            int angle = motion.getAngle(f, i); // motionbuffer[f].angle[i];
            if (angle < 0)
            {
                angle = 65536 + angle * 10;
            }
            else
                angle = angle * 10;

            sprintf(hex_angle, "%04x", angle);
            cmd += String(hex_angle);
            if (i == 8 || i == 17)
            {
                cmd += "000000000000";
            }
        }
        flushBuffer();
        writeMsg(cmd);
        delay(10);
        String cmd2 = "*mt";
        cmd2 += String(hex_frame);
        char hex_time[5];
        int time = motion.getTime(f); // motionbuffer[f - startFrame].time;
        sprintf(hex_time, "%04x", time);
        cmd2 += String(hex_time);
        flushBuffer();
        writeMsg(cmd2);
        delay(10);
        cnt++;
    }
    String cmd3 = "*pm";
    char hex_length[3];
    sprintf(hex_length, "%02x", cnt);
    cmd3 += String(hex_length);
    flushBuffer();
    writeMsg(cmd3);
    delay(10);
    waitUntilStopped();
}

/*
    임시동작 (framenum)번 프레임의 (motornum)번 모터의 각도를 (angle)도로 설정합니다.
    여러 모터를 동시에 움직일 시에는 이 함수를 여러 번 사용하면 됩니다.
    0 <= framenum <= 20,  0 <= motornum <= 17,  -90 <= angle <= 90
*/
void TempMotion::setMotorDegree(uint8_t framenum, uint8_t motornum, int8_t angle)
{
    if (framenum < 0 || framenum > 20)
    {
        return;
    }
    if (motornum < 0 || motornum > 17)
    {
        return;
    }
    if (angle < -90)
    {
        angle = -90;
    }
    else if (angle > 90)
    {
        angle = 90;
    }
    motionbuffer[framenum].angle[motornum] = angle;
}
/*
    임시동작 (framenum)번 프레임의 시간을 (time)밀리초로 설정합니다.
    time >= 0
*/
void TempMotion::setFrameTime(uint8_t framenum, uint16_t time)
{
    if (framenum < 0 || framenum > 20)
    {
        return;
    }
    if (time < 0)
    {
        Serial.println("The time variable must be greater than or equal to 0.");
        return;
    }
    motionbuffer[framenum].time = time;
}
int8_t TempMotion::getAngle(uint8_t frame, uint8_t motornum)
{
    return motionbuffer[frame].angle[motornum];
}
uint16_t TempMotion::getTime(uint8_t frame)
{
    return motionbuffer[frame].time;
}