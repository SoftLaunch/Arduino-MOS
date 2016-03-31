# Arduino-MOS
[Arduino-MOS](https://github.com/joe7575/Arduino-MOS), a ultra lightweight cooperative multitasking scheduler for Arduino devices. Tested on Arduino UNO, DUE, ATtiny85, and ESP8266. 


## History
- 2016-03-07  V0.1  Initial Version
- 2016-03-16  V0.2  Simplified Version
- 2016-03-17  V0.3  Medium demo added
- 2016-03-26  V0.4  MOS_WaitFor() / MOS_Signal() added, button demo added
- 2016-03-27  V0.5  MOS_WaitTimedFor() added, timer demo added
- 2016-03-28  V0.6  MOS_ResetTask() added
- 2016-03-31  V0.7  MOS_WaitForCond() / MOS_WaitTimedForCond() added, documentation improved


## Documentation
**Yet another lightweight multitasking kernel for Arduino ...  :-)**

But MOS is different. MOS stands for Macro based Operating System. 
It consists of only a few macros and can therefore be used on all Arduino devices.
It allows multitasking with blocking, cooperative tasks, but does not require additional memory.
Of course you could do the same with state machines but with MOS your code looks nicer and is easier to understand.


### API Functions

 - [MOS_Break(tcb)](https://github.com/joe7575/Arduino-MOS/blob/master/mos_api_docu.md#mos_break)
 - [MOS_Continue(tcb)](mos_api_docu.md#mos_continue)
 - [MOS_Delay(tcb, msec)](https://github.com/joe7575/Arduino-MOS/blob/master/mos_api_docu.md#mos_delay)
 - [MOS_DelaySec(tcb, sec)](https://github.com/joe7575/Arduino-MOS/blob/master/mos_api_docu.md#mos_delaysec)
 - [MOS_WaitFor(tcb, flag)](https://github.com/joe7575/Arduino-MOS/blob/master/mos_api_docu.md#mos_waitfor)
 - [MOS_WaitTimedFor(tcb, flag, msec)](https://github.com/joe7575/Arduino-MOS/blob/master/mos_api_docu.md#mos_waittimedfor)
 - [MOS_WaitForCond(tcb cond)](https://github.com/joe7575/Arduino-MOS/blob/master/mos_api_docu.md#mos_waitforcond)
 - [MOS_WaitTimedForCond(tcb, cond, time)](https://github.com/joe7575/Arduino-MOS/blob/master/mos_api_docu.md#mos_waittimedforcond)
 - [MOS_Signal(flag)](https://github.com/joe7575/Arduino-MOS/blob/master/mos_api_docu.md#mos_signal)
 - [MOS_ResetTask(tcb)](https://github.com/joe7575/Arduino-MOS/blob/master/mos_api_docu.md#mos_resettask)
 - [MOS_Call(task)](https://github.com/joe7575/Arduino-MOS/blob/master/mos_api_docu.md#mos_call)


## Example

Several sketches to demonstrate the features of MOS are available.
You can find these in the examples folder. Here is the "small" example, blinking with the LED:

```C++
#include <MOS.h>

int cnt = 0;

void setup() 
{
  pinMode(13, OUTPUT);
  Serial.begin(115600);
}


void LedTask(PTCB tcb) 
{
  MOS_Continue(tcb);          // continue at previous suspended program position

  while(1) 
  {
    digitalWrite(13, HIGH);
    MOS_Delay(tcb, 100);      // Suspend task for the given time

    digitalWrite(13, LOW);
    MOS_Delay(tcb, 100);      // Suspend task for the given time
  }
}


void DemoTask(PTCB tcb) 
{
  MOS_Continue(tcb);          // continue at previous suspended program position

  while(1) 
  {
    Serial.print("DemoTask ");
    Serial.println(cnt);
    cnt++;
    MOS_Delay(tcb, 500);      // Suspend task for the given time
  }
}


void loop() 
{
  MOS_Call(LedTask);
  MOS_Call(DemoTask);
}
```

## Hardware
There are no limitation, as far as you use the GCC compiler. So you can use any Arduino device. 

## Installation
Download the ZIP file and install it with the Library Manager.
[How to install Additional Arduino Libraries](https://www.arduino.cc/en/Guide/Libraries)


## Hints
To use the MOS macros successful, some rules have to be fulfilled:
* Implement each task as endless-loop ```while(1) {...}```
* Start each task with ```MOS_Continue(tcb)```
* Use ```MOS_Break(tcb)```, ```MOS_WaitFor(tcb, flag)```, and ```MOS_Delay(tcb, time)``` to suspend your task
* Local variables lose their value with each call of MOS_Break/MOS_Delay/MOS_WaitFor. Therefore, use static or global variables inside tasks

## Questions/Feedback
Questions about this project should be posted to joe.stolberg(at)gmx(dot)de

## License
Arduino-MOS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino-MOS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Arduino-MOS.  If not, see <http://www.gnu.org/licenses/>.






