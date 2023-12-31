#include <ezButton.h>
#include <ps5Controller.h>
#include "math.h"
#include "Controller_Commands.h"

// const int motorPinFwd[2] = {25, 26};
// const int reversePin[2] = {16, 17};
// const int motorPinFwd[2] = {5, 12};
// const int reversePin[2] = {13, 23};
const int resetFlashPin = 18;
const int resetESPin = 12;
const int buzzerPin = 19;

const int motorSpeedPin[2] = {14, 26};
const int directionPin[2][2] = {{27, 16}, {17, 25}};

// const int greenLight = 2;
// const int yellowLight = 2;
// const int redLight = 2;

bool revState[2] = {0, 0};
int speed[2] = {0, 0};
bool reset = 0;

ezButton resetButton(resetFlashPin);

//======================================================================

// Interrupt service routine to reset the ESP32
void IRAM_ATTR resetModule() {
  Serial.println("Resetting module...");
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);
    delay(200);
  }
  // esp_restart();
}

void setup() {

  Serial.begin(115200);

  resetButton.setDebounceTime(50);

  // attach the interrupt to the reset button when it is pressed
  // attachInterrupt(digitalPinToInterrupt(resetESPin), resetModule, FALLING);

  // MAC Address Controller Giga 48:18:8D:EC:69:F7
  ps5.begin("48:18:8D:EC:69:F7"); 
  
  Serial.println("Ready.");
  
  // Pin Mode Declaration
  pinMode(buzzerPin, OUTPUT);
  for(int i = 0; i < 2; i++)
  {
    // pinMode(motorPinFwd[i], OUTPUT);
    // pinMode(reversePin[i], OUTPUT);
    pinMode(motorSpeedPin[i], OUTPUT);
    pinMode(directionPin[i][0], OUTPUT);
    pinMode(directionPin[i][1], OUTPUT);
  }
  
  // pinMode(redLight, OUTPUT);
  // pinMode(greenLight, OUTPUT);
  // pinMode(yellowLight, OUTPUT);
}

void loop() {
  resetButton.loop();
  if(resetButton.isPressed() && !reset)
  {
    Serial.println("=========== ESP32 RESET ===========");
    digitalWrite(buzzerPin, HIGH);
    delay(2000);
    digitalWrite(buzzerPin, LOW);
    esp_restart();
    reset = true;
  } 
  else if(!resetButton.isPressed() && reset)
  {
    reset = false;
  }

  // When the PS5 controller is connected, the robot will move
  if (ps5.isConnected()) {
    // if (!ps5.Right() && !ps5.Right() && !ps5.Down() && !ps5.Left() && !ps5.UpLeft() && !ps5.UpRight() && !ps5.DownLeft() && !ps5.DownRight()) {
    //   // leftAnalog.updateValue(ps5.LStickX(), ps5.LStickY());
    //   // rightAnalog.updateValue(ps5.RStickX(), ps5.RStickY());
    // }

    // controllerCommands.updateButtons();
    updateSpeed();
    // updateSpeedManual(speed[0], speed[1]);
    printSpeed();

  }
  else {
    killswitch();

    Serial.println("Not Connected...");

    speed[0] = 1;
    speed[1] = 1;

    delay(100);
  }

  new_motorWrite();
}

void killswitch()
{
  for(int i = 0; i < 2; i++)
  {
    speed[i] = 0;
  }
}

void updateSpeed()
{
  for(int i = 0; i < 2; i++)
  {
    speed[i] = ps5.LStickY() * 1.5 + 0.25 * ps5.R2() - 0,25 * ps5.L2();
    if(i % 2 == 0)
      speed[i] += ps5.RStickX() * 1.5 + ps5.Right() * 255 - ps5.Left() * 255 + ps5.Up() * 255 - ps5.Down() * 255;
    else if(i % 2 == 1)
      speed[i] -= ps5.RStickX() * 1.5 + ps5.Right() * 255 - ps5.Left() * 255 - ps5.Up() * 255 + ps5.Down() * 255;

    if(speed[i] > 255)
      speed[i] = 255;
    else if(speed[i] < -255)
      speed[i] = -255;
    else if(abs(speed[i]) < 15)
      speed[i] = 0;

    if(speed[i] < 0){
      revState[i] = HIGH;
    }
    else{
      revState[i] = LOW;
    }
    digitalWrite(reversePin[i], revState[i]);
    analogWrite(motorPinFwd[i], abs(speed[i]));
  }
}

void updateSpeedManual(int &speed1, int &speed2)
{
  speed1 = ps5.LStickY() * 1.5 + 0.25 * ps5.R2() - 0,25 * ps5.L2();
  speed2 = ps5.LStickY() * 1.5 + 0.25 * ps5.R2() - 0,25 * ps5.L2();

  speed1 -= ps5.RStickX() * 0.5 - ps5.Right() * 255 + ps5.Left() * 255 - ps5.Up() * 255 + ps5.Down() * 255;
  speed2 += ps5.RStickX() * 0.5 - ps5.Right() * 255 + ps5.Left() * 255 + ps5.Up() * 255 - ps5.Down() * 255;

  if(speed1 > 255)
    speed1 = 255;
  else if(speed1 < -255)
    speed1 = -255;
  else if(abs(speed1) < 15)
    speed1 = 0;

  if(speed2 > 255)
    speed2 = 255;
  else if(speed2 < -255)
    speed2 = -255;
  else if(abs(speed2) < 15)
    speed2 = 0;
}

unsigned long int printSpeedTime;
void printSpeed()
{
  if(millis() - printSpeedTime >= 1)
  {
    Serial.printf("%d\tSpeed1: %d\tSpeed2: %d\n", millis(), speed[0], speed[1]);
    if(ps5.Cross())
    {
        digitalWrite(buzzerPin, HIGH);
        Serial.println("HONK");
    } else{
        digitalWrite(buzzerPin, LOW);
    }
    

    printSpeedTime = millis();
  }
}

// void motorWrite()
// {
//   for(int i = 0; i < 2; i++)
//   {
//     if(speed[i] < 0){
//       revState[i] = HIGH;
//     }
//     else{
//       revState[i] = LOW;
//     }
//     digitalWrite(reversePin[i], revState[i]);
//     analogWrite(motorPinFwd[i], abs(speed[i]));
//   }
// }

void new_motorWrite()
{
  for(int i = 0; i < 2; i++)
  {
    if(speed[i] < 0){
      digitalWrite(directionPin[i][0], HIGH);
      digitalWrite(directionPin[i][1], LOW);
    }
    else{
      digitalWrite(directionPin[i][0], LOW);
      digitalWrite(directionPin[i][1], HIGH);
    }
    // digitalWrite(reversePin[i], revState[i]);
    analogWrite(motorSpeedPin[i], abs(speed[i]));
  }
}