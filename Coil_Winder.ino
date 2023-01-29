/*
 * Coil winder
 * 
 */
#include <Motor.h>

#define DEBUG 1

const int buttonPin = 2;
const int motorPin1 = 3;
const int motorPin2 = 4;
const int pwmPin = 5;
Motor motor(motorPin1, motorPin2, pwmPin);

const int potPin = A7;

const int maxAnalogVal = 1023;

const int debounceDelay = 50; // Delay between button state changes (ms)
bool currentButtonState = HIGH; // Default to button not being pressed (logic high)
bool previousButtonState = HIGH; 
unsigned long lastStateTime = 0; // Holds the time of the last button state change

bool wind = false;

// Function declariations
int AnalogReadToPWM(int,int);

void setup() {
  pinMode(buttonPin,INPUT_PULLUP);
  pinMode(potPin, INPUT);

  #if DEBUG
  Serial.begin(9600);
  #endif
}

void loop() {
  // Check button input and determine to wind the coil or not
  currentButtonState = digitalRead(buttonPin); 
  
  if(currentButtonState != previousButtonState)
  {
    lastStateTime = millis();
  }
  
  if((millis() - lastStateTime) > debounceDelay) // Check if it has been longer than the debounce time between button state changes
    {
      wind = !currentButtonState;
    }

  if (wind) // Wind the coil
  {
    motor.SetSpeed(AnalogReadToPWM(potPin,maxAnalogVal)); 
    motor.DriveMotor('f');
  }
  else
  {
    motor.DriveMotor('s'); // Turn off the winder
  }
  
  previousButtonState = currentButtonState;
}

int AnalogReadToPWM(int readPin, int maxAnalogVal) // Reads an analog pin and converts it to a value for analog write
{
  int readVal = analogRead(readPin); // Gets analog value in the range 0-1023
  int pwmVal = (int)round( ((float)255/(float)maxAnalogVal)*(float)readVal );// Scale the value and round it to an integer

  #if DEBUG
  Serial.print("Analog value: ");
  Serial.print(readVal);
  Serial.print(", PWM value: ");
  Serial.println(pwmVal);
  #endif
  
  return pwmVal; 
}
