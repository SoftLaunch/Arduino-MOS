/*
 * MOS Medium Demo
 *
 * Demo with 3 tasks:
 *  - the LedTask blinks with LED 13 with variable LED on/off ratio (dimming)
 *  - the RatioTask changes the LED dimming ratio
 *  - the ControlTask controls the dimming frequency
 *
 * This example code is in the public domain.
 *
 * Created 2016-03-17 by Joachim Stolberg
 */

#include <MOS.h>

byte ratio = 0;
int  count = 0;
uint16_t delayVal = 20;


void setup() 
{
  pinMode(13, OUTPUT);
  Serial.begin(115600);
}


void LedTask(PTCB ptcb) 
{
  MOS_Continue(ptcb);                   // continue at previous interrupted position      
  
  while(1) 
  {
    digitalWrite(13, HIGH);
    MOS_Delay(ptcb, ratio);             // interrupt task for the given time      

    digitalWrite(13, LOW);
    MOS_Delay(ptcb, 5-ratio);           // interrupt task for the given time  
  }
}


void RatioTask(PTCB ptcb) 
{
  MOS_Continue(ptcb);                   // continue at previous interrupted position       
  
  while(1) 
  {
    for(count=0; count<=5; count++)
    {
      ratio = count;
      MOS_Delay(ptcb, delayVal);        // interrupt task for the given time 
    }
    for(count=5; count>=0; count--)
    {
      ratio = count;
      MOS_Delay(ptcb, delayVal);        // interrupt task for the given time  
    }
  }
}


void ControlTask(PTCB ptcb) 
{
  MOS_Continue(ptcb);                   // continue at previous interrupted position
        
  Serial.println("Input LED dimming frequency (1 - 5):");

  while(1) 
  {
    if(Serial.available())
    {
      char c = Serial.read();
      if((c >= '1') && (c <= '5'))
      {
        delayVal = (c - '0') * 20;
        Serial.print(delayVal);
        Serial.println(" ms");
      }
    }
    MOS_Break(ptcb);                    // interrupt task for one loop cycle
  }
}


void loop() 
{
  MOS_Call(LedTask);
  MOS_Call(RatioTask);
  MOS_Call(ControlTask);
}