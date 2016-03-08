# Arduino-MOS
[Arduino-MOS](https://github.com/joe7575/Arduino-MOS), a ultra lightweight cooperative multitasking schema for Arduino devices. Tested on Arduino UNO, DUE and ESP8266. 


## History
2016-03-07  V0.1  Initial Version 


## Documentation
**Yet another lightweight multitasking kernel for Arduino ...  :-)**

But MOS is different. MOS stands for Macro based Operating System. 
It consists of only a view macros and thus needs per se **zero memory** (Flash and RAM)!
But it allows multitasking with blocking, cooperative tasks in pure C (GCC is required).
Of course you could do the same with state machines but with MOS your code looks nicer and is easier to understand.


### API Functions

Function | Description
--------|------------
MOS_Break(tcb) | Give up the CPU for a task with higher priority. If no other task is in the state 'READY', the task will be reactivated immediately
MOS_Continue(tcb) | Continue the task execution at the previous interrupted position
MOS_Delay(tcb, time) | Give up for the given amount of milliseconds
MOS_Suspend(tcb) | Suspend the task. Only a MOS_Resume will activate the task again
MOS_Resume(tcb) | Reactivate a suspended task referenced by the given 'tcb'
MOS_Reset(tcb) | Restart a task at the beginning
MOS_Call(task, tcb, time, obj) | If the task is in the state 'READY' (not suspended and not in waiting state), call the given task and then return from loop()

* 'tcb' is the reference to the TaskControlBlock of the related task.

## Example

I have added two sketches "small" and "medium" to show the usage of MOS.
You can find these in the examples folder. Here is the "small" example, blinking with the LED:

```
#include <MOS.h>

// Task control blocks
MOS_TCB_t LedTaskTcb = {NULL, 0, 0};
MOS_TCB_t DemoTaskTcb = {NULL, 0, 0};

// DemoTask Variables
typedef struct{
  int cnt;
}DemoTaskVar_t;

DemoTaskVar_t DemoTaskVar;


void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(115600);
}


void LedTask(MOS_TCB_t *p_tcb, void *p_var) {
  (void)p_var;                // to prevent compiler warnings
  MOS_Continue(p_tcb);        // continue at previous interrupted position

  while(1) {
    digitalWrite(13, HIGH);
    MOS_Delay(p_tcb, 100);   // interrupt for the given time

    digitalWrite(13, LOW);
    MOS_Delay(p_tcb, 100);   // interrupt for the given time
  }
}


void DemoTask(MOS_TCB_t *p_tcb, DemoTaskVar_t *p_var) {
  MOS_Continue(p_tcb);        // continue at previous interrupted position

  p_var->cnt = 0;             // Reset task data once
  while(1) {
    Serial.print("DemoTask ");
    Serial.println(p_var->cnt);
    p_var->cnt++;
    MOS_Delay(p_tcb, 500);   // interrupt for the given time
  }
}


void loop() {
  uint32_t time = millis();
  MOS_Call(LedTask,  &LedTaskTcb,  time, NULL);
  MOS_Call(DemoTask, &DemoTaskTcb, time, &DemoTaskVar);
}
```

## Hardware
There are no limitation, as far as you use the GCC compiler. So you can use any Arduino device. 

## Installation
Download the ZIP file and extract the sources into your Arduino 'libraries' folder.

## Do's and Don'ts
To use the MOS macros successful, some rules have to be fulfilled:
* Implement each task as endless-loop ```while(1) {...}```
* Start each task with ```MOS_Continue()```
* Use ```MOS_Break()``` and ```MOS_Delay()``` to interrupt your task
* Don't use local variables inside your tasks because:
  - the variable get destroyed every time the task is interrupted
  - the variable changes the stack load and this could lead to unpredictable behaviour
* Take care that your code will be interrupted regularly, especially inside loops, so that other tasks with 
  potential higher priority get a chance to be executed.
* Bring you task calls via ```MOS_Call()``` inside the loop() function into the right order
  (tasks with higher priority first).
* The macro MOS_Call() will return from the loop() function each time a task call was successful. So do not 
  place any code behind the MOS_Call() because this will normally not be executed.
  
### Some examples

Template for a task:
```
void DemoTask(MOS_TCB_t *p_tcb, void *p_var) {
  MOS_Continue(p_tcb);        // continue at previous interrupted position

  while(1) {
    ...
    MOS_Delay(p_tcb, msec);   // interrupt for the given time
    ...
  }
}
```  

Some do's and don'ts:
```
  task(...){
    int i;                              <== not allowed !!!
    static int k;                       <== OK
    
    MOS_Continue(...);                  <== has to be placed here
    
    for (int j = 0; j <100; j++){       <== 'j' is not allowed !!!
      ...
      MOS_Delay(...);                  
    }
    ...
  }
```  

How the loop() function should look like:
```
loop() {
  ....
  myLib.loop();                         <== OK
  
  MOS_Call(HighPrioTask,  ...);         <== OK
  MOS_Call(LowPrioTask, ...);           <== OK
  MOS_Call(AnotherHighPrioTask, ...);   <== wrong order !!!

  anotherLib.loop();                    <== wrong position !!!
}
```

## Questions/Feedback
Questions about this project should be posted to joe.stolberg(at)gmx(dot)de







