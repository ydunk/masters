// ==++==
// 
//   
//    Copyright (c) 2002 Microsoft Corporation.  All rights reserved.
//   
//    The use and distribution terms for this software are contained in the file
//    named license.txt, which can be found in the root of this distribution.
//    By using this software in any fashion, you are agreeing to be bound by the
//    terms of this license.
//   
//    You must not remove this notice, or any other, from this software.
//   
// 
// ==--==
//*****************************************************************************
// Implementation for CBlobFetcher
//
//
//*****************************************************************************
#include "stdafx.h" // for ASSERTE and friends
#include "blobfetcher.h"

//-----------------------------------------------------------------------------
//  round a pointer back down to something aligned
static inline char* truncateTo(char* val, unsigned align) {
    _ASSERTE((align & (align - 1)) == 0);       // align must be a power of 2

    return((char*) (unsigned(val) & ~(align-1)));
}

//-----------------------------------------------------------------------------
//  round up to a certain alignment
static inline unsigned roundUp(unsigned val, unsigned align) {
    _ASSERTE((align & (align - 1)) == 0);       // align must be a power of 2

    return((val + (align-1)) & ~(align-1));
}

//-----------------------------------------------------------------------------
//  round up to a certain alignment
static inline unsigned padForAlign(unsigned val, unsigned align) {
    _ASSERTE((align & (align - 1)) == 0);       // align must be a power of 2
    return ((-int(val)) & (align-1));
}

//*****************************************************************************
// Pillar implementation
//*****************************************************************************
//-----------------------------------------------------------------------------
CBlobFetcher::CPillar::CPillar() {
    m_dataAlloc = m_dataStart = m_dataCur = m_dataEnd = NULL;

    m_nTargetSize = 0x10000;    
}

//-----------------------------------------------------------------------------
CBlobFetcher::CPillar::~CPillar() {
// Sanity check to make sure nobody messed up the pts
    _ASSERTE((m_dataCur >= m_dataStart) && (m_dataCur <= m_dataEnd));

    delete [] m_dataAlloc;
}


//-----------------------------------------------------------------------------
// Transfer ownership of data, so src will lose data and this will get it.
// Data itself will remain untouched, just ptrs & ownership change
//-----------------------------------------------------------------------------
void CBlobFetcher::CPillar::StealDataFrom(CBlobFetcher::CPillar & src)
{
// We should only be moving into an empty Pillar
    _ASSERTE(m_dataStart == NULL);


    m_dataAlloc     = src.m_dataAlloc;
    m_dataStart     = src.m_dataStart;
    m_dataCur       = src.m_dataCur;
    m_dataEnd       = src.m_dataEnd;

    m_nTargetSize   = src.m_nTargetSize;

// Take away src's claim to data. This prevents multiple ownership and double deleting
    src.m_dataAlloc = src.m_dataStart = src.m_dataCur = src.m_dataEnd = NULL;

}

//-----------------------------------------------------------------------------
// Allocate a block in this particular pillar
//-----------------------------------------------------------------------------
/* make a new block 'len' bytes long'  However, move the pointer 'pad' bytes
   over so that the memory has the correct alignment characteristics */

char * CBlobFetcher::CPillar::MakeNewBlock(unsigned len, unsigned pad) {

    _ASSERTE(pad < maxAlign);

// Make sure we have memory in this block to allocatate
    if (m_dataStart == NULL) {
        // make sure allocate at least as big as length
        m_nTargetSize = max(m_nTargetSize, len);

        //
        // We need to allocate memory with an offset of "pad" from
        // being "maxAlign" aligned. (data % maxAlign == pad).
        // Since "new" doesn't do this, allocate some extra 
        // to handle the worst possible alignment case.
        // 

        m_dataAlloc = new char[m_nTargetSize+(maxAlign-1)];
        if (m_dataAlloc == NULL)
            return NULL;

        m_dataStart = m_dataAlloc + 
          ((pad - unsigned(m_dataAlloc)) & (maxAlign-1));

        _ASSERTE(unsigned(m_dataStart) % maxAlign == pad);
        
        m_dataCur = m_dataStart;
    
        m_dataEnd = &m_dataStart[m_nTargetSize];
    }

    _ASSERTE(m_dataCur >= m_dataStart);
    _ASSERTE((int) len > 0);

// If this block is full, then get out, we'll have to try another block
    if (m_dataCur + len > m_dataEnd)  { 
        return NULL;
    }
    
    char* ret = m_dataCur;
    m_dataCur += len;
    _ASSERTE(m_dataCur <= m_dataEnd);
    return(ret);
}


