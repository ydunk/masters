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
.assembly test_14_catch {}
.class TestClass{
.method public static void test_14_catch(){
.locals init(int32 locInt1)
  ldc.i4.1
  stloc locInt1
  ldstr      "Test is running"
  call       void [mscorlib]System.Console::WriteLine(class System.String)  
lbl_14_startTry:
  leave lbl_14_exitSEH
lbl_14_endTry:
lbl_14_startCatch:
    newobj     instance void [mscorlib]System.Exception::.ctor()
    throw
lbl_14_endCatch:
.try  lbl_14_startTry to lbl_14_endTry catch ['mscorlib']System.Exception handler lbl_14_startCatch to lbl_14_endCatch
lbl_14_exitSEH:
  ret
}
.method public static void Main(class ['mscorlib']System.String[] arg)
{
    call void TestClass::test_14_catch()
    .entrypoint
    ret
}
  .method public specialname rtspecialname instance void .ctor() il managed
  {
    .maxstack  8
    IL_0000:  ldarg.0
    IL_0001:  call       instance void ['mscorlib']System.Object::.ctor()
    IL_0006:  ret
  } 
}
