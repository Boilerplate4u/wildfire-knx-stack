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
**
**  Interface-Object-Server.
**
*/

#include "knx_ios.h"
#include "Appl.h"
#include "knx_sys_objs.h"
#include "knx_layer_application.h"

const uint8_t KNX_PDT_LEN_TAB[32] = {
    (uint8_t)0, (uint8_t)1, (uint8_t)1,  (uint8_t)2,  (uint8_t)2,   (uint8_t)2,   (uint8_t)3,   (uint8_t)3,
    (uint8_t)4, (uint8_t)4, (uint8_t)4,  (uint8_t)8,  (uint8_t)10,  (uint8_t)3,   (uint8_t)5,   (uint8_t)0,
    (uint8_t)0, (uint8_t)1, (uint8_t)2,  (uint8_t)3,  (uint8_t)4,   (uint8_t)5,   (uint8_t)6,   (uint8_t)7,
    (uint8_t)8, (uint8_t)9, (uint8_t)10, (uint8_t)0,  (uint8_t)0,   (uint8_t)0,   (uint8_t)0,   (uint8_t)0
};

#define OBJECT_READ_ALLOWED(o)      (((((o)->access_level & (uint8_t)0xf0) >> 4) >= DEV_Current_Accesslevel) ? TRUE : FALSE)
#define OBJECT_WRITE_ALLOWED(o)     ((((o)->access_level & (uint8_t)0x0f) >= DEV_Current_Accesslevel) ? TRUE : FALSE)

#define GET_PROPERTY_CONTROL(p)     (((p)->property_ctrl) >> 5)
#define GET_PROPERTY_TYPE(p)        (((p)->property_ctrl) & (uint8_t)0x1f)

#define GET_TYPE_LEN(t)             KNX_PDT_LEN_TAB[(t)]

#define IS_PROPERTY_WRITEABLE(p)    (((GET_PROPERTY_CONTROL(p) & 0x04) == 0x04) ? TRUE : FALSE)
#define IS_PROP_CTL_VALID(p)        (((GET_PROPERTY_CONTROL(p) & 0x03) != 0x02) ? TRUE : FALSE)
#define IS_ARRAY_VARIABLE(p)        (((GET_PROPERTY_CONTROL(p) & 0x03) == 0x03) ? TRUE : FALSE)

#define IS_POINTER_TO_FUNC(p)       ((((GET_PROPERTY_CONTROL(p) & (uint8_t)0x03) == (uint8_t)0x01) && \
                                      ((p)->property_func == (uint8_t)0x01)) ? TRUE : FALSE)

#define GET_START_INDEX(m)          ((*(uint16_t *)&(m)->num_elems) & (uint16_t)0x0fffu)
#define GET_NUM_ELEMS(m)            (((m)->num_elems) >> 4)

/* Property_Handling. */
#define PH_VALUE_INPLACE    ((uint8_t)0x00)
#define PH_PTR_TO_VAL_FN    ((uint8_t)0x01)
#define PH_PTR_TO_ARRAY     ((uint8_t)0x03)

typedef void (*PROPERTY_FUNC)(KnxMsg_Buffer * pBuffer, boolean write);

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) ios_test(void);
#else
void ios_test(void);


#endif /* KSTACK_MEMORY_MAPPING */

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_START_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) IOS_Dispatch(const KnxMsg_Buffer * pBuffer, uint8_t service, boolean connected)
#else
void IOS_Dispatch(const KnxMsg_Buffer * pBuffer, uint8_t service, boolean connected)
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t data[MAX_PROP_DATA_LEN];
    KNX_PropertyFrameRefType pmsg;
    Knx_InterfaceObjectType const * pobj;
    Knx_PropertyType const * pprop;
    uint8_t num_elems;
    uint16_t start_index;
    uint8_t type;
    uint8_t type_len;
    PROPERTY_FUNC pf;
    Knx_AddressType source;
    Knx_AddressType dest;
    uint8_t ctl;

    if (pBuffer == (KnxMsg_Buffer *)NULL) {
        return;
    }

    pmsg = KnxMsg_GetProperyFramePtr(pBuffer);

/*
**      1. object exists?
*/
    pobj = IOS_GetInterfaceObjectByIndex(pmsg->obj_id);

    if (pobj == (Knx_InterfaceObjectType *)NULL) {
        /* 'Object does not exist'. */
        goto empty_answer;
    }

    if (service == IOS_PROP_DESC_READ) {

        if (pmsg->prop_id == 0) {
            pprop = IOS_GetPropertyByIndex(pobj, pmsg->num_elems);
        } else {
            pprop = IOS_FindProperty(pobj, pmsg->prop_id);
        }

        if (pprop == (Knx_PropertyType *)NULL) {
            /*  A_PropertyDescription_Read_Res_NoData */
            goto empty_answer;
        }

        source = KnxADR_GetPhysAddr();
        dest   = KnxMsg_GetSourceAddress(pBuffer);

        A_PropertyDescription_Read_Res(pBuffer, source, dest, pmsg->obj_id, pprop->property_id,
                                       pmsg->num_elems, GET_PROPERTY_TYPE(pprop), (uint8_t)1, pobj->access_level);
        return;
    }

    num_elems = GET_NUM_ELEMS(pmsg);

    if (num_elems == (uint8_t)0) {
        goto empty_answer;
    }

    start_index = GET_START_INDEX(pmsg);

