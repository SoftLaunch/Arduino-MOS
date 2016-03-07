/*
 * MOS - Macro based Operating System
 * A ultra lightweight cooperative multitasking schema for Arduino devices
 *
 * V0.1 - 2016-03-07
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

// Task Control Block
typedef struct{
  void      *pv_jmp;          // next continue position
  uint32_t  u32_time;         // delay in msec
  boolean   stopped;          // stopped flag
}MOS_TCB_t;


/*
 * API "Functions"
 */

/* Give up for a task with higher priority. If no other task is in the state 'READY',
 * the task will be reactivated immediately.
 */
#define MOS_Break(tcb)        (tcb)->pv_jmp = &&MOS_LABEL_(__LINE__); return; MOS_LABEL_(__LINE__):

/*
 * Continue task execution at the previous interrupted position.
 */
#define MOS_Continue(tcb)     if((tcb)->pv_jmp != NULL) goto *(tcb)->pv_jmp

/*
 * Give up for the given amount of milliseconds.
 */
#define MOS_Delay(tcb, time)  (tcb)->u32_time = millis() + time; MOS_Break(tcb)

/*
 * Suspend task. Only a MOS_Resume will aktivate the task again.
 */
#define MOS_Suspend(tcb)      (tcb)->stopped = 1; (tcb)->u32_time = 0; MOS_Break(tcb)

/*
 * Reactivate a suspended task referenced by the given 'tcb'.
 */
#define MOS_Resume(tcb)       (tcb)->stopped = 0;

/*
 * Restart a task at the beginning.
 */
#define MOS_Reset(tcb)        (tcb)->pv_jmp = NULL; (tcb)->u32_time = 0; (tcb)->stopped = 0

/* If the task is in the state 'READY' (not suspended and not in waiting state),
 * call the given task and then return from loop().
 */
#define MOS_Call(task, tcb, time, obj) \
                              if((!(tcb)->stopped) && ((tcb)->u32_time < time)) \
                              { task((tcb), obj); return; }

#endif //MOS_H
