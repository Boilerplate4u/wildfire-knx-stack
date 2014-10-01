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

/*
**  Application-Layer / Managment.
*/

/*
   #ifdef DEBUG
    assert(r==TRUE)
    DEBUG_PRINT("Error releasing Message-Buffer @ " ## __FILE__ ## "(" ## __LINE__ ## ")")
   #endif
 */

#include "knx_layer_application.h"

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_Broadcast_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, uint16_t apci,
                                        P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) data, uint8_t len
                                        );
FUNC(void, KSTACK_CODE) A_Individual_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint16_t apci,
                                         uint8_t * data,
                                         uint8_t len
                                         );
FUNC(void, KSTACK_CODE) KnxAl_SetPropertyHeader(KnxMSG_BufferPtr pBuffer, uint8_t obj_index, uint8_t prop_id, uint8_t nr_of_elem,
                                             uint16_t start_index
                                             );
#else
void A_Broadcast_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, uint16_t apci, uint8_t * data, uint8_t len);
void A_Individual_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint16_t apci, uint8_t * data,
                         uint8_t len);
void KnxAl_SetPropertyHeader(KnxMSG_BufferPtr pBuffer, uint8_t obj_index, uint8_t prop_id, uint8_t nr_of_elem, uint16_t start_index);


#endif /* KSTACK_MEMORY_MAPPING */

#define APDU_PHYS_ADDR ((uint8_t)0)        /* A_IndividualAddress_Write-PDU */

/*
   M_USER_DATA_INDIVIDUAl_REQ
   M_USER_DATA_CONNECTED_REQ
   M_INTERFACEOBJ_DATA_REQ
 */

/*
**  Prefixes:
**  ==========================
**  GOS_ : GroupObject-Server.
**  MMS_ : Memory-Server (check: getrennter UserMemory-Server? - ('MEM_' ?)).
**  PPS_ : Property-Server ('IOS' - 'Interface-Object-Server').
**  DMO_ : Device-Model.
*/

#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_Connect_Ind(void), T_Connect_Con(void), T_Disconnect_Ind(void);
STATIC FUNC(void, KSTACK_CODE) T_Disconnect_Con(void), T_DataConnected_Ind(void), T_DataConnected_Con(void);
STATIC FUNC(void, KSTACK_CODE) T_DataBroadcast_Ind(void), T_DataBroadcast_Con(void), T_DataIndividual_Ind(void);
STATIC FUNC(void, KSTACK_CODE) T_DataIndividual_Con(void);


#else
STATIC void T_Connect_Ind(void), T_Connect_Con(void), T_Disconnect_Ind(void);
STATIC void T_Disconnect_Con(void), T_DataConnected_Ind(void), T_DataConnected_Con(void);
STATIC void T_DataBroadcast_Ind(void), T_DataBroadcast_Con(void), T_DataIndividual_Ind(void);
STATIC void T_DataIndividual_Con(void);


#endif /* KSTACK_MEMORY_MAPPING */

STATIC const Knx_LayerServiceFunctionType ALM_Services[] = {
/*      Service                     Handler                 */
/*      ====================================================*/
/*      T_CONNECT_IND           */ T_Connect_Ind,
/*      T_CONNECT_CON           */ T_Connect_Con,
/*      T_DISCONNECT_IND        */ T_Disconnect_Ind,
/*      T_DISCONNECT_CON        */ T_Disconnect_Con,
/*      T_DATA_CONNECTED_IND    */ T_DataConnected_Ind,
/*      T_DATA_CONNECTED_CON    */ T_DataConnected_Con,
/*      T_DATA_INDIVIDUAL_IND   */ T_DataIndividual_Ind,
/*      T_DATA_INDIVIDUAL_CON   */ T_DataIndividual_Con,
/*      T_DATA_BROADCAST_IND    */ T_DataBroadcast_Ind,
/*      T_DATA_BROADCAST_CON    */ T_DataBroadcast_Con
/*      ====================================================*/
};

STATIC const Knx_LayerServicesType ALM_ServiceTable[] = {
    { KNX_ALM_SERVICES, SIZEOF_ARRAY(ALM_Services), ALM_Services }
};

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxALM_Task(void)
#else
void KnxALM_Task(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxDisp_DispatchLayer(TASK_MG_ID, ALM_ServiceTable);
}


