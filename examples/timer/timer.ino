/*
 * MOS Timer Demo
 *
 * This file is part of Arduino-MOS.
 *
 * Arduino-MOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Arduino-MOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 
 * Demo with 2 tasks:
 *  - LedTask blinks with LED 13
 *  - TimerTask outputs the timer ticks in seconds
 *
 * Created 2016-03-27 by Joachim Stolberg
 */

#include <MOS.h>

uint32_t ticks = 0;


void LedTask(PTCB tcb) 
{
  MOS_Continue(tcb);            // Continue at previous suspended program position

  while(1) 
  {
    digitalWrite(13, HIGH);
    MOS_Delay(tcb, 500);        // Suspend task for the given time

    digitalWrite(13, LOW);
    MOS_Delay(tcb, 500);        // Suspend task for the given time
  }
}


void TimerTask(PTCB tcb) 
{
  MOS_Continue(tcb);            // Continue at previous suspended program position

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
    MOS_DelaySec(tcb, 1);       // Suspend task for the given time
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


