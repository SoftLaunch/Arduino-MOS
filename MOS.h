/*
 * MOS - Macro based Operating System
 * A ultra lightweight cooperative multitasking scheduler for Arduino devices.
 *
 * V0.7 - 2016-03-30
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
 *
 * References / Inspired by:
 * [1] "Coroutines in C" by Simon Tatham (2000)
 *     http://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
 * [2] "Coroutinen in C" by Joachim Stolberg (2004)
 *     http://web.archive.org/web/20070704161820/http://www.jstolberg.de/...
 *                              JoesCms/data/Programming/CoroutinenInC.php
 * [3] Labels as Values - Using the GNU Compiler Collection (GCC)
 *     https://gcc.gnu.org/onlinedocs/gcc/Labels-as-Values.html
  */

#ifndef MOS_H
#define MOS_H

#include <inttypes.h>


// MOS internal macros
#define MOS_MERGE_(a,b)       a##b
#define MOS_LABEL_(line)      MOS_MERGE_(LBL, line)
#define MOS_TIME_OVER_M(t)    ((uint16_t)(millis() - (t)) < 0x8000U)
#define MOS_TIME_OVER_S(t)    ((uint16_t)((millis() >> 10) - (t)) < 0x8000U)
#define MOS_BREAK(tcb)        { (tcb)->pv_jmp = &&MOS_LABEL_(__LINE__);   \
                                return; }                                 \
                                MOS_LABEL_(__LINE__):

// Task Control Block
typedef struct{
  void      *pv_jmp;          // next resume program position
  uint16_t  u16_time;         // delay in msec. or sec.
}MOS_TCB_t;

typedef MOS_TCB_t* PTCB;


/*******************************************************************************
** MOS API "Functions"
*******************************************************************************/

/*******************************************************************************
  @Function:
    MOS_Break()
 
  Description:
    Suspend task execution for one loop cycle. The task will be resumed with
    the next call of the loop function.
 
  Syntax:
    MOS_Break(tcb);
  
  Parameters:
    - tcb: Task control block, passed to the task as first parameter (PTCB)
 
  Returns:
    none
 
  See also:
    MOS_Continue

  Example:
    -
*/
#define MOS_Break(tcb)            { (tcb)->u16_time = millis(); MOS_BREAK(tcb); }

/*******************************************************************************
  @Function:
    MOS_Continue()
 
  Description:
    Continue the task execution at the previous suspended program position.
 
  Syntax:
    MOS_Continue(tcb);
  
  Parameters:
    - tcb: Task control block, passed to the task as parameter (PTCB)
 
  Returns:
    none
 
  See also:
    MOS_Break

  Example:
    void LedTask(PTCB tcb) 
    {
      MOS_Continue(tcb);            // Continue at previous suspended program position

      while(1) 
      {
        ...
        
        MOS_Break(tcb);
      }
    }
*/
#define MOS_Continue(tcb)         { if((tcb)->pv_jmp != NULL) goto *(tcb)->pv_jmp; }

/*******************************************************************************
  @Function:
    MOS_Delay()
 
  Description:
    Suspend task for the given amount of time in milliseconds.
    After the time is elapsed, the task will be resumed.
 
  Syntax:
    MOS_Delay(tcb, time);
  
  Parameters:
    - tcb: Task control block, passed to the task as parameter (PTCB)
    - time: Time value in milliseconds (1..30000)
 
  Returns:
    none
 
  See also:
    MOS_DelaySec

  Example:
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
*/
#define MOS_Delay(tcb, time)      { (tcb)->u16_time = millis() + time;        \
                                     while(!MOS_TIME_OVER_M((tcb)->u16_time)) \
                                     { MOS_BREAK(tcb); } }

