/*
 * MOS - Macro based Operating System
 * A ultra lightweight cooperative multitasking schema for Arduino devices
 *
 * V0.3 - 2016-03-17
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

// Task Control Block
typedef struct{
  void      *pv_jmp;          // next continue position
  uint32_t  u32_time;         // delay in msec
}MOS_TCB_t;

typedef MOS_TCB_t* PTCB;

/*
 * API "Functions"
 */

/* Interrupt task execution for one loop. If no other task is in the state 'READY',
 * the task will be reactivated immediately.
 */
#define MOS_Break(tcb)        (tcb)->pv_jmp = &&MOS_LABEL_(__LINE__); return; MOS_LABEL_(__LINE__):

/*
 * Continue task execution at the previous interrupted position.
 */
#define MOS_Continue(tcb)     if((tcb)->pv_jmp != NULL) goto *(tcb)->pv_jmp

/*
 * Give up for the given amount of milliseconds (1..2^31).
 */
#define MOS_Delay(tcb, time)  (tcb)->u32_time = millis() + time; MOS_Break(tcb)

/* 
 * If the task is not in waiting state (via MOS_Delay), call the task.
 */
#define MOS_Call(task)  	  static MOS_TCB_t MOS_MERGE_(task, tcb); \
							  if(MOS_TIME_OVER(MOS_MERGE_(task, tcb).u32_time)) \
                              task((&MOS_MERGE_(task, tcb)))
                              
#endif //MOS_H
