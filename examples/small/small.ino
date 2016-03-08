/*
 * MOS Small Demo
 *
 * Demo with 2 tasks:
 *  - LedTask blinks with LED 13
 *  - DemoTask outputs some data cyclically on the Serial Monitor
 *
 * This example code is in the public domain.
 *
 * Created 2016-03-07 by Joachim Stolberg
 */

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
