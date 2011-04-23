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

#include "knx_messaging.h"

#define HOP_COUNT ((uint8)6)

/* check: 'static' ??? */
PMSG_Buffer GetBufferAddress(uint8 buf_num);
uint8       GetBufferNumber(const PMSG_Buffer buffer);
void        ClearMessageBuffer(uint8 buf_num);

static const uint8 MSG_MessageRedirectionTable[16] = {
    TASK_FREE_ID, TASK_LL_ID, TASK_NL_ID, TASK_TL_ID, TASK_TC_ID,   TASK_FREE_ID, TASK_FREE_ID, TASK_AL_ID,
    TASK_MG_ID,   TASK_MG_ID, TASK_PM_ID, TASK_LC_ID, TASK_FREE_ID, TASK_US_ID,   TASK_US_ID,   TASK_US_ID
};

static uint8        MSG_Queues[MSG_NUM_TASKS];
static MSG_Buffer   MSG_Buffers[MSG_NUM_BUFFERS];

/*  Get Destination Queue from Message-Code. */
#define MSG_GetQueueForService(service) ((uint8)MSG_MessageRedirectionTable[(service) >> 4])

PMSG_Buffer GetBufferAddress(uint8 buf_num)
{
    if (buf_num >= MSG_NUM_BUFFERS) {
        return (PMSG_Buffer)NULL;
    } else {
        return &MSG_Buffers[buf_num];
    }
}

uint8 GetBufferNumber(const PMSG_Buffer buffer)
{
    PMSG_Buffer tmp_buf;
    uint8       idx;

    for (idx = (uint8)0; idx < MSG_NUM_BUFFERS; idx++) {
        tmp_buf = &MSG_Buffers[idx];

        if (tmp_buf == buffer) {
            return idx;
        }
    }

    return MSG_INVALID_BUFFER;
}

void ClearMessageBuffer(uint8 buf_num)
{
    PMSG_Buffer ptr;
    uint8 *     pb;

    ptr = GetBufferAddress(buf_num);

    if (ptr == (PMSG_Buffer)NULL) {
        return;
    }

    pb = (uint8 *)ptr;
    pb++;

    Utl_MemSet(pb, '\0', (uint16)sizeof(MSG_Buffer) - (uint16)1);
}

static uint16 AllocCount = (uint16)0, ReleaseCount = (uint16)0;

PMSG_Buffer MSG_AllocateBuffer(void)
{
    uint8       fp;
    PMSG_Buffer ptr;

    DISABLE_ALL_INTERRUPTS();

    AllocCount++;

    if ((fp = MSG_Queues[TASK_FREE_ID]) == MSG_NO_NEXT) {
        ENABLE_ALL_INTERRUPTS();
        return (PMSG_Buffer)NULL;       /* no Buffer available. */
    }

    ptr = &MSG_Buffers[fp];

    if (ptr->next == MSG_NO_NEXT) {
        MSG_Queues[TASK_FREE_ID] = MSG_NO_NEXT;
    } else {
        MSG_Queues[TASK_FREE_ID]   = ptr->next;
        ptr->next                  = MSG_NO_NEXT;
    }

    ENABLE_ALL_INTERRUPTS();
    return &MSG_Buffers[fp];
}

boolean MSG_ReleaseBuffer(PMSG_Buffer ptr)
{
    uint8 buf_num, old_fp, t_fp;

    DISABLE_ALL_INTERRUPTS();

    if ((buf_num = GetBufferNumber(ptr)) == MSG_INVALID_BUFFER) {
        ENABLE_ALL_INTERRUPTS();
        return FALSE;
    }

    ReleaseCount++;

    old_fp = MSG_Queues[TASK_FREE_ID];
    t_fp   = old_fp;

    while (t_fp != MSG_NO_NEXT) {
        if (t_fp == buf_num) {
            ENABLE_ALL_INTERRUPTS();
            return FALSE;   /* not allocated. */
        }

        t_fp = MSG_Buffers[t_fp].next;
    }

    MSG_Queues[TASK_FREE_ID]   = buf_num;
    MSG_Buffers[buf_num].next  = old_fp;
    ClearMessageBuffer(buf_num);

    ptr = (PMSG_Buffer)NULL;  /* invalidate Buffer. */
    ENABLE_ALL_INTERRUPTS();
    return TRUE;
}

boolean MSG_ClearBuffer(PMSG_Buffer ptr)
{
    uint8 * pb;

    if (ptr == (PMSG_Buffer)NULL) {
        return FALSE;
    }

    pb = (uint8 *)ptr;
    pb++;

    Utl_MemSet(pb, '\0', sizeof(MSG_Buffer) - 1);

    return TRUE;
}

