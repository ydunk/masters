/*=====================================================================
**
** Source:  GetLongPathNameW.c FreeBSD version(test 2)
**
** Purpose: Tests the PAL implementation of the GetLongPathNameW function
**          as expected under FreeBSD
**
** Depends on:
**      CreateDirectoryA
**      RemoveDirectoryW
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
**===================================================================*/
/*
tests:
    - test invalid path names
    - test an already short path name
    - test a long path name
    - test with buffer size too small
*/

#include <palsuite.h>


int __cdecl main(int argc, char *argv[])
{
/* Since GetLongPathNameW operates differently under FreeBSD and Win32 this test
   is for freeBSD only. It runs the same tests as the Win32 version but checks for
   different results
*/
#if !(WIN32) /* Only execute if the is Free BSD */

    DWORD dwRc = 0;
    WCHAR szwReturnedPath[MAX_PATH];
    WCHAR szwSmallBuff[3];
    const char szShortPathName[] = {"testing"};
    const char szLongPathName[] = {"This_is_a_long_directory_name"};
    /* since BSD doesn't shorten long dir names, it will only use the long name */
    const char szShortenedPathName[] = {"This_is_a_long_directory_name"};
    WCHAR* wLongPathPtr = NULL;
    WCHAR* wShortPathPtr = NULL;



    if (0 != PAL_Initialize(argc,argv))
    {
        return FAIL;
    }
    memset(szwReturnedPath, 0, MAX_PATH*sizeof(WCHAR));
    memset(szwSmallBuff, 0, 3*sizeof(WCHAR));
    wLongPathPtr = convert((char*)szLongPathName);
    wShortPathPtr = convert((char*)szShortenedPathName);
    
    /* do some clean up just to be safe */
    RemoveDirectoryW(wLongPathPtr);
    RemoveDirectoryW(wShortPathPtr);


    /* attempt call on an invalid short path name */
    dwRc = GetLongPathNameW(wShortPathPtr, szwReturnedPath, MAX_PATH);
    if (dwRc != 0)
    {
        Trace("GetLongPathNameW: ERROR -> Call made with an invalid short "
            "path \"%S\" returned \"%S\"\n",
            wShortPathPtr,
            szwReturnedPath);
        free (wLongPathPtr);
        free (wShortPathPtr);
        Fail("");
    }


    /* attempt call on an invalid long path name */
    dwRc = GetLongPathNameW(wLongPathPtr, szwReturnedPath, MAX_PATH);
    if (dwRc != 0)
    {
        Trace("GetLongPathNameW: ERROR -> Call made with an invalid long "
            "path \"%S\" returned \"%S\"\n",
			wLongPathPtr,
            szwReturnedPath);
        free (wLongPathPtr);
        free (wShortPathPtr);
        Fail("");
    }


    /* create a long directory name */
    if (TRUE != CreateDirectoryW(wLongPathPtr, NULL))
    {
        free(wLongPathPtr);
        free(wShortPathPtr);
        Fail("GetLongPathNameW: ERROR -> CreateDirectoryW failed with an error"
            " code of %ld when asked to create a directory called \"%s\".\n",
            GetLastError(),
            szLongPathName);
    }


    /* get the long path name */
    memset(szwReturnedPath, 0, MAX_PATH*sizeof(WCHAR));
    dwRc = GetLongPathNameW(wShortPathPtr, szwReturnedPath, MAX_PATH);
    if (dwRc != 0)
    {
        Trace("GetLongPathNameW: ERROR -> Under FreeBSD, this test should"
            " have returned 0 but instead returned %d.\n",
            dwRc);
        if (RemoveDirectoryW(wLongPathPtr) != TRUE)
        {
            Trace("GetLongPathNameW: ERROR -> RemoveDirectoryW failed to "
                " remove the directory \"%S\" with an error code of %ld.\n",
                wLongPathPtr,
                GetLastError());
        }
        free(wLongPathPtr);
        free(wShortPathPtr);
        Fail("");
    }
    if (szwReturnedPath[0] != 0)
    {
        Trace("GetLongPathNameW: ERROR -> Under FreeBSD, this test should"
            " not have touched lpszLongPath but instead it returned\"%s\".\n",
            szwReturnedPath);
        if (RemoveDirectoryW(wLongPathPtr) != TRUE)
        {
            Trace("GetLongPathNameW: ERROR -> RemoveDirectoryW failed to "
                " remove the directory \"%S\" with an error code of %ld.\n",
                wLongPathPtr,
                GetLastError());
        }
        free(wLongPathPtr);
        free(wShortPathPtr);
        Fail("");
    }

    if (RemoveDirectoryW(wLongPathPtr) != TRUE)
    {
        Trace("GetLongPathNameW: ERROR -> RemoveDirectoryW failed to "
            " remove the directory \"%S\" with an error code of %ld.\n",
            wLongPathPtr,
            GetLastError());
        free(wLongPathPtr);
        free(wShortPathPtr);
        Fail("");
    }
    free(wShortPathPtr);
    free(wLongPathPtr);


    /* test an actual short name */
    /* create a long directory name */
    wShortPathPtr = convert((char*)szShortPathName);
    RemoveDirectoryW(wShortPathPtr);

    if (TRUE != CreateDirectoryW(wShortPathPtr, NULL))
    {
        free(wShortPathPtr);
        Fail("GetLongPathNameW: ERROR -> CreateDirectoryW failed with an error"
            " code of %ld when asked to create a directory called \"%s\".\n",
            GetLastError(),
            szShortPathName);
    }


    /* get the long path name */
    memset(szwReturnedPath, 0, MAX_PATH*sizeof(WCHAR));
    dwRc = GetLongPathNameW(wShortPathPtr, szwReturnedPath, MAX_PATH);
    if (dwRc != 0)
    {
        Trace("GetLongPathNameW: ERROR -> Under FreeBSD, this test should"
            " have returned 0 but instead returned %d.\n",
            dwRc);
        if (RemoveDirectoryW(wShortPathPtr) != TRUE)
        {
            Trace("GetLongPathNameW: ERROR -> RemoveDirectoryW failed to "
                " remove the directory \"%S\" with an error code of %ld.\n",
                wShortPathPtr,
                GetLastError());
        }
        free(wShortPathPtr);
        Fail("");
    }
    if (szwReturnedPath[0] != 0)
    {
        Trace("GetLongPathNameW: ERROR -> Under FreeBSD, this test should"
            " not have touched lpszLongPath but instead it returned\"%s\".\n",
            szwReturnedPath);
        if (RemoveDirectoryW(wShortPathPtr) != TRUE)
        {
            Trace("GetLongPathNameW: ERROR -> RemoveDirectoryW failed to "
                " remove the directory \"%S\" with an error code of %ld.\n",
                wShortPathPtr,
                GetLastError());
        }
        free(wShortPathPtr);
        Fail("");
    }


    /* test using a too small return buffer */
    dwRc = GetLongPathNameW(wShortPathPtr, szwSmallBuff, 3);
    if (dwRc != 0)
    {
        Trace("GetLongPathNameW: ERROR -> Under FreeBSD, this test should"
            " have returned 0 but instead returned %d.\n",
            dwRc);
        if (RemoveDirectoryW(wShortPathPtr) != TRUE)
        {
            Trace("GetLongPathNameW: ERROR -> RemoveDirectoryW failed to "
                " remove the directory \"%S\" with an error code of %ld.\n",
                wShortPathPtr,
                GetLastError());
        }
        free(wShortPathPtr);
        Fail("");
    }
    if (szwSmallBuff[0] != 0)
    {
        Trace("GetLongPathNameW: ERROR -> Under FreeBSD, this test should"
            " not have touched lpszLongPath but instead it returned\"%s\".\n",
            szwReturnedPath);
        if (RemoveDirectoryW(wShortPathPtr) != TRUE)
        {
            Trace("GetLongPathNameW: ERROR -> RemoveDirectoryW failed to "
                " remove the directory \"%S\" with an error code of %ld.\n",
                wShortPathPtr,
                GetLastError());
        }
        free(wShortPathPtr);
        Fail("");
    }

    /* clean up */
    if (RemoveDirectoryW(wShortPathPtr) != TRUE)
    {
        Trace("GetLongPathNameW: ERROR -> RemoveDirectoryW failed to "
            " remove the directory \"%S\" with an error code of %ld.\n",
            wShortPathPtr,
            GetLastError());
        free(wShortPathPtr);
        Fail("");
    }
    free(wShortPathPtr);

    PAL_Terminate();

#endif /* Free BSD */
    return PASS;
}