/*
**
**  Services from Transport-Layer.
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_Connect_Ind(void)
#else
STATIC void T_Connect_Ind(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_Disconnect_Ind(void)
#else
STATIC void T_Disconnect_Ind(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_DataConnected_Ind(void)
#else
STATIC void T_DataConnected_Ind(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t apci_type = KnxAl_GetAPCIType(KnxMSG_GetMessagePtr(KnxMSG_ScratchBufferPtr));

    switch (apci_type) {
        case APCI_MEMORY_READ:
            break;
        case APCI_MEMORY_WRITE:
            break;
        case APCI_ESCAPE:
            break;
        case APCI_ADC_READ:
            break;
        case APCI_RESTART:
            break;
        case APCI_USER_MSG:
            break;
        default:
            KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
            break;
    }

    KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_DataBroadcast_Ind(void)
#else
STATIC void T_DataBroadcast_Ind(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    Knx_APCITypeType apci_type = KnxAl_GetAPCIType(KnxMSG_GetMessagePtr(KnxMSG_ScratchBufferPtr));
    Knx_AddressType addr;

    switch (apci_type) {
        case APCI_INDIVIDUAL_ADDRESS_WRITE:
            if (KnxADR_InProgrammingMode()) {  /* todo: only if 'PID_SERVICE_CONTROL' is activatet! */
                KnxAl_GetAPDUData(KnxMSG_GetMessagePtr(KnxMSG_ScratchBufferPtr), APDU_PHYS_ADDR, (uint8_t *)&addr, (uint8_t)2);
