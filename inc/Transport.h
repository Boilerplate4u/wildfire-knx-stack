/*
*   Wildfire - The Open Source KNX/EIB-Protocol Stack.
*
*  (C) 2007-2014 by Christoph Schueler <github.com/Christoph2,
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
#if !defined(__TRANSPORT_H)
#define __TRANSPORT_H

#include "Address.h"
#include "Messaging.h"
#include "StateMachineKNX.h"
#include "Timer.h"
#include "knx_disp.h"

#if defined(__cplusplus)
extern "C"
{
#endif  /* __cplusplus */

/*
**
** Global defines.
**
*/

/*
**  TPCI-Codings.
*/
#define TPCI_UDT                ((uint8_t)0x00)       /* Unnumbered Data. */
#define TPCI_NDT                ((uint8_t)0x40)       /* Numbered Data (T_DATA_CONNECTED_REQ_PDU). */
#define TPCI_UCD                ((uint8_t)0x80)       /* Unnumbered Control. */
#define TPCI_NCD                ((uint8_t)0xC0)       /* Numbered Control (TACK/TNACK). */

#define T_ACK_PDU               ((uint8_t)0xC2)
#define T_NAK_PDU               ((uint8_t)0xC3)

#define T_CONNECT_REQ_PDU       ((uint8_t)0x80)
#define T_DISCONNECT_REQ_PDU    ((uint8_t)0x81)

#define T_DATA_TAG_GROUP_PDU    ((uint8_t)0x04)    /* Interface-Objects using Group-Addressing (LTE-HEE). */

/*
** Global functions.
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE)         TL_Init(void);
FUNC(void, KSTACK_CODE)         TLG_Task(void);
FUNC(void, KSTACK_CODE)         TLC_Task(void);

FUNC(void, KSTACK_CODE)         T_Connect_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source,
                                              Knx_AddressType dest
                                              );
FUNC(void, KSTACK_CODE)         T_Disconnect_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source,
                                                 Knx_AddressType dest
                                                 );

FUNC(void, KSTACK_CODE)         T_Ack_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source,
                                          Knx_AddressType dest, uint8_t SeqNo
                                          );
FUNC(void, KSTACK_CODE)         T_Nak_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source,
                                          Knx_AddressType dest, uint8_t SeqNo
                                          );

/* TODO: 'knx_tlc.h' !!! */
FUNC(uint8_t, KSTACK_CODE)        KNXTlc_GetSequenceNumberSend(void);
FUNC(uint8_t, KSTACK_CODE)        KNXTlc_GetSequenceNumberReceived(void);
FUNC(uint8_t, KSTACK_CODE)        KNXTlc_GetRepetitionCount(void);
FUNC(uint8_t, KSTACK_CODE)        KNXTlc_GetSequenceNumberOfPDU(void);
FUNC(Knx_AddressType, KSTACK_CODE)  KNXTlc_GetSourceAddress(void);
FUNC(Knx_AddressType, KSTACK_CODE)  Knx_AddressType KNXTlc_GetConnectionAddress(void);


FUNC(void, KSTACK_CODE)         KNXTlc_SetSequenceNumberSend(uint8_t SequenceNumberSend);
FUNC(void, KSTACK_CODE)         KNXTlc_SetSequenceNumberReceived(uint8_t SequenceNumberReceived);
FUNC(void, KSTACK_CODE)         KNXTlc_SetRepetitionCount(uint8_t RepetitionCount);
FUNC(void, KSTACK_CODE)         KNXTlc_SetSequenceNumberOfPDU(uint8_t SequenceNumberOfPDU);
FUNC(void, KSTACK_CODE)         KNXTlc_SetSourceAddress(Knx_AddressType SourceAddress);
FUNC(void, KSTACK_CODE)         KNXTlc_SetConnectionAddress(Knx_AddressType ConnectionAddress);
#else
void    TL_Init(void);
void    TLG_Task(void);
void    TLC_Task(void);

void    T_Connect_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);
void    T_Disconnect_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest);

void    T_Ack_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t SeqNo);
void    T_Nak_Req(KnxMsg_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t SeqNo);


/* TODO: 'knx_tlc.h' !!! */
uint8_t           KNXTlc_GetSequenceNumberSend(void);
uint8_t           KNXTlc_GetSequenceNumberReceived(void);
uint8_t           KNXTlc_GetRepetitionCount(void);
uint8_t           KNXTlc_GetSequenceNumberOfPDU(void);
Knx_AddressType KNXTlc_GetSourceAddress(void);
Knx_AddressType KNXTlc_GetConnectionAddress(void);

void    KNXTlc_SetSequenceNumberSend(uint8_t SequenceNumberSend);
void    KNXTlc_SetSequenceNumberReceived(uint8_t SequenceNumberReceived);
void    KNXTlc_SetRepetitionCount(uint8_t RepetitionCount);
void    KNXTlc_SetSequenceNumberOfPDU(uint8_t SequenceNumberOfPDU);
void    KNXTlc_SetSourceAddress(Knx_AddressType SourceAddress);
void    KNXTlc_SetConnectionAddress(Knx_AddressType ConnectionAddress);


#endif /* KSTACK_MEMORY_MAPPING */

#if defined(__cplusplus)
}
#endif  /* __cplusplus */

#endif  /* __TRANSPORT_H */

