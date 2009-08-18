#if !defined(__EMI_IMI2_H)
#define __EMI_IMI2_H

/* Hinweis: Da kein 'External-Message-Interface' unterst�tzt wird, ist 'KNXImi2.h' wohl ein besserer Name!!! */
/* Hinweis: wird zu Implementations-spezifischem 'IMI', s. 'knx_disp.c' !!! */

/*
L_SystemBroadcast.req           15h 15h 17h
L_SystemBroadcast.con           2Ch 4Ch 26h
L_SystemBroadcast.ind           2Dh 4Dh 28h
T_Data_SystemBroadcast.req      25h 25h -
T_Data_SystemBroadcast.con      3Ch 4Ch -
T_Data_SystemBroadcast.ind      3Dh 4Dh -
*/

typedef enum tagKNXServiceTypeType {
/*
**      Link-Layer.
*/
    L_PLAIN_DATA_REQ            =   0x10,
    L_DATA_REQ                  =   0x11,
    L_POLL_DATA_REQ             =   0x13,

/*
**      Network-Layer.
*/
    N_DATA_INDIVIDUAL_REQ       =   0x21,
    N_DATA_GROUP_REQ            =   0x22,
    N_POLL_DATA_REQ             =   0x23,
    L_POLL_DATA_CON             =   0x25,
    L_DATA_IND                  =   0x29,
    L_BUSMON_IND                =   0x2B,
    N_DATA_BROADCAST_REQ        =   0x2C,
    L_DATA_CON                  =   0x2E,

/*
**      Transport-Layer/group-oriented.
*/
    T_DATA_GROUP_REQ            =   0x32,
    T_POLL_DATA_REQ             =   0x33,
    N_POLL_DATA_CON             =   0x35,
    N_DATA_GROUP_IND            =   0x3A,
    N_DATA_GROUP_CON            =   0x3E,

/*
**      Transport-Layer/connection-oriented.
*/
    T_DATA_CONNECTED_REQ        =   0x41,
    T_CONNECT_REQ               =   0x43,
    T_DISCONNECT_REQ            =   0x44,
    N_DATA_INDIVIDUAL_IND       =   0x49,
    T_DATA_INDIVIDUAL_REQ       =   0x4A,
    T_DATA_BROADCAST_REQ        =   0x4C,
    N_DATA_BROADCAST_IND        =   0x4D,
    N_DATA_INDIVIDUAL_CON       =   0x4E,
    N_DATA_BROADCAST_CON        =   0x4F,

/*
**      Application-Layer/group-oriented.
*/
    U_VALUE_WRITE_REQ           =   0x71,
    A_DATA_GROUP_REQ            =   0x72,
    A_POLL_DATA_REQ             =   0x73,
    U_VALUE_READ_REQ            =   0x74,
    T_POLL_DATA_CON             =   0x75,
    T_DATA_GROUP_IND            =   0x7A,
    U_FLAGS_READ_REQ            =   0x7C,
    T_DATA_GROUP_CON            =   0x7E,

/*
**      Application-Layer/managment-part.
*/
    M_USER_DATA_INDIVIDUAL_REQ  =   0x81,
    M_USER_DATA_CONNECTED_REQ   =   0x82,
    T_CONNECT_IND               =   0x85,
    T_CONNECT_CON               =   0x86,
    T_DISCONNECT_IND            =   0x87,
    T_DISCONNECT_CON            =   0x88,
    T_DATA_CONNECTED_IND        =   0x89,
    T_DATA_CONNECTED_CON        =   0x8E,
    T_DATA_BROADCAST_IND        =   0x8D,
    T_DATA_BROADCAST_CON        =   0x8F,

    T_DATA_INDIVIDUAL_IND       =   0x94,
    M_INTERFACEOBJ_DATA_REQ     =   0x9A,
    T_DATA_INDIVIDUAL_CON       =   0x9C,

/*
**      Physical-External-Interface.
*/
    PC_SETVALUE_REQ             =   0xA6,
    PEI_IDENTIFY_REQ            =   0xA7,
    PEI_IDENTIFY_CON            =   0xA8,
    PEI_SWITCH_REQ              =   0xA9,
    PC_GETVALUE_CON             =   0xAB,
    PC_GETVALUE_REQ             =   0xAC,

/*
**      Application running in the BAU (PEI if stopped).
*/
    TM_TIMER_IND                =   0xC1,

/* #define U_USER_DATA                           >D0     USER */
    M_USER_DATA_CONNECTED_CON   =   0xD1,
    M_USER_DATA_CONNECTED_IND   =   0xD2,
    M_INTERFACEOBJ_DATA_IND     =   0xD4,
    M_CONNECT_IND               =   0xD5,
    M_DISCONNECT_IND            =   0xD7,
    M_USER_DATA_INDIVIDUAL_IND  =   0xD9,
    M_INTERFACEOBJ_DATA_CON     =   0xDC,
    M_USER_DATA_INDIVIDUAL_CON  =   0xDE,
/* #define M_CONNECT_REQ */
/* #define M_CONNECT_CON */
/* #define M_DISCONNECT_REQ */
/* #define M_DISCONNECT_CON */

    U_VALUE_READ_CON            =   0xE4,
    A_POLL_DATA_CON             =   0xE5,
    U_EVENT_IND                 =   0xE7,
    A_DATA_GROUP_IND            =   0xEA,
    U_FLAGS_READ_CON            =   0xEC,
    A_DATA_GROUP_CON            =   0xEE
    
} KNXServiceTypeType;

#endif /* __EMI_IMI2_H */
