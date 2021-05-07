#include <LiquidCrystal.h>
#include <Servo.h>
#include <TimerOne.h>
#include "EncoderMonitor.h" //Include EncoderMonitor.h
#include "Arduino.h"
Servo myservo;

LiquidCrystal LcdDriver(A5,A4,5,6,7,8);

unsigned long Timer; //Create Timer
 unsigned long Timer2;
 unsigned long Timer3;
long seconds;
long minutes;
long hours;
long timeCombined;
double lowVoltageAngle;
int servoPin = 10;
int interruptPin1 = 2;
int interruptPin2 = 3;
int sensorPin = A0;

int sensor1 = 0;

int sensorPin2 = A1;

int sensor2 = 0;

int AnalogOutputPin;
int turnDirection;
int angle;
int bestAngle;

bool finishedSearching;



float voltage;
float highestVoltage;

float threshold;





void setup(){
  angle = 0;
  pinMode(sensorPin,INPUT);
  pinMode(sensorPin2,INPUT);
  
  myservo.attach(servoPin);
  myservo.write(angle);

  LcdDriver.begin(16,2);
  Timer = millis();

  finishedSearching = false;
  turnDirection = 0;
  highestVoltage = 0;
  bestAngle = 0;
  AnalogOutputPin = 10;
  threshold = 1;
   EncoderInitialize();
  Timer1.initialize(500);
  Timer1.pwm(AnalogOutputPin,512);
  Timer1.setPwmDuty(AnalogOutputPin,voltage);
}


//Sun is at 90 degrees at noon, 0 degrees at 
void loop(){
 
  if (encoderPosition >= 60){
  encoderPosition = 0;
  minutes = minutes + 1;
  
 }

 
 if (encoderPosition < 0){
  minutes -= 1;
  encoderPosition = 59;
  
 }
 if (minutes < 0 ){
  hours -= 1;
  minutes = 59;
  encoderPosition = 59;
 }
 if (minutes >= 60){
  hours = hours + 1;
  minutes = 0;
 }

 if (hours < 0){
  hours = 23;
  minutes = 59;
  encoderPosition = 59;
 }

 if (hours >= 24){
  hours = 0;
  minutes = 0;
  encoderPosition = 0;
 }
  if(millis() - Timer >= 1000/30){
  sensor1 = analogRead(sensorPin);
  sensor2 = analogRead(sensorPin2);

  
    if (finishedSearching == true and voltage >= threshold){ //The solar tracker will point in the direction of the sun if this is true
      if (lowVoltageAngle > 0 or lowVoltageAngle < 180){
            myservo.write(lowVoltageAngle);
 }
     
    }
    voltage = (9*analogRead(sensorPin))/1024;
    
    if (finishedSearching == false) { 
      angle = angle + 1;
      myservo.write(angle);


      if (highestVoltage < voltage){
          highestVoltage = voltage;
          bestAngle = angle;
      }

      if (angle >= 180){
       // myservo.write(bestAngle);
       // finishedSearching = true;
    }

    }

     Timer += 1000/30;
     //LcdDriver.print(voltage); 
  }
  if (millis() - Timer2 >= 1000){ //Increases time
  encoderPosition += 1;
  Timer2 += 1000;
 }

  if(millis() - Timer3 >= 100){ //For every 100 milliseconds
 LcdDriver.clear(); //Clear the display
 LcdDriver.setCursor(0,0); //Set the cursor for lcd
 if (hours <= 9){
  LcdDriver.print("0");
 }
  LcdDriver.print(hours);
 LcdDriver.print(":");
if (minutes <= 9){
  LcdDriver.print("0");
 }
 LcdDriver.print(minutes); //Display encoderPosition on lcd
 LcdDriver.print(":");
 if (encoderPosition <= 9){
  LcdDriver.print("0");
 }
 LcdDriver.print(encoderPosition); //Display encoderPosition on lcd

 
 timeCombined = encoderPosition + (minutes*60)+(hours*3600);
 LcdDriver.setCursor(0,1);
 LcdDriver.print("Angle ");
 LcdDriver.print(lowVoltageAngle);

 lowVoltageAngle = (timeCombined/240.00)-90;

 
 
 
 Timer3 += 100; //Update Timer 
 }


}
