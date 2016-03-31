# MOS API Documentation

MOS is not a real operating system or scheduler. It only consists of a view macros to handle
suspend and resume positions inside the task function.
To store the task position and the task related timer, only 4 bytes of RAM memory per task is needed.
This allows a ultra lightweight cooperative multitasking on Ardunio devices with plenty of tasks
even on devices with less than 1 KByte RAM.


## How it works

MOS uses a feature of the GNU Compiler which is called [Labels as Values](https://gcc.gnu.org/onlinedocs/gcc/Labels-as-Values.html)
It allows a "goto" statement with a variable programm position:
```C++
void *ptr;
/* ... */
ptr = &&LABEL1;
goto *ptr;
/* ... */
LABEL1:
```
By means of ```goto *ptr;``` the program jumps to the "LABEL1" position. 
The function "MOS_Continue()" does exactly this (simplified principle):
```C++
#define MOS_Continue(tcb)   if(tcb != NULL) goto tcb
```

On the opposite the function "MOS_Break()" stores the current programm position 
for the next call of the task and returns (simplified principle):
```C++
#define MOS_Break(tcb)   tcb = &&LABEL_XY;          \
                         return;                    \
                         LABEL_XY:
                         
```


## Reference


### MOS_Break()

##### Description

Suspend task execution for one loop cycle. The task will be resumed with
the next call of the loop function.

<br>
##### Syntax

```C++
MOS_Break(tcb);
```

##### Parameters

| Parameter |Description  |
|-----------|------------ |
| tcb | Task control block, passed to the task as first parameter (PTCB) |

<br>
##### Returns
none

<br>
##### See also
MOS_Continue

------------------------------------------------

### MOS_Continue()

##### Description

Continue the task execution at the previous suspended program position.

<br>
##### Syntax

```C++
MOS_Continue(tcb);
```

##### Parameters

| Parameter |Description  |
|-----------|------------ |
| tcb | Task control block, passed to the task as parameter (PTCB) |

<br>
##### Returns
none

<br>
##### See also
MOS_Break

<br>
##### Example

```C++
void LedTask(PTCB tcb)
{
  MOS_Continue(tcb);            // Continue at previous suspended program position

  while(1)
  {
    ...

    MOS_Break(tcb);
  }
}
```

------------------------------------------------

### MOS_Delay()

##### Description

Suspend task for the given amount of time in milliseconds.
After the time is elapsed, the task will be resumed.

<br>
##### Syntax

```C++
MOS_Delay(tcb, time);
```

##### Parameters

| Parameter |Description  |
|-----------|------------ |
| tcb | Task control block, passed to the task as parameter (PTCB) - time: Time value in milliseconds (1..30000) |

<br>
##### Returns
none

<br>
##### See also
MOS_DelaySec

<br>
##### Example

```C++
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
```

------------------------------------------------

### MOS_DelaySec()

##### Description

Suspend task for the given amount of time in seconds.
After the time is elapsed, the task will be resumed.

<br>
##### Syntax

```C++
MOS_DelaySec(tcb, time);
```

##### Parameters

| Parameter |Description  |
|-----------|------------ |
| tcb | Task control block, passed to the task as parameter (PTCB) - time: Time value in seconds (1..30000) |

<br>
##### Returns
none

<br>
##### See also
MOS_Delay

<br>
##### Example

```C++
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
```

------------------------------------------------

### MOS_WaitFor()

##### Description

Suspend task until the given flag is set by means of MOS_Signal().
With each call of MOS_WaitFor() a probably set flag will be cleared. If the
flag wil be set by means of MOS_Signal() or directly (flag = true;) the already
suspended task will resume.

<br>
##### Syntax

```C++
MOS_WaitFor(tcb, flag);
```

##### Parameters

| Parameter |Description  |
|-----------|------------ |
| tcb | Task control block, passed to the task as parameter (PTCB) - flag: Boolean variable to signal the event (bool) |

<br>
##### Returns
none

<br>
##### See also
MOS_WaitTimedFor

<br>
##### Example

```C++
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
```

------------------------------------------------

### MOS_WaitTimedFor()

##### Description

Suspend task until the given flag is set by means of MOS_Signal() or
the timeout period in milliseconds has elapsed.
With each call of MOS_WaitFor() a probably set flag will be cleared. If the
flag wil be set by means of MOS_Signal() or directly (flag = true;), or
the timeout has elapsed, the suspended task will resume.

<br>
##### Syntax

```C++
MOS_WaitTimedFor(tcb, flag, time);
```

##### Parameters

| Parameter |Description  |
|-----------|------------ |
| tcb | Task control block, passed to the task as parameter (PTCB) - flag: Boolean variable to signal the event (bool) - time: Timeout value in milliseconds (1..30000) |

<br>
##### Returns
none

<br>
##### See also
MOS_WaitFor

<br>
##### Example

```C++
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
```

------------------------------------------------

### MOS_WaitForCond()

##### Description

Suspend task for the time the given binary condition is false.
The condition will be cyclically checked. If the condition becomes true
the suspended task will resume.

<br>
##### Syntax

```C++
MOS_WaitForCond(tcb, condition);
MOS_WaitForCond(tcb, cnt > 100);
MOS_WaitForCond(tcb, a == b);
```

##### Parameters

| Parameter |Description  |
|-----------|------------ |
| tcb | Task control block, passed to the task as parameter (PTCB) - condition: binary condition which becomes true or false |

<br>
##### Returns
none

<br>
##### See also
MOS_WaitTimedForCond

<br>
##### Example

```C++
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
```

------------------------------------------------

### MOS_WaitTimedForCond()

##### Description

Suspend task for the time the given binary condition is false or
the timeout period in milliseconds has elapsed.
The condition will be cyclically checked. If the condition becomes true
or the timeout has elapsed, the suspended task will resume.

<br>
##### Syntax

```C++
MOS_WaitTimedForCond(tcb, condition, time);
MOS_WaitTimedForCond(tcb, (cnt > 100), 200);
MOS_WaitTimedForCond(tcb, (a == b), 200);
```

##### Parameters

| Parameter |Description  |
|-----------|------------ |
| tcb | Task control block, passed to the task as parameter (PTCB) - condition: binary condition which becomes true or false - time: Timeout value in milliseconds (1..30000) |

<br>
##### Returns
none

<br>
##### See also
MOS_WaitTimedFor

<br>
##### Example

```C++
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
```

------------------------------------------------

### MOS_Signal()

##### Description

With call of MOS_Signal(), a suspended task waiting on the given flag will
resume. This function is "syntactical sugar".
You could also write "flag = true;".

<br>
##### Syntax

```C++
MOS_Signal(flag);
```

##### Parameters

| Parameter |Description  |
|-----------|------------ |
| flag | Boolean variable to signal the event (bool) |

<br>
##### Returns
none

<br>
##### See also
MOS_WaitFor

<br>
##### Example

```C++
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
```

------------------------------------------------

### MOS_ResetTask()

##### Description

Reset the task control block (tcb) and resume the task.
With the next loop cycle the task will be restarted at the very beginning.
That means, the code before the "while(1)" loop will be executed again.

<br>
##### Syntax

```C++
MOS_ResetTask(tcb);
```

##### Parameters

| Parameter |Description  |
|-----------|------------ |
| tcb | Task control block, passed to the task as parameter (PTCB) |

<br>
##### Returns
none

------------------------------------------------

### MOS_Call()

##### Description

Call the given task. If the task is not resumed, it will be executed.
A MOS_Call() statement has to be inserted for each task in the system,
so that the tasks can be executed.

<br>
##### Syntax

```C++
MOS_Call(task);
```

##### Parameters

| Parameter |Description  |
|-----------|------------ |
| task | The identifier of the task |

<br>
##### Returns
none

<br>
##### Example

```C++
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
```

------------------------------------------------



