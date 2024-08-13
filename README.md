# KAMU_Library_for_Arduino
로보까무 제어를 위한 아두이노 라이브러리 입니다.


# How to Use
* **모듈 연결 방법**

* **라이브러리 설치 및 사용법**
  1. ZIP형식의 소스파일을 다운로드 받습니다.<br/><br/>
  
    
  2. 아두이노 IDE를 실행 한 뒤,  **스케치** -> **라이브러리포함** -> **.ZIP 라이브러리 추가...** 에서 <br/> 다운로드 받은 **KAMU_Library_for_Arduino-main.zip**을 추가해 주세요.<br/><br/>

    
  3. 라이브러리 설명

    ```c++
    #include "Kamu.h"
    ```
    * kamu.h 라이브러리를 include 합니다.<br/><br/>

    ```c++
    KAMU kamu;
    ```
    * KAMU 객체를 선언합니다.<br/><br/>

    ```c++
    unsigned long BaudRate = 115200;
    ```
    * 까무 및 Serial 통신 BaudRate를 설정합니다.
    * BaudRate 는 115200으로 설정해 주세요.<br/><br/>

    ```c++
    void setup()
    {
      kamu.setConnectionType(CONNECTION_BT);
      //kamu.setConnectionType(CONNECTION_UART);
      kamu.init(BaudRate);
    }
    ```
    * 까무 통신 방식을 설정합니다.
    * 블루투스 동글이를 사용해 **블루투스**로 연결할 경우 ***CONNECTION_BT*** 를,<br/>USB-C 케이블을 사용해 **직접 연결**할 경우 ***CONNECTION_UART*** 를 입력해 주세요.
    * **init()** 함수를 사용해 까무와 아두이노의 통신 연결을 Initializing 해 주세요.<br/><br/> 

     ```c++
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
    * **run()** 함수는 까무와 아두이노의 연결상태를 확인 및 업데이트 하는 함수입니다.<br/>loop문 안에서 수시로 호출 될 수 있게 작성 해 주세요.<br/><br/>

  4. 함수 설명
   
      **까무의 모터의** ***0°*****는 처음 전원을 켰을때의 '*****차렷*****' 자세를 기준으로 합니다.** 

      * **playBasicMotion(int *<span style="color:Green">motionnum</span>*)**
        * 기본 동작 (***<span style="color:Green">motionnum</span>***) 번 동작을 실행 합니다.
      * **playCustomMotion(int *<span style="color:Green">motionnum</span>*)**
        * 커스텀 동작 (***<span style="color:Green">motionnum</span>***) 번 동작을 실행 합니다.
      * **rotateMotor(int *<span style="color:Green">motornum</span>*, int *<span style="color:Red">angle</span>*)**
        * (***<span style="color:Green">motornum</span>***) 번 모터를 (***<span style="color:Red">angle</span>***) 도로 회전 합니다.
      * **rotateMotor(int *<span style="color:Green">motornum</span>*, int *<span style="color:Red">angle</span>*, int *<span style="color:Blueviolet">time</span>*)**
        * (***<span style="color:Green">motornum</span>***) 번 모터를 (***<span style="color:Red">angle</span>***) 도로 (***<span style="color:Blueviolet">time</span>***) 밀리초 동안 회전 합니다.
      <br/><br/>
      ---
      **나만의 동작을 만들기 위해서는 *motion.h* 파일의 작성이 필요합니다.<br/>**
      * **motion.h**
      ```c++
      #include "Kamu.h"

      //=======playTempMotion,saveTempMotion=======
      //---------------필수 설정-------------------
      /*재생할 프레임의 총 길이를 의미합니다.*/
      /*만들 수 있는 최대 프레임의 길이는 20 입니다.*/
      #define FRAME_LENGTH 5

      //============================================

      //==============saveTempMotion==============
      //---------------필수 설정-------------------
      /*저장할 슬롯의 번호를 지정합니다.*/
      #define CUSTOM_SLOT 0

      /*저장할 동작의 이름을 지정합니다.
      최대 20-Byte 까지 설정 가능하며,
      영어는 글자당 1-Byte, 한글은 3-Byte가 사용됩니다.
      */
      #define MOTION_NAME "TEST"

      /*
      모션 저장에 사용되는 구조체 입니다.
      수정할 필요 없습니다.
      */
      const HEADER motionheader = {
          CUSTOM_SLOT,
          MOTION_NAME,
          FRAME_LENGTH};

      /*------------------동작 추가 기능 활성화----------------------*/
      //============================================================
      /* <== 추가 기능을 활성화 하려면 주석을 제거해 주세요.

      //동작의 반복 여부를 설정합니다.
      #define LOOP_ENABLE 0

      //반복 시작 프레임을 설정합니다.
      #define LOOP_START 0

      //반복 끝 프레임을 설정합니다.
      #define LOOP_END 0

      //반복 횟수를 설정합니다.
      #define LOOP_CNT 0

      //자동 일어서기 모드가 켜져있을때 넘어짐 감지 활성화 여부를 설정합니다.
      #define FALL_DETECTION 0

      //==============================================================
      const HEADER_EXTENDED motionheader_extended = {
          LOOP_ENABLE,
          LOOP_START,
          LOOP_END,
          LOOP_CNT,
          FALL_DETECTION};
      ==> */
      /*-----------------------------------------------------------------*/

      /**************************************************************************************************************************************************************/

      //===============================TempMotion 각도 설정===========================
      /*
      각 프레임의 개별 모터의 각도를 설정합니다.
      FRAME_LENGTH 개수 만큼 작성해 주세요.
      */
      const int8_t frame0[MOTORS_KAMU] = {
        90, 0, 0, 
        0, 0, 0, 
        0, 0, 0, 
        0, 0, 0, 
        0, 0, 0, 
        0, 0, 0};
      const int8_t frame1[MOTORS_KAMU] = {
          0, 0, 0,
          0, 0, 0,
          0, 0, 0,
          0, 0, 0,
          0, 0, 0,
          0, 0, 0};
      const int8_t frame2[MOTORS_KAMU] = {
          90, 0, 0,
          0, 0, 0,
          0, 0, 0,
          0, 0, 0,
          0, 0, 0,
          0, 0, 0};
      const int8_t frame3[MOTORS_KAMU] = {
          0, 0, 0,
          0, 0, 0,
          0, 0, 0,
          0, 0, 0,
          0, 0, 0,
          0, 0, 0};
      const int8_t frame4[MOTORS_KAMU] = {
          90, 0, 0,
          0, 0, 0,
          0, 0, 0,
          0, 0, 0,
          0, 0, 0,
          0, 0, 0};

      /*
      FRAME_LENGTH 크기에 맞춰 배열을 작성해 주세요.
      */
      const int8_t *motion0_angles[FRAME_LENGTH] = {
          (int8_t *)&frame0, (int8_t *)&frame1, (int8_t *)&frame2,
          (int8_t *)&frame3, (int8_t *)&frame4};

      //===============================TempMotion 시간 설정===========================
      /*
      프레임별 동작 시간을 설정합니다.
      FRAME_LENGTH 크기에 맞춰 배열을 작성해 주세요.
      */
      const uint16_t motion0_times[FRAME_LENGTH] = {
          300, 300, 500, 500, 700
          };

      ```
      * **void playTempMotion(uint8_t *<span style="color:Green">framelength</span>*, const uint16_t *<span style="color:Blueviolet">\*time_arr</span>*, const int8_t *<span style="color:Red">\*\*angledata_arr</span>*)**
        * *motion.h* 에서 작성한 동작을 재생합니다.
        * ex) `kamu.playTempMotion(FRAME_LENGTH, motion0_times, motion0_angles);`
      * **void saveTempMotion(const HEADER *<span style="color:Green">\*header</span>*, const uint16_t *<span style="color:Blueviolet">\*time_arr</span>*, const int8_t *<span style="color:Red">\*\*angledata_arr</span>*)**
        * *motion.h* 에서 작성한 동작을 까무에 저장합니다.
        * ex) `kamu.saveTempMotion(&motionheader, motion0_times, motion0_angles);`
      * **void saveTempMotion(const HEADER *<span style="color:Green">\*header</span>*, const HEADER_EXTENDED *<span style="color:Yellowgreen">\*header_extended</span>*, const uint16_t *<span style="color:Blueviolet">\*time_arr</span>*, const int8_t *<span style="color:Red">\*\*angledata_arr</span>*)**
        * motion.h 에서 작성한 동작을 까무에 저장합니다.(추가기능 포함)
        * ex) `kamu.saveTempMotion(&motionheader, &motionheader_extended, motion0_times, motion0_angles);`<br/><br/>
# Licence
The code is released under the GNU General Public License.<br/><br/>


# Additional Resources
* USB_Host_Shield_Library_2.0  : <https://github.com/felis/USB_Host_Shield_2.0>
* Forked "USB_Host_Shield_2.0" with CP210x driver : <https://github.com/henla464/USB_Host_Shield_Library_2.0>
