#include "Kamu.h"

//=======playTempMotion,saveTempMotion=======
//---------------필수 설정-------------------
/*재생할 프레임의 총 길이를 의미합니다.*/
#define FRAME_LENGTH 20

//============================================

//==============saveTempMotion==============
//---------------필수 설정-------------------
/*저장할 슬롯의 번호를 지정합니다.*/
#define CUSTOM_SLOT 0
/*저장할 동작의 이름을 지정합니다.
최대 20-Byte 까지 설정 가능하며,
영어는 글자당 1-Byte, 한글은 3-Byte가 사용됩니다.
*/
#define MOTION_NAME "박수              "

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
0, 0, 0, 
0, 0, 0, 
0, 0, 0, 
0, 0, 0, 
0, 0, 0, 
0, 0, 0};
const int8_t frame1[MOTORS_KAMU] = {
70, 0, 0, 
0, 0, 0, 
0, 0, 0, 
-70, 0, 0, 
0, 0, 0, 
0, 0, 0};
const int8_t frame2[MOTORS_KAMU] = {
70, 0, -3, 
46, 0, 0, 
0, 0, 0, 
-70, 0, 3, 
-46, 0, 0, 
0, 0, 0};
const int8_t frame3[MOTORS_KAMU] = {
70, 0, 20, 
55, 0, 0, 
0, 0, 0, 
-70, 0, -20, 
-55, 0, 0, 
0, 0, 0};
const int8_t frame4[MOTORS_KAMU] = {
70, 0, -3, 
46, 0, 0, 
0, 0, 0, 
-70, 0, 3, 
-46, 0, 0, 
0, 0, 0};
const int8_t frame5[MOTORS_KAMU] = {
70, 0, 20, 
55, 0, 0, 
0, 0, 0, 
-70, 0, -20, 
-55, 0, 0, 
0, 0, 0};
const int8_t frame6[MOTORS_KAMU] = {
70, 0, -3, 
46, 0, 0, 
0, 0, 0, 
-70, 0, 3, 
-46, 0, 0, 
0, 0, 0};
const int8_t frame7[MOTORS_KAMU] = {
70, 0, 20, 
55, 0, 0, 
0, 0, 0, 
-70, 0, -20, 
-55, 0, 0, 
0, 0, 0};
const int8_t frame8[MOTORS_KAMU] = {
70, 0, -3, 
46, 0, 0, 
0, 0, 0, 
-70, 0, 3, 
-46, 0, 0, 
0, 0, 0};
const int8_t frame9[MOTORS_KAMU] = {
70, 0, 20, 
55, 0, 0, 
0, 0, 0, 
-70, 0, -20, 
-55, 0, 0, 
0, 0, 0};
const int8_t frame10[MOTORS_KAMU] = {
70, 0, -3, 
46, 0, 0, 
0, 0, 0, 
-70, 0, 3, 
-46, 0, 0, 
0, 0, 0};
const int8_t frame11[MOTORS_KAMU] = {
70, 0, 20, 
55, 0, 0, 
0, 0, 0, 
-70, 0, -20, 
-55, 0, 0, 
0, 0, 0};
const int8_t frame12[MOTORS_KAMU] = {
70, 0, -3, 
46, 0, 0, 
0, 0, 0, 
-70, 0, 3, 
-46, 0, 0, 
0, 0, 0};
const int8_t frame13[MOTORS_KAMU] = {
70, 0, 20, 
55, 0, 0, 
0, 0, 0, 
-70, 0, -20, 
-55, 0, 0, 
0, 0, 0};
const int8_t frame14[MOTORS_KAMU] = {
70, 0, -3, 
46, 0, 0, 
0, 0, 0, 
-70, 0, 3, 
-46, 0, 0, 
0, 0, 0};
const int8_t frame15[MOTORS_KAMU] = {
70, 0, 20, 
55, 0, 0, 
0, 0, 0, 
-70, 0, -20, 
-55, 0, 0, 
0, 0, 0};
const int8_t frame16[MOTORS_KAMU] = {
70, 0, -3, 
46, 0, 0, 
0, 0, 0, 
-70, 0, 3, 
-46, 0, 0, 
0, 0, 0};
const int8_t frame17[MOTORS_KAMU] = {
70, 0, 20, 
55, 0, 0, 
0, 0, 0, 
-70, 0, -20, 
-55, 0, 0, 
0, 0, 0};
const int8_t frame18[MOTORS_KAMU] = {
70, 0, -16, 
20, 0, 0, 
0, 0, 0, 
-70, 0, 16, 
-20, 0, 0, 
0, 0, 0};
const int8_t frame19[MOTORS_KAMU] = {
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
(int8_t *)&frame5, (int8_t *)&frame6, (int8_t *)&frame7, (int8_t *)&frame8, (int8_t *)&frame9, 
(int8_t *)&frame10, (int8_t *)&frame11, (int8_t *)&frame12, (int8_t *)&frame13, (int8_t *)&frame14, 
(int8_t *)&frame15, (int8_t *)&frame16, (int8_t *)&frame17, (int8_t *)&frame18, (int8_t *)&frame19};


//===============================TempMotion 시간 설정===========================
/*
프레임별 동작 시간을 설정합니다.
FRAME_LENGTH 크기에 맞춰 배열을 작성해 주세요.
*/
const uint16_t motion0_times[FRAME_LENGTH] = {
100, 500, 220, 220, 
220, 220, 220, 220, 220, 
220, 220, 220, 220, 220, 
220, 220, 220, 220, 400, 
800};
