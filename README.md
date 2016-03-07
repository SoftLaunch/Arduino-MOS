# Arduino-MOS
A ultra lightweight cooperative multitasking schema for Arduino devices. Tested on UNO, DUE and ESP8266 devices.

## History
2016-03-07  V0.1  Initial Version 

## Documentation


## Example
I have added two sketches "small" and "medium" to show the usage of MOS.
You can find these in the examples folder. Here is the "small" example blinking with the LED:

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


void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(115600);
}


void LedTask(MOS_TCB_t *p_tcb, void *p_var)
{
  (void)p_var;                // to prevent compiler warnings
  MOS_Continue(p_tcb);        // continue at previous interrupted position

  while(1)
  {
    digitalWrite(13, HIGH);
    MOS_Delay(p_tcb, 100);   // interrupt for the given time

    digitalWrite(13, LOW);
    MOS_Delay(p_tcb, 100);   // interrupt for the given time
  }
}


void DemoTask(MOS_TCB_t *p_tcb, DemoTaskVar_t *p_var)
{
  MOS_Continue(p_tcb);        // continue at previous interrupted position

  p_var->cnt = 0;             // Reset task data once
  while(1)
  {
    Serial.print("DemoTask ");
    Serial.println(p_var->cnt);
    p_var->cnt++;
    MOS_Delay(p_tcb, 500);   // interrupt for the given time
  }
}


void loop()
{
  uint32_t time = millis();
  MOS_Call(LedTask,  &LedTaskTcb,  time, NULL);
  MOS_Call(DemoTask, &DemoTaskTcb, time, &DemoTaskVar);
}
```

## Hardware
There are no limitation, as far as you use the GCC compiler. So you can use any Arduino device. 

## Installation
Download the ZIP file and extract the sources into your Arduino 'libraries' folder.

## Questions/Feedback
Questions about this project should be posted to joe.stolberg(at)gmx(dot)de







