/*
 * MOS Button Demo
 *
 * Demo with 2 tasks:
 *  - the LedTask blinks with LED 13 and suspends after 4 seconds.
 *  - the ButtonTask cyclically reads a digital input to detect a "pressed button".
 *    If pressed, the LedTask will be reactivated.
 *
 * The demo shows the use of the functions MOS_WaitFor() / MOS_Signal()
 * 
 * To simulate a button, use two Dupont Wire (jumper cable), 
 * one for pin 2 and one for GND.
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


#define BUTTON_PIN            2       // Used pin for the button
#define NUM_SAMPLES           3       // number of necessary debounce samples (1..4)


bool LedTaskSignal;                   // Flag to control the LedTask


/*
 * Toggles LED 13 for 4 seconds and suspends
 */
void LedTask(PTCB tcb) 
{
  static uint16_t idx;
  
  MOS_Continue(tcb);                    // Continue at previous suspended position      
  
  while(1) 
  {
    for(idx = 0; idx < 10; idx++)
    {
      digitalWrite(13, HIGH);
      MOS_Delay(tcb, 200);              // Suspend task for the given time      

      digitalWrite(13, LOW);
      MOS_Delay(tcb, 200);              // Suspend task for the given time
    }
                                        // Suspend task (maximum 5 sec) for an external event
    MOS_WaitTimedFor(tcb, LedTaskSignal, 5000);
    if(!LedTaskSignal)                  // Signal not occurred?
    {
      Serial.println(F("Timout occurred"));
    }
  }
}

/*
 * Debounce filter function for a digital input pin.
 * For some clarification to debounce problems, see: 
 * https://www.arduino.cc/en/Tutorial/Debounce
 */
bool debounce_filter()
{
  static uint8_t BitPattern = 0;

  // read and negate the input pin (0 = active)
  bool value = !digitalRead(BUTTON_PIN);

  // "Add" to the value to the Filter bit-field
  BitPattern = (BitPattern << 1) | value;

  // check if the necessary bit pattern is available
  if(BitPattern == ((1U << NUM_SAMPLES) - 1))
  {
    return true;
  }
  return false;
}

/*
 * Read button pin cyclically and resume LedTask if pressed
 */
void ButtonTask(PTCB tcb)
{
  static uint16_t counter = 0;
  
  MOS_Continue(tcb);                    // Continue at previous suspended position       

  pinMode(BUTTON_PIN, INPUT);           // switch to input
  digitalWrite(BUTTON_PIN, HIGH);       // activate pull-up resistor

  while(1) 
  {
    if(debounce_filter())
    {
      counter++;
      Serial.print(F("Button presssed "));
      Serial.print(counter);
      Serial.println(F(" times"));
      
      MOS_Signal(LedTaskSignal);        // resume LedTask
      MOS_Delay(tcb, 200);              // Input lock time
    }
    else
    {
      MOS_Delay(tcb, 10);                // normal poll cycle time
    }
  }
}


void setup() 
{
  pinMode(13, OUTPUT);
  Serial.begin(115600);
  Serial.println(F("### MOS Button Demo ###"));
  Serial.println("");
}


void loop() 
{
  MOS_Call(LedTask);
  MOS_Call(ButtonTask);
}

