/*
 * MOS - Macro based Operating System
 * A ultra lightweight cooperative multitasking scheduler for Arduino devices.
 *
 * V0.5 - 2016-03-27
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
 * MOS API "Functions"
 */

/* Suspend task execution for one cycle. The task will be resumed with the next
 * call of the loop function.
 */
#define MOS_Break(tcb)            { (tcb)->u16_time = millis(); MOS_BREAK(tcb); }

/*
 * Continue the task execution at the previous suspended program position.
 */
#define MOS_Continue(tcb)         { if((tcb)->pv_jmp != NULL) goto *(tcb)->pv_jmp; }

/*
 *  Suspend task for the given amount of milliseconds (1..30000).
 */
#define MOS_Delay(tcb, time)      { (tcb)->u16_time = millis() + time;        \
                                     while(!MOS_TIME_OVER_M((tcb)->u16_time)) \
                                     { MOS_BREAK(tcb); } }

/*
 *  Suspend task for the given amount of seconds (1..30000).
 */
#define MOS_DelaySec(tcb, time)   { (tcb)->u16_time = (millis() / 1000) + time; \
                                    while(!MOS_TIME_OVER_S((tcb)->u16_time))   \
                                    { MOS_BREAK(tcb); } }

/*
 * Suspend task until the task is resumed by means of MOS_Signal().
 */
#define MOS_WaitFor(tcb, flag)    { flag = false;         \
                                    while(flag == false)  \
                                    { MOS_BREAK(tcb); } }

/*
 * Suspend task until the task is resumed by means of MOS_Signal() or by means of the
 * given timeout in milliseconds (1..30000).
 */
#define MOS_WaitTimedFor(tcb, flag, time) {                                                       \
                                    flag = false;                                                 \
                                    (tcb)->u16_time = millis() + time;                            \
                                    while((flag == false) && (!MOS_TIME_OVER_M((tcb)->u16_time))) \
                                    { MOS_BREAK(tcb); }}

/*
 * Resume the suspended task waiting on the given flag.
 */
#define MOS_Signal(flag)          { flag = true; }


/*
 * Execute the given task.
 */
#define MOS_Call(task)            { static MOS_TCB_t MOS_MERGE_(task, tcb);   \
                                    task((&MOS_MERGE_(task, tcb))); }

#endif //MOS_H
