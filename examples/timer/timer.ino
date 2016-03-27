/*
 * MOS Timer Demo
 *
 * Demo with 2 tasks:
 *  - LedTask blinks with LED 13
 *  - TimerTask outputs the timer ticks in seconds
 *
 * This example code is in the public domain.
 *
 * Created 2016-03-27 by Joachim Stolberg
 */

#include <MOS.h>

uint32_t ticks = 0;


void LedTask(PTCB ptcb) 
{
  MOS_Continue(ptcb);         // Continue at previous suspended program position

  while(1) 
  {
    digitalWrite(13, HIGH);
    MOS_Delay(ptcb, 500);     // Suspend task for the given time

    digitalWrite(13, LOW);
    MOS_Delay(ptcb, 500);     // Suspend task for the given time
  }
}


void TimerTask(PTCB ptcb) 
{
  MOS_Continue(ptcb);           // Continue at previous suspended program position

  while(1) 
  {
    Serial.print(ticks / 60);   // minutes
    Serial.print(':');
    if((ticks % 60) < 10)       // leading '0'
    {
      Serial.print('0');
    }
    Serial.println(ticks % 60); // seconds
    ticks++;
    MOS_DelaySec(ptcb, 1);      // Suspend task for the given time
  }
}


void setup() 
{
  pinMode(13, OUTPUT);
  Serial.begin(115600);
  Serial.println(F("### MOS Timer Demo ###"));
  Serial.println(F("Ouputs the current CPU time in minutes:seconds (approximately)"));
  Serial.println("");
}


void loop() 
{
  MOS_Call(LedTask);
  MOS_Call(TimerTask);
}


