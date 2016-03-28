# Arduino-MOS
[Arduino-MOS](https://github.com/joe7575/Arduino-MOS), a ultra lightweight cooperative multitasking scheduler for Arduino devices. Tested on Arduino UNO, DUE, ATtiny85, and ESP8266. 


## History
- 2016-03-07  V0.1  Initial Version
- 2016-03-16  V0.2  Simplified Version
- 2016-03-17  V0.3  Medium demo added
- 2016-03-26  V0.4  MOS_WaitFor() / MOS_Signal() added, button demo added
- 2016-03-27  V0.5  MOS_WaitTimedFor() added, timer demo added
- 2016-03-28  V0.6  MOS_ResetTask() added


## Documentation
**Yet another lightweight multitasking kernel for Arduino ...  :-)**

But MOS is different. MOS stands for Macro based Operating System. 
It consists of only a few macros and can therefore be used on all Arduino devices.
It allows multitasking with blocking, cooperative tasks, but does not require additional memory.
Of course you could do the same with state machines but with MOS your code looks nicer and is easier to understand.


### API Functions

Function | Description
--------|------------
MOS_Break(tcb) | Suspend task execution for one cycle. The task will be resumed with the next call of the loop function
MOS_Continue(tcb) | Continue the task execution at the previous suspended program position
MOS_Delay(tcb, msec) | Suspend task for the given amount of milliseconds (1..30000)
MOS_DelaySec(tcb, sec) | Suspend task for the given amount of seconds (1..30000)
MOS_WaitFor(tcb, flag) | Suspend task until the task is resumed by means of MOS_Signal()
MOS_WaitTimedFor(tcb, flag, msec) | Suspend task until the task is resumed by means of MOS_Signal() or by means of the given timeout in milliseconds (1..30000)
MOS_Signal(flag) | Resume the suspended task waiting on the given flag
MOS_ResetTask(tcb) | Reset the calling task so that it starts from the beginning
MOS_Call(task) | Execute the given task


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
Download and extract the ZIP file, rename the folder "Arduino-MOS-master" into 'MOS' and copy the 'MOS' folder 
into the Arduino 'libraries' folder.

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
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.






