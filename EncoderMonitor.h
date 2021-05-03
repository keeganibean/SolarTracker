#ifndef EncoderMonitor_H
#define EncoderMonitor_H //Define the header file

#include "Arduino.h" //Arduino library
volatile int encoderPosition = 0; //Integer position that can change at any time

void EncoderMonitorA(){ //Interrupt service routine A
if(digitalRead(2) == digitalRead(3)){ //If Input A = Input B
encoderPosition = encoderPosition + 5; //Increment encoderPosition
 }
 else{
 encoderPosition = encoderPosition - 5; //Decrement encoderPosition
 }
 }
 void EncoderMonitorB(){ //Interrupt service routine B
 if(digitalRead(2) == digitalRead(3)){ //If Input A = Input B
 encoderPosition = encoderPosition - 5; //Decrement encoderPosition
 }
 else{
 encoderPosition = encoderPosition + 5; //Increment encoderPosition
 }
 }

 void EncoderInitialize(){ //Function to start encoder monitoring
 pinMode(2, INPUT); //Set pin 2 as Input
 pinMode(3, INPUT); //Set pin 3 as Input
 attachInterrupt(digitalPinToInterrupt(2), EncoderMonitorA, CHANGE);
//Send ISR for pin 2
 attachInterrupt(digitalPinToInterrupt(3), EncoderMonitorB, CHANGE);
//Send ISR for pin 3
 }

 #endif
