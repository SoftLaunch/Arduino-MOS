# Arduino-MOS
[Arduino-MOS](https://github.com/joe7575/Arduino-MOS), a ultra lightweight cooperative multitasking schema for Arduino devices. Tested on Arduino UNO, DUE, ATtiny85, and ESP8266. 


## History
- 2016-03-07  V0.1  Initial Version
- 2016-03-16  V0.2  Simplified Version
- 2016-03-17  V0.3  Medium demo added


## Documentation
**Yet another lightweight multitasking kernel for Arduino ...  :-)**

But MOS is different. MOS stands for Macro based Operating System. 
It consists of only a view macros and thus needs per se **zero memory** (Flash and RAM)!
But it allows multitasking with blocking, cooperative tasks in pure C (GCC is required).
Of course you could do the same with state machines but with MOS your code looks nicer and is easier to understand.


### API Functions

Function | Description
--------|------------
MOS_Break(tcb) | Interrupt task execution for one loop. If no other task is in the state 'READY', the task will be reactivated immediately
MOS_Continue(tcb) | Continue the task execution at the previous interrupted position
MOS_Delay(tcb, time) | Give up for the given amount of milliseconds (1..2^31)
MOS_Call(task) | If the task is not waiting (via MOS_Delay), the task will be called


## Example

I have added the sketch "small" to show the usage of MOS.
You can find these in the examples folder. Here is the "small" example, blinking with the LED:

```
#include <MOS.h>

int cnt = 0;

void setup() 
{
  pinMode(13, OUTPUT);
  Serial.begin(115600);
}


void LedTask(PTCB ptcb) 
{
  MOS_Continue(ptcb);        // continue at previous interrupted position

  while(1) 
  {
    digitalWrite(13, HIGH);
    MOS_Delay(ptcb, 100);   // interrupt for the given time

    digitalWrite(13, LOW);
    MOS_Delay(ptcb, 100);   // interrupt for the given time
  }
}


void DemoTask(PTCB ptcb) 
{
  MOS_Continue(ptcb);        // continue at previous interrupted position

  while(1) 
  {
    Serial.print("DemoTask ");
    Serial.println(cnt);
    cnt++;
    MOS_Delay(ptcb, 500);   // interrupt for the given time
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
* Start each task with ```MOS_Continue(ptcb)```
* Use ```MOS_Break(ptcb)``` and ```MOS_Delay(ptcb, time)``` to interrupt your task
  

## Questions/Feedback
Questions about this project should be posted to joe.stolberg(at)gmx(dot)de







