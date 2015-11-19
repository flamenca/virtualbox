/* $Id$ */
/** @file
 * BS3Kit - Bs3SlabListAlloc
 */

/*
 * Copyright (C) 2007-2015 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 *
 * The contents of this file may alternatively be used under the terms
 * of the Common Development and Distribution License Version 1.0
 * (CDDL) only, as it comes in the "COPYING.CDDL" file of the
 * VirtualBox OSE distribution, in which case the provisions of the
 * CDDL are applicable instead of those of the GPL.
 *
 * You may elect to license modified versions of this file under the
 * terms and conditions of either the GPL or the CDDL or both.
 */

#include "bs3kit-template-header.h"
#include <iprt/asm.h>


BS3_DECL(void BS3_FAR *) Bs3SlabListAlloc(PBS3SLABHEAD pHead)
{
    if (pHead->cFreeChunks)
    {
        PBS3SLABCLT pCur;
        for (pCur = BS3_XPTR_GET(BS3SLABCLT, pHead->pFirst);
             pCur != NULL;
             pCur = BS3_XPTR_GET(BS3SLABCLT, pCur->pNext))
        {
            if (pCur->cFreeChunks)
            {
                int32_t iBit = ASMBitFirstClear(&pCur->bmAllocated, pCur->cChunks);
                if (iBit >= 0)
                {
                    BS3_XPTR_AUTO(void, pvRet);
                    ASMBitSet(&pCur->bmAllocated, iBit);
                    pCur->cFreeChunks  -= 1;
                    pHead->cFreeChunks -= 1;

                    BS3_XPTR_SET_FLAT(void, pvRet,
                                      BS3_XPTR_GET_FLAT(uint8_t, pCur->pbStart) + ((uint32_t)iBit << pCur->cChunkShift));
                    return BS3_XPTR_GET(void, pvRet);
                }
            }
        }
    }
    return NULL;
}
