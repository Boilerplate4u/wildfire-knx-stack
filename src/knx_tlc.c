/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2011 by Christoph Schueler <chris@konnex-tools.de,
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
#include "Transport.h"

static void Disp_T_DataConnectedReq(void),Disp_T_ConnectReq(void),Disp_T_DisconnectReq(void);
static void Disp_N_Data_IndividualInd(void),Disp_T_DataIndividualReq(void),Disp_T_DataBroadcastReq(void);
static void Disp_N_DataBroadcastInd(void),Disp_N_DataIndividualCon(void),Disp_N_DataBroadcast_Con(void);

static uint8 KNXTlc_SequenceNumberSend,KNXTlc_SequenceNumberReceived;
static uint8 KNXTlc_RepetitionCount,KNXTlc_SequenceNumberOfPDU;
static Knx_AddressType KNXTlc_SourceAddress,KNXTlc_ConnectionAddress;

static const KNXLayerServiceFunctionType TLC_Services[]={
/*      Service                     Handler                 */
/*      ====================================================*/
/*      N_DATA_INDIVIDUAL_IND   */  Disp_N_Data_IndividualInd,
/*      N_DATA_INDIVIDUAL_CON   */  Disp_N_DataIndividualCon,
/*      N_DATA_BROADCAST_IND    */  Disp_N_DataBroadcastInd,
/*      N_DATA_BROADCAST_CON    */  Disp_N_DataBroadcast_Con,
/*      T_CONNECT_REQ           */  Disp_T_ConnectReq,
/*      T_DISCONNECT_REQ        */  Disp_T_DisconnectReq,
/*      T_DATA_CONNECTED_REQ    */  Disp_T_DataConnectedReq,
/*      T_DATA_INDIVIDUAL_REQ   */  Disp_T_DataIndividualReq,
/*      T_DATA_BROADCAST_REQ    */  Disp_T_DataBroadcastReq
/*      ====================================================*/
};

static const KNXLayerServicesType TLC_ServiceTable[]={
    {KNX_TLC_SERVICES,9,TLC_Services}
};

void TLC_Task(void)
{
	if (TM_IsExpired(TM_TIMER_TLC_CON_TIMEOUT)) {
		KNXTlc_StateMachine(tlcTIMEOUT_CON);
	}

	if (TM_IsExpired(TM_TIMER_TLC_ACK_TIMEOUT)) {
		KNXTlc_StateMachine(tlcTIMEOUT_ACK);
	}

        KNXDispDispatchLayer(TASK_TC_ID,TLC_ServiceTable);
}


void TL_Init(void)
{
    KNXTlc_SetSequenceNumberSend((uint8)0);
    KNXTlc_SetSequenceNumberReceived((uint8)0);
    KNXTlc_SetRepetitionCount((uint8)0);
    KNXTlc_SetSequenceNumberOfPDU((uint8)0);

    KNXTlc_SetState(CLOSED);
}


/********************************************************************/
/********************************************************************/
/********************************************************************/

/*
**
**  Services from Network-Layer.
**
*/

