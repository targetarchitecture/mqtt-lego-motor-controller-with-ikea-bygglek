#include <Arduino.h>
/*
 Lego Powerfunctions car model controlled over bluetooth
 Arduino Uno controls motors and servo
 Remote controlled with android app developed with MIT App Inventor 2
 
 Circuit:
 * Serial communication   (uses Uno pin 0,1)    Bluetooth-module is attached (has to be detached when loading program over USB)
 * L293D with motor       (uses Uno digital pins 2,5,9)
 * L293D with servo       (uses Uno digital pins 6,8,3)
 
 
*/

// Servo control digital output pins defined as global constants (Servo steering with 1 Lego servo):
const int controlPin3A = 16;                  // L293D driver input 3A on pin no 10 connected to Arduino digital output pin 6
const int controlPin4A = 17;                  // L293D driver input 4A on pin no 15 connected to Arduino digital output pin 8 
const int servoENablePin = 23;                // L293D ENable(3,4) input on pin no 9 connected to Arduino digital output pin 3

// Servo control global variables:
int steering = 0;                            // Servo position 0..255
int steeringDirection = 0;                   // Left (0) and Right (1)



void setup() 
{
   Serial.begin(9600);                       // initialize serial communication
   Serial.setTimeout(1000);                  // 1000 ms time out
   // Declare digital output pins:
   pinMode(controlPin3A, OUTPUT);      // 3A
   pinMode(controlPin4A, OUTPUT);      // 4A
   pinMode(servoENablePin, OUTPUT);    // EN3,4

   digitalWrite(servoENablePin, LOW);  // steering centered
}


void loop() 
{
  if (steeringDirection == 0)            //Left
    {
       digitalWrite(controlPin3A, HIGH);
       digitalWrite(controlPin4A, LOW);
    }
  else                                   //Right
    {
       digitalWrite(controlPin3A, LOW);
       digitalWrite(controlPin4A, HIGH);
    } 
  analogWrite(servoENablePin, steering); //Servo position
}

/*
L293 logic:              
               EN3,4   3A    4A
               H       H     L    Servo turns left  (steeringDirection == 0)
               H       L     H    Servo turns right (steeringDirection == 1)

Servo position:PWM signal on EN3,4 (490 Hz; digital output value 0..255 for position; 0 is straight ahead)
*/
