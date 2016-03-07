/*
 * MOS Medium Demo
 *
 * Demo with 4 tasks:
 *  - Led13Task blinks with LED 13
 *  - Led2Task blinks 10 times with LED 2 and then suspends
 *  - ResumeTask resumes Led2Task after 5 seconds
 *  - DemoTask outputs some data cyclically on the Serial Monitor
 *
 * This example code is in the public domain.
 *
 * Created 2016-03-07 by Joachim Stolberg
 */

#include <MOS.h>

#define NUM_TASKS   4     // number of tasks

// Task control blocks for NUM_TASKS tasks
MOS_TCB_t aTaskTcb[NUM_TASKS];

// DemoTask Variables
typedef struct{
  int cnt;
}DemoTaskVar_t;

DemoTaskVar_t DemoTaskVar;

// Led2Task Variables
typedef struct{
  int cnt;
}Led2TaskVar_t;

Led2TaskVar_t Led2TaskVar;


void setup() {
  pinMode(13, OUTPUT);
  pinMode(2, OUTPUT);
  Serial.begin(115600);
  // Initialize the Task Control Block structures
  memset(aTaskTcb, 0, sizeof(aTaskTcb));
}


void Led13Task(MOS_TCB_t *p_tcb, void *p_var) {
  (void)p_var;                // to prevent compiler warnings
  MOS_Continue(p_tcb);        // continue at previous interrupted position

  while(1) {
    digitalWrite(13, HIGH);
    MOS_Delay(p_tcb, 200);   // interrupt for the given time

    digitalWrite(13, LOW);
    MOS_Delay(p_tcb, 200);   // interrupt for the given time
  }
}

void Led2Task(MOS_TCB_t *p_tcb, Led2TaskVar_t *p_var) {
  MOS_Continue(p_tcb);        // continue at previous interrupted position

  while(1) {
    for(p_var->cnt=0; p_var->cnt<10; p_var->cnt++) {
      digitalWrite(2, HIGH);
      MOS_Delay(p_tcb, 100);   // interrupt for the given time

      digitalWrite(2, LOW);
      MOS_Delay(p_tcb, 100);   // interrupt for the given time
    }
    MOS_Suspend(p_tcb);        // suspend task
  }
}

void ResumeTask(MOS_TCB_t *p_tcb, void *p_var) {
  (void)p_var;                // to prevent compiler warnings
  MOS_Continue(p_tcb);        // continue at previous interrupted position

  while(1) {
    MOS_Delay(p_tcb, 5000);
    MOS_Resume(&aTaskTcb[1]); // resume Led2Task
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
  // Execute the tasks according to their priority (high priority first)
  MOS_Call(Led13Task,   &aTaskTcb[0], time, NULL);
  MOS_Call(Led2Task,    &aTaskTcb[1], time, &Led2TaskVar);
  MOS_Call(ResumeTask,  &aTaskTcb[2], time, NULL);
  MOS_Call(DemoTask,    &aTaskTcb[3], time, &DemoTaskVar);
}
