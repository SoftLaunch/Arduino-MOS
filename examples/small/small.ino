/*
 * MOS Small Demo
 *
 * Demo with 2 tasks:
 *  - LedTask blinks with LED 13
 *  - DemoTask outputs some data cyclically on the Serial Monitor
 *
 * This example code is in the public domain.
 *
 * Created 2016-03-16 by Joachim Stolberg
 */

#include <MOS.h>

int cnt = 0;


void LedTask(PTCB tcb) 
{
  MOS_Continue(tcb);            // Continue at previous suspended program position

  while(1) 
  {
    digitalWrite(13, HIGH);
    MOS_Delay(tcb, 100);        // Suspend task for the given time

    digitalWrite(13, LOW);
    MOS_Delay(tcb, 100);        // Suspend task for the given time
  }
}


void DemoTask(PTCB tcb) 
{
  MOS_Continue(tcb);            // Continue at previous suspended program position

  while(1) 
  {
    Serial.print("DemoTask ");
    Serial.println(cnt);
    cnt++;
    MOS_Delay(tcb, 500);        // Suspend task for the given time
  }
}


void setup() 
{
  pinMode(13, OUTPUT);
  Serial.begin(115600);
  Serial.println(F("### MOS Small Demo ###"));
  Serial.println("");
}


void loop() 
{
  MOS_Call(LedTask);
  MOS_Call(DemoTask);
}


