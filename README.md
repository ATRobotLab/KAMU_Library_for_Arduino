# KAMU_Library_for_Arduino
로보까무 제어를 위한 아두이노 라이브러리 입니다.


# How to Use
 1. ZIP형식의 소스파일을 다운로드 받습니다.

    
 2. 아두이노 IDE를 실행 한 뒤,  **스케치** -> **라이브러리포함** -> **.ZIP 라이브러리 추가...** 에서 <br/> 다운로드 받은 **KAMU_Library_for_Arduino-main.zip**을 추가해 주세요.

    
 3. 라이브러리 설명
  
   ```
   #include "Kamu.h"
   ```
  * kamu.h 라이브러리를 include 합니다.
  
   ```
   KAMU kamu;
   ```
  * KAMU 객체를 선언합니다.
  
   ```
   unsigned long BaudRate = 115200;
   ```
  * 까무 및 Serial 통신 BaudRate를 설정합니다.
  * BaudRate 는 115200으로 설정해 주세요.
  
   ```
   void setup()
   {
     kamu.setConnectionType(CONNECTION_BT);
     kamu.init(BaudRate);
   }
   ```
  * 까무 통신 방식을 설정합니다.
  * 블루투스 동글이를 사용해 **블루투스**로 연결할 경우 ***CONNECTION_BT*** 를,<br/>USB-C 케이블을 사용해 **직접 연결**할 경우 ***CONNECTION_UART*** 를 입력해 주세요.
  * **init()** 함수를 사용해 까무와 아두이노의 통신 연결을 Initializing 해 주세요. 
  
   ```
   void loop()
   {
     kamu.run();
     //0번 모터를 1초동안 90도로 회전합니다.
     kamu.rotateMotor(0, 90, 1000);
     //9번 모터를 1초동안 -90도로 회전합니다.
     kamu.rotateMotor(9, -90, 1000);
     //0번 모터를 1초동안 0도로 회전합니다.
     kamu.rotateMotor(0, 0, 1000);
     //9번 모터를 1초동안 0도로 회전합니다.
     kamu.rotateMotor(9, 0, 1000);
     //1번 동작을 실행합니다.
     kamu.playBasicMotion(1);
   }
   ```
  * **run()** 함수는 까무와 아두이노의 연결상태를 확인 및 업데이트 하는 함수입니다.<br/>loop문 안에서 수시로 호출 될 수 있게 작성해 주세요.
   4. 함수 설명
      * **playBasicMotion(int motionnum)**
      * **playCustomMotion(int motionnum)**
      * **rotateMotor(int motornum, int angle)**
      * **rotateMotor(int motornum, int angle, int time)**
      * **void playTempMotion(uint8_t framelength, const uint16_t *time_arr, const int8_t **angledata_arr)**
      * **void saveTempMotion(const HEADER *header, const uint16_t *time_arr, const int8_t **angledata_arr)**
      * **void saveTempMotion(const HEADER *header, const HEADER_EXTENDED *header_extended, const uint16_t *time_arr, const int8_t **angledata_arr)**
# Licence
The code is released under the GNU General Public License.


# Additional Resources
* USB_Host_Shield_Library_2.0  : <https://github.com/felis/USB_Host_Shield_2.0>
* Forked "USB_Host_Shield_2.0" with CP210x driver : <https://github.com/henla464/USB_Host_Shield_Library_2.0>
