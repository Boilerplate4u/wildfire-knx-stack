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
#include "KNXNwps.h"

void NWPS_Dispatch(KnxMSG_BufferPtr pBuffer, uint8 service /*,boolean connected*/);

/*
   � check the usage of a Group Address (Group Address Check)
    object_type: 1 (Group Address Table)
    PID: 23 (List of group addresses)
    test_info: range8 + start_address16
    test_result: none
    comm_mode response: point-to-point, connectionless

   - check the existence of an Application Interface Object (Functional Block Scan)
    object_type: [50..50.000]
    PID: 1 (PID_OBJECT_TYPE)
    test_info: reserved8 = 00h
    test_result: object_index
    comm_mode response: point-to-point, connectionless

   - get the serial numbers of all devices object_type:
    0 (Device Object)
    PID: 11 (PID_SERIAL_NUMBER)
    test_info: reserved8 = 00h
    test_result: serial number
    comm_mode response: point-to-point, connectionless
 */

/*
   4.1 Property for Device Object (0x0000) :
    SubnetAddress - PID_SUBNET_ADDRESS(0x39)
        This property includes the Subnetwork Address of the Device (size: 1 octet).

   4.2 Property for Device Object :
    AddressReport - PID_ADDRESS_REPORT(0x3C)
        This property includes the serial number of the Device (size: 6 octets).
        This value shall in datagrams be set to 000000000000h if the device has no valid serial number.

   4.3 Property for Device Object :
    Address_Check - PID_ADDRESS_CHECK (0x3D)
        This property is void (size: 1 octet; value = 00h always).
        Having no effect, it is used for checking the presence of a device at a given Individual Address.
 */

/*
    PDU/Read(Resp):
    -----------------------
    Object-Type:    8+9
    PID:            10
    Test-Info:      11..N
    Test-Result     N+1..21

    PDU/Write:
    -----------------------
    Object-Type:    8+9
    PID:            10
    Value:          11..21
 */

typedef boolean (*NWPS_FUNC)(KnxMSG_BufferPtr pBuffer);

typedef struct tagNWPSFunctions {
    uint16      ObjectType_from, ObjectType_to;
    uint8       Pid;
    NWPS_FUNC   func;
} NWPSFunctions;

boolean NWPS_GroupAddressCheck(KnxMSG_BufferPtr pBuffer);
boolean NWPS_FunctionalBlockScan(KnxMSG_BufferPtr pBuffer);
boolean NWPS_GetSerialNumber(KnxMSG_BufferPtr pBuffer);

static NWPSFunctions NWPSReadFunctions[] = {
    {KNX_OT_ADDRESSTABLE_OBJECT, KNX_OT_ADDRESSTABLE_OBJECT,     KNX_PID_GROUP_ADDRESS_LIST,           NWPS_GroupAddressCheck                                       },
    {(uint8)50,                  (uint16)50000,                  KNX_PID_OBJECT_TYPE,                  NWPS_FunctionalBlockScan                                     },
    {KNX_OT_DEVICE_OBJECT,       KNX_OT_DEVICE_OBJECT,           KNX_PID_SERIAL_NUMBER,                NWPS_GetSerialNumber                                         }
};

/* A_NetworkParameter_Read.req(hop_count_type, parameter_type, priority, test_info); */

void NWPS_Dispatch(KnxMSG_BufferPtr pBuffer, uint8 service /*,boolean connected*/)
{
    KNX_StandardFrameRefType    pmsg;
    uint8                       len, Pid;
    uint16                      objectType;

    pmsg = KnxMSG_GetMessagePtr(pBuffer);

    objectType = btohs((uint16)pmsg->data[0]);
    Pid        = pmsg->data[2];

    len = KnxMSG_GetLSDULen(pBuffer);

    if (service == NWPS_READ) {

    } else if (service == NWPS_WRITE) {

    } else {
        (void)KnxMSG_ReleaseBuffer(pBuffer);
    }
}

boolean NWPS_GroupAddressCheck(KnxMSG_BufferPtr pBuffer)
{
    return TRUE;
}

boolean NWPS_FunctionalBlockScan(KnxMSG_BufferPtr pBuffer)
{
    return TRUE;
}

boolean NWPS_GetSerialNumber(KnxMSG_BufferPtr pBuffer)
{
    return TRUE;
}

