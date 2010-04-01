/*
 *   KONNEX/EIB-Protocol-Stack.
 *
 *  (C) 2007-2010 by Christoph Schueler <chris@konnex-tools.de,
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
**  todo: Benutzer-Callbacks f. Memory-Read()/-Write(), um bspw. BCU-RAM emulieren zu k�nnen.
**      Hinweis:        'MEM_'-Prefixe sind besser!!!
**                              Die RAM-Routinen sind im Gegensatz zu den EEPROM-Routinen Plattform-unabh�ngig!!!
**
**  todo: aus Effizienz-Gr�nden 'SetByte' implementieren.
*/

#include "Memory.h"

/*
Memory-Types (Absolute Code/Data Allocation Record)
---------------------------------------------------
1   Zero page RAM
2   RAM
3   EEPROM
*/


#define NUM_MCBs        1

uint8 UserLowRAM[0x23];

MemoryControlBlock MCBs[NUM_MCBs]=
{
        {0x00BD,&UserLowRAM,0x23,mtRAM,0x00},
};

void MM_ClearMCBs(void)
{
        ZeroRAM(MCBs,sizeof(MemoryControlBlock)*NUM_MCBs);
}

/*
** EIB-Adresse in Target-Adresse �bersetzen.
**
**      check: Fehler-Behandlung bei den folgenden beiden Funktionen!!!
*/
uint16 MM_MapAddressToTarget(uint16 Address)
{
        return  0xffff;
}

/*
** Target-Adresse in EIB-Adresse �bersetzen.
*/
uint16 MM_MapAddressFromTarget(uint16 Address)
{
        return  0xffff;
}

/* check: �berfl�ssig?! */
/*
int MM_SetByte(uint16 Address,uint8 value)
{
        return 0;
}

int MM_GetByte(uint16 Address,uint8 *value)
{
        return 1;
}
*/

/*
**      todo: die folgenden Funktionen sollten in ???RAM umbenannt werden.
**                      ausserdem Funktionen implementieren, die RAM und EEPROM transparent handeln,
**                      wobei 'Comp' lediglich ein Synonym darstellt.
*/

/*
void FillMem(void *p,uint8 b,uint16 len)
{
    uint8 *bp=(uint8*)p;
    
    while (len--) {
        *bp++=b;
    }
}

void CopyMem(void *d,void *s,uint16 len)
{
    uint8 *bd=(uint8*)d;
    uint8 *bs=(uint8*)s;
    
    while (len--) {
        *bd++=*bs++;
    }
}


void ZeroMem(void *p,uint16 len)
{
    uint8 *bp=(uint8*)p;
    
    while (len--) {
        *bp++=(uint8)0;
    }
}
*/

boolean CompMem(void *p1,void *p2,uint16 len)
{
    uint8 *bp1=(uint8*)p1;
    uint8 *bp2=(uint8*)p2;

    while (len--) {
        if ((*bp1++)!=(*bp2++)) {
            return FALSE;
        }
    }
    return TRUE;
}

void FillRAM(void *p,uint8 b,uint16 len)
{
    uint8 *bp=(uint8*)p;
    
    while (len--) {
        *bp++=b;
    }
}

void CopyRAM(void *d,void *s,uint16 len)
{
    uint8 *bd=(uint8*)d;
    uint8 *bs=(uint8*)s;
    
    while (len--) {
        *bd++=*bs++;
    }
}


void ZeroRAM(void *p,uint16 len)
{
    uint8 *bp=(uint8*)p;
    
    while (len--) {
        *bp++=(uint8)0;
    }
}
