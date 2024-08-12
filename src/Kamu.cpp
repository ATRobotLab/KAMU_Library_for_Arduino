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
                writeMsg("^et");
                delay(1000);
                String rcvMsg = readMsg();
                int length = rcvMsg.length();
                if (length > 2)
                {
                    if (rcvMsg.indexOf("entry") >= 0)
                    {
                        Serial.println("Connection Succeed");
                        ConnectionStatus = CONNECTED;
                        trycnt = 0;
                        delay(500);
                        flushBuffer();
                        return;
                    }
                }
                flushBuffer();
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
        OSC 에러 발생시, 모듈이 제대로 장착 되어있는지 확인하고,
        usb전원을 뽑았다 다시 연결시켜 주세요.
*/
void KAMU::init(unsigned long baud)
{
    Serial.begin(baud);
    Serial.print("Waiting for connection...");
    int connection = Usb.Init();
    Serial.print("connection : ");
    Serial.println(connection);
    int cnt = 0;
    while (connection == -1)
    {
        switch (cnt)
        {
        case 0:
            Serial.println("OSC Failed");
            connection = Usb.Init();
            delay(10);
            cnt++;
            break;
        case 1:
        case 2:
            Serial.println(cnt);
            connection = Usb.Init();
            delay(10);
            cnt++;
            break;
        case 3:
            Serial.println("Initializing failed. After Checking your system connection, remove the power and reconnect it.");
            delay(100);
            cnt++;
            break;
        default:
            delay(100);
        }
    }
    usbConfig();
    while (ConnectionStatus == DISCONNECTED)
    {
        delay(10);
    };
    Serial.println(cnt);
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
void KAMU::waitUntilReceived()
{
    String rcvMsg = readMsg();
    while (true)
    {
        if (ConnectionType == CONNECTION_BT)
        {
            if (rcvMsg.length() > 0)
            {
                int index = rcvMsg.indexOf("\n");
                if (index >= 0)
                {
                    String data = rcvMsg.substring(0, index);
                    String remain = rcvMsg.substring(index + 1, rcvMsg.length() + 1);
                    rcvMsg = remain;
                    if (data.indexOf("ATfinRobot") >= 0)
                    {
                        Serial.println("Received");
                        return;
                    }
                }
                else
                {
                    rcvMsg += readMsg();
                }
            }
            else
                rcvMsg = readMsg();
        }
        else
        {
            if (rcvMsg.length() > 0)
            {
                if (rcvMsg.indexOf("Written") >= 0)
                {
                    Serial.println("Received");
                    return;
                }
                else
                {
                    rcvMsg = readMsg();
                }
            }
            else
                rcvMsg = readMsg();
        }
    }
}
void KAMU::waitUntilStopped(unsigned long timeout)
{
    unsigned long time = millis();
    switch (ConnectionType)
    {
    case CONNECTION_UART:
        while (true)
        {
            if (timeout > 0)
            {
                if (millis() - time > timeout)
                {
                    return;
                }
            }
            Usb.Task();
            if (Usb.getUsbTaskState() == USB_DETACHED_SUBSTATE_WAIT_FOR_DEVICE)
                return;
            String rcvMsg = readMsg();
            int length = rcvMsg.length();
            if (length >= 8)
            {
                String compare = rcvMsg.substring(length - 8, length);
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
            if (timeout > 0)
            {
                if (millis() - time > timeout)
                {
                    return;
                }
            }
            if (!checkStatus())
            {
                Serial.println("error occured;");
                return;
            }

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
        waitUntilStopped(TIMEOUT);
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
        waitUntilStopped(TIMEOUT);
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
        writeMsg(cmd);
        waitUntilStopped(TIMEOUT);
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
        waitUntilStopped(TIMEOUT);
    }
}

int KAMU::angleCorrection(int8_t angle)
{
    if (angle > 90)
    {
        return 900;
    }
    else if (angle < -90)
    {
        return -900;
    }
    else
    {
        return angle * 10;
    }
}
/*
    임시동작을 실행 합니다.
    임시동작은 예제의 헤더파일 형식을 참고해 주세요.
    framelength = 총 프레임 길이, *time_arr = 각 프레임의 동작 시간(*uint16_t[]),
    **angledata_arr = 각 프레임의 모터값들(**int8_t[])
*/
void KAMU::playTempMotion(uint8_t framelength, const uint16_t *time_arr, const int8_t **angledata_arr)
{
    if (framelength < 0 || framelength > 20)
    {
        return;
    }
    for (uint8_t frame = 0; frame < framelength; frame++)
    {
        String cmd = "*mf";
        char hex_frame[3];
        sprintf(hex_frame, "%02x", frame);
        cmd += String(hex_frame);
        for (uint8_t motor = 0; motor < MOTORS_KAMU; motor++)
        {
            int angle = angledata_arr[frame][motor];
            angle = angleCorrection(angle);
            char hex_angle[5];
            if (angle < 0)
            {
                angle = 65536 + angle;
            }
            sprintf(hex_angle, "%04x", angle);
            cmd += String(hex_angle);
            if (motor == 8 || motor == 17)
            {
                cmd += "000000000000";
            }
        }
        flushBuffer();
        writeMsg(cmd);
        delay(10);
        uint16_t time = time_arr[frame];
        String cmd2 = "*mt";
        cmd2 += String(hex_frame);
        char hex_time[5];
        sprintf(hex_time, "%04x", time);
        cmd2 += String(hex_time);
        flushBuffer();
        writeMsg(cmd2);
        delay(10);
    }
    String cmd3 = "*pm";
    char hex_length[3];
    sprintf(hex_length, "%02x", framelength);
    cmd3 += String(hex_length);
    flushBuffer();
    writeMsg(cmd3);
    delay(10);
    waitUntilStopped(TIMEOUT);
}
uint8_t utf8LengthOf(String str)
{
    uint8_t utf8Length = 0;
    for (uint8_t i = 0; i < (uint8_t)str.length(); i++)
    {
        char c = str.charAt(i);
        if (c < 0x80)
        {
            utf8Length++;
        }
        else if (c < 0x800)
        {
            utf8Length += 2;
        }
        else if (c < 0x10000)
        {
            utf8Length += 3;
        }
        else
        {
            utf8Length += 4;
        }
    }
    return utf8Length;
}
void stringToUtf8(String str, byte *utf8Bytes, int utf8Length)
{
    uint8_t byteIndex = 0;
    for (uint8_t i = 0; i < (uint8_t)str.length(); i++)
    {
        char c = str.charAt(i);
        if (c < 0x80)
        {
            utf8Bytes[byteIndex++] = c;
        }
        else if (c < 0x800)
        {
            utf8Bytes[byteIndex++] = 0xC0 | (c >> 6);
            utf8Bytes[byteIndex++] = 0x80 | (c & 0x3F);
        }
        else if (c < 0x10000)
        {
            utf8Bytes[byteIndex++] = 0xE0 | (c >> 12);
            utf8Bytes[byteIndex++] = 0x80 | ((c >> 6) & 0x3F);
            utf8Bytes[byteIndex++] = 0x80 | (c & 0x3F);
        }
        else
        {
            return;
        }
    }
}
String parsingHeader(const HEADER *data)
{
    String result;
    char buf[3];
    sprintf(buf, "%01x", 0);
    result += buf;
    sprintf(buf, "%02x", 0);
    result += buf;
    sprintf(buf, "%02x", 0);
    result += buf;
    sprintf(buf, "%02x", 0);
    result += buf;
    sprintf(buf, "%01x", 0);
    result += buf;
    result += "000";
    return result;
}
String parsingHeader(const HEADER_EXTENDED *data)
{
    String result;
    char buf[3];
    sprintf(buf, "%01x", data->loop);
    result += buf;
    sprintf(buf, "%02x", data->loop_start);
    result += buf;
    sprintf(buf, "%02x", data->loop_end);
    result += buf;
    sprintf(buf, "%02x", data->loop_count);
    result += buf;
    sprintf(buf, "%01x", data->detect);
    result += buf;
    result += "000";
    return result;
}
void KAMU::saveProcess(const HEADER *header, const uint16_t *time_arr, const int8_t **angledata_arr, String header_add)
{
    //=============================================================================================================
    //|     | slot |         name         | loop | loop start | loop end | loop count | detect |  x  | frame lngth |
    //-------------------------------------------------------------------------------------------------------------
    //| >MH |  00  | 00000000000000000000 |   0  |     00     |    00    |     00     |   0    | 000 |      00     |
    //=============================================================================================================
    //|     | slot | frame num | frame time |  angle  |
    //-------------------------------------------------
    //| >MF |  00  |     00    |    0000    | 0000*24 |
    //=================================================
    if (header->framelength < 0 || header->framelength > 20)
    {
        return;
    }
    if (header->slot < 0 || header->slot > 89)
    {
        return;
    }
    uint8_t nameLength = utf8LengthOf(header->name);
    if (nameLength > 20 || nameLength <= 0)
    {
        return;
    }
    Serial.println("Saving...");
    String cmd_mh = ">mh";
    String cmd_ch = ">ch";
    String cmd_mf = ">mf";

    char hex[3];
    sprintf(hex, "%02x", header->slot + 90);
    cmd_mh += String(hex);
    cmd_mf += String(hex);
    cmd_ch += String(hex);
    byte utf8Bytes[nameLength];
    stringToUtf8(header->name, utf8Bytes, nameLength);
    for (uint8_t i = 0; i < nameLength; i++)
    {
        cmd_mh += (char)utf8Bytes[i];
    }
    for (uint8_t i = nameLength; i < 20; i++)
    {
        cmd_mh += " ";
    }

    /*헤더 나머지 추가*/
    cmd_mh += header_add;
    /**/
    sprintf(hex, "%02x", header->framelength);
    cmd_mh += hex;
    flushBuffer();
    writeMsg(cmd_mh);
    waitUntilReceived();
    for (uint8_t frame = 0; frame < header->framelength; frame++)
    {
        String mf = cmd_mf;
        char hex_frame[3];
        char hex_time[5];
        sprintf(hex_time, "%04x", time_arr[frame]);
        sprintf(hex_frame, "%02x", frame);
        mf += String(hex_frame);
        mf += String(hex_time);
        for (uint8_t motor = 0; motor < MOTORS_KAMU; motor++)
        {
            int angle = angledata_arr[frame][motor];
            angle = angleCorrection(angle);
            char hex_angle[5];
            if (angle < 0)
            {
                angle = 65536 + angle;
            }
            sprintf(hex_angle, "%04x", angle);
            mf += String(hex_angle);
            if (motor == 8 || motor == 17)
            {
                mf += "000000000000";
            }
        }
        flushBuffer();
        writeMsg(mf);
        waitUntilReceived();
        delay(100);
    }
    flushBuffer();
    writeMsg(cmd_ch);
    waitUntilReceived();
    delay(100);
    Serial.println("Saved!");
}
void KAMU::saveTempMotion(const HEADER *header, const uint16_t *time_arr, const int8_t **angledata_arr)
{
    String header_add = parsingHeader(header);
    return saveProcess(header, time_arr, angledata_arr, header_add);
}

void KAMU::saveTempMotion(const HEADER *header, const HEADER_EXTENDED *header_extended, const uint16_t *time_arr, const int8_t **angledata_arr)
{
    String header_add = parsingHeader(header_extended);
    return saveProcess(header, time_arr, angledata_arr, header_add);
}