/*******************************************************************************
  @Function:
    MOS_DelaySec()
 
  Description:
    Suspend task for the given amount of time in seconds.
    After the time has elapsed, the task will be resumed.
 
  Syntax:
    MOS_DelaySec(tcb, time);
  
  Parameters:
    - tcb: Task control block, passed to the task as parameter (PTCB)
    - time: Time value in seconds (1..30000)
 
  Returns:
    none
 
  See also:
    MOS_Delay

  Example:
    void TimerTask(PTCB tcb) 
    {
      MOS_Continue(tcb);            // Continue at previous suspended program position

      seconds = 0;
      while(1) 
      {
        Serial.print(seconds);
        Serial.println(" seconds elapsed.");
        seconds++;
        MOS_DelaySec(tcb, 1);       // Suspend task for one second
      }
    }
*/
#define MOS_DelaySec(tcb, time)   { (tcb)->u16_time = (millis() / 1000) + time; \
                                    while(!MOS_TIME_OVER_S((tcb)->u16_time))   \
                                    { MOS_BREAK(tcb); } }

/*******************************************************************************
  @Function:
    MOS_WaitFor()
 
  Description:
    Suspend task until the given flag is set by means of MOS_Signal().
    With each call of MOS_WaitFor() a probably set flag will be cleared. If the
    flag wil be set by means of MOS_Signal() or directly (flag = true;) the already
    suspended task will resume. 
 
  Syntax:
    MOS_WaitFor(tcb, flag);
  
  Parameters:
    - tcb: Task control block, passed to the task as parameter (PTCB)
    - flag: Boolean variable to signal the event (bool) 
 
  Returns:
    none
 
  See also:
    MOS_WaitTimedFor

  Example:
    bool myFlag;

    void func()
    { 
      MOS_Signal(flag);
    }
    void myTask(PTCB tcb) 
    {
      MOS_Continue(tcb);            // Continue at previous suspended program position

      while(1) 
      {
        MOS_WaitFor(tcb, myFlag);   // wait until the flag is set
        Serial.println("myFlag is set");
      }
    }
*/
#define MOS_WaitFor(tcb, flag)    { flag = false;         \
                                    while(flag == false)  \
                                    { MOS_BREAK(tcb); } }

/*******************************************************************************
  @Function:
    MOS_WaitTimedFor()
 
  Description:
    Suspend task until the given flag is set by means of MOS_Signal() or 
    the timeout period in milliseconds has elapsed.
    With each call of MOS_WaitFor() a probably set flag will be cleared. If the
    flag wil be set by means of MOS_Signal() or directly (flag = true;), or
    the timeout has elapsed, the suspended task will resume. 
 
  Syntax:
    MOS_WaitTimedFor(tcb, flag, time);
  
  Parameters:
    - tcb: Task control block, passed to the task as parameter (PTCB)
    - flag: Boolean variable to signal the event (bool) 
    - time: Timeout value in milliseconds (1..30000)
 
  Returns:
    none
 
  See also:
    MOS_WaitFor

  Example:
    void func()
    { 
      MOS_Signal(flag);
    }
    void myTask(PTCB tcb) 
    {
      MOS_Continue(tcb);                      // Continue program execution

      while(1) 
      {
        MOS_WaitTimedFor(tcb, myFlag, 100);   // wait until the flag is set
        if(myFlag)                            
        {
          Serial.println("myFlag is set");
        }
        else
        {
          Serial.println("Timout occurred");
        }
      }
    }
*/
#define MOS_WaitTimedFor(tcb, flag, time) {                                                       \
                                    flag = false;                                                 \
                                    (tcb)->u16_time = millis() + time;                            \
                                    while((flag == false) && (!MOS_TIME_OVER_M((tcb)->u16_time))) \
                                    { MOS_BREAK(tcb); }}

/*******************************************************************************
  @Function:
    MOS_WaitForCond()
 
  Description:
    Suspend task for the time the given binary condition is false.
    The condition will be cyclically checked. If the condition becomes true
    the suspended task will resume. 
 
  Syntax:
    MOS_WaitForCond(tcb, condition);
    MOS_WaitForCond(tcb, cnt > 100);
    MOS_WaitForCond(tcb, a == b);
  
  Parameters:
    - tcb: Task control block, passed to the task as parameter (PTCB)
    - condition: binary condition which becomes true or false 
 
  Returns:
    none
 
  See also:
    MOS_WaitTimedForCond

  Example:
    void RxTask(PTCB tcb) 
    {
      MOS_Continue(tcb);

      while(1) 
      {
        MOS_WaitForCond(tcb, Serial.available());
        Serial.print("character ");
        Serial.print(Serial.read());
        Serial.println(" received");
      }
    }
*/
#define MOS_WaitForCond(tcb, condition)                                                              \
                                    { while((condition) == false)                                    \
                                    { MOS_BREAK(tcb); } }

/*******************************************************************************
  @Function:
    MOS_WaitTimedForCond()
 
  Description:
    Suspend task for the time the given binary condition is false or 
    the timeout period in milliseconds has elapsed.
    The condition will be cyclically checked. If the condition becomes true
    or the timeout has elapsed, the suspended task will resume. 
 
  Syntax:
    MOS_WaitTimedForCond(tcb, condition, time);
    MOS_WaitTimedForCond(tcb, (cnt > 100), 200);
    MOS_WaitTimedForCond(tcb, (a == b), 200);
  
  Parameters:
    - tcb: Task control block, passed to the task as parameter (PTCB)
    - condition: binary condition which becomes true or false 
    - time: Timeout value in milliseconds (1..30000)
 
  Returns:
    none
 
  See also:
    MOS_WaitTimedFor

  Example:
    void RxTask(PTCB tcb) 
    {
      MOS_Continue(tcb);

      while(1) 
      {
        MOS_WaitTimedForCond(tcb, Serial.available(), 100);   
        if(Serial.available())                            
        {
          Serial.print("character ");
          Serial.print(Serial.read());
          Serial.println(" received");
        }
        else
        {
          Serial.println("Timout occurred");
        }
      }
    }
*/
#define MOS_WaitTimedForCond(tcb, cond, time) {                                                     \
                                    (tcb)->u16_time = millis() + time;                              \
                                    while(((cond) == false) && (!MOS_TIME_OVER_M((tcb)->u16_time))) \
                                    { MOS_BREAK(tcb); }}

/*******************************************************************************
  @Function:
    MOS_Signal()
 
  Description:
    With call of MOS_Signal(), a suspended task waiting on the given flag will
    resume. This function is "syntactical sugar".
    You could also write "flag = true;".
 
  Syntax:
    MOS_Signal(flag);
  
  Parameters:
    - flag: Boolean variable to signal the event (bool) 
 
  Returns:
    none
 
  See also:
    MOS_WaitFor

  Example:
    bool myFlag;

    void func()
    { 
      MOS_Signal(flag);
    }
    void myTask(PTCB tcb) 
    {
      MOS_Continue(tcb);            

      while(1) 
      {
        MOS_WaitFor(tcb, myFlag);   
        Serial.println("myFlag is set");
      }
    }
*/
#define MOS_Signal(flag)          { flag = true; }


/*
 * Reset the calling task so that it starts from the beginning.
 */
/*******************************************************************************
  @Function:
    MOS_ResetTask()
 
  Description:
    Reset the task control block (tcb) and resume the task.
    With the next loop cycle the task will be restarted at the very beginning.
    That means, the code before the "while(1)" loop will be executed again.
 
  Syntax:
    MOS_ResetTask(tcb);
  
  Parameters:
    - tcb: Task control block, passed to the task as parameter (PTCB)
 
  Returns:
    none
 
  See also:
    -

  Example:
    -
*/
#define MOS_ResetTask(tcb)        { (tcb)->pv_jmp = NULL; return; }

/*
 * Execute the given task.
 */
/*******************************************************************************
  @Function:
    MOS_Call()
 
  Description:
    Call the given task. If the task is not resumed, it will be executed.
    A MOS_Call() statement has to be inserted for each task in the system,
    so that the tasks can be executed.
 
  Syntax:
    MOS_Call(task);
  
  Parameters:
    - task: The identifier of the task
 
  Returns:
    none
 
  See also:
    -

  Example:
    void LedTask(PTCB tcb) 
    {
    ...
    }
    void DemoTask(PTCB tcb) 
    {
    ...
    }
    void loop() 
    {
      MOS_Call(LedTask);
      MOS_Call(DemoTask);
    }
*/
#define MOS_Call(task)            { static MOS_TCB_t MOS_MERGE_(task, tcb);   \
                                    task((&MOS_MERGE_(task, tcb))); }

#endif //MOS_H