//*****************************************************************************
// Blob Fetcher Implementation
//*****************************************************************************

//-----------------------------------------------------------------------------
CBlobFetcher::CBlobFetcher()
{
// Setup storage
    m_pIndex = NULL;
    m_nIndexMax = 1; // start off with arbitrary small size  @@@ (minimum is 1) 
    m_nIndexUsed = 0;
    _ASSERTE(m_nIndexUsed < m_nIndexMax); // use <, not <=

    m_nDataLen = 0;

    m_pIndex = new CPillar[m_nIndexMax];
    _ASSERTE(m_pIndex);
}

//-----------------------------------------------------------------------------
CBlobFetcher::~CBlobFetcher()
{
    delete [] m_pIndex;
}


//-----------------------------------------------------------------------------
// Dynamic mem allocation, but we can't move old blocks (since others
// have pointers to them), so we need a fancy way to grow
//-----------------------------------------------------------------------------
char* CBlobFetcher::MakeNewBlock(unsigned len, unsigned align) {

    _ASSERTE(m_pIndex);
    _ASSERTE(0 < align && align <= maxAlign);

    // deal with alignment 
    unsigned pad = padForAlign(m_nDataLen,align);
    char* pChRet = NULL;
    if (pad != 0) {
        pChRet = m_pIndex[m_nIndexUsed].MakeNewBlock(pad, 0);
        // if don't have space for the pad, then need to allocate a new pillar
        // the allocation will handle the padding for the alignment of m_nDataLen
        if (pChRet) {
            memset(pChRet, 0, pad);
            m_nDataLen += pad;
            pad = 0;
        }
    }
#ifdef _DEBUG
    if (pChRet)
        _ASSERTE(m_nDataLen % align == 0);
#endif

// Quickly compute total data length is tough since we have alignment problems
// We'll do it by get the length of all the completely full pillars so far
// and then adding on the size of the current pillar
    unsigned nPreDataLen = m_nDataLen - m_pIndex[m_nIndexUsed].GetDataLen();

    pChRet = m_pIndex[m_nIndexUsed].MakeNewBlock(len + pad, 0);
    if (pChRet == NULL) {
        m_nIndexUsed ++; // current pillar is full, move to next
        nPreDataLen = m_nDataLen;

        if (m_nIndexUsed == m_nIndexMax) { // entire array of pillars are full, re-org
            const unsigned nNewMax = m_nIndexMax * 2; // arbitrary new size             

            CPillar* pNewIndex = new CPillar[nNewMax];
            _ASSERTE(pNewIndex);
            
            // Copy old stuff
            for(unsigned i = 0; i < m_nIndexMax; i++) pNewIndex[i].StealDataFrom(m_pIndex[i]);
            
            delete [] m_pIndex;

            m_nIndexMax = nNewMax;
            m_pIndex = pNewIndex;
        }
    
    // Make sure the new pillar is large enough to hold the data
    // How we do this is *totally arbitrary* and has been optimized for how
    // we intend to use this.       
        if (m_pIndex[m_nIndexUsed].GetAllocateSize() < len) {
            m_pIndex[m_nIndexUsed].SetAllocateSize(roundUp(len, maxAlign));
        }

    // Now that we're on new pillar, try again
        pChRet = m_pIndex[m_nIndexUsed].MakeNewBlock(len + pad, m_nDataLen % maxAlign);
        if (pChRet == NULL)
            return NULL;
        _ASSERTE(pChRet);

    // The current pointer picks up at the same alignment that the last block left off
        _ASSERTE(nPreDataLen % maxAlign == ((unsigned) pChRet) % maxAlign);
    }
    if (pad != 0) {
        memset(pChRet, 0, pad);
        pChRet += pad;
    }

    m_nDataLen = nPreDataLen + m_pIndex[m_nIndexUsed].GetDataLen();

    _ASSERTE(((unsigned) m_nDataLen -len) % align == 0);
    _ASSERTE(((unsigned) pChRet) % align == 0);
    return pChRet;
}

