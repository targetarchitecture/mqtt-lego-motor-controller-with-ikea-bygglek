#include "motors.h"

Motors::Motors() : leftMotors(0x09), rightMotors(DEFAULT_I2C_MOTOR_ADDRESS)
{
  Log("Motor Shield load");
}

void Motors::Begin()
{
  Log("Motor Shield init");

  while (leftMotors.PRODUCT_ID != PRODUCT_ID_I2C_MOTOR) //wait motor shield ready.
  {
    leftMotors.getInfo();
  }

  while (rightMotors.PRODUCT_ID != PRODUCT_ID_I2C_MOTOR) //wait motor shield ready.
  {
    rightMotors.getInfo();
  }

  Log("Change A to CCW, B to CW, Freq: 1000Hz");

  leftMotors.changeFreq(MOTOR_CH_BOTH, 1000);  //Change A & B 's Frequency to 1000Hz.
  rightMotors.changeFreq(MOTOR_CH_BOTH, 1000); //Change A & B 's Frequency to 1000Hz.
}

void Motors::setMapped(int mapx, int mapy, int laserRangeMilliMeter) //, int medianCompassHeading)
{
  int maxDuty = 50;         //100;
  int maxRotationDuty = 50; //50;
  int Duty = 0;
  int SafeDistanceMM = 300;
  int DeadzoneMM = 60;
  int minimumDuty = 16;
  String Direction = "";

  if (laserRangeMilliMeter > SafeDistanceMM)
  {
    Duty = maxDuty;
  }
  else if (laserRangeMilliMeter <= SafeDistanceMM && laserRangeMilliMeter >= DeadzoneMM)
  {
    Duty = map(laserRangeMilliMeter, DeadzoneMM, SafeDistanceMM, minimumDuty, maxDuty);
  }
  else
  {
    Duty = 0; //failsafe
  }

  int maxTurnDuty = maxDuty / 2;

  Log("mapx: " + String(mapx) + " mapy: " + String(mapy) + " Duty: " + String(Duty));

  if (mapx == 0 && mapy == 1)
  {
    //North

    //take only the first reading as the starting heading
    // if (compassHeadingWhenStartedLinear == -1)
    // {
    //   compassHeadingWhenStartedLinear = medianCompassHeading;
    // }

    int DutyLeft = Duty;
    int DutyRight = Duty;

    // if (autoCorrectWithCompass == true)
    // {
    //   if (medianCompassHeading > compassHeadingWhenStartedLinear)
    //   {
    //     DutyRight = DutyRight * 0.9;
    //   }

    //   if (medianCompassHeading < compassHeadingWhenStartedLinear)
    //   {
    //     DutyLeft = DutyLeft * 0.9;
    //   }
    // }

    leftMotors.changeDuty(MOTOR_CH_BOTH, DutyLeft);
    rightMotors.changeDuty(MOTOR_CH_BOTH, DutyRight);
    leftMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CW);
    rightMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CW);
    Direction = "NORTH";
  }
  else if (mapx == 1 and mapy == 1)
  {
    //North East
    leftMotors.changeDuty(MOTOR_CH_BOTH, maxDuty);
    rightMotors.changeDuty(MOTOR_CH_BOTH, maxTurnDuty);
    leftMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CW);
    rightMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CW);
    Direction = "NORTH EAST";
  }
  else if (mapx == 1 and mapy == 0)
  {
    //East
    leftMotors.changeDuty(MOTOR_CH_BOTH, maxRotationDuty);
    rightMotors.changeDuty(MOTOR_CH_BOTH, maxRotationDuty);
    leftMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CW);
    rightMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CCW);
    Direction = "EAST";
  }
  else if (mapx == 1 and mapy == -1)
  {
    //South East
    leftMotors.changeDuty(MOTOR_CH_BOTH, maxDuty);
    rightMotors.changeDuty(MOTOR_CH_BOTH, maxTurnDuty);
    leftMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CCW);
    rightMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CCW);
    Direction = "SOUTH EAST";
  }
  else if (mapx == 0 and mapy == -1)
  {
    //South
    leftMotors.changeDuty(MOTOR_CH_BOTH, maxDuty);
    rightMotors.changeDuty(MOTOR_CH_BOTH, maxDuty);
    leftMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CCW);
    rightMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CCW);
    Direction = "SOUTH";
  }
  else if (mapx == -1 and mapy == -1)
  {
    //South West
    leftMotors.changeDuty(MOTOR_CH_BOTH, maxTurnDuty);
    rightMotors.changeDuty(MOTOR_CH_BOTH, maxDuty);
    leftMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CCW);
    rightMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CCW);
    Direction = "SOUTH WEST";
  }
  else if (mapx == -1 and mapy == 0)
  {
    //West
    leftMotors.changeDuty(MOTOR_CH_BOTH, maxRotationDuty);
    rightMotors.changeDuty(MOTOR_CH_BOTH, maxRotationDuty);
    leftMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CCW);
    rightMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CW);
    Direction = "WEST";
  }
  else if (mapx == -1 and mapy == 1)
  {
    //North West
    leftMotors.changeDuty(MOTOR_CH_BOTH, maxTurnDuty);
    rightMotors.changeDuty(MOTOR_CH_BOTH, maxDuty);
    leftMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CW);
    rightMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_CW);
    Direction = "NORTH WEST";
  }
  else
  {
    //Stop..
    leftMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_STOP);
    rightMotors.changeStatus(MOTOR_CH_BOTH, MOTOR_STATUS_STOP);
    Direction = "STOP";
    //compassHeadingWhenStartedLinear = -1;
  }
  // publish direction to topic
  if (Direction != "STOP")
  {
    Log(MQTT_DIRECTION_TOPIC, Direction);
  }
}