/*                                addr[0]=KnxAl_GetAPDUDataByte(pmsg,APDU_PHYS_ADDR); */
/*                                addr[1]=KnxAl_GetAPDUDataByte(pmsg,APDU_PHYS_ADDR+1); */
                KnxADR_SetPhysAddr(addr);
            }

            KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
            break;
        case APCI_INDIVIDUAL_ADDRESS_READ:
            if (KnxADR_InProgrammingMode()) {
                addr = KnxADR_GetPhysAddr();
                (void)KnxMSG_ClearBuffer(KnxMSG_ScratchBufferPtr);
                A_IndividualAddress_Read_Res(KnxMSG_ScratchBufferPtr, addr);
            }
            break;
        case APCI_ESCAPE:
            break;
        default:
            KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
            break;
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_DataIndividual_Ind(void)
#else
STATIC void T_DataIndividual_Ind(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t apci_type = KnxAl_GetAPCIType(KnxMSG_GetMessagePtr(KnxMSG_ScratchBufferPtr));

    if (apci_type == APCI_ESCAPE) {
        switch (KnxMSG_GetAPCI(KnxMSG_ScratchBufferPtr)) {
            case A_PROPERTYVALUE_READ:
                IOS_Dispatch(KnxMSG_ScratchBufferPtr, IOS_PROP_READ, FALSE);
                break;
            case A_PROPERTYVALUE_WRITE:
                IOS_Dispatch(KnxMSG_ScratchBufferPtr, IOS_PROP_WRITE, FALSE);
                break;
            case A_PROPERTYDESCRIPTION_READ:
                IOS_Dispatch(KnxMSG_ScratchBufferPtr, IOS_PROP_DESC_READ, FALSE);
                break;
            default:
                KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
                break;
        }
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_Connect_Con(void)
#else
STATIC void T_Connect_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_Disconnect_Con(void)
#else
STATIC void T_Disconnect_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_DataConnected_Con(void)
#else
STATIC void T_DataConnected_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_DataBroadcast_Con(void)
#else
STATIC void T_DataBroadcast_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
STATIC FUNC(void, KSTACK_CODE) T_DataIndividual_Con(void)
#else
STATIC void T_DataIndividual_Con(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxMSG_ReleaseBuffer(KnxMSG_ScratchBufferPtr);
}


/*
**
**  Services from User-Layer.
**
*/

/*****************************************************************************************************************/
/*****************************************************************************************************************/
/*****************************************************************************************************************/

/* REF: EIB-Handbuch 2/1/1 "EIB-System-Introduction" s. 43. */
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) KnxAl_GetAPCIType(const KNX_StandardFrameRefType pmsg)
#else
Knx_APCITypeType KnxAl_GetAPCIType(const KNX_StandardFrameRefType pmsg)
#endif /* KSTACK_MEMORY_MAPPING */
{
    return (Knx_APCITypeType)((pmsg->tpci & (uint8_t)0x03) << 2) | ((pmsg->apci & (uint8_t)0xC0) >> 6);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(uint8_t, KSTACK_CODE) KnxAl_GetAPDUDataByte(const KNX_StandardFrameRefType pmsg, uint8_t offset)
#else
uint8_t KnxAl_GetAPDUDataByte(const KNX_StandardFrameRefType pmsg, uint8_t offset)
#endif /* KSTACK_MEMORY_MAPPING */
{
    offset = MIN(offset, MAX_ADPU_LEN - (uint8_t)1);

    return pmsg->data[offset];
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxAl_SetAPDUDataByte(const KNX_StandardFrameRefType pmsg, uint8_t offset, const uint8_t value)
#else
void KnxAl_SetAPDUDataByte(const KNX_StandardFrameRefType pmsg, uint8_t offset, const uint8_t value)
#endif /* KSTACK_MEMORY_MAPPING */
{
    offset = MIN(offset, MAX_ADPU_LEN - (uint8_t)1);

    pmsg->data[offset] = value;
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxAl_GetAPDUData(const KNX_StandardFrameRefType pmsg, uint8_t offset,
                                          P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) data, uint8_t len
                                          )
#else
void KnxAl_GetAPDUData(const KNX_StandardFrameRefType pmsg, uint8_t offset, uint8_t * data, uint8_t len)
#endif /* KSTACK_MEMORY_MAPPING */
{
    offset = MIN(offset, MAX_ADPU_LEN - (uint8_t)1);
    len    = MIN(len, MAX_ADPU_LEN - offset);

    if (len > (uint8_t)0 && (data != (uint8_t *)NULL)) {
        Utl_MemCopy(data, (uint8_t *)pmsg->data + offset, len);
    }
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxAl_SetAPDUData(const KNX_StandardFrameRefType pmsg, uint8_t offset,
                                          P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) data, uint8_t len
                                          )
#else
void KnxAl_SetAPDUData(const KNX_StandardFrameRefType pmsg, uint8_t offset, uint8_t * data, uint8_t len
                       )
#endif /* KSTACK_MEMORY_MAPPING */
{
    offset = MIN(offset, MAX_ADPU_LEN - (uint8_t)1);
    len    = MIN(len, MAX_ADPU_LEN - offset);

    if ((len > (uint8_t)0) && (data != (uint8_t *)NULL)) {
        Utl_MemCopy((uint8_t *)pmsg->data + offset, data, len);
    }
}


/*
**
**
**  BROADCAST-Services.
**
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_Broadcast_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, uint16_t apci,
                                        P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) data, uint8_t len
                                        )
#else
void A_Broadcast_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, uint16_t apci, uint8_t * data, uint8_t len)
#endif /* KSTACK_MEMORY_MAPPING */
{

    len = MIN(len, MAX_ADPU_LEN);

    (void)KnxMSG_ClearBuffer(pBuffer);

    KnxMSG_SetAPCI(pBuffer, apci);
    KnxMSG_SetSourceAddress(pBuffer, source);
    KnxMSG_SetPriority(pBuffer, KNX_OBJ_PRIO_SYSTEM);
    KnxMSG_SetLen(pBuffer, (uint8_t)8 + len);
    KnxAl_SetAPDUData(KnxMSG_GetMessagePtr(pBuffer), (uint8_t)0, data, len);
    pBuffer->service = T_DATA_BROADCAST_REQ;

    (void)KnxMSG_Post(pBuffer);
}


/*
**  A_IndividualAddress
*/

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_IndividualAddress_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source)
#else
void A_IndividualAddress_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source)
#endif /* KSTACK_MEMORY_MAPPING */
{
    A_Broadcast_Req(pBuffer, source, A_PHYSICALADDRESS_RESPONSE, (uint8_t *)NULL, (uint8_t)0);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_IndividualAddress_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source)
#else
void A_IndividualAddress_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source)
#endif /* KSTACK_MEMORY_MAPPING */
{
    A_Broadcast_Req(pBuffer, source, A_PHYSICALADDRESS_READ, (uint8_t *)NULL, (uint8_t)0);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_IndividualAddress_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType newaddress)