/*
**  2. access to object allowed?
*/
    if (service == IOS_PROP_READ) {
        if (!OBJECT_READ_ALLOWED(pobj)) {
            /* 'Read-Access denied'. */
            goto empty_answer;
        }
    }

    if (service == IOS_PROP_WRITE) {
        if (!OBJECT_WRITE_ALLOWED(pobj)) {
            /* 'Write-Access denied'. */
            goto empty_answer;
        }
    }

/*
**  3. property exists?
*/
    pprop = IOS_FindProperty(pobj, pmsg->prop_id);

    if (pprop == (Knx_PropertyType *)NULL) {
        /* 'Property does not exist'. */
        goto empty_answer;
    }

    if (!IS_PROP_CTL_VALID(pprop)) {
        goto invalid;
    }

    if (((start_index != (uint8_t)0) || (num_elems > (uint8_t)1)) && (!IS_ARRAY_VARIABLE(pprop))) {
        goto empty_answer;
    }

    ctl        = GET_PROPERTY_CONTROL(pprop);
    type       = GET_PROPERTY_TYPE(pprop);
    type_len   = GET_TYPE_LEN(type);

    if (service == IOS_PROP_READ) {
        if (type == KNX_PDT_CONTROL) {
            if ((!IS_POINTER_TO_FUNC(pprop)) || (pprop->property_var == NULL)) {
                goto invalid;
            }

            pf = (PROPERTY_FUNC)pprop->property_var;
            pf(pBuffer, FALSE);
        } else if (type_len > 0) {
            switch (GET_PROPERTY_CONTROL(pprop) & (uint8_t)0x03) {
                case PH_VALUE_INPLACE:
                    Utl_MemCopy((void *)data, (void *)&pprop->property_var, type_len);
                    break;
                case PH_PTR_TO_VAL_FN:

                    if (pprop->property_func == (uint8_t)0x00) {
                        Utl_MemCopy((void *)data, (void *)pprop->property_var, type_len);
                    } else {
                    }

                    break;
                case PH_PTR_TO_ARRAY:
                    break;
                default:
                    ASSERT(FALSE);
            }
        }  else {
            goto empty_answer;
        }
    } else if (service == IOS_PROP_WRITE) {
/*
**  4. access to property allowed(R/W)?
*/
        if (!IS_PROPERTY_WRITEABLE(pprop)) {
            goto empty_answer;
        }

        if (type == KNX_PDT_CONTROL) {
        } else if (type_len > (uint8_t)0) {
        }  else {
            goto empty_answer;
        }
    } else {
        goto invalid;
    }

    return;

pr_desc_empty:
    source = KnxADR_GetPhysAddr();
    dest   = KnxMsg_GetSourceAddress(pBuffer);
    A_PropertyDescription_Read_Res_NoData(pBuffer, source, dest, pmsg->obj_id, pmsg->prop_id, pmsg->num_elems);
    return;

empty_answer:   /* Property-Read /wo Data. */
    /*  A_PropertyValue_Read_Res_NoData */
    return;

invalid:
    KnxMsg_ReleaseBuffer(pBuffer);
}


#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(Knx_InterfaceObjectType const *, KSTACK_CODE)  IOS_GetInterfaceObjectByIndex(uint16_t object_index)
#else
Knx_InterfaceObjectType const * IOS_GetInterfaceObjectByIndex(uint16_t object_index)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (object_index < Knx_SystemInterfaceObjCount) {
        return *((Knx_SystemInterfaceObjs) + object_index);
    } else if ((Knx_UserInterfaceObjCount != (uint8_t)0) && ((Knx_InterfaceObjectType * *)Knx_UserInterfaceObjPtr !=
                                                           (Knx_InterfaceObjectType * *)NULL))
    {
        object_index -= Knx_SystemInterfaceObjCount;

        if (object_index < Knx_UserInterfaceObjCount) {
            return *((Knx_UserInterfaceObjPtr) + object_index);
        } else {
            return (Knx_InterfaceObjectType const *)NULL;
        }
    } else {
        return (Knx_InterfaceObjectType const *)NULL;
    }
}

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(Knx_PropertyType const *, KSTACK_CODE) IOS_FindProperty(Knx_InterfaceObjectType const * pobj, uint16_t prop_id)  /* todo: binary search!? */
#else
Knx_PropertyType const * IOS_FindProperty(Knx_InterfaceObjectType const * pobj, uint16_t prop_id)                     /* todo: binary search!? */
#endif /* KSTACK_MEMORY_MAPPING */
{
    uint8_t                 idx;
    Knx_PropertyType const *    pprop = (Knx_PropertyType const *)NULL;

    for (idx = 0; idx < pobj->property_count; idx++) {
        pprop = &pobj->properties[idx];

        if (pprop->property_id == prop_id) {
            break;
        }
    }

    return pprop;
}

