/*=============================================================
**
** Source:  createfilemappingw.c (test 8)
**
** Purpose: Positive test the CreateFileMappingW API.
**          Test the un-verifiable parameter combinations.
**
** 
**  Copyright (c) 2002 Microsoft Corporation.  All rights reserved.
** 
**  The use and distribution terms for this software are contained in the file
**  named license.txt, which can be found in the root of this distribution.
**  By using this software in any fashion, you are agreeing to be bound by the
**  terms of this license.
** 
**  You must not remove this notice, or any other, from this software.
** 
**
**============================================================*/
#define UNICODE
#include <palsuite.h>

const   int MAPPINGSIZE = 2048;
HANDLE  SWAP_HANDLE     = ((VOID *)(-1));

int __cdecl main(int argc, char *argv[])
{
    WCHAR   lpObjectName[] = {'m','y','O','b','j','e','c','t','\0'};
    
    HANDLE  hFileMap;

    /* Initialize the PAL environment.
     */
    if(0 != PAL_Initialize(argc, argv))
    {
        return FAIL;
    }

    /* Create a READONLY, "swap", un-named file mapping.
     * This test is unverifiable since there is no hook back to the file map
     * because it is un-named. As well, since it resides in "swap", and is
     * initialized to zero, there is nothing to read.
     */
    hFileMap = CreateFileMapping(
                            SWAP_HANDLE,
                            NULL,               /*not inherited*/
                            PAGE_READONLY,      /*read only*/
                            0,                  /*high-order size*/
                            MAPPINGSIZE,        /*low-order size*/
                            NULL);              /*un-named object*/

    if(NULL == hFileMap)
    {
        Fail("ERROR:%u: Failed to create File Mapping.\n", 
              GetLastError());
    }

    
    /* Create a COPYWRITE, "swap", un-named file mapping.
     * This test is unverifiable, here is a quote from MSDN:
     * 
     * Copy on write access. If you create the map with PAGE_WRITECOPY and 
     * the view with FILE_MAP_COPY, you will receive a view to file. If you 
     * write to it, the pages are automatically swappable and the modifications
     * you make will not go to the original data file. 
     *
     */
    hFileMap = CreateFileMapping(
                            SWAP_HANDLE,
                            NULL,               /*not inherited*/
                            PAGE_WRITECOPY,     /*write copy*/
                            0,                  /*high-order size*/
                            MAPPINGSIZE,        /*low-order size*/
                            NULL);              /*unnamed object*/

    if(NULL == hFileMap)
    {
        Fail("ERROR:%u: Failed to create File Mapping.\n", 
              GetLastError());
    }

    /* Create a COPYWRITE, "swap", named file mapping.
     * This test is unverifiable, here is a quote from MSDN:
     * 
     * Copy on write access. If you create the map with PAGE_WRITECOPY and 
     * the view with FILE_MAP_COPY, you will receive a view to file. If you 
     * write to it, the pages are automatically swappable and the modifications
     * you make will not go to the original data file. 
     *
     */
    hFileMap = CreateFileMapping(
                            SWAP_HANDLE,
                            NULL,           /*not inherited*/
                            PAGE_WRITECOPY, /*writecopy*/
                            0,              /*high-order size*/
                            MAPPINGSIZE,    /*low-order size*/
                            lpObjectName);  /*named object*/

    if(NULL == hFileMap)
    {
        Fail("ERROR:%u: Failed to create File Mapping.\n", 
              GetLastError());
    }



    /* Terminate the PAL.
     */ 
    PAL_Terminate();
    return PASS;
}