#else
void A_IndividualAddress_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType newaddress)
#endif /* KSTACK_MEMORY_MAPPING */
{
/*    uint8_t data[2]; */

/*
**    data[0]=newaddress[0];
**    data[1]=newaddress[1];
**    A_Broadcast_Req(pBuffer,source,A_PHYSICALADDRESS_WRITE,(uint8_t*)data,2);
*/
    A_Broadcast_Req(pBuffer, source, A_PHYSICALADDRESS_WRITE, (uint8_t *)&source, (uint8_t)2);
}


/*
**  A_IndividualAddressSerialNumber
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_IndividualAddressSerialNumber_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                                 Knx_SerialNumberType serial_number,
                                                                 Knx_AddressType domain_addr)
#else
void A_IndividualAddressSerialNumber_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_SerialNumberType serial_number,
                                              Knx_AddressType domain_addr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxAl_SetAPDUData(KnxMSG_GetMessagePtr(pBuffer), (uint8_t)0, serial_number, sizeof(Knx_SerialNumberType));
    KnxAl_SetAPDUData(KnxMSG_GetMessagePtr(pBuffer), (uint8_t)6, (uint8_t *)&domain_addr, sizeof(Knx_AddressType));

    A_Broadcast_Req(pBuffer, source, A_PHYSICALADDRESSSERIALNUMBER_RESPONSE, (uint8_t *)NULL, (uint8_t)10);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_IndividualAddressSerialNumber_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                                 Knx_SerialNumberType serial_number)
#else
void A_IndividualAddressSerialNumber_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_SerialNumberType serial_number)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxAl_SetAPDUData(KnxMSG_GetMessagePtr(pBuffer), (uint8_t)0, serial_number, sizeof(Knx_SerialNumberType));

    A_Broadcast_Req(pBuffer, source, A_PHYSICALADDRESSSERIALNUMBER_READ, (uint8_t *)NULL, (uint8_t)6);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_IndividualAddressSerialNumber_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                                  Knx_SerialNumberType serial_number,
                                                                  Knx_AddressType new_addr)
#else
void A_IndividualAddressSerialNumber_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_SerialNumberType serial_number,
                                               Knx_AddressType new_addr)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxAl_SetAPDUData(KnxMSG_GetMessagePtr(pBuffer), (uint8_t)0, serial_number, sizeof(Knx_SerialNumberType));
    KnxAl_SetAPDUData(KnxMSG_GetMessagePtr(pBuffer), (uint8_t)6, (uint8_t *)&new_addr, sizeof(Knx_AddressType));

    A_Broadcast_Req(pBuffer, source, A_PHYSICALADDRESSSERIALNUMBER_WRITE, (uint8_t *)NULL, (uint8_t)12);
}


/*
**  A_ServiceInformation_Indication_Write
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_ServiceInformation_Indication_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, uint8_t info)
#else
void A_ServiceInformation_Indication_Write_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, uint8_t info)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t data[3];

    data[0]    = info;
    data[1]    = data[2] = (uint8_t)0x00; /* reserved. */

    A_Broadcast_Req(pBuffer, source, A_SERVICEINFORMATION_INDICATION_WRITE, (uint8_t *)data, 3);
}


/*
**  A_DomainAddress
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_DomainAddress_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source)
#else
void A_DomainAddress_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source)
#endif /* KSTACK_MEMORY_MAPPING */
{
    A_Broadcast_Req(pBuffer, source, A_DOMAINADDRESS_RESPONSE, (uint8_t *)NULL, (uint8_t)0);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_DomainAddress_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source)
#else
void A_DomainAddress_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source)
#endif /* KSTACK_MEMORY_MAPPING */
{
    A_Broadcast_Req(pBuffer, source, A_DOMAINADDRESS_READ, (uint8_t *)NULL, (uint8_t)0);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_DomainAddress_Write_req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType domain_ddress)
#else
void A_DomainAddress_Write_req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType domain_ddress)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t data[2];

    data[0] = HIBYTE(domain_ddress);
    data[1] = LOBYTE(domain_ddress);

    A_Broadcast_Req(pBuffer, source, A_DOMAINADDRESS_WRITE, (uint8_t *)data, (uint8_t)2);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_DomainAddressSelective_Read_req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                          Knx_AddressType domain_ddress,
                                                          Knx_AddressType start_address,
                                                          uint8_t range)
