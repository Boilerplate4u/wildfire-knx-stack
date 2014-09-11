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
 *
 * Common Services are supported by every implementation, i.e. TPUART1, TPUART2 and NCN5120.
 *
 */

#include "link-layer\uart_bif.h"
#include "knx_platform.h"

#include <stdio.h>

#define KNX_LL_BUF_SIZE     (0xff)

/*
 *  Local Defines.
 */

/*
** Offsets into KNX Frame.
*/
#define OFFS_CTRL           (0)
#define OFFS_SOURCE_ADDR_H  (1)
#define OFFS_SOURCE_ADDR_L  (2)
#define OFFS_DEST_ADDR_H    (3)
#define OFFS_DEST_ADDR_L    (4)
#define OFFS_NPCI           (5)
#define OFFS_TPCI           (6)
#define OFFS_APCI           (7)


/*
** Local Function-like Macros.
*/
#define KNX_LL_DESTINATION_ADDRESS()    (MAKEWORD(KnxLL_Buffer[OFFS_DEST_ADDR_H], KnxLL_Buffer[OFFS_DEST_ADDR_L]))

/*!
 *
 *  Local Types.
 *
 */

typedef enum tagKnxLL_StateType {
    KNX_LL_STATE_IDLE,
    KNX_LL_STATE_RECEIVING,
    KNX_LL_STATE_SENDING,
    KNX_LL_STATE_AWAITING_RESPONSE_LOCAL,
    KNX_LL_STATE_AWAITING_RESPONSE_TRANSMISSION,
    KNX_LL_STATE_AWAITING_RECEIPTION,
    KNX_LL_STATE_TIMED_OUT
} KnxLL_StateType;

typedef enum tagKnxLL_ReceiverStageType {
  KNX_LL_RECEIVER_STAGE_HEADER,
  KNX_LL_RECEIVER_STAGE_TRAILER
} KnxLL_ReceiverStageType;

typedef enum tagKnxLL_EventType {
    KNX_LL_EVENT_REQUEST,
    KNX_LL_EVENT_INDICATION,
    KNX_LL_EVENT_TIMEOUT,
} KnxLL_EventType;

typedef struct tagKnxLL_ExpectationType {
    uint8_t ExpectedByteCount;
    uint8_t ExpectedService;
    uint8_t ExpectedMask;
} KnxLL_ExpectationType;


#if 0
typedef struct tagKnxLL_LayerStateType {
    #if 0
 KnxLL_StateType KnxLL_State;
 uint8_t KnxLL_SequenceNo;

 uint8_t KnxLL_Buffer[KNX_LL_BUF_SIZE];

 KnxLL_ExpectationType KnxLL_Expectation = {0};
 uint8_t KnxLL_ReceiverIndex;
 KnxLL_ReceiverStageType KnxLL_ReceiverStage;
 uint8_t KnxLL_RunningFCB;
 KnxLL_LocalConfirmationType KnxLL_LocalConfirmation;
    #endif

} KnxLL_LayerStateType;
#endif

typedef enum tagKnxLL_LocalConfirmationType {
    KNX_LL_CONF_NEGATIVE,
    KNX_LL_CONF_POSITIVE
} KnxLL_LocalConfirmationType;

/*!
 *
 *  Local Variables.
 *
 */

static KnxLL_StateType KnxLL_State;
static uint8_t KnxLL_SequenceNo;

static uint8_t KnxLL_Buffer[KNX_LL_BUF_SIZE];

static KnxLL_ExpectationType KnxLL_Expectation = {0};
static uint8_t KnxLL_ReceiverIndex;
static KnxLL_ReceiverStageType KnxLL_ReceiverStage;
static uint8_t KnxLL_RunningFCB;
static KnxLL_LocalConfirmationType KnxLL_LocalConfirmation;

KNX_IMPLEMENT_MODULE_STATE_VAR(UART_BIF);

/*!
 *
 *  Local Function Prototypes.
 *
 */
static boolean KnxLL_InternalCommand(uint8_t const * frame, uint8_t length, KnxLL_StateType desiredState);
static boolean KnxLL_InternalCommandUnconfirmed(uint8_t const * frame, uint8_t length);
static boolean KnxLL_InternalCommandConfirmed(uint8_t const * frame, uint8_t length);
static uint8_t KnxLL_Checksum(uint8_t const * frame, uint8_t length);
static void KnxLL_Expect(uint8_t service, uint8_t mask, uint8_t byteCount);

