/*============================================================================
**
** Source:  test2.c
**
** Purpose: Test #2 for the wcstol function. Does a simple test with radix
**          10.
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
**==========================================================================*/



#include <palsuite.h>


/*
 * Notes: wcstol should depend on the current locale's LC_NUMERIC category, 
 * this is not currently tested.
 */

int __cdecl main(int argc, char *argv[])
{
    WCHAR *end;    
    WCHAR teststr[] = {'1','2','3','4','5',0};
    long result = 12345;
    long l;
    
    if (0 != PAL_Initialize(argc, argv))
    {
        return FAIL;
    }


    l = wcstol(teststr, &end, 10);

    if (l != result)
    {
        Fail("ERROR: Expected wcstol to return %d, got %d\n", result, l);
    }

    if (end != teststr + 5)
    {
        Fail("ERROR: Expected wcstol to give an end value of %p, got %p\n",
            teststr + 5, end);
    }

    PAL_Terminate();
    return PASS;
}