#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(Knx_PropertyType const *, KSTACK_CODE) IOS_GetPropertyByIndex(Knx_InterfaceObjectType const * pobj, uint16_t prop_index)
#else
Knx_PropertyType const * IOS_GetPropertyByIndex(Knx_InterfaceObjectType const * pobj, uint16_t prop_index)
#endif /* KSTACK_MEMORY_MAPPING */
{
    if (prop_index > pobj->property_count - (uint8_t)1) {
        return (Knx_PropertyType const *)NULL;
    } else {
        return &pobj->properties[prop_index];
    }
}

#if 0
#if KSTACK_MEMORY_MAPPING == STD_ON
FUNC(void, KSTACK_CODE) ios_test(void)
#else
void ios_test(void)
#endif /* KSTACK_MEMORY_MAPPING */
{
    Knx_InterfaceObjectType *   pobj;
    Knx_PropertyType *          pprop;

    pobj   = IOS_GetInterfaceObjectByIndex(0);
    pprop  = IOS_FindProperty(pobj, KNX_PID_ORDER_INFO);
}


#endif

/* Intitialisation-Mode: */
/*
    The BCU switches to initialization mode if during startup the following conditions are fulfilled:
        - PEI-Type 20 is detected
        - programming push-button is pressed

    In initialization mode the following actions take place
        - Physical Address is set to FFFFh
        - Addresstable length is set to 1
        - Optionregister is set to FFh.
        - All load state machines are set to UNLOAD
        - the whole memory from 200h up to 46Fh is cleared
        - the pointer to serial protocol is set to 0000h
        - the EIB-Objectpointer is set to 0000h
        - the EIB-Objectcount is set to 00h
        - all keys are set to FFFFFFFFh


    void DEV_MasterReset(void)
    {
   //        - Addresstable length is set to 1
   //        - Optionregister is set to FFh.
   //        - All load state machines are set to UNLOAD
   //        - the whole memory from 200h up to 46Fh is cleared
   //        - the pointer to serial protocol is set to 0000h
   //        - the EIB-Objectpointer is set to 0000h
   //        - the EIB-Objectcount is set to 00h
   //        - all keys are set to FFFFFFFFh


        KNX_AddressType addr;

        addr[0]=0xff;
        addr[1]=0xff;
        // Check: gibt es keine 'SetAddr'-Funktion mit einem uint16_t-Parameter.
        //  Set Physical-Address to 0xFFFF.
        ADR_SetPhysAddr(addr);
    }

 */

/* boolean IOS_IsArrayProperty(KNX_INTERFACE_OBJ* pobj,KNX_PROPERTY *pprop); */

/* uint16_t IOS_GetPropertyMaxElementCount(KNX_INTERFACE_OBJ* pobj,KNX_PROPERTY *pprop); */

/* uint16_t IOS_GetPropertyActualElementCount(KNX_INTERFACE_OBJ* pobj,KNX_PROPERTY *pprop); */

/*void IOS_GetPropertyDescription(KNX_INTERFACE_OBJ* pobj,KNX_PROPERTY *pprop,??? *pdescr); */
/* type,max_elem,RdAcc,WrAccc */

/*
**      ******************
**      MSC-spezifische Makros: _WIN32 und _MSC_VER
**      ******************
**
**      ******************
**      Metroworks-spezifische Makros:  __HIWARE__,__MWERKS__
**      ******************
**
*/

/*
   Allocation in battery buffered RAM:
   // Extract from source file "bufram.c"

   #pragma DATA_SEG Buffered_RAM
        int done;
        int status[100];
   #pragma DATA_SEG DEFAULT
   // End of extract from "bufram.c"

   // SmartLinker parameter file:
        LINK bufram.ABS
        NAMES
                bufram.o startup.o
        END
        STACKSIZE 0x200
        SECTIONS
                BatteryRAM = NO_INIT 0x1000 TO 0x13FF;
                EEPROM_AREA = NO_INIT    0x0800 TO 0x0FFF;
                MyRAM = READ_WRITE 0x5000 TO 0x5FFF;
        PLACEMENT
                DEFAULT_ROM INTO READ_ONLY 0x2000 TO 0x2800;
                DEFAULT_RAM INTO MyRAM;
                EEPROM  INTO  EEPROM_AREA;
                Buffered_RAM INTO BatteryRAM;
        END
 */

#if KSTACK_MEMORY_MAPPING == STD_ON
    #define KSTACK_STOP_SEC_CODE
    #include "MemMap.h"
#endif /* KSTACK_MEMORY_MAPPING */
