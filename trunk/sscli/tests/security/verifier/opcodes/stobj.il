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
.assembly extern mscorlib { }
.assembly 'stobj' { }
.class public auto ansi beforefieldinit Test extends [mscorlib]System.Object {
.method public hidebysig instance void Negative1() cil managed
{
	.maxstack  8
	.locals init (int64)
	.zeroinit
	ldloca.s 0
	ldc.i4.0
	stobj int32
	ret
}
.method public hidebysig instance void Negative2() cil managed
{
	.maxstack  8
	.locals init (int64)
	.zeroinit
	ldloc.0
	ldc.i4.0
	stobj int32
	ret
}
.method public hidebysig instance void Negative3() cil managed
{
	.maxstack  8
	ldc.i4 8
	newarr int32
	ldc.i4.0
	ldelema int32
	ldc.i4.0
	stobj class [mscorlib]System.SByte
	ret
}
.method public hidebysig instance void Negative4() cil managed
{
	.maxstack  8
	ldftn instance void Test::Negative4()
	ldc.i4.0
	stobj class [mscorlib]System.IntPtr
	ret
}
.method public hidebysig instance void Negative5() cil managed
{
	.maxstack  8
	ldnull
	ldc.i8 0
	stobj float64
	ret
}
.method public hidebysig instance void Negative6() cil managed
{
	.maxstack  8
	.locals init (int32)
	.zeroinit
	ldloca.s 0
	ldc.i4.1
	.emitbyte 0x81
	.emitbyte 0xfe
	.emitbyte 0xfe
	.emitbyte 0xfe
	.emitbyte 0xfe
	ret
}
.method public hidebysig instance void Negative7() cil managed
{
	.maxstack  8
	ldc.i4 8
	newarr int32
	ldc.i4.1
	stobj int32
	ret
}
.method public hidebysig instance void Negative8() cil managed
{
	.maxstack  8
	ldc.i4 8
	newarr int32
	dup
	stobj int32[]
	ret
}
.method public hidebysig static int32 Main() cil managed
{
	.entrypoint
	.maxstack 8
	.locals init (class Test V_0)
	newobj     instance void Test::.ctor()
	stloc.0
	.try {
		ldloc.0
		callvirt instance void Test::Negative1()
		ldstr "Negative1() passed verification."
		call void [mscorlib]System.Console::WriteLine(string)
		leave IL_FAIL
	} catch [mscorlib]System.Security.VerificationException {
		leave.s IL_NEXT_1
	}
IL_NEXT_1:
	.try {
		ldloc.0
		callvirt instance void Test::Negative2()
		ldstr "Negative2() passed verification."
		call void [mscorlib]System.Console::WriteLine(string)
		leave IL_FAIL
	} catch [mscorlib]System.Security.VerificationException {
		leave.s IL_NEXT_2
	}
IL_NEXT_2:
	.try {
		ldloc.0
		callvirt instance void Test::Negative3()
		ldstr "Negative3() passed verification."
		call void [mscorlib]System.Console::WriteLine(string)
		leave IL_FAIL
	} catch [mscorlib]System.Security.VerificationException {
		leave.s IL_NEXT_3
	}
IL_NEXT_3:
	.try {
		ldloc.0
		callvirt instance void Test::Negative4()
		ldstr "Negative4() passed verification."
		call void [mscorlib]System.Console::WriteLine(string)
		leave IL_FAIL
	} catch [mscorlib]System.Security.VerificationException {
		leave.s IL_NEXT_4
	}
IL_NEXT_4:
	.try {
		ldloc.0
		callvirt instance void Test::Negative5()
		ldstr "Negative5() passed verification."
		call void [mscorlib]System.Console::WriteLine(string)
		leave IL_FAIL
	} catch [mscorlib]System.Security.VerificationException {
		leave.s IL_NEXT_5
	}
IL_NEXT_5:
	.try {
		ldloc.0
		callvirt instance void Test::Negative6()
		ldstr "Negative6() passed verification."
		call void [mscorlib]System.Console::WriteLine(string)
		leave IL_FAIL
	} catch [mscorlib]System.Security.VerificationException {
		leave.s IL_NEXT_6
	}
IL_NEXT_6:
	.try {
		ldloc.0
		callvirt instance void Test::Negative7()
		ldstr "Negative7() passed verification."
		call void [mscorlib]System.Console::WriteLine(string)
		leave IL_FAIL
	} catch [mscorlib]System.Security.VerificationException {
		leave.s IL_NEXT_7
	}
IL_NEXT_7:
	.try {
		ldloc.0
		callvirt instance void Test::Negative8()
		ldstr "Negative8() passed verification."
		call void [mscorlib]System.Console::WriteLine(string)
		leave IL_FAIL
	} catch [mscorlib]System.Security.VerificationException {
		leave.s IL_NEXT_8
	}
IL_NEXT_8:
	ldc.i4 0
	ldstr "PASSED"
	br.s IL_OUT
IL_FAIL:
	ldc.i4 1
	ldstr "FAILED!"
IL_OUT:
	call void [mscorlib]System.Console::WriteLine(string)
	ret
}
.method public hidebysig specialname rtspecialname instance void .ctor() cil managed
{
	.maxstack  8
	ldarg.0
	call instance void [mscorlib]System.Object::.ctor()
	ret
}
}
