#include "Kamu.h"

//=======playTempMotion,saveTempMotion=======
//---------------필수 설정-------------------
/*재생할 프레임의 총 길이를 의미합니다.*/
#define FRAME_LENGTH 7

//============================================

//==============saveTempMotion==============
//---------------필수 설정-------------------
/*저장할 슬롯의 번호를 지정합니다.*/
#define CUSTOM_SLOT 0
/*저장할 동작의 이름을 지정합니다.
최대 20-Byte 까지 설정 가능하며,
영어는 글자당 1-Byte, 한글은 3-Byte가 사용됩니다.
*/
#define MOTION_NAME "물건 던지기    "

/*모션 저장에 사용되는 구조체 입니다.
수정할 필요 없습니다.
*/

/*
모션 저장에 사용되는 구조체 입니다.
수정할 필요 없습니다.
*/
const HEADER motionheader = {
    CUSTOM_SLOT,
    MOTION_NAME,
    FRAME_LENGTH};

/**************************************************************************************************************************************************************/

//===============================TempMotion 각도 설정===========================
/*
각 프레임의 개별 모터의 각도를 설정합니다.
FRAME_LENGTH 개수 만큼 작성해 주세요.
*/
const int8_t frame0[MOTORS_KAMU] = {
61, 0, 2, 
33, 0, 0, 
0, 0, 0, 
-62, 0, -2, 
-33, 0, 0, 
0, 0, 0};
const int8_t frame1[MOTORS_KAMU] = {
38, 0, 2, 
33, 0, 0, 
0, 0, 0, 
-38, 0, -2, 
-33, 0, 0, 
0, 0, 0};
const int8_t frame2[MOTORS_KAMU] = {
38, 0, 2, 
33, 0, 0, 
0, 0, 0, 
-38, 0, -2, 
-33, 0, 0, 
0, 0, 0};
const int8_t frame3[MOTORS_KAMU] = {
47, 0, 9, 
34, 0, 0, 
0, 0, 0, 
-62, 0, -8, 
-14, 0, 0, 
0, 0, 0};
const int8_t frame4[MOTORS_KAMU] = {
27, 0, -23, 
7, 0, 0, 
0, 0, 0, 
-71, 0, -6, 
-59, 0, 0, 
0, 0, 0};
const int8_t frame5[MOTORS_KAMU] = {
13, 0, -14, 
11, 0, 0, 
0, 0, 0, 
-13, 0, 15, 
-27, 0, 0, 
0, 0, 0};
const int8_t frame6[MOTORS_KAMU] = {
0, 0, 0, 
0, 0, 0, 
0, 0, 0, 
0, 0, 0, 
0, 0, 0, 
0, 0, 0};

/*
FRAME_LENGTH 크기에 맞춰 배열을 작성해 주세요.
*/
const int8_t *motion0_angles[FRAME_LENGTH] = {
(int8_t *)&frame0, (int8_t *)&frame1, (int8_t *)&frame2, (int8_t *)&frame3, (int8_t *)&frame4, 
(int8_t *)&frame5, (int8_t *)&frame6};

//===============================TempMotion 시간 설정===========================
/*
프레임별 동작 시간을 설정합니다.
FRAME_LENGTH 크기에 맞춰 배열을 작성해 주세요.
*/
const uint16_t motion0_times[FRAME_LENGTH] = {
700, 460, 460, 220, 
200, 440, 700};
