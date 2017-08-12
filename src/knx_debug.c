/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2016 by Christoph Schueler <github.com/Christoph2,
*                                       cpu12.gems@googlemail.com>
*
*   All Rights Reserved
*
*  This program is free softwKNXe; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free SoftwKNXe Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WKNXRANTY; without even the implied wKNXranty of
*  MERCHANTABILITY or FITNESS FOR A PKNXTICULKNX PURPOSE.  See the
*  GNU General Public License for more KnxEtails.
*
*  You should have received a copy of the GNU General Public License along
*  with this program; if not, write to the Free SoftwKNXe Foundation, Inc.,
*  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*
*/


#include "knx_debug.h"
#if (KNX_BUILD_TYPE == KNX_BUILD_DEBUG) && (defined(_MSC_VER) || defined(WIN32) || defined(WIN64) || defined(__CYGWIN32__) \
     || defined(__CYGWIN64__) || defined(__GNUC__))
#include <stdio.h>
#include "knx_ffi.h"

STATIC /* LARGE_INTEGER */ uint64_t timerFrequency;
STATIC boolean hrcAvailable;

void Dbg_Init(void)
{
    if (QueryPerformanceFrequency(&timerFrequency) == 0) {
        hrcAvailable = FALSE;
    }
    else {
        hrcAvailable = TRUE;
    }
}

boolean Dbg_IsHRTAvailable(void)
{
    return hrcAvailable;
}

#if 0
void Dbg_TimerInit(Dbg_TimerType * timerContext)
{
    if (!hrcAvailable) {
        return;
    }
    timerContext->running = FALSE;
}

void Dbg_TimerStart(Dbg_TimerType * timerContext)
{
    LARGE_INTEGER timerValue;
    if (!hrcAvailable || timerContext->running) {
        return;
    }
    timerContext->running = TRUE;
    QueryPerformanceCounter(&timerValue);
    timerContext->start = timerValue.QuadPart;
}

void Dbg_TimerStop(Dbg_TimerType * timerContext)
{
    LARGE_INTEGER timerValue;
    if (!hrcAvailable || !timerContext->running) {
        return;
    }
    QueryPerformanceCounter(&timerValue);
    timerContext->stop = timerValue.QuadPart;
}

__int64 Dbg_TimerElapsedTime(Dbg_TimerType const * timerContext)
{
    if (!hrcAvailable || !timerContext->running) {
        return (ULONG64)0ULL;
    }
    return (timerContext->stop - timerContext->start) / (timerFrequency.QuadPart / 1000);
}
#endif

#if 0
void KnxEt_DumpHex(__uint8_t const * frame, __uint16_t length)
{
    __uint8_t idx;

    for (idx = 0; idx < length; ++idx) {
        printf("%02x ", frame[idx]);
    }
    printf("\n");
}
#endif


void Dbg_TraceFunctionEntry(__uint8_t ModuleId, __uint8_t ApiId)
{
    if (ModuleId == 1 && ApiId == 5)
        printf("Dbg_TraceFunctionEntry -- module: %u api: %u\n", ModuleId, ApiId);
    KNX_API_TRACE(0, ModuleId, ApiId);

}

void Dbg_TraceFunctionExit(__uint8_t ModuleId, __uint8_t ApiId)
{
    //printf("Dbg_TraceFunctionExit -- module: %u api: %u\n", ModuleId, ApiId);
    KNX_API_TRACE(1, ModuleId, ApiId);
}

#endif /* defined(WIN32) || defined(WIN64) */

