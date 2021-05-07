#include <LiquidCrystal.h> //Library for the LCD
#include <Servo.h> //Servo library for the motor
#include <TimerOne.h> //Basic timre library
#include "EncoderMonitor.h" //Include EncoderMonitor.h
#include "Arduino.h" //Basic arduino language
Servo myservo; //Variable for our servo

LiquidCrystal LcdDriver(A5, A4, 5, 6, 7, 8); //Set the correct pins for the LCD

unsigned long Timer; //Create Timer
unsigned long Timer2; //Timer for adding the seconds to clock
unsigned long Timer3; //Timer for displaying to the LCD
long seconds; //Variable for seconds
long minutes; //Variable for minutes
long hours; //Variable for hours
long timeCombined; //Variable to track the total time
double lowVoltageAngle; //Angle where it does not detect enough voltage
int servoPin = 10; //Pin for the servo
int sensorPin = A0; //Set the pin for sensor 1 to A0
int sensor1 = 0; 
int sensorPin2 = A1; //Set the pin for sensor 2 to A1
int sensor2 = 0;
int turnDirection; //Variable to track which direction it is turning
int angle; //Tracker for the angle


void setup() {
  angle = 0; //Initialize the angle to zero
  pinMode(sensorPin, INPUT); //Set the sensors as inputs
  pinMode(sensorPin2, INPUT);
  myservo.attach(servoPin); //Attach the servo pin 10 to myservo
  myservo.write(angle); //Write the angle of the servo
  LcdDriver.begin(16, 2); //Initialize the LCD
  Timer = millis(); //Initialize the timer
  EncoderInitialize(); //Call the initialize for the encoder header file
}


void loop() {  
  if (encoderPosition >= 60) { //If greater than or equal to 60
    encoderPosition = 0; //Set the seconds to zero
    minutes = minutes + 1; //Increment minutes
  }
  if (encoderPosition < 0) { //If less than zero
    minutes -= 1; //decrement minutes
    encoderPosition = 59; //Set seconds to 59
  }
  if (minutes < 0 ) { //If minutes is less than zero
    hours -= 1; //decrement hours
    minutes = 59; //Set minutes to 59
    encoderPosition = 59; //Set seconds to 59
  }
  if (minutes >= 60) { //If minutes is greater than or equal to 60
    hours = hours + 1; //Increment hours
    minutes = 0; //Set minutes to 0
  }
  if (hours < 0) { //If hours is less than 0
    hours = 23; //Set hours to 23
    minutes = 59; //Set minutes to 59
    encoderPosition = 59; //Set seconds to 59
  }
  if (hours >= 24) { //If hours is greater than or equal to 24
    hours = 0; //Set hours, minutes, and seconds back to zero
    minutes = 0;
    encoderPosition = 0;
  }
  
  if (millis() - Timer >= 1000 / 30) { //Check 30 roughly times per second for light
    sensor1 = analogRead(sensorPin); //Set sensor1
    sensor2 = analogRead(sensorPin2); //Set sensor2
    //If there is not enough light the solar tracker points in the direction of the sun
    if (sensor2 <= 500) { //If sensor 2 is over 500
      if (sensor2 / 2 > sensor1) { //If sensor 2 divided by two is greater than sensor1
        angle--; //decrement angle
        if (angle <= 0) { //If angle is less than or equal to zero
          angle = 0; //Set angle to zero
        }
      } else {
        angle++; //Increment angle
        if (angle >= 180) { //If angle is greater than or equal to 180
          angle = 180; //Set angle to 180
        }
      }
    }
    else{
      angle = lowVoltageAngle; //Otherwise set angle back to lowVoltageAngle
      myservo.write(angle); //Write the angle to the servo motor
    }
    myservo.write(angle); //Write the angle to the servo motor
    Timer += 1000 / 30;
  }


  if (millis() - Timer2 >= 1000) { //Increases time
    encoderPosition += 1; //Increment seconds
    Timer2 += 1000; //Update timer2
  }
  if (millis() - Timer3 >= 100) { //For every 100 milliseconds
    LcdDriver.clear(); //Clear the display
    LcdDriver.setCursor(0, 0); //Set the cursor for lcd
    if (hours <= 9) { //If hours is less than or equal to 9
      LcdDriver.print("0");
    }
    LcdDriver.print(hours); //Print hours to the LCD
    LcdDriver.print(":"); //Print colon
    if (minutes <= 9) { //If minutes is less than or equal to 9
      LcdDriver.print("0"); //Print 0
    }
    LcdDriver.print(minutes); //Display encoderPosition on lcd
    LcdDriver.print(":"); //Print colon
    if (encoderPosition <= 9) { //If seconds is less than or equal to 9
      LcdDriver.print("0"); //Print 0
    }
    LcdDriver.print(encoderPosition); //Display encoderPosition on lcd
    //Gets the angle of the sun based on time
    timeCombined = encoderPosition + (minutes * 60) + (hours * 3600);
    LcdDriver.setCursor(0, 1); //Set the curspr for the LCD   
    lowVoltageAngle = (timeCombined / 240.00) - 90; //Turns the time of hours, minutes, and seconds into a single number and changes it to angle
    LcdDriver.print("Angle: "); //Print out the angles
    LcdDriver.print(abs(angle)); //Print angle to lcd
    myservo.write(abs(angle)); //Write the angle to the servo
    Timer3 += 100; //Update Timer
  }
  
}
