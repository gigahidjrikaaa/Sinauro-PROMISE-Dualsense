#ifndef CONTROLLER_COMMANDS_H
#define CONTROLLER_COMMANDS_H

class ControllerCommands{
    unsigned long pushTime = 0;
    bool secondLoop = false;

    public:
        void updateButtons()
        {
            if (ps5.Right())
            {
                
            }
            if (ps5.Down())
            {
                
            }
            if (ps5.Up())
            {
                
            }
            if (ps5.Left())
            {
                
            }

            if (ps5.UpRight()) 
            {
                // leftAnalog.updateValue(128, 128);
            }
            if (ps5.DownRight())
            {
                // leftAnalog.updateValue(128, -128);
            }
            if (ps5.UpLeft())
            {
                // leftAnalog.updateValue(-128, 128);
            }
            if (ps5.DownLeft())
            {
                // leftAnalog.updateValue(-128, -128);
            }

            if (ps5.Square())
            {
                
            }
            
            if (ps5.Cross())
            {
                // digitalWrite(buzzerPin, HIGH);
            } else{
                // digitalWrite(buzzerPin, LOW);
            }

            if (ps5.Circle()) 
            {
                
            } 

            if (ps5.Triangle())
            {
                
            }

            if (ps5.L1()) 
            {
                
            }
            if (ps5.R1()) 
            {

            }

            if (ps5.Share()) 
            {

            }
            if (ps5.Options())
            {
                
            }

            if (ps5.L3()) 
            {
                
            }

            if (ps5.R3()) 
            {
                
            }

            if (ps5.PSButton()) 
            {
                
            }
            if (ps5.Touchpad()) 
            {
                // Killswitch the robot. To turn off, restart the robot.
                // diffDrive.killswitch();
                // while(true);
            }

            if (ps5.L2() >= 128) {
                // Serial.printf("L2 button at %d\n", ps5.L2Value());
            }
            if (ps5.R2() >= 128) {
                // Serial.printf("R2 button at %d\n", ps5.R2Value());
            }

            // if no buttons are pressed
            if(!ps5.Right() && !ps5.Right() && !ps5.Down() && !ps5.Left() && !ps5.UpLeft() && !ps5.UpRight() && !ps5.DownLeft() && !ps5.DownRight() && !ps5.Square() && !ps5.Cross() && !ps5.Circle() && !ps5.Triangle() && !ps5.L1() && !ps5.R1() && !ps5.Share() && !ps5.Options() && !ps5.L3() && !ps5.R3() && !ps5.PSButton() && !ps5.Touchpad() && !ps5.L2() && !ps5.R2())
            {
                secondLoop = false;
                pushTime = millis();
            }
        }
};

ControllerCommands controllerCommands;

#endif