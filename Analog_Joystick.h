#ifndef ANALOG_JOYSTICK_H
#define ANALOG_JOYSTICK_H

#include "math.h"

class Analog
{
    
  public:
    const int analogMax = 128;
    int hori = 0, vert = 0;
    int lengths;
    float angle;
    int mappedHori, mappedVert;
    unsigned int lasttime = 0;
    
    int getHori(){return hori;}
    int getVert(){return vert;}
    int getLengths(){return lengths;}
    float getAngle(){return angle;}
    int getMappedHori(){return mappedHori;}
    int getmappedVert(){return mappedVert;}
    
    void updateValue(int8_t horizontal, int8_t vertical)
    {
        hori = horizontal;
        vert = vertical;
        mappedHori = map(hori, -128, 127, -255, 255);
        mappedVert = map(vert, -128, 127, -255, 255);
        lengths = sqrt(mappedHori*mappedHori + mappedVert*mappedVert);
        if(lengths > 255)
        lengths = 255;
        angle = atan2((float)mappedVert, (float)mappedHori);
    }

    float radToDeg(float rad)
    {
        return rad/(2*PI) * 360;
    }
    
    void printInfo()
    {
        if(millis()-lasttime > 2000)
        {
        Serial.println("========================");
        Serial.print("Analog angle: ");
        Serial.println(radToDeg(angle));
        Serial.print("Length: ");
        Serial.println(lengths);
        lasttime = millis();
        }
    }
};

//Declare Global Variable
Analog leftAnalog, rightAnalog;

#endif