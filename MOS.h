/*
 * MOS - Macro based Operating System
 * A ultra lightweight cooperative multitasking scheduler for Arduino devices
 *
 * V0.4 - 2016-03-26
 *
 * Copyright (c) 2016 Joachim Stolberg.  All rights reserved.
 * This file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * See file LICENSE.txt for further informations on licensing terms.
 */

#ifndef MOS_H
#define MOS_H

#include <inttypes.h>


// internal macros
#define MOS_MERGE_(a,b)       a##b
#define MOS_LABEL_(line)      MOS_MERGE_(LBL, line)
#define MOS_TIME_OVER(t)      ((uint32_t)(millis() - (t)) < 0x80000000UL)
#define MOS_BREAK(tcb)        { (tcb)->pv_jmp = &&MOS_LABEL_(__LINE__);   \
                              return; }                                   \
                              MOS_LABEL_(__LINE__):

// Task Control Block
typedef struct{
  void      *pv_jmp;          // next continue position
  uint32_t  u32_time;         // delay in msec
}MOS_TCB_t;

typedef MOS_TCB_t* PTCB;

/*
 * API "Functions"
 */

/* Suspend task execution. The task will be resumed with the next 
 * call of the loop function.
 */
#define MOS_Break(tcb)        { (tcb)->u32_time = millis(); MOS_BREAK(tcb); }

/*
 * Continue the task execution at the previous suspended program position.
 */
#define MOS_Continue(tcb)     { if((tcb)->pv_jmp != NULL) goto *(tcb)->pv_jmp; }

/*
 *  Suspend task for the given amount of milliseconds (1..2^31).
 */
#define MOS_Delay(tcb, time)  { (tcb)->u32_time = millis() + time; MOS_BREAK(tcb); }

/*
 * Suspend task until the task is resumed by means of MOS_Signal().
 */
#define MOS_WaitFor(ptcb, flag)       { flag = false; while(flag == false) \
                                      { MOS_BREAK(ptcb); }}

                                   
/*
 * Resume the suspended task waiting on the given flag.
 */
#define MOS_Signal(flag)              { flag = true; }


/* 
 * If the task is not waiting (via MOS_Delay/MOS_WaitFor), the task 
 * will be called and resumed on the previous suspended program position.
 */
#define MOS_Call(task)        { static MOS_TCB_t MOS_MERGE_(task, tcb);   \
                              if(MOS_TIME_OVER(MOS_MERGE_(task, tcb).u32_time)) \
                                  task((&MOS_MERGE_(task, tcb))); }
                              
#endif //MOS_H
