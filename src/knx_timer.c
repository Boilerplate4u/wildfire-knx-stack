/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2011 by Christoph Schueler <github.com/Christoph2,
 *                                       cpu12.gems@googlemail.com>
 *
 *   All Rights Reserved
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "knx_timer.h"

/*
   DWORD WINAPI MSTickerThread(LPVOID *lpThreadParm);
   HANDLE hTickerThread;
   DWORD TickerThreadID;
 */

#define DISABLE_ALL_INTERRUPTS()
#define ENABLE_ALL_INTERRUPTS()

static TM_TimerType KNX_Timer[TM_NUM_TIMERS];
static TM_TickType  TM_SysMsCounter;
static TM_TickType  TM_SysSecondCounter;

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

void KnxTMR_Init(void)
{
    uint8_least idx;

    TM_SysMsCounter = TM_SysSecondCounter = (uint32)0UL;

    DISABLE_ALL_INTERRUPTS();

    for (idx = (uint8)0; idx < TM_NUM_TIMERS; idx++) {
        KNX_Timer[idx].expire_counter  = (uint32)0UL;
        KNX_Timer[idx].state           = TM_STATE_STOPPED;
        KNX_Timer[idx].base            = TM_BASE_MS;
    }

    ENABLE_ALL_INTERRUPTS();
}


boolean KnxTMR_Start(uint8 timer, TM_BaseType base, TM_TickType ticks)
{
    if (timer < TM_NUM_TIMERS) {
        if (!(KNX_Timer[timer].state & TM_STATE_RUNNING)) {
            DISABLE_ALL_INTERRUPTS();
            KNX_Timer[timer].expire_counter    = ticks;
            KNX_Timer[timer].state             = TM_STATE_RUNNING;
            KNX_Timer[timer].base              = base;
            ENABLE_ALL_INTERRUPTS();
            return TRUE;
        } else {
            return FALSE;
        }
    } else {
        return FALSE;
    }
}


boolean KnxTMR_Stop(uint8 timer)
{
    if (timer < TM_NUM_TIMERS) {
        DISABLE_ALL_INTERRUPTS();
        KNX_Timer[timer].state = TM_STATE_STOPPED;
        ENABLE_ALL_INTERRUPTS();
        return TRUE;
    } else {
        return FALSE;
    }
}


boolean KnxTMR_IsExpired(uint8 timer)
{
    TM_StateType state;

    if (timer < TM_NUM_TIMERS) {
        DISABLE_ALL_INTERRUPTS();
        state                      = KNX_Timer[timer].state;
        KNX_Timer[timer].state    &= (~TM_STATE_EXPIRED);

        ENABLE_ALL_INTERRUPTS();
        return (state & TM_STATE_EXPIRED) == TM_STATE_EXPIRED;
    } else {
        return FALSE;   /* Invalid Timer. */
    }
}


boolean KnxTMR_IsRunning(uint8 timer)
{
    if (timer < TM_NUM_TIMERS) {
        return (KNX_Timer[timer].state & TM_STATE_RUNNING) == TM_STATE_RUNNING;
    } else {
        return FALSE;   /* Invalid Timer. */
    }
}


boolean KnxTMR_GetRemainder(uint8 timer, TM_TickRefType remainder)
{
    if (timer < TM_NUM_TIMERS) {
        if (!(KNX_Timer[timer].state & TM_STATE_RUNNING)) {
            return FALSE;
        } else {
            DISABLE_ALL_INTERRUPTS();
            *remainder = KNX_Timer[timer].expire_counter;
            ENABLE_ALL_INTERRUPTS();
            return TRUE;
        }
    } else {
        return FALSE;
    }
}


TM_TickType KnxTMR_GetSystemTime(TM_BaseType base)
{
    TM_TickType t;

    DISABLE_ALL_INTERRUPTS();

    if (base == TM_BASE_MS) {
        t = TM_SysMsCounter;
    } else if (base == TM_BASE_SEC) {
        t = TM_SysSecondCounter;
    }

    ENABLE_ALL_INTERRUPTS();

    return t;
}


/*
   void TM_Delay(TM_TickType ms
   {

   }

   void TM_DelayHMS(WORD H,WORD M,WORD S)
   {
    TM_TickType delay_time,end_time;

    delay_time=((TM_TickType)H*60*60)+((TM_TickType)M*60)+((TM_TickType)S);	// in Seconds !!!

    end_time=delay_time+TM_SysSecondCounter;

   //	while
   }
 */

void KnxTMR_SecondCallback(void)
{

}


void KnxTMR_SystemTimeHandler(void)
{
    TM_TimerType *  tm;
    uint8           idx;
    boolean         SecondChanged = FALSE;

    TM_SysMsCounter++;

    if ((TM_SysMsCounter % (uint32)1000UL) == (uint32)0UL) {
        TM_SysSecondCounter++;
        SecondChanged = TRUE;
        KnxTMR_SecondCallback();
    }

    for (idx = (uint8)0; idx < TM_NUM_TIMERS; idx++) {
        tm = &KNX_Timer[idx];

        if ((tm->state & TM_STATE_RUNNING) == TM_STATE_RUNNING) {
            if (tm->base == TM_BASE_MS) {
                if (--tm->expire_counter == (uint32)0UL) {
                    tm->state = TM_STATE_EXPIRED;
                }
            } else if (tm->base == TM_BASE_SEC) {
                if ((SecondChanged == TRUE) && (--tm->expire_counter == (uint32)0UL)) {
                    tm->state = TM_STATE_EXPIRED;
                }
            }
        }
    }
}


/*
   ---
   2.3.4 Function TmAddStart
   Prototype:
    void TmAddStart(TIMER* pTimer, ULONG ticks)
   Description:
    This function restarts a timer for the specified �ticks�
    measured from the last expiration of the timer. Use this
    function if you want to get timer intervals that do not
    drift away from the real time like it would be if you use �TmStart�.

   Parameters:
   TIMER* pTimer:
    A pointer to a timer that should be started from the
    last expiration
   ULONG ticks:
    The number of ticks the timer should run before it will
    be expired.
   Return values:
    none
   Comment:
    Before a call of �TmAddStart� there must be at least one
    call of �TmStart�.
 */

/*
   DWORD WINAPI MSTickerThread(LPVOID *lpThreadParm)
   {
   //      struct timeval tv;
   //      tv.tv_sec = 0;
   //      tv.tv_usec = 35000;     // 35000 Mikrosekunden = 35 Millisekunden
   //      select(0, NULL, NULL, NULL, &tv);
    UNREFERENCED_PARAMETER(lpThreadParm);

    while (TRUE) {
        Sleep(1);
        TM_Ticker_Callback();
    }

    return 0;
   }
 */

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
