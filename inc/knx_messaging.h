/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2012 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__KNX_MESSAGING_H)
#define __KNX_MESSAGING_H

#include "KNXDefs.h"
#include "knx_address.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */


/*
** Global defines.
*/	
#define MSG_NO_ROUTING_CTRL ((uint8)7)

#define MSG_NUM_TASKS       ((uint8)10)
#define MSG_NUM_BUFFERS     ((uint8)4)

#define MSG_QUEUE_EMPTY     ((uint8)0xff)
#define MSG_INVALID_BUFFER  ((uint8)0xff)
#define MSG_NO_NEXT         ((uint8)0xff)

#define TPCI_INVALID        ((uint8)0xbf)
#define APCI_INVALID        ((uint16)0x3FF)

#define TASK_FREE_ID        ((uint8)0)          /* Free message buffers.                   */
#define TASK_LL_ID          ((uint8)1)          /* Link-Layer.                             */
#define TASK_NL_ID          ((uint8)2)          /* Network-Layer.                          */
#define TASK_TL_ID          ((uint8)3)          /* Transport-Layer/Multicast.              */
#define TASK_TC_ID          ((uint8)4)          /* Transport-Layer/Multicast&one-to-one.   */
#define TASK_LC_ID          ((uint8)5)          /* Transport-Layer/Local.                  */
#define TASK_AL_ID          ((uint8)6)          /* Application-Layer/Group-Oriented.       */
#define TASK_MG_ID          ((uint8)7)          /* Application-Layer/Managment.            */
#define TASK_PM_ID          ((uint8)8)          /* PEI-System.                             */
#define TASK_US_ID          ((uint8)9)          /* User.                                   */
#define TASK_INVALID_ID     ((uint8)0xff)       /* Invalid Queue.                          */

/* todo: Support f. Extended-Frames!!! */
#define MSG_LEN             ((uint8)22)
#define MAX_ADPU_LEN        ((uint8)14)
#define MAX_PROP_DATA_LEN   ((uint8)10)


/*
** Global function-like macros.
*/		
#define KnxMSG_GetMessagePtr(pBuffer)           ((KNX_StandardFrameRefType)(pBuffer)->msg)
#define KnxMSG_GetProperyFramePtr(pBuffer)      ((KNX_PropertyFrameRefType)(pBuffer)->msg)
#define KnxMSG_GetPollingFramePtr(pBuffer)      ((KNX_PollingFrameRefType)(pBuffer)->msg)

/**************/
#define KnxMSG_GetFrameType(pBuffer)            ((KNX_FrameTypeType)(KnxMSG_GetMessagePtr((pBuffer))->ctrl) & (uint8)0xc0)
#define KnxMSG_SetFrameType(pBuffer, type)      (KnxMSG_GetMessagePtr((pBuffer))->ctrl |= ((type) & (uint8)0xc0))

#define KnxMSG_GetSourceAddress(pBuffer)        ((Knx_AddressType)btohs(*(uint16 *)&KnxMSG_GetMessagePtr((pBuffer))->source))
#define KnxMSG_GetDestAddress(pBuffer)          ((Knx_AddressType)btohs(*(uint16 *)&KnxMSG_GetMessagePtr((pBuffer))->dest))
#define KnxMSG_SetSourceAddress(pBuffer, addr)  (*(uint16 *)&KnxMSG_GetMessagePtr((pBuffer))->source = htobs((addr)))
#define KnxMSG_SetDestAddress(pBuffer, addr)    (*(uint16 *)&KnxMSG_GetMessagePtr((pBuffer))->dest = htobs((addr)))

#define KnxMSG_GetPriority(pBuffer)             ((KNX_PriorityType)(KnxMSG_GetMessagePtr((pBuffer))->ctrl & (uint8)0x0C) >> 2)
#define KnxMSG_SetPriority(pBuffer, priority)   (KnxMSG_GetMessagePtr((pBuffer))->ctrl |= (((priority) & (uint8)0x03) << 2))

/* check: Daf-Type, DestionationAddressType??? */
#define KnxMSG_GetAddressType(pBuffer)          ((uint8)KnxMSG_GetMessagePtr((pBuffer))->ncpi & (uint8)0x80)
#define KnxMSG_SetAddressType(pBuffer, at)      (KnxMSG_GetMessagePtr((pBuffer))->ncpi |= ((at) & (uint8)0x80))

/* check: ist 'LSDU' richtig??? */
#define KnxMSG_GetLSDULen(pBuffer)              (KnxMSG_GetMessagePtr((pBuffer))->ncpi & (uint8)0x0f)
#define KnxMSG_SetLSDULen(pBuffer, len_lsdu)    (KnxMSG_GetMessagePtr((pBuffer))->ncpi = ((len_lsdu) & (uint8)0x0f))

#define KnxMSG_GetTPCI(pBuffer)                 ((uint8)KnxMSG_GetMessagePtr((pBuffer))->tpci)
#define KnxMSG_SetTPCI(pBuffer, tp)             (KnxMSG_GetMessagePtr((pBuffer))->tpci |= (tp))

#define KnxMSG_GetSeqNo(pBuffer)                ((uint8)((KnxMSG_GetMessagePtr((pBuffer))->tpci) & (uint8)0x3c) >> 2)
#define KnxMSG_SetSeqNo(pBuffer, SeqNo)         (KnxMSG_GetMessagePtr((pBuffer))->tpci |= (((SeqNo) & (uint8)0x0f) << 2))

