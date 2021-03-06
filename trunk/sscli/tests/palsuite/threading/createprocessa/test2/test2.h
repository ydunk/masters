/*============================================================
**
** Source: test2.h
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
**===========================================================*/


const char *szChildFileA = "childprocess";
const char *szArgs = " A B C";
const char *szArg1 = "A";
const char *szArg2 = "B";
const char *szArg3 = "C";

const char *szPathDelimA = "\\";

const char *szTestString = "Copyright (c) Microsoft";

const DWORD EXIT_OK_CODE   = 100;
const DWORD EXIT_ERR_CODE1 = 101;
const DWORD EXIT_ERR_CODE2 = 102;
const DWORD EXIT_ERR_CODE3 = 103;
const DWORD EXIT_ERR_CODE4 = 104;
const DWORD EXIT_ERR_CODE5 = 105;

#define BUF_LEN  64

/*
 * Take two wide strings representing file and directory names
 * (dirName, fileName), join the strings with the appropriate path
 * delimiter and populate a wide character buffer (absPathName) with
 * the resulting string.
 *
 * Returns: The number of wide characters in the resulting string.
 * 0 is returned on Error.
 */
int 
mkAbsoluteFilenameA ( 
    LPSTR dirName,  
    DWORD dwDirLength, 
    LPCSTR fileName, 
    DWORD dwFileLength,
    LPSTR absPathName )
{
    extern const char *szPathDelimA;

    DWORD sizeDN;
    DWORD sizeFN;
    DWORD sizeAPN;
    
    sizeDN = strlen( dirName );
    sizeFN = strlen( fileName );
    sizeAPN = (sizeDN + 1 + sizeFN + 1);
    
    /* insure ((dirName + DELIM + fileName + \0) =< _MAX_PATH ) */
    if ( sizeAPN > _MAX_PATH )
    {
        return ( 0 );
    }
    
    strncpy(absPathName, dirName, dwDirLength +1);
    strcat(absPathName, szPathDelimA);
    strcat(absPathName, fileName);
    
    return (sizeAPN);
  
} 
