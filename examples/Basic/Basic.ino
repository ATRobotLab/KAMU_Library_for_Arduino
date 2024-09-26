#include "Kamu.h"

KAMU kamu;
unsigned long BaudRate = 115200;
void setup()
{
  kamu.setConnectionType(CONNECTION_DONGLE);
  kamu.init(BaudRate);
}

void loop()
{
  kamu.run();
  kamu.rotateMotor(0, 90, 1000);
  kamu.rotateMotor(9, -90, 1000);
  kamu.rotateMotor(0, 0, 1000);
  kamu.rotateMotor(9, 0, 1000);
  kamu.playBasicMotion(1);
}
