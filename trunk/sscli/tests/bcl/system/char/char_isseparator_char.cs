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
using System;
using System.IO;
using System.Globalization;
using System.Text;
using System.Collections;
public class IsSeparator_char
{
 protected const string  UNABLE_TO_OPEN_FILE   = "Could not open data file";
 protected const string  BAD_DATA_FILE         = "Read error on data file";
 protected const string  UNSUPPORTED_CATEGORY 
   = "Unsupported Unicode category: ";
 protected const string  DATA_FILENAME         = "UnicodeData.txt";
 protected static readonly char FIELD_SEP    = ';';
 protected static readonly char FRAC_SEP     = '/';
 protected SortedList    m_CharData;     
 protected uint          m_uCharsRead;   
 public IsSeparator_char()
   {
   m_CharData = new SortedList();
   m_uCharsRead = 0;
   }
 public virtual bool runTest()
   {
   IList charData;          
   int iCountErrors = 0;
   int iCountTestcases = 0;
   charData = m_CharData.GetValueList();
   for (int iCurChar = 0; iCurChar < m_CharData.Count; iCurChar++)
     {
     CharInfo curCharInfo;    
     curCharInfo = (CharInfo) charData[iCurChar];
     if ( (curCharInfo.eCategory == UnicodeCategory.SpaceSeparator) || 
	  (curCharInfo.eCategory == UnicodeCategory.LineSeparator)  || 
	  (curCharInfo.eCategory == UnicodeCategory.ParagraphSeparator) )
       {
       iCountTestcases++ ;
       if ( Char.IsSeparator(curCharInfo.chChar) )
	 {                    
	 Console.WriteLine("Passed :: " + 
			   "Categories match for char '" + 
			   ((int)(curCharInfo.chChar)).ToString("x4") + 
			   "' at position " + iCurChar.ToString("d3") + " Category :: " + 
			   curCharInfo.eCategory);
	 }
       else
	 {
	 iCountErrors++ ;
	 Console.WriteLine("Failed :: " +
			   "Category mismatch for char '0x" + 
			   ((int)(curCharInfo.chChar)).ToString("x4") + 
			   "' at position " + iCurChar.ToString("d3") + 
			   " UCD  Category : " + curCharInfo.eCategory + 
			   " NLS+ Category : " + 
			   Char.IsSeparator(curCharInfo.chChar) );
	 }
       }         
     }
   if ( iCountErrors == 0 ){
   Console.Error.Write( "Char_IsSeparator_char: paSs. iCountTestcases== " + iCountTestcases.ToString("d3"));
   return true;
   }
   else{
   Console.Error.Write( "Char_IsSeparator_char: Fail. iCountErrors== " + iCountErrors.ToString("d3"));
   return false;
   }
   }
 protected UnicodeCategory TranslateUnicodeCategory(string strCatAbbrev) 
   {
   UnicodeCategory eCategory = (UnicodeCategory)(-1);
   if ( (strCatAbbrev == null) || (strCatAbbrev.Length == 0) )
     {
     throw new ArgumentException("String null/empty","strCatAbbrev");
     }
   switch (strCatAbbrev)
     {
     case "Lu":
       eCategory = UnicodeCategory.UppercaseLetter;
       break;
     case "Ll":
       eCategory = UnicodeCategory.LowercaseLetter;
       break;
     case "Lt":
       eCategory = UnicodeCategory.TitlecaseLetter;
       break;
     case "Mn":
       eCategory = UnicodeCategory.NonSpacingMark;
       break;
     case "Mc":
       eCategory = UnicodeCategory.SpacingCombiningMark;
       break;
     case "Me":
       eCategory = UnicodeCategory.EnclosingMark;
       break;
     case "Nd":
       eCategory = UnicodeCategory.DecimalDigitNumber;
       break;
     case "Nl":
       eCategory = UnicodeCategory.LetterNumber;
       break;
     case "No":
       eCategory = UnicodeCategory.OtherNumber;
       break;
     case "Zs":
       eCategory = UnicodeCategory.SpaceSeparator;
       break;
     case "Zl":
       eCategory = UnicodeCategory.LineSeparator;
       break;
     case "Zp":
       eCategory = UnicodeCategory.ParagraphSeparator;
       break;
     case "Cc":
       eCategory = UnicodeCategory.Control;
       break;
     case "Cf":
       eCategory = UnicodeCategory.Format;
       break;
     case "Cs":
       eCategory = UnicodeCategory.Surrogate;
       break;
     case "Co":
       eCategory = UnicodeCategory.PrivateUse;
       break;
     case "Cn":
       eCategory = UnicodeCategory.OtherNotAssigned;
       break;
     case "Lm":
       eCategory = UnicodeCategory.ModifierLetter;
       break;
     case "Lo":
       eCategory = UnicodeCategory.OtherLetter;
       break;
     case "Pc":
       eCategory = UnicodeCategory.ConnectorPunctuation;
       break;
     case "Pd":
       eCategory = UnicodeCategory.DashPunctuation;
       break;
     case "Ps":
       eCategory = UnicodeCategory.OpenPunctuation;
       break;
     case "Pe":
       eCategory = UnicodeCategory.ClosePunctuation;
       break;
     case "Pi":
       eCategory = UnicodeCategory.InitialQuotePunctuation;
       break;
     case "Pf":
       eCategory = UnicodeCategory.FinalQuotePunctuation;
       break;
     case "Po":
       eCategory = UnicodeCategory.OtherPunctuation;
       break;
     case "Sm":
       eCategory = UnicodeCategory.MathSymbol;
       break;
     case "Sc":
       eCategory = UnicodeCategory.CurrencySymbol;
       break;
     case "Sk":
       eCategory = UnicodeCategory.ModifierSymbol;
       break;
     case "So":
       eCategory = UnicodeCategory.OtherSymbol;
       break;
     default:
       throw new NotSupportedException
	 (UNSUPPORTED_CATEGORY + strCatAbbrev);
     }
   return eCategory;
   }
 protected double ConvertToDouble(string strDouble) 
   {
   int iNumerator = 0,                 
     iDenominator = 0;               
   string[] strFracParts = null;       
   if ( (strDouble == null) || (strDouble.Length == 0) )
     {
     throw new ArgumentException("String null/empty","strDouble");
     }
   strDouble = strDouble.Trim();
   if (strDouble.IndexOf(FRAC_SEP) == -1)
     return Convert.ToDouble(strDouble);
   strFracParts = strDouble.Split(FRAC_SEP);
   if (strFracParts.Length != 2)
     throw new ArgumentException
       ("Missing numerator/denominator",strDouble);
   iNumerator = Convert.ToInt32(strFracParts[0]);
   iDenominator = Convert.ToInt32(strFracParts[1]);
   return ((double) iNumerator / iDenominator);
   }
 public bool ReadUnicodeData()
   {
   FileStream    dataFile = null;     
   StreamReader  inputStream = null;  
   string        strCurLine;          
   string[]      strLineFields;       
   CharInfo      curCharInfo;         
   try
     {
     dataFile = 
       new FileStream(DATA_FILENAME,FileMode.Open,FileAccess.Read);
     inputStream = new StreamReader(dataFile,Encoding.ASCII);
     }
   catch(Exception exc)
     {
     Console.WriteLine("TestCase blocked", 
		       UNABLE_TO_OPEN_FILE + " '" + DATA_FILENAME + "'.",exc);
     if (inputStream != null)
       inputStream.Close();
     if (dataFile != null)
       dataFile.Close();
     return false;
     }
   try 
     {
     m_uCharsRead = 0;   
     while ( (strCurLine = inputStream.ReadLine()) != null )
       {
       strLineFields = strCurLine.Split(FIELD_SEP);
       curCharInfo = new CharInfo();
       if (strLineFields[0].Length > 4)
	 continue;
       else
	 curCharInfo.chChar =
	   Convert.ToChar(Convert.ToInt32(strLineFields[0],16));
       curCharInfo.eCategory =
	 TranslateUnicodeCategory(strLineFields[2]);
       if (strLineFields[8].Length > 0)
	 {
	 curCharInfo.dNumericValue = 
	   ConvertToDouble(strLineFields[8]);
	 }
       m_CharData.Add(curCharInfo.chChar,curCharInfo);
       m_uCharsRead++;
       }
     }
   catch(Exception exc)
     {
     Console.WriteLine("Testcase blocked", 
		       BAD_DATA_FILE + " '" + DATA_FILENAME + 
		       "', Line " + (m_uCharsRead + 1) + ".",
		       exc);
     if (inputStream != null)
       inputStream.Close();
     if (dataFile != null)
       dataFile.Close();
     return false;
     }
   inputStream.Close();
   dataFile.Close();
   return true;
   }
 public static void Main(String[] strArgList)
   {
   bool bResult = false; 
   bool bDataOK;            
   try
     {
     IsSeparator_char ObjIsSeparator_char = new IsSeparator_char();
     bDataOK = ObjIsSeparator_char.ReadUnicodeData();
     if(! bDataOK )
       {
       Console.WriteLine("Not able to read the UniCode data from the file");
       return ;
       }
     bResult = ObjIsSeparator_char.runTest();
     }
   catch ( Exception exc )
     {
     bResult = false;
     Console.Error.WriteLine( "IsSeparator_char main caught an Exception!" + exc.ToString());
     Console.Error.WriteLine( exc.ToString() );
     }
   if ( bResult == true ) Environment.ExitCode = 0; else Environment.ExitCode = 1;
   }
}
public class CharInfo 
{
 public char            chChar;        
 public UnicodeCategory eCategory;     
 public double          dNumericValue; 
 public CharInfo()
   {
   chChar = (char) 0;
   eCategory = (UnicodeCategory)(-1);
   dNumericValue = -1;     
   }
}