/*!
 *
 *  Global Functions.
 *
 */


/**
 * Initialises the Data-Link-Layer.
 *
 */
void KnxLL_Init(void)
{
    KNX_MODULE_INITIALIZE(UART_BIF);

    KnxLL_State = KNX_LL_STATE_IDLE;
    KnxLL_SequenceNo = (uint8_t)0x00;
    KnxLL_ReceiverIndex = (uint8_t)0x00;
    KnxLL_RunningFCB = (uint8_t)0x00;
    KnxLL_LocalConfirmation = KNX_LL_CONF_NEGATIVE;
    Utl_MemSet(&KnxLL_Expectation, '\x00', sizeof(KnxLL_ExpectationType));
}


/**
 *  Gets called from interrupt-context if a timeout occured.
 *
 */
void KnxLL_TimeoutCB(void)
{
    DBG_PRINTLN("L2 TIMEOUT.");
    KnxLL_State = KNX_LL_STATE_TIMED_OUT;
    KnxLL_FeedReceiver(0x00);
}

// This constitutes the link-layer statemachine.
void KnxLL_FeedReceiver(uint8_t octet)
{
#if 0
    static void U_Reset_res(void);
    static void U_Reset_ind(void);
    static void U_State_res(void);
    static void U_State_ind(void);
    static void L_Data_con(void);
    static void L_DataExtended_ind(void);
    static void L_DataStandard_ind(void);
    static void L_PollData_ind(void);

    uint8_t   ctrl;
    uint8_t   source[2];
    uint8_t   dest[2];
    uint8_t   npci;
    uint8_t   tpci;
    uint8_t   apci;
#endif

    if (KnxLL_State == KNX_LL_STATE_AWAITING_RESPONSE_LOCAL) {
        if (KnxLL_Expectation.ExpectedService == (octet & KnxLL_Expectation.ExpectedMask)) {
            if (KnxLL_Expectation.ExpectedByteCount == 1) {
                //printf("fo: 0x%02x\n", octet);
                TMR_STOP_DL_TIMER();
                KnxLL_State = KNX_LL_STATE_IDLE;
                switch (KnxLL_Expectation.ExpectedService) {
                case U_RESET_IND:
                    DBG_PRINTLN("U_Reset_Res");
                    break;
                case U_STATE_IND:
                    DBG_PRINTLN("U_State_Res");
                    break;
                default:
                    break;
                }
            }
        }
        // KnxLL_Buffer[0] = octet;
    }
    else if (KnxLL_State == KNX_LL_STATE_AWAITING_RESPONSE_TRANSMISSION) {
        TMR_STOP_DL_TIMER();
        KnxLL_ReceiverIndex++;
        TMR_START_DL_TIMER();
        if (KnxLL_ReceiverIndex == 0x01) {
            if ((octet & U_STATE_IND) == U_STATE_IND) {
                DBG_PRINTLN("Receiver Error!\n");
            }
            else if ((octet & L_DATA_CON) == L_DATA_CON) {
                TMR_STOP_DL_TIMER();
                if ((octet & 0x80) == 0x80) {
                    KnxLL_LocalConfirmation = KNX_LL_CONF_POSITIVE;
                } else {
                    KnxLL_LocalConfirmation = KNX_LL_CONF_NEGATIVE;
                }
                KnxLL_State = KNX_LL_STATE_IDLE;
            }
            else if ((octet & 0x10) == 0x10) {    /* Weak check. */
                //printf("Control-Field: 0x%02x\n", octet);
            }
        }
        else if (KnxLL_ReceiverIndex == KnxLL_Expectation.ExpectedByteCount) {
            //printf("Finished. [0x%02x]\n", octet);
            KnxLL_ReceiverIndex = (uint8_t)0x00;
        }
    }
    else if (KnxLL_State == KNX_LL_STATE_AWAITING_RECEIPTION) {
        TMR_STOP_DL_TIMER();
        KnxLL_ReceiverIndex++;
        KnxLL_Buffer[KnxLL_ReceiverIndex] = octet;
        KnxLL_RunningFCB ^= octet;
        printf("R: %02x ", octet);
        TMR_START_DL_TIMER();
#if 0
#define OFFS_SOURCE_ADDR_H  (1)
#define OFFS_SOURCE_ADDR_L  (2)
#define OFFS_DEST_ADDR_H    (3)
#define OFFS_DEST_ADDR_L    (4)
#endif
        if (KnxLL_ReceiverStage == KNX_LL_RECEIVER_STAGE_HEADER) {
            if (KnxLL_ReceiverIndex == OFFS_NPCI) {
                KnxLL_Expectation.ExpectedByteCount = (octet & 0x0f) + 2;
                KnxLL_ReceiverStage = KNX_LL_RECEIVER_STAGE_TRAILER;
            } else if (KnxLL_ReceiverIndex == OFFS_DEST_ADDR_H) {

                printf("\nDA: 0x%04X\n", KNX_LL_DESTINATION_ADDRESS());
            }
        } else if (KnxLL_ReceiverStage == KNX_LL_RECEIVER_STAGE_TRAILER) {
            if (KnxLL_ReceiverIndex == KnxLL_Expectation.ExpectedByteCount + OFFS_NPCI) {
                KnxLL_State = KNX_LL_STATE_IDLE;
                /* TODO: Check FCB. */
                /* TODO: Callback. */
            }
        }

    } else if (KnxLL_State == KNX_LL_STATE_TIMED_OUT) {
        // TODO: Callback/Callout.
        KnxLL_State = KNX_LL_STATE_IDLE;
    } else if (KnxLL_State == KNX_LL_STATE_IDLE) {
        if ((octet & U_STATE_IND) == U_STATE_IND) {
            printf("U_State_Ind [0x%02x]\n", octet);
        } else if ((octet & 0x7f) == L_DATA_CON) {
            DBG_PRINTLN("L_Data_Con");    // ???
        } else if ((octet & 0xd3) == L_DATA_EXTENDED_IND) { // #if defined()
            DBG_PRINTLN("L_DataExtended_Ind");
        } else if ((octet & 0xd3) == L_DATA_STANDARD_IND)  {
            DBG_PRINTLN("L_DataStandard_Ind");
            printf("%02x ", octet);
            KnxLL_State = KNX_LL_STATE_AWAITING_RECEIPTION; /* TODO: Distiguish Standard/Extendend Frames */
            KnxLL_ReceiverStage = KNX_LL_RECEIVER_STAGE_HEADER;
            KnxLL_ReceiverIndex = 0;
            KnxLL_RunningFCB = (uint8_t)0xff ^ octet;
            KnxLL_Buffer[KnxLL_ReceiverIndex] = octet;
            TMR_START_DL_TIMER();
        } else if (octet == U_RESET_IND) {
            DBG_PRINTLN("U_Reset_Ind");
        } else if (octet == L_POLL_DATA_IND) { // #if defined()
            DBG_PRINTLN("L_PollData_Ind");
        }
#if 0
    MASK: 0x00
        U_ProductID.response == > xxxxxxxx[0x00]
    MASK: 0x07
          State.response / indication == > xxxxx111[0x07]
      MASK: 0x7f
            L_DATA.confirm == > x0001011[0x0b]
        MASK: 0xd3
              L_EXT_DATA.req == > 00x1xx00[0x10]
              L_DATA.req == > 10x1xx00[0x90]
          MASK: 0xff
                Reset - Indication == > 00000011[0x03]
                NotAcknowledge frame == > 00001100[0x0c]
                Busy frame == > 11000000[0xc0]
                Acknowledge frame == > 11001100[0xcc]
                L_POLLDATA.req == > 11110000[0xf0]
#endif

    } else {
       /* Ignore anything else. */
    }

}

void KnxLL_Task(void)
{

}

boolean KnxLL_IsBusy(void)
{
    boolean result;

    PORT_LOCK_TASK_LEVEL();
    result = (KnxLL_State != KNX_LL_STATE_IDLE);
    PORT_UNLOCK_TASK_LEVEL();
    return result;
}


/**
* Wait for availability of link-layer.
*
* Services shall never be issued while link-layer is busy.
*/
void KnxLL_BusyWait(void)
{   
    while (KnxLL_IsBusy()) {
    };
}

boolean KnxLL_IsConfirmed(void)
{
    boolean result;

    PORT_LOCK_TASK_LEVEL();
    result = KnxLL_LocalConfirmation == KNX_LL_CONF_POSITIVE;
    PORT_UNLOCK_TASK_LEVEL();

    return result;
}

/*!
 *
 * Local Functions.
 *
 */

/**
 * Issues an internal Command, i.e. related to the interface IC, not EIB.
 *
 * @param frame Octets to be send.
 * @param length Number of octets.
 * @param desiredState Desired state to enter (in case of confirmed commands, we need to wait for an answer).
 *
 * @return boolean
 */
static boolean KnxLL_InternalCommand(uint8_t const * frame, uint8_t length, KnxLL_StateType desiredState)
{
    boolean result;

    PORT_LOCK_TASK_LEVEL();
    if (KnxLL_State != KNX_LL_STATE_IDLE) {
        PORT_UNLOCK_TASK_LEVEL();
        return FALSE;
    }
    //KnxLL_State = KNX_LL_STATE_SENDING;
    KnxLL_State = desiredState;
    result = (boolean)Port_WriteToBusInterface(frame, length);
    PORT_UNLOCK_TASK_LEVEL();
    TMR_START_DL_TIMER();
    //KnxLL_State = desiredState;
    return result;
}

