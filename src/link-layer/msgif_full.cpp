
/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2017 by Christoph Schueler <github.com/Christoph2,
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

#include "knx_msgif.hpp"

namespace knx {

void KnxMsgIf_Post(uint8_t const * const frame, Knx_ServiceType service, Knx_StatusType status)
{
    KnxMsg_Buffer * txBuffer;
    uint16_t length;

    if (KnxMsg_AllocateBuffer(&txBuffer) == KNX_E_OK) {
        txBuffer->service = service;
        txBuffer->status = status;

        txBuffer->len = length = (frame[5] & (uint8_t)0x0f) + (uint8_t)7;
        Utl_MemCopy((void *)txBuffer->msg.raw, (void *)frame, length);

        (void)KnxMsg_Post(txBuffer);
    } else {
        ASSERT(FALSE);
    }
}

} // namespace knx


