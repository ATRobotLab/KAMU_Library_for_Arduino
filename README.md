# KAMU_Library_for_Arduino
로보까무 제어를 위한 아두이노 라이브러리 입니다.


# How to Use
1. ZIP형식의 소스파일을 다운로드 받습니다.
2. 아두이노 IDE를 실행 한 뒤,  스케치 -> 라이브러리포함 -> .ZIP 라이브러리 추가... 에서 다운로드 받은 KAMU_Library_for_Arduino-main.zip를 추가해 주세요.
3. 라이브러리 설명

   ```
   //kamu.h 라이브러리를 include 합니다.
   #include "Kamu.h"
   
   ```

   ```
   //KAMU 객체를 선언합니다.
   KAMU kamu;
   ```

   ```
   //까무 및 Serial 통신 BaudRate를 설정합니다.
   //BaudRate 는 115200으로 설정해 주세요.
   unsigned long BaudRate = 115200;
   ```
   
   ```
   void setup()
   {
     //까무 통신 방식을 설정합니다.
     //블루투스 동글이를 사용해 블루투스로 연결할 경우 CONNECTION_BT 를
     //USB-C 케이블을 사용해 직접 연결할 경우 CONNECTION_UART 를 입력해 주세요.
     kamu.setConnectionType(CONNECTION_BT);

     //KAMU와 Arduino의 통신 연결을 Initializing 해주는 함수 입니다.
     kamu.init(BaudRate);
   }
   ```
   
   ```
   void loop()
   {
     kamu.run();
     kamu.rotateMotor(0, 90, 1000);
     kamu.rotateMotor(9, -90, 1000);
     kamu.rotateMotor(0, 0, 1000);
     kamu.rotateMotor(9, 0, 1000);
     kamu.playBasicMotion(1);
   }
   ```
   
# Licence
The code is released under the GNU General Public License.


# Additional Resources
* USB_Host_Shield_Library_2.0  : <https://github.com/felis/USB_Host_Shield_2.0>
* Forked "USB_Host_Shield_2.0" with CP210x driver : <https://github.com/henla464/USB_Host_Shield_Library_2.0>