/* check: geht 'GetAPCI' nicht effizienter??? */
#define KnxMSG_GetAPCI(pBuffer)                 ((uint16)(KnxMSG_GetMessagePtr((pBuffer))->tpci << \
                                                          8) | (KnxMSG_GetMessagePtr((pBuffer))->apci))
#define KnxMSG_SetAPCI(pBuffer, apci)           (*(uint16 *)&(pBuffer)->msg[6] = htobs((apci)))

#define KnxMSG_GetIAK(pBuffer)                  ((uint8)KnxMSG_GetMessagePtr((pBuffer))->ctrl & (uint8)0x01)


/*
** Global types.
*/	
typedef uint8 Knx_MessageType[MSG_LEN];

typedef struct tagKnxMSG_Buffer {
    uint8               next;
    uint8               len;
    Knx_ServiceTypeType service;
    uint8               sap;
    Knx_MessageType     msg;
} KnxMSG_Buffer, * KnxMSG_BufferPtr;

typedef struct tagKNX_StandardFrameType {
    uint8   ctrl;
    uint8   source[2];
    uint8   dest[2];
    uint8   ncpi;
    uint8   tpci;
    uint8   apci;
    uint8   data[MAX_ADPU_LEN];
} KNX_StandardFrameType, * KNX_StandardFrameRefType;    /* KNX_StandardFrameType */

typedef struct tagKNX_PropertyFrameType {
    uint8   ctrl;
    uint8   source[2];
    uint8   dest[2];
    uint8   ncpi;
    uint8   tpci;
    uint8   apci;
    uint8   obj_id;
    uint8   prop_id;
    uint8   num_elems;
    uint8   start_index;
    uint8   data[MAX_PROP_DATA_LEN];
} KNX_PropertyFrameType, * KNX_PropertyFrameRefType;    /* KNX_PropertyFrameType */

typedef struct tagKNX_PollingFrameType {
    uint8   ctrl;
    uint8   source[2];
    uint8   poll_addr[2];
    uint8   num_slots;
    uint8   slot[MAX_ADPU_LEN];
} KNX_PollingFrameType, * KNX_PollingFrameRefType;  /* KNX_PollingFrameType */



/*
** Global functions.
*/	
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE)		    KnxMSG_Init(void);
FUNC(KnxMSG_BufferPtr, KSTACK_CODE) KnxMSG_AllocateBuffer(void);
FUNC(boolean, KSTACK_CODE)	    KnxMSG_ReleaseBuffer(KnxMSG_BufferPtr ptr);
FUNC(boolean, KSTACK_CODE)	    KnxMSG_ClearBuffer(KnxMSG_BufferPtr ptr);
FUNC(boolean, KSTACK_CODE)	    KnxMSG_Post(KnxMSG_BufferPtr ptr);
FUNC(KnxMSG_BufferPtr, KSTACK_CODE) KnxMSG_Get(uint8 task);
FUNC(void, KSTACK_CODE)		    KnxMSG_RedirectToUser(uint8 layer); /* U_MS_Switch */

FUNC(void, KSTACK_CODE)		    KnxMSG_SetLen(KnxMSG_BufferPtr pBuffer, uint8 len);
FUNC(uint8, KSTACK_CODE)	    KnxMSG_GetLen(const KnxMSG_BufferPtr pBuffer);

FUNC(uint8, KSTACK_CODE)	    KnxMSG_GetRoutingCount(const KnxMSG_BufferPtr pBuffer);
FUNC(void, KSTACK_CODE)		    KnxMSG_SetRoutingCount(KnxMSG_BufferPtr pBuffer);

FUNC(void, KSTACK_CODE)		    KnxMSG_SetRoutingCtrl(KnxMSG_BufferPtr pBuffer, boolean on);
FUNC(boolean, KSTACK_CODE)	    KnxMSG_GetRoutingCtrl(const KnxMSG_BufferPtr pBuffer);
#else
void		    KnxMSG_Init(void);
KnxMSG_BufferPtr    KnxMSG_AllocateBuffer(void);
boolean		    KnxMSG_ReleaseBuffer(KnxMSG_BufferPtr ptr);
boolean		    KnxMSG_ClearBuffer(KnxMSG_BufferPtr ptr);
boolean		    KnxMSG_Post(KnxMSG_BufferPtr ptr);
KnxMSG_BufferPtr    KnxMSG_Get(uint8 task);
void		    KnxMSG_RedirectToUser(uint8 layer); /* U_MS_Switch */

void		    KnxMSG_SetLen(KnxMSG_BufferPtr pBuffer, uint8 len);
uint8		    KnxMSG_GetLen(const KnxMSG_BufferPtr pBuffer);

uint8		    KnxMSG_GetRoutingCount(const KnxMSG_BufferPtr pBuffer);
void		    KnxMSG_SetRoutingCount(KnxMSG_BufferPtr pBuffer);

void		    KnxMSG_SetRoutingCtrl(KnxMSG_BufferPtr pBuffer, boolean on);
boolean		    KnxMSG_GetRoutingCtrl(const KnxMSG_BufferPtr pBuffer);
#endif /* KSTACK_MEMORY_MAPPING */

/* uint8 GetQueueForService(uint8 service); */
/* PMSG_Buffer GetBufferAddress(uint8 buf_num); */
/* uint8 GetBufferNumber(PMSG_Buffer addr); */
/* void ClearMessageBuffer(uint8 buf_num); */
/* void MSG_Test(void); */
/* void MSG_SetHopCount(PMSG_Buffer pBuffer,uint8 hop_count); */
/* void MSG_GetHopCount(PMSG_Buffer pBuffer,uint8 *hop_count); */

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /*__KNX_MESSAGING_H */