static boolean KnxLL_InternalCommandUnconfirmed(uint8_t const * frame, uint8_t length)
{
    return KnxLL_InternalCommand(frame, length, KNX_LL_STATE_IDLE);
}

static boolean KnxLL_InternalCommandConfirmed(uint8_t const * frame, uint8_t length)
{
    return KnxLL_InternalCommand(frame, length, KNX_LL_STATE_AWAITING_RESPONSE_LOCAL);
}

static void KnxLL_Expect(uint8_t service, uint8_t mask, uint8_t byteCount)
{
    KnxLL_Expectation.ExpectedService = service;
    KnxLL_Expectation.ExpectedMask = mask;
    KnxLL_Expectation.ExpectedByteCount = byteCount;
}

static uint8_t KnxLL_Checksum(uint8_t const * frame, uint8_t length)
{
    uint8_t checksum = (uint8_t)0xff;
    uint8_t idx;

    for (idx = 0; idx < length; ++idx) {
        checksum ^= frame[idx];
    }

    return checksum;
}

#define WRITE(ch)   \
    printf("0x%02x ", ch);   \
    Port_WriteToBusInterface((uint8_t*)&ch, 1)

void KnxLL_WriteFrame(uint8_t const * frame, uint8_t length)
{
    uint8_t idx;
    uint8_t checksum;
    char buffer[128];

    KNX_ASSERT_MODULE_IS_INITIALIZED(UART_BIF, KNX_SERVICE_LL_WRITE_FRAME);

    checksum = KnxLL_Checksum(frame, length);
    buffer[0] = U_L_DATASTART_REQ;
    buffer[1] = frame[0];
    for (idx = 2; idx < (length << 1); idx += 2) {
        buffer[idx] = U_L_DATACONT_REQ | (idx >> 1);
        buffer[idx + 1] = frame[idx >> 1];
    }
    buffer[idx] = U_L_DATAEND_REQ | (idx >> 1);
    buffer[idx + 1] = checksum;
    Port_WriteToBusInterface(buffer, idx + 2);
    KnxLL_Expect(0x00, 0x00, length + 1);
    KnxLL_State = KNX_LL_STATE_AWAITING_RESPONSE_TRANSMISSION;
    KnxLL_ReceiverIndex = (uint8_t)0x00;
    TMR_START_DL_TIMER();
}


/**
 *
 *  Local unconfirmed services.
 *
 */
void U_ActivateBusmon_req(void)
{
    DBG_PRINTLN("U_ActivateBusmon_req");
    KnxLL_Buffer[0] = U_BUSMON_REQ;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 1);
}

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_NCN5120
void U_ActivateBusyMode_req(void)
{
    DBG_PRINTLN("U_ActivateBusyMode_req");
    KnxLL_Buffer[0] = U_SETBUSY_REQ;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 1);
}

void U_ResetBusyMode_req(void)
{
    DBG_PRINTLN("U_ResetBusyMode_req");
    KnxLL_Buffer[0] = U_QUITBUSY_REQ;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 1);
}
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2
void U_ActivateBusyMode_req(void)
{
    DBG_PRINTLN("U_ActivateBusyMode_req");
    KnxLL_Buffer[0] = U_ACTIVATEBUSYMODE_REQ;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 1);
}


void U_ResetBusyMode_req(void)
{
    DBG_PRINTLN("U_ResetBusyMode_req");
    KnxLL_Buffer[0] = U_RESETBUSYMODE_REQ;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 1);
}

void U_SetRepetition_req(uint8_t rst)
{
    DBG_PRINTLN("U_SetRepetition_req");
    KnxLL_Buffer[0] = U_MXRSTCNT_REQ;
    KnxLL_Buffer[1] = rst;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 2);
}

