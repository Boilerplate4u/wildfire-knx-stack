#if !defined(__MESSAGING_H)
#define __MESSAGING_H

/*
**  Hinweis: es w�re _sehr_ sinnvoll, wenn Puffer auf ohne 'MSG_ClearBuffer'  benutzbar w�ren!!!
*/

/*
**  todo: Konstanten f�r der einzelnen Felder der Message!!!
*/

/*
**  todo: Get-Funktionen �berarbeiten (Wert zur�ckliefern,statt Pointer-�bergabe).
**
**
*/

#include "KNXDefs.h"
#include "Address.h"

#define MSG_NO_ROUTING_CTRL ((uint8)7)

#define MSG_NUM_TASKS       ((uint8)10)
#define MSG_NUM_BUFFERS     ((uint8)4)

#define MSG_QUEUE_EMPTY     ((uint8)0xff)
#define MSG_INVALID_BUFFER  ((uint8)0xff)
#define MSG_NO_NEXT         ((uint8)0xff)

#define TPCI_INVALID        ((uint8)0xbf)
#define APCI_INVALID        ((uint16)0x3FF)

#define TASK_FREE_ID        ((uint8)0)       /* Free message buffers. */
#define TASK_LL_ID          ((uint8)1)       /* Link-Layer. */
#define TASK_NL_ID          ((uint8)2)       /* Network-Layer. */
#define TASK_TL_ID          ((uint8)3)       /* Transport-Layer/Multicast. */
#define TASK_TC_ID          ((uint8)4)       /* Transport-Layer/Multicast&one-to-one. */
#define TASK_LC_ID          ((uint8)5)       /* Transport-Layer/Local. */
#define TASK_AL_ID          ((uint8)6)       /* Application-Layer/Group-Oriented. */
#define TASK_MG_ID          ((uint8)7)       /* Application-Layer/Managment. */
#define TASK_PM_ID          ((uint8)8)       /* PEI-System. */
#define TASK_US_ID          ((uint8)9)       /* User. */
#define TASK_INVALID_ID     ((uint8)0xff)    /* Invalid Queue. */

/* todo: Support f. Extended-Frames!!! */
#define MSG_LEN             ((uint8)22)
#define MAX_ADPU_LEN        ((uint8)14)
#define MAX_PROP_DATA_LEN   ((uint8)10)


typedef uint8 KNX_MSG_T[MSG_LEN];    /* todo: extended Frames ber�cksichtigen!!! */
                                    /* KNX_MessageType (ist das gegl�ckt???). */

typedef struct tagMSG_Buffer {  /* check: macht es Sinn, die Puffer-Nr. mitzuf�hren??? */
    uint8 next;
    uint8 len;
    KNXServiceTypeType service;
    uint8 sap;
    KNX_MSG_T msg;
} MSG_Buffer,* PMSG_Buffer;      /* KNX_MessageBufferType    (hier ist 'KNX_MessageType' sinnvoll!!!). */

typedef struct tagKNX_StandardFrameType {
    uint8 ctrl;
    uint8 source[2];
    uint8 dest[2];
    uint8 ncpi;                      /* check: richtiger Name??? */
    uint8 tpci;                      /*      "                               " */
    uint8 apci;                      /*      "                               " */
    uint8 data[MAX_ADPU_LEN];
} KNX_StandardFrameType,*KNX_StandardFrameRefType;    /* KNX_StandardFrameType */

typedef struct tagKNX_PropertyFrameType {
    uint8 ctrl;
    uint8 source[2];
    uint8 dest[2];        
    uint8 ncpi;                      /* check: richtiger Name??? */
    uint8 tpci;                      /*      "                               " */
    uint8 apci;      
    uint8 obj_id;
    uint8 prop_id;
    uint8 num_elems;
    uint8 start_index;
    uint8 data[MAX_PROP_DATA_LEN];
} KNX_PropertyFrameType,*KNX_PropertyFrameRefType;    /* KNX_PropertyFrameType */

typedef struct tagKNX_PollingFrameType {
    uint8 ctrl;
    uint8 source[2];
    uint8 poll_addr[2];          
    uint8 num_slots;
    uint8 slot[MAX_ADPU_LEN];
} KNX_PollingFrameType,*KNX_PollingFrameRefType;  /* KNX_PollingFrameType */


/* uint8 GetQueueForService(uint8 service); */
/* PMSG_Buffer GetBufferAddress(uint8 buf_num); */
/* uint8 GetBufferNumber(PMSG_Buffer addr); */
/* void ClearMessageBuffer(uint8 buf_num); */

