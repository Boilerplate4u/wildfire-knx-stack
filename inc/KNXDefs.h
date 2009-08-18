#if !defined(__KNX_DEFS_H)
#define __KNX_DEFS_H

/*
**  Hinweis: alle Definitionen/Deklarationen k�nnen eigentlich auf andere Module verteilt werden...
*/

/* #include "target.h" */
/* todo: ins Target-Verzeichnis!!! */
#define KNX_BIG_ENDIAN

#define DISABLE_ALL_INTERRUPTS()
#define ENABLE_ALL_INTERRUPTS()

#include <Std_Types.h>
/* #include "emi_imi2.h" */
#include "knx_imi.h"

typedef uint16 Knx_AddressType;
typedef uint8 Knx_SerialNumberType[6];

typedef struct tagKnx_PollGroupSettingType {
    uint16 group_addr;
    uint8 slot_no;
} Knx_PollGroupSettingType;

typedef struct tagKNX_CommObjDescriptorType {
    uint8 DataPtr,Config,Type;
} KNX_CommObjDescriptorType;

typedef enum tagKNX_DafType {
    atINDIVIDUAL=0x00,
    atMULTICAST=0x80
} KNX_DafType;  /* check: nicht so ganz gegl�ckt... */
                /* (aber w�rde sich mit KNX_AddressType beissen...) */
typedef enum tagKNX_FrameTypeType {
    ftExtended=0x00,
    ftStandard=0x80,
    ftPolling=0xC0
} KNX_FrameTypeType;

typedef enum tagKNX_ObjectType {
    otUINT1,otUINT2,otUINT3,otUINT4,otUINT5,otUINT6,otUINT7,
    otUINT8,otUINT16,otBYTE3,otFLOAT,otDATA6,otDOUBLE,otDATA10,otMAXDATA,otVARDATA  /* Hinweis: 'VARDATA' ist nicht mehr korrekt!!! */
} KNX_ObjectType;    /* Type-Byte / Object-Descriptor. */

typedef uint8 KNX_PriorityType;  /* check: evtl. 'enum'??? */

/* 
**  START: LAYER-Dispatching-Functions. 
*/
typedef void (*KNXLayerServiceFunctionType)(void);

typedef struct tagKNXServiceEntryType {
    KNXServiceTypeType Service;
    KNXLayerServiceFunctionType Function;
} KNXServiceEntryType ;


typedef struct tagKNXLayerServicesType {
    uint8 LayerOffset;    
    uint8 NumServices;
    const KNXLayerServiceFunctionType *Functions;
} KNXLayerServicesType;
/* 
**  END: LAYER-Dispatching-Functions. 
*/


/*
typedef enum tagKNX_PriorityType {
    prSystem,prNormal,
    prUrgent,prLow
} KNX_PriorityType;
*/

typedef uint8 KNX_HopCountType;

#define IAK_OK      ((uint8)0x00)
#define IAK_NOT_OK  ((uint8)0x01)

/*
#define MSG_NO_ROUTING_CTRL ((BYTE)7)
NoRouting,Parameter,Force,
*/

/* RunError-Codes. */
#define KNX_RUNERROR_SYS3_ERR           ((uint8)0x40)   /* internal system error (not for each default MCB [ROM] an. */
                                                        /* MCB [EEPROM] exists with the same start address). */
#define KNX_RUNERROR_SYS2_ERR           ((uint8)0x20)   /* internal system error (stuck/ overheat detection). */
#define KNX_RUNERROR_OBJ_ERR            ((uint8)0x10)   /* RAM flag table error (*PTR not in ZPAGE range). */
#define KNX_RUNERROR_STK_OVL            ((uint8)0x08)   /* stack overflow was detected. */
#define KNX_RUNERROR_EEPROM_ERR         ((uint8)0x04)   /* EEPROM check detected an CRC error. */
#define KNX_RUNERROR_SYS1_ERR           ((uint8)0x02)   /* internal system error (parity bit in SystemState is corrupt). */
#define KNX_RUNERROR_SYS0_ERR           ((uint8)0x01)   /* internal system error (msg buffer offset corrupt). */

    /*
    ** Config-Byte / Object-Descriptor.
    */
#define KNX_OBJ_UPDATE_ENABLE           ((uint8)0x80)    /* GroupValueRead()_Res wird verarbeitet - nur Mask 0020h, 0021h. */
#define KNX_OBJ_TRANSMIT_ENABLE         ((uint8)0x40)    /* Lese- und Schreib-Anforderungen (Requests) vom App.-Layer werden verarbeitet. */
#define KNX_OBJ_EEPROM_VALUE            ((uint8)0x20)    /* Objektwert steht im EEPROM (statt im RAM). */
#define KNX_OBJ_WRITE_ENABLE            ((uint8)0x10)    /* Objektwert kann �ber den Bus geschrieben werden. */
#define KNX_OBJ_READ_ENABLE             ((uint8)0x08)    /* Objektwert kann �ber den Bus gelesen werden. */
#define KNX_OBJ_COMM_ENABLE             ((uint8)0x04)    /* "Hauptschalter" f. die Kommunikation. */

    /*
    **  Config-Byte / Priorities.
    */
#define KNX_OBJ_PRIO_SYSTEM             ((uint8)0)
#define KNX_OBJ_PRIO_URGENT             ((uint8)2)
#define KNX_OBJ_PRIO_NORMAL             ((uint8)1)
#define KNX_OBJ_PRIO_LOW                ((uint8)3)

/*    
    Transmission Priorities
    =======================
    11 = low operational priority
    10 = high operational priority
    01 = alarm priority
    00 = system priority
*/    
        /*
        ** Hinweis: im KNX-App-Layer ist von URGENT statt ALARM die Rede
        ** und 'NORMAL' statt 'HIGH'.
        */

    /*
    ** Comm-Flags.
    */


#define KNX_OBJ_UPDATED                 ((uint8)0x08)
#define KNX_OBJ_DATA_REQUEST            ((uint8)0x04)

    /*
    ** Transmission-Status
    */

#define KNX_OBJ_IDLE_OK                 ((uint8)0x00)
#define KNX_OBJ_IDLE_ERROR              ((uint8)0x01)
#define KNX_OBJ_TRANSMITTING            ((uint8)0x02)
#define KNX_OBJ_TRANSMIT_REQ            ((uint8)0x03)

/*  Zugriffsrechte BCU2.0    - Hinweis: 'BCU20_' ist nicht so gl�cklich!!! (irgendetwas mit 'KNX_PRIVILEDGE_???' ist besser!!! */
/*  Hinweis: geh�rt zum 'Access'-Server ('KNXAcc.c') */
#define BCU20_PRIVILEGE_CONFIGURATION   ((uint8)0)
#define BCU20_PRIVILEGE_SERVICE         ((uint8)1)
#define BCU20_PRIVILEGE_USER            ((uint8)2)
#define BCU20_PRIVILEGE_NO              ((uint8)3)

/*  Hinweis: geh�rt zum Transport-Layer!!! */
#define KNX_UNUSED_TSAP                 ((uint8)0x0fe)   /* weg!!! */
#define KNX_INVALID_TSAP                ((uint8)0x00)   /* '0xff' */

#endif  /* __KNX_DEFS_H */