boolean MSG_Post(PMSG_Buffer ptr)
{
    uint8 queue, buf_num, qp;

    if ((buf_num = GetBufferNumber(ptr)) == MSG_INVALID_BUFFER) {
        return FALSE;
    }

    if ((queue = MSG_GetQueueForService(ptr->service)) == TASK_FREE_ID) {
        return FALSE;
    }

    qp = MSG_Queues[queue];

    if (qp == MSG_QUEUE_EMPTY) {
        MSG_Queues[queue] = buf_num;
    } else {

        while (MSG_Buffers[qp].next != MSG_QUEUE_EMPTY) {
            qp = MSG_Buffers[qp].next;
        }

        MSG_Buffers[qp].next = buf_num;
    }

    return TRUE;
}

PMSG_Buffer MSG_Get(uint8 task)
{
    uint8 qp;

    if ((task < 1) || (task > MSG_NUM_TASKS)) {
        return (PMSG_Buffer)NULL;
    }

    if ((qp = MSG_Queues[task]) == MSG_QUEUE_EMPTY) {
        return (PMSG_Buffer)NULL;   /* no message for task. */
    }

    if (MSG_Buffers[qp].next != MSG_QUEUE_EMPTY) {
        MSG_Queues[task]       = MSG_Buffers[qp].next;
        MSG_Buffers[qp].next   = MSG_NO_NEXT; /* unlink Message-Buffer. */
    } else {
        MSG_Queues[task] = MSG_QUEUE_EMPTY;
    }

/*     */
    return &MSG_Buffers[qp];
}

void MSG_Init(void)
{
    uint8 t;

    for (t = (uint8)0; t < MSG_NUM_BUFFERS; t++) {
        ClearMessageBuffer(t);
    }

    MSG_Queues[TASK_FREE_ID] = (uint8)0x00;         /* the first Queue contains the Freelist. */

    for (t = (uint8)0; t <= MSG_NUM_BUFFERS; t++) { /* Setup Freelist. */
        MSG_Buffers[t].next = t + (uint8)1;
    }

    MSG_Buffers[MSG_NUM_BUFFERS - 1].next = MSG_NO_NEXT;

    for (t = (uint8)1; t < MSG_NUM_TASKS; t++) {
        MSG_Queues[t] = MSG_QUEUE_EMPTY;
    }
}

void MSG_SetLen(PMSG_Buffer pBuffer, uint8 len)
{
    pBuffer->len                       = len;
    MSG_GetMessagePtr(pBuffer)->ncpi  |= ((len - (uint8)7) & (uint8)0x0f);
}

uint8 MSG_GetLen(const PMSG_Buffer pBuffer)
{
    return pBuffer->len;
}

/*
   void MSG_SetHopCount(PMSG_Buffer pBuffer,uint8 hop_count)
   {
    MSG_GetMessagePtr(pBuffer)->ncpi|=((hop_count & 0x07)<<4);
   }

   void MSG_GetHopCount(PMSG_Buffer pBuffer,uint8 *hop_count)
   {
   *hop_count=((MSG_GetMessagePtr(pBuffer)->ncpi) & 0x70)>>4;
   }
 */

void MSG_SetRoutingCount(PMSG_Buffer pBuffer)
{
    uint8 ctrl, hop_count;

    ctrl = MSG_GetMessagePtr(pBuffer)->ctrl;

    if ((ctrl & (uint8)0x02) == (uint8)0x02) {
        hop_count                          = MSG_NO_ROUTING_CTRL;
        ctrl                              &= ~(uint8)0x02;
        MSG_GetMessagePtr(pBuffer)->ctrl   = ctrl;
    } else {
        hop_count = HOP_COUNT;
    }

    MSG_GetMessagePtr(pBuffer)->ncpi |= ((hop_count & (uint8)0x07) << 4);
}

uint8 MSG_GetRoutingCount(const PMSG_Buffer pBuffer)
{
    return ((MSG_GetMessagePtr(pBuffer)->ncpi) & (uint8)0x70) >> 4;
}

void MSG_SetRoutingCtrl(PMSG_Buffer pBuffer, boolean on)
{
    uint8 r;

    (on == TRUE) ? (r = (uint8)0x02) : (r = (uint8)0x00);
    MSG_GetMessagePtr(pBuffer)->ctrl |= r;
}

/*
   boolean MSG_GetRoutingCtrl(const PMSG_Buffer pBuffer)
   {
    return ((MSG_GetMessagePtr(pBuffer)->ctrl & 0x02)==0x02);
   }
 */

/*
   uint8 MSG_GetLSDULen(PMSG_Buffer pBuffer)
   {
    return MSG_GetMessagePtr(pBuffer)->ncpi & 0x0f;
   }

   void MSG_SetLSDULen(PMSG_Buffer pBuffer,uint8 len_lsdu)
   {
    MSG_GetMessagePtr(pBuffer)->ncpi=(len_lsdu & 0x0f);
   }
 */