#else
void A_DomainAddressSelective_Read_req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType domain_ddress,
                                       Knx_AddressType start_address, uint8_t range)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t data[5];

    data[0]=HIBYTE(domain_ddress);
    data[1]=LOBYTE(domain_ddress);
    data[2]=HIBYTE(start_address);
    data[3]=LOBYTE(start_address);

    data[4] = range;

    A_Broadcast_Req(pBuffer, source, A_DOMAINADDRESSSELECTIVE_READ, (uint8_t *)data, (uint8_t)5);
}


/*
**
**
**  Point-to-Point-Connectionless-Services.
**
**
*/
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_Individual_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint16_t apci,
                                         P2VAR(uint8_t, AUTOMATIC, KSTACK_APPL_DATA) data, uint8_t len
                                         )
#else
void A_Individual_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint16_t apci, uint8_t * data, uint8_t len)
#endif /* KSTACK_MEMORY_MAPPING */
{

    len = MIN(len, MAX_ADPU_LEN);

    (void)KnxMSG_ClearBuffer(pBuffer);

    KnxMSG_SetAPCI(pBuffer, apci);
    KnxMSG_SetSourceAddress(pBuffer, source);
    KnxMSG_SetDestAddress(pBuffer, dest);
    KnxMSG_SetPriority(pBuffer, KNX_OBJ_PRIO_SYSTEM);
    KnxMSG_SetLen(pBuffer, (uint8_t)8 + len);
    KnxAl_SetAPDUData(KnxMSG_GetMessagePtr(pBuffer), (uint8_t)0, data, len);
    pBuffer->service = T_DATA_INDIVIDUAL_REQ;

    (void)KnxMSG_Post(pBuffer);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) KnxAl_SetPropertyHeader(KnxMSG_BufferPtr pBuffer, uint8_t obj_index, uint8_t prop_id, uint8_t nr_of_elem,
                                             uint16_t start_index)
#else
void KnxAl_SetPropertyHeader(KnxMSG_BufferPtr pBuffer, uint8_t obj_index, uint8_t prop_id, uint8_t nr_of_elem, uint16_t start_index)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t data[4];

    data[0]    = obj_index;
    data[1]    = prop_id;
    data[2]    = ((nr_of_elem & (uint8_t)0x0f) << 4) | (HIBYTE(start_index) & (uint8_t)0x0f);
    data[3]    = LOBYTE(start_index);

    KnxAl_SetAPDUData(KnxMSG_GetMessagePtr(pBuffer), (uint8_t)0, data, (uint8_t)4);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_PropertyValue_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest,
                                                 uint8_t obj_index,
                                                 uint8_t prop_id, uint8_t nr_of_elem, uint16_t start_index,
                                                 uint8_t * data)
#else
void A_PropertyValue_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t obj_index,
                              uint8_t prop_id, uint8_t nr_of_elem, uint16_t start_index, uint8_t * data)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxAl_SetPropertyHeader(pBuffer, obj_index, prop_id, nr_of_elem, start_index);

/* Data-Offset = 4 */
/* A_PROPERTYVALUE_RESPONSE */
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_PropertyValue_Read_Res_NoData(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest,
                                                        uint8_t obj_index,
                                                        uint8_t prop_id, uint8_t nr_of_elem,
                                                        uint16_t start_index)
#else
void A_PropertyValue_Read_Res_NoData(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t obj_index,
                                     uint8_t prop_id, uint8_t nr_of_elem, uint16_t start_index)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxAl_SetPropertyHeader(pBuffer, obj_index, prop_id, nr_of_elem, start_index);
    A_Individual_Req(pBuffer, source, dest, A_PROPERTYVALUE_RESPONSE, (uint8_t *)NULL, (uint8_t)4);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_PropertyValue_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest,
                                                 uint8_t obj_index,
                                                 uint8_t prop_id, uint8_t nr_of_elem,
                                                 uint16_t start_index)
#else
void A_PropertyValue_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t obj_index,
                              uint8_t prop_id, uint8_t nr_of_elem, uint16_t start_index)
#endif /* KSTACK_MEMORY_MAPPING */
{
    KnxAl_SetPropertyHeader(pBuffer, obj_index, prop_id, nr_of_elem, start_index);
    A_Individual_Req(pBuffer, source, dest, A_PROPERTYVALUE_READ, (uint8_t *)NULL, (uint8_t)4);
}


