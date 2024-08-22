#include "Kamu.h"
#include "Motion.h"

KAMU kamu;
unsigned long BaudRate = 115200;
void setup()
{
  kamu.setConnectionType(CONNECTION_BT);
  kamu.init(BaudRate);
}

void loop()
{
  kamu.run();
  kamu.playTempMotion(FRAME_LENGTH, motion0_times, motion0_angles);
}
