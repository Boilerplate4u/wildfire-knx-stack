/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
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

/*
**
**  NOTE: THIS IS JUST AN EXAMPLE CONFIGURATION.
**        PLEASE COPY THIS FILE TO THE APPROPRIATE DIRECTORY OF YOUR PROJECT.
**        FOR DETAILS SEE DOCUMENTATION.
**
*/

#if !defined(__KPS_CONFIG_H)
#define __KPS_CONFIG_H

#include "KNXDefs.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

void Port_TimerLockMainTimer(void);
void Port_TimerUnlockMainTimer(void);

#define TMR_NUM_TIMERS      5
#define TMR_TICK_RESOLUTION 10  /* Note: 1000 must be divisible by TMR_TICK_RESOLUTION without remainder! */

#define TMR_LOCK_MAIN_TIMER()       Port_TimerLockMainTimer()
#define TMR_UNLOCK_MAIN_TIMER()     Port_TimerUnlockMainTimer()

#define ENABLE_ALL_INTERRUPTS()
#define DISABLE_ALL_INTERRUPTS()

#define KNX_BUS_INTERFACE           KNX_BIF_TPUART_NCN5120

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __KPS_CONFIG_H */