/*
   A_PROPERTYVALUE_WRITE
 */
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_PropertyDescription_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest,
                                                       uint8_t obj_index,
                                                       uint8_t prop_id, uint8_t prop_index, uint8_t type, uint16_t nr_of_elem,
                                                       uint8_t access)
#else
void A_PropertyDescription_Read_Res(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t obj_index,
                                    uint8_t prop_id, uint8_t prop_index, uint8_t type, uint16_t nr_of_elem, uint8_t access)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t data[7];

/* todo: KnxAl_SetADPUData */
    data[0]    = obj_index;
    data[1]    = prop_id;
    data[2]    = prop_index;
    data[3]    = type;
    data[4]    = HIBYTE(nr_of_elem);
    data[5]    = LOBYTE(nr_of_elem);
    data[6]    = access;

    A_Individual_Req(pBuffer, source, dest, A_PROPERTYDESCRIPTION_RESPONSE, (uint8_t *)data, (uint8_t)7);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_PropertyDescription_Read_Res_NoData(KnxMSG_BufferPtr pBuffer, Knx_AddressType source,
                                                              Knx_AddressType dest, uint8_t obj_index,
                                                              uint8_t prop_id,
                                                              uint8_t prop_index)
#else
void A_PropertyDescription_Read_Res_NoData(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t obj_index,
                                           uint8_t prop_id, uint8_t prop_index)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t data[3];

/* todo: KnxAl_SetADPUData */
    data[0]    = obj_index;
    data[1]    = prop_id;
    data[2]    = prop_index;

    A_Individual_Req(pBuffer, source, dest, A_PROPERTYDESCRIPTION_RESPONSE, (uint8_t *)data, (uint8_t)3);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) A_PropertyDescription_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest,
                                                       uint8_t obj_index,
                                                       uint8_t prop_id,
                                                       uint8_t prop_index)
#else
void A_PropertyDescription_Read_Req(KnxMSG_BufferPtr pBuffer, Knx_AddressType source, Knx_AddressType dest, uint8_t obj_index,
                                    uint8_t prop_id, uint8_t prop_index)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t data[3];

/* todo: KnxAl_SetADPUData */
    data[0]    = obj_index;
    data[1]    = prop_id;
    data[2]    = prop_index;

    A_Individual_Req(pBuffer, source, dest, A_PROPERTYDESCRIPTION_READ, (uint8_t *)data, (uint8_t)3);
}


/*
   Device-Descriptor:

   Descriptor Type 0 (mask version)
   --------------------------------

   Mask Type (8 bits)          Firmware Version (8 bits)
   MMMM        TTTT            VVVV    SSSS
   Medium Type Firmware Type   Version Sub-code

   Medium Type                         Firmware Type
   -------------------------------------------------------------------------
   0000b 0h Twisted Pair 1             0000b 0h Standard BAU
   0001b 1h Power line 110             0111b 7h Control Unit
   0010b 2h Radio Frequency            1000b 8h IR-Decoder
   0011b 3h Twisted Pair 0             1001b 9h Coupler to TP
   0100b 4h Power line 132             1111b Fh Escape see Descriptor Type 1
   0101b 5h Reserved for fast Media
   1111b Fh Escape


   Mask    Mask    Medium          Device                  Version
   Type    Version
   00h     10h     Twisted Pair 1  BCU 1, BIM M111         0
   00h     11h     Twisted Pair 1  BCU 1, BIM M111         1
   00h     12h     Twisted Pair 1  BCU 1, BIM M111         2
   10h     12h     Power Line 110  BCU 1                   2
   10h     13h     Power Line 110  BCU 1                   3
   00h     20h     Twisted Pair 1  BCU 2, BIM M113         0
   07h     00h     Twisted Pair 1  BIM M112                0
   07h     01h     Twisted Pair 1  BIM M112                1
   08h     10h     Twisted Pair 1  IR-Decoder              0
   08h     11h     Twisted Pair 1  IR-Decoder              1
   09h     10h     Twisted Pair 1  Coupler 1.0             0
   09h     11h     Twisted Pair 1  Coupler 1.1             1
   19h     00h     Twisted Pair 1  Media Coupler PL-TP     0
   30h     12h     Twisted Pair 0  BCU 1                   2
   40h     12h     Power Line 132  BCU 1                   2

 */

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
