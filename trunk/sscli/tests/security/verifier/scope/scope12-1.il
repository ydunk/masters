.assembly extern mscorlib {}
//access another class public function. Should not get VE

.module 'scope12-1.exe'

.assembly 'scope12-1' // as "scope12-1.exe"
 { 
.custom instance void [mscorlib]System.Security.AllowPartiallyTrustedCallersAttribute::.ctor() = ( 01 00 00 00 )
 
  .hash algorithm 0x00008004
  .ver 0:0:0:0
}
 

.assembly extern anothermethod
{
  .hash = (93 46 58 D4 EC BB 4A 70 4F 77 A3 B8 8B 8D E8 3B   // .FX...JpOw.....;
           1C 69 8E 34 )                                     // .i.4
  .ver 0:0:0:0
}

.class public auto ansi TestClass extends [anothermethod]A
{
  .method private instance void Bar() il managed
  {
    // Code size       7 (0x7)
    .maxstack  8
    IL_0000:  ldarg.0
    IL_0001:  call       instance void [anothermethod]A::PublicFoo()
    IL_0006:  ret
  } // end of method 'TestClass::Bar'

  .method public static void  Main(class ['mscorlib']System.String[] args) il managed
  {
    .entrypoint
    // Code size       13 (0xd)
    .maxstack  1
    .locals init (class TestClass V_0)
    IL_0000:  newobj     instance void TestClass::.ctor()
    IL_0005:  stloc.0
    IL_0006:  ldloc.0
    IL_0007:  call       instance void TestClass::Bar()
    IL_000c:  ret
  } // end of method 'TestClass::Main'

  .method public specialname rtspecialname instance void .ctor() il managed
  {
    // Code size       7 (0x7)
    .maxstack  8
    IL_0000:  ldarg.0
    IL_0001:  call       instance void [anothermethod]A::.ctor()
    IL_0006:  ret
  } // end of method 'TestClass::.ctor'

} // end of class 'TestClass'

//*********** DISASSEMBLY COMPLETE ***********************