void U_ActivateCRC_req(void)
{
    DBG_PRINTLN("U_ActivateCRC_req");
    KnxLL_Buffer[0] = U_ACTIVATECRC_REQ;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 1);
}

void U_SetAddress_req(uint16_t address)
{
    DBG_PRINTLN("U_SetAddress_req");
    KnxLL_Buffer[0] = U_SETADDRESS_REQ;
    KnxLL_Buffer[1] = HIBYTE(address);
    KnxLL_Buffer[2] = LOBYTE(address);
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 3);
}
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_NCN5120
void U_SetRepetition_req(uint8_t rst)
{
    DBG_PRINTLN("U_SetRepetition_req");
    KnxLL_Buffer[0] = U_SETREPETITION_REQ;
    KnxLL_Buffer[1] = rst;
    KnxLL_Buffer[2] = 0x00; /* Dummy */
    KnxLL_Buffer[3] = 0x00; /* Dummy */
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 4);
}
#endif /* KNX_BUS_INTERFACE */

/**
 *
 *  Local confirmed services.
 *
 */

void U_Reset_req(void)
{
    DBG_PRINTLN("U_Reset_req");
    KnxLL_Buffer[0] = U_RESET_REQ;
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 1);
    KnxLL_Expect(U_RESET_IND, 0xff, 1);
}

void U_State_req(void)
{
    DBG_PRINTLN("U_State_req");
    KnxLL_Buffer[0] = U_STATE_REQ;
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 1);
    KnxLL_Expect(U_STATE_IND, 0x07, 1);
}

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_2
void U_ProductID_req(void)
{
    DBG_PRINTLN("U_ProductID_req");
    KnxLL_Buffer[0] = U_PRODUCTID_REQUEST;
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 1);
    KnxLL_Expect(0x00, 0x00, 1);    /* Expect any single byte value. */
}
#endif /* KNX_BUS_INTERFACE */

#if KNX_BUS_INTERFACE == KNX_BIF_TPUART_NCN5120
void U_SetAddress_req(uint16_t address) /* NB: TPUART2's SetAddress is unconfirmed! */
{
    DBG_PRINTLN("U_SetAddress_req");
    KnxLL_Buffer[0] = U_SETADDRESS_REQ;
    KnxLL_Buffer[1] = HIBYTE(address);
    KnxLL_Buffer[2] = LOBYTE(address);
    KnxLL_Buffer[3] = 0x00; /* Dummy */
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 4);
    KnxLL_Expect(U_CONFIGURE_IND, 0x83, 1);
}
#endif /* KNX_BUS_INTERFACE */

/**
 *  Transmission services.
 */


#if 0
void U_SystemState_req(void)
{
    DBG_PRINTLN("U_SystemState_req");
    KnxLL_Buffer[0] = U_SYSTEMSTAT_REQ;
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 2);
    KnxLL_Expect(U_SYSTEMSTAT_IND, 0xff, 2);
}

void U_StopMode_req(void)
{
    DBG_PRINTLN("U_StopMode_req");
    KnxLL_Buffer[0] = U_STOPMODE_REQ;
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 1);
    KnxLL_Expect(U_STOPMODE_IND, 0xff, 1);
}

void U_ExitStopMode_req(void)
{
    DBG_PRINTLN("U_ExitStopMode_req");
    KnxLL_Buffer[0] = U_EXITSTOPMODE_REQ;
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 1);
    KnxLL_Expect(U_RESET_IND, 0xff, 1);
}

void U_Configure_req(uint8_t conf)
{
    DBG_PRINTLN("U_Configure_req");
    KnxLL_Buffer[0] = U_CONFIGURE_REQ | (conf & 0x07);
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 1);
    KnxLL_Expect(U_CONFIGURE_IND, 0xff, 1);
}

void U_IntRegWr_req(uint8_t addr, uint8_t value)
{
    DBG_PRINTLN("U_IntRegWr_req");
    KnxLL_Buffer[0] = U_INTREGWR_REQ | (addr & 0x03);
    KnxLL_Buffer[1] = value;
    KnxLL_InternalCommandUnconfirmed(KnxLL_Buffer, 2);
}

void U_IntRegRd_req(uint8_t addr)
{
    DBG_PRINTLN("U_IntRegRd_req");
    KnxLL_Buffer[0] = U_INTREGRD_REQ;
    KnxLL_InternalCommandConfirmed(KnxLL_Buffer, 1);
    KnxLL_Expect(0x00, 0x00, 1);
}
#endif