static void Disp_N_Data_IndividualInd(void)
{
    uint8 tpci;

    KNXTlc_SetSourceAddress(MSG_GetSourceAddress(MSG_ScratchBuffer)); /* todo: !!! TESTEN !!! */

    tpci=MSG_GetTPCI(MSG_ScratchBuffer) & (uint8)0xc0;
    switch (tpci) {
        case TPCI_UDT:   /* Unnumbered Data (1:1-Connection-Less). */
            MSG_ScratchBuffer->service=T_DATA_INDIVIDUAL_IND;
            (void)MSG_Post(MSG_ScratchBuffer);
            break;
        case TPCI_NDT:   /* Numbered Data (T_DATA_CONNECTED_REQ_PDU, 1:1-Connection-Oriented). */
            KNXTlc_SetSequenceNumberOfPDU(MSG_GetSeqNo(MSG_ScratchBuffer));
            KNXTlc_StateMachine((KNX_TlcEventType)tlcDATA_CONNECTED_IND);
            break;
        case TPCI_UCD:   /* Unnumbered Control. (CONNECT|DISCONNECT). */
            tpci=MSG_GetTPCI(MSG_ScratchBuffer);

            if (tpci==T_CONNECT_REQ_PDU) {
                /* T_CONNECT_IND */
                KNXTlc_StateMachine((KNX_TlcEventType)tlcCONNECT_IND);
            } else if (tpci==T_DISCONNECT_REQ_PDU) {
                /* T_DISCONNECT_IND */
                KNXTlc_StateMachine((KNX_TlcEventType)tlcDISCONNECT_IND);
            } else {
                (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
            }
            break;
        case TPCI_NCD:   /* Numbered Control (TACK|TNACK). */
            tpci=MSG_GetTPCI(MSG_ScratchBuffer) & (uint8)0xC3;  /* 0x03 */
            KNXTlc_SetSequenceNumberOfPDU(MSG_GetSeqNo(MSG_ScratchBuffer));

            if (tpci==T_ACK_PDU) {
                KNXTlc_StateMachine((KNX_TlcEventType)tlcACK_IND);
            } else if (tpci==T_NAK_PDU) {
                KNXTlc_StateMachine((KNX_TlcEventType)tlcNAK_IND);
            } else {
                (void)MSG_ReleaseBuffer(MSG_ScratchBuffer);
            }
            break;
        default:
            ASSERT(FALSE);
    }
}

static void Disp_N_DataBroadcastInd(void)
{
    MSG_ScratchBuffer->service=T_DATA_BROADCAST_IND;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_N_DataIndividualCon(void)
{
    /* todo: Implement !!! */
}

static void Disp_N_DataBroadcast_Con(void)
{
    MSG_ScratchBuffer->service=T_DATA_BROADCAST_CON;
    (void)MSG_Post(MSG_ScratchBuffer);
}

/*
**
**  Services from Application-Layer.
**
*/

static void Disp_T_DataIndividualReq(void)
{
    MSG_SetTPCI(MSG_ScratchBuffer,TPCI_UDT);
    MSG_ScratchBuffer->service=N_DATA_INDIVIDUAL_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_T_DataConnectedReq(void)
{
    MSG_SetTPCI(MSG_ScratchBuffer,TPCI_NDT);
    MSG_ScratchBuffer->service=N_DATA_INDIVIDUAL_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_T_ConnectReq(void)
{
    MSG_SetTPCI(MSG_ScratchBuffer,TPCI_UCD);
    MSG_ScratchBuffer->service=N_DATA_INDIVIDUAL_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_T_DisconnectReq(void)
{
    MSG_SetTPCI(MSG_ScratchBuffer,TPCI_UCD);
    MSG_ScratchBuffer->service=N_DATA_INDIVIDUAL_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}

static void Disp_T_DataBroadcastReq(void)
{
    MSG_SetTPCI(MSG_ScratchBuffer,TPCI_UDT);
    MSG_ScratchBuffer->service=N_DATA_BROADCAST_REQ;
    (void)MSG_Post(MSG_ScratchBuffer);
}


/********************************************************************/
/********************************************************************/
/********************************************************************/

void T_Connect_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest)
{
    MSG_SetTPCI(pBuffer,T_CONNECT_REQ_PDU);
    MSG_SetSourceAddress(pBuffer,source);
    MSG_SetDestAddress(pBuffer,dest);
    MSG_SetPriority(pBuffer,KNX_OBJ_PRIO_SYSTEM);
    MSG_SetLen(pBuffer,(uint8)7);
    pBuffer->service=N_DATA_INDIVIDUAL_REQ; /* T_CONNECT_REQ; */

    (void)MSG_Post(pBuffer);
}

void T_Disconnect_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest)
{
    MSG_SetTPCI(pBuffer,T_DISCONNECT_REQ_PDU);
    MSG_SetSourceAddress(pBuffer,source);
    MSG_SetDestAddress(pBuffer,dest);
    MSG_SetPriority(pBuffer,KNX_OBJ_PRIO_SYSTEM);
    MSG_SetLen(pBuffer,7);
    pBuffer->service=N_DATA_INDIVIDUAL_REQ; /* T_DISCONNECT_REQ; */

    (void)MSG_Post(pBuffer);
}

void T_Ack_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest,uint8 SeqNo)
{
    MSG_SetTPCI(pBuffer,T_ACK_PDU | ((SeqNo & (uint8)0x0f)<<2));
    MSG_SetSourceAddress(pBuffer,source);
    MSG_SetDestAddress(pBuffer,dest);
    MSG_SetPriority(pBuffer,KNX_OBJ_PRIO_SYSTEM);
    MSG_SetLen(pBuffer,7);
    pBuffer->service=N_DATA_INDIVIDUAL_REQ;

    (void)MSG_Post(pBuffer);
}

void T_Nak_Req(PMSG_Buffer pBuffer,Knx_AddressType source,Knx_AddressType dest,uint8 SeqNo)
{
    MSG_SetTPCI(pBuffer,T_NAK_PDU | ((SeqNo & (uint8)0x0f)<<2));
    MSG_SetSourceAddress(pBuffer,source);
    MSG_SetDestAddress(pBuffer,dest);
    MSG_SetPriority(pBuffer,KNX_OBJ_PRIO_SYSTEM);
    MSG_SetLen(pBuffer,7);
    pBuffer->service=N_DATA_INDIVIDUAL_REQ;

    (void)MSG_Post(pBuffer);
}


uint8 KNXTlc_GetSequenceNumberSend(void)
{
    return KNXTlc_SequenceNumberSend;
}
uint8 KNXTlc_GetSequenceNumberReceived(void)
{
    return KNXTlc_SequenceNumberReceived;
}

uint8 KNXTlc_GetRepetitionCount(void)
{
    return KNXTlc_RepetitionCount;
}

uint8 KNXTlc_GetSequenceNumberOfPDU(void)
{
    return KNXTlc_SequenceNumberOfPDU;
}

Knx_AddressType KNXTlc_GetSourceAddress(void)
{
    return KNXTlc_SourceAddress;
}

Knx_AddressType KNXTlc_GetConnectionAddress(void)
{
    return KNXTlc_ConnectionAddress;
}

void KNXTlc_SetSequenceNumberSend(uint8 SequenceNumberSend)
{
    KNXTlc_SequenceNumberSend=(SequenceNumberSend & ((uint8)0x0f));
}
void KNXTlc_SetSequenceNumberReceived(uint8 SequenceNumberReceived)
{
    KNXTlc_SequenceNumberReceived=(SequenceNumberReceived & ((uint8)0x0f));
}

void KNXTlc_SetRepetitionCount(uint8 RepetitionCount)
{
    KNXTlc_RepetitionCount=RepetitionCount;
}

void KNXTlc_SetSequenceNumberOfPDU(uint8 SequenceNumberOfPDU)
{
    KNXTlc_SequenceNumberOfPDU=SequenceNumberOfPDU;
}

void KNXTlc_SetSourceAddress(Knx_AddressType SourceAddress)
{
    KNXTlc_SourceAddress=SourceAddress;
}

void KNXTlc_SetConnectionAddress(Knx_AddressType ConnectionAddress)
{
    KNXTlc_ConnectionAddress=ConnectionAddress;
}
