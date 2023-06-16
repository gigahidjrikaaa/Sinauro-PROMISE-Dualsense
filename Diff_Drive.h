#IFNDEF DIFF_DRIVE_H
#DEFINE DIFF_DRIVE_H

#include "Analog_Joystick.h"

class DiffDrive
{
  public:
    int move_speed[2] = {0, 0};
    int rotate_speed[2] = {0, 0};
    int total_speed[2] = {0, 0};
    int LMove = 0;
    int RMove = 0;
    int LRotate = 0;
    int RRotate = 0;
    double LTotalSpeed = 0;
    double RTotalSpeed = 0;
    int motorPinFwd[2], motorPinBwd[2];

    DiffDrive(int leftMotorPin, int rightMotorPin)
    {
      this->leftMotorPin = leftMotorPin;
      this->rightMotorPin = rightMotorPin;
      pinMode(leftMotorPin, OUTPUT);
      pinMode(rightMotorPin, OUTPUT);
    }
    
    void updateValue(Analog leftAnalog, Analog rightAnalog)
    {
      LMove = leftAnalog.getVert();
      RMove = leftAnalog.getVert();
      LRotate = rightAnalog.getHori();
      RRotate = rightAnalog.getHori();
      LTotalSpeed = LMove * MOVE_WEIGHT + LRotate * ROTATE_WEIGHT;
      RTotalSpeed = RMove * MOVE_WEIGHT - RRotate * ROTATE_WEIGHT;
      total_speed[0] = LTotalSpeed;
      total_speed[1] = RTotalSpeed;
    }

    void robotMove()
    {
      if(LTotalSpeed < -225)
        LTotalSpeed = -225;
      else if(LTotalSpeed > 225)
        LTotalSpeed = 225;
      if(RTotalSpeed < -225)
        RTotalSpeed = -225;
      else if(RTotalSpeed > 225)
        RTotalSpeed = 225;

      // Deadzone
    //   if(LTotalSpeed < 10 && LTotalSpeed > -10)
    //     LTotalSpeed = 0;
    //   if(RTotalSpeed < 10 && RTotalSpeed > -10)
    //     RTotalSpeed = 0;

      total_speed[0] = LTotalSpeed;
      total_speed[1] = RTotalSpeed;

      for(int i = 0; i < 2; i++)
      {
        singleMotorWrite(i, total_speed[i]);
      }
    }

    void singleMotorWrite(int index, int value)
    {
      if(value >= 0)
      {
        analogWrite(diffDriveFwd[index], value);
        analogWrite(diffDriveBck[index], 0);
      }
      else
      {
        analogWrite(diffDriveFwd[index], 0);
        analogWrite(diffDriveBck[index], abs(value));
      }
    }

    void killswitch()
    {
      for(int i = 0; i < 2; i++)
      {
        total_speed[i] = 0;
        singleMotorWrite(i, total_speed[i]);
      }
    }

    // Diff Drive RPM Calculation
    int L_RPM = 0, R_RPM = 0;
    double LOutput = 0, ROutput = 0;
    double LInput = 0, RInput = 0;
    int counterL_RPM = 0, counterR_RPM = 0;

    void RPM_Calculation_DiffDrive()
    {
      L_RPM = (float)counterL_RPM*0.5; 
      R_RPM = (float)counterR_RPM*0.5;
      counterL_RPM = 0;
      counterR_RPM = 0;
    }
};

DiffDrive diffDrive;

#ENDIF