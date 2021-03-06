/*
 * MOS Small Demo
 *
 * Demo with 2 tasks:
 *  - LedTask blinks with LED 13
 *  - DemoTask outputs some data cyclically on the Serial Monitor
 *
 * Copyright (c) 2016 Joachim Stolberg. All rights reserved.
 *
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
 * along with Arduino-MOS.  If not, see <http://www.gnu.org/licenses/>.
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
  Serial.begin(115200);
  Serial.println(F("### MOS Small Demo ###"));
  Serial.println("");
}


void loop() 
{
  MOS_Call(LedTask);
  MOS_Call(DemoTask);
}