void MSG_Init(void);
PMSG_Buffer MSG_AllocateBuffer(void);
boolean MSG_ReleaseBuffer(PMSG_Buffer ptr);
boolean MSG_ClearBuffer(PMSG_Buffer ptr);
boolean MSG_Post(PMSG_Buffer ptr);
PMSG_Buffer MSG_Get(uint8 task);
void MSG_RedirectToUser(uint8 layer);    /* U_MS_Switch */

/* void MSG_Test(void); */

#define MSG_GetMessagePtr(pBuffer)          ((KNX_StandardFrameRefType)(pBuffer)->msg)
#define MSG_GetProperyFramePtr(pBuffer)     ((KNX_PropertyFrameRefType)(pBuffer)->msg)
#define MSG_GetPollingFramePtr(pBuffer)     ((KNX_PollingFrameRefType)(pBuffer)->msg)

/**************/
#define MSG_GetFrameType(pBuffer)           ((KNX_FrameTypeType)(MSG_GetMessagePtr((pBuffer))->ctrl) & 0xc0))
#define MSG_SetFrameType(pBuffer,type)      (MSG_GetMessagePtr((pBuffer))->ctrl|=((type) & 0xc0))

#define MSG_GetSourceAddress(pBuffer)       ((Knx_AddressType)btohs(*(uint16*)&MSG_GetMessagePtr((pBuffer))->source))
#define MSG_GetDestAddress(pBuffer)         ((Knx_AddressType)btohs(*(uint16*)&MSG_GetMessagePtr((pBuffer))->dest))
#define MSG_SetSourceAddress(pBuffer,addr)  (*(uint16*)&MSG_GetMessagePtr((pBuffer))->source=htobs((addr)))
#define MSG_SetDestAddress(pBuffer,addr)    (*(uint16*)&MSG_GetMessagePtr((pBuffer))->dest=htobs((addr)))

#define MSG_GetPriority(pBuffer)            ((KNX_PriorityType)(MSG_GetMessagePtr((pBuffer))->ctrl & 0x0C)>>2)
#define MSG_SetPriority(pBuffer,priority)   (MSG_GetMessagePtr((pBuffer))->ctrl|= (((priority) & 0x03)<<2)) 

/* check: Daf-Type, DestionationAddressType??? */
#define MSG_GetAddressType(pBuffer)         ((uint8)MSG_GetMessagePtr((pBuffer))->ncpi & 0x80)
#define MSG_SetAddressType(pBuffer,at)      (MSG_GetMessagePtr((pBuffer))->ncpi |= ((at) & 0x80))

/* check: ist 'LSDU' richtig??? */
#define MSG_GetLSDULen(pBuffer)             (MSG_GetMessagePtr((pBuffer))->ncpi & 0x0f)
#define MSG_SetLSDULen(pBuffer,len_lsdu)    (MSG_GetMessagePtr((pBuffer))->ncpi=((len_lsdu) & 0x0f))

#define MSG_GetTPCI(pBuffer)                ((uint8)MSG_GetMessagePtr((pBuffer))->tpci)
#define MSG_SetTPCI(pBuffer,tp)             (MSG_GetMessagePtr((pBuffer))->tpci|=(tp))   /* todo: !!!TESTEN!!! */

#define MSG_GetSeqNo(pBuffer)               ((uint8)((MSG_GetMessagePtr((pBuffer))->tpci) & 0x3c) >> 2)
#define MSG_SetSeqNo(pBuffer,SeqNo)         (MSG_GetMessagePtr((pBuffer))->tpci|=(((SeqNo) & 0x0f) << 2))

/* check: geht 'GetAPCI' nicht effizienter??? */
#define MSG_GetAPCI(pBuffer)                ((uint16)(MSG_GetMessagePtr((pBuffer))->tpci << 8) | (MSG_GetMessagePtr((pBuffer))->apci))
#define MSG_SetAPCI(pBuffer,apci)           (*(uint16*)&(pBuffer)->msg[6]=htobs((apci)))

#define MSG_GetIAK(pBuffer)                 ((uint8)MSG_GetMessagePtr((pBuffer))->ctrl & 0x01)


void MSG_SetLen(PMSG_Buffer pBuffer,uint8 len);
uint8 MSG_GetLen(PMSG_Buffer pBuffer);

uint8 MSG_GetRoutingCount(PMSG_Buffer pBuffer);
void MSG_SetRoutingCount(PMSG_Buffer pBuffer);

void MSG_SetRoutingCtrl(PMSG_Buffer pBuffer,boolean on);
boolean MSG_GetRoutingCtrl(PMSG_Buffer pBuffer);

/* void MSG_SetHopCount(PMSG_Buffer pBuffer,uint8 hop_count); */
/* void MSG_GetHopCount(PMSG_Buffer pBuffer,uint8 *hop_count); */

#endif /*__MESSAGING_H */