//-----------------------------------------------------------------------------
// Index segment as if this were linear (middle weight function)
//-----------------------------------------------------------------------------
char * CBlobFetcher::ComputePointer(unsigned offset) const
{
    _ASSERTE(m_pIndex);
    unsigned idx = 0;

    if (offset == 0) {
        // if ask for a 0 offset and no data, return NULL
        if (m_pIndex[0].GetDataLen() == 0)
            return NULL;
    } else {
        while (offset >= m_pIndex[idx].GetDataLen()) {
            offset -= m_pIndex[idx].GetDataLen();
            idx ++;
            // Overflow - have asked for an offset greater than what exists
            if (idx > m_nIndexUsed) {
                _ASSERTE(!"CBlobFetcher::ComputePointer() Overflow");
                return NULL;
            }
        }
    }

    char * ptr = (char*) (m_pIndex[idx].GetRawDataStart() + offset);
    return ptr;
}

//-----------------------------------------------------------------------------
// See if a pointer came from this blob fetcher
//-----------------------------------------------------------------------------
BOOL CBlobFetcher::ContainsPointer(char *ptr) const
{
    _ASSERTE(m_pIndex);

    CPillar *p = m_pIndex;
    CPillar *pEnd = p + m_nIndexUsed;

    unsigned offset = 0;

    while (p <= pEnd) {
        if (p->Contains(ptr))
            return TRUE;

        offset += p->GetDataLen();
        p++;
    }

    return FALSE;
}

//-----------------------------------------------------------------------------
// Find a pointer as if this were linear (middle weight function)
//-----------------------------------------------------------------------------
unsigned CBlobFetcher::ComputeOffset(char *ptr) const
{
    _ASSERTE(m_pIndex);

    CPillar *p = m_pIndex;
    CPillar *pEnd = p + m_nIndexUsed;

    unsigned offset = 0;

    while (p <= pEnd) {
        if (p->Contains(ptr))
            return offset + p->GetOffset(ptr);

        offset += p->GetDataLen();
        p++;
    }

    _ASSERTE(!"Pointer not found");
    return 0;
}

//-----------------------------------------------------------------------------
// Truncate our complex storage as if it were an array
//-----------------------------------------------------------------------------
HRESULT CBlobFetcher::Truncate(unsigned newLen)
{
// Implementation design: we simply need to move the m_dataCur pointers
    if (newLen == m_nDataLen) return S_OK;

    unsigned idx = 0; // start at end and work up

// Find which pillar (idx) new length will leave us in
    unsigned nLen = 0;
    while(nLen < newLen) {
        nLen += m_pIndex[idx].GetDataLen();
        idx ++;
    }

// Set new pillars data pointer accordingly
    idx --;
    nLen -= m_pIndex[idx].GetDataLen(); // goto start of pillar
    
    unsigned newCur = newLen - nLen;

    m_pIndex[idx].m_dataCur = m_pIndex[idx].m_dataStart + newCur;

// Reset rest of pillars
    for(unsigned i = idx + 1; i < m_nIndexUsed; i ++) m_pIndex[i].m_dataCur = m_pIndex[i].m_dataStart;
    
    m_nIndexUsed = idx;

    return S_OK;
}


//Take the data from our previous blob and copy it into our new blob 
//after whatever was already in that blob.
HRESULT CBlobFetcher::Merge(CBlobFetcher *destination) {
    unsigned dataLen;
    char *dataBlock;
    char *dataCurr;
    unsigned idx;
    _ASSERTE(destination);

    dataLen = GetDataLen();
    _ASSERTE( dataLen >= 0 );

    // Make sure there actually is data in the previous blob before trying to append it.
    if ( 0 == dataLen )
    {
        return S_OK;
    }

    //Get the length of our data and get a new block large enough to hold all of it.
    dataBlock = destination->MakeNewBlock(dataLen,1);
    if (dataBlock==NULL) {
        return E_OUTOFMEMORY;
    }
    
    //Copy all of the bytes using the write algorithm from PEWriter.cpp
    dataCurr=dataBlock;
    for (idx=0; idx<=m_nIndexUsed;  idx++) {
        if (m_pIndex[idx].GetDataLen()>0) {
            _ASSERTE(dataCurr<dataBlock+dataLen);
            memcpy(dataCurr, m_pIndex[idx].GetRawDataStart(), m_pIndex[idx].GetDataLen());
            dataCurr+=m_pIndex[idx].GetDataLen();
        }
    }

    return S_OK;

}