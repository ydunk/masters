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
.class public _brfalse {
.method public static	int32 main(class [mscorlib]System.String[]) {
.entrypoint
.maxstack		20
.locals	(int32,int32)
	ldc.i4		0x0
	dup
	stloc	0
	stloc	1
BEGIN:
	ldc.i4		0x1
	ldloc	0
	add
	dup
	stloc	0
	ldc.i4		0x1
	ceq
	ldloc	1
	add
	stloc	1
	ldc.i4		0x0
	brfalse		TAG3
TAG1:
	ldc.i4		0x2
	ldloc	0
	add
	dup
	stloc	0
	ldc.i4		0xB
	ceq
	ldloc	1
	add
	stloc	1
	ldc.i4		0x1
	brfalse		END
	ldc.i4		0x0
	brfalse		0
TAG2:
	ldc.i4		0x4
	ldloc	0
	add
	dup
	stloc	0
	ldc.i4		0xF
	ceq
	ldloc	1
	add
	stloc	1
	ldc.i4		0x0
	brfalse		BOUNDS_TEST
TAG3:	
	ldc.i4		0x8
	ldloc	0
	add
	dup
	stloc	0
	ldc.i4		0x9
	ceq
	ldloc	1
	add
	stloc	1
	ldc.i4		0x1
	brfalse		END
	ldc.i4		0x0
	brfalse		TAG1
BOUNDS_TEST:
	ldc.i4		0xFFFFFFFF
	brfalse		END
	ldc.i4		0x1
	ldloc	1
	add
	stloc	1
	ldc.i4		0x7FFFFFFF
	brfalse		END
	ldc.i4		0x1
	ldloc	1
	add
	stloc	1
	ldc.i4		0x80000000
	brfalse		END
	ldc.i4		0x1
	ldloc	1
	add
	stloc	1
	ldc.i4		0x55555555
	brfalse		END
	ldc.i4		0x1
	ldloc	1
	add
	stloc	1
	ldc.i4		0xAAAAAAAA
	brfalse		END
	ldc.i4		0x1
	ldloc	1
	add
	stloc	1
	ldc.i4		0x00000000
	brfalse		END
	ldc.i4		0x1
	ldloc	1
	add
	stloc	1
END:
	ldloc	1
	ldc.i4		0x00000009
	ceq
	brfalse		FAIL
PASS:
	ldc.i4	0x0000
	ret
FAIL:
	ldc.i4	0x0001
	ret
}
}
.assembly brfalse.exe{}
