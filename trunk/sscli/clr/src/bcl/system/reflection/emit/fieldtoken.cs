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
/*============================================================
**
** Class:  FieldToken
**
**                                        
**
** Purpose: Represents a Field to the ILGenerator Class
**
** Date:  December 15, 1998
** 
===========================================================*/
namespace System.Reflection.Emit {
    
	using System;
	using System.Reflection;
    // The FieldToken class is an opaque representation of the Token returned
    // by the MetaData to represent the field.  FieldTokens are generated by 
    // Module.GetFieldToken().  There are no meaningful accessors on this class,
    // but it can be passed to ILGenerator which understands it's internals.
    /// <include file='doc\FieldToken.uex' path='docs/doc[@for="FieldToken"]/*' />
	[Serializable()] 
    public struct FieldToken {
    
		/// <include file='doc\FieldToken.uex' path='docs/doc[@for="FieldToken.Empty"]/*' />
		public static readonly FieldToken Empty = new FieldToken();

        internal int m_fieldTok;
        internal Object m_class;
    
        // Creates an empty FieldToken.  A publicly visible constructor so that
        // it can be created on the stack.
        //public FieldToken() {
        //    m_fieldTok=0;
        //    m_attributes=0;
        //    m_class=null;
        //}
        // The actual constructor.  Sets the field, attributes and class
        // variables
    
        internal FieldToken (int field, Type fieldClass) {
            m_fieldTok=field;
            m_class = fieldClass;
        }
    	
        /// <include file='doc\FieldToken.uex' path='docs/doc[@for="FieldToken.Token"]/*' />
        public int Token {
            get { return m_fieldTok; }
        }
        
    
        // Generates the hash code for this field. 
        /// <include file='doc\FieldToken.uex' path='docs/doc[@for="FieldToken.GetHashCode"]/*' />
        public override int GetHashCode()
        {
            return (m_fieldTok);
        }
    
        // Returns true if obj is an instance of FieldToken and is 
        // equal to this instance.
        /// <include file='doc\FieldToken.uex' path='docs/doc[@for="FieldToken.Equals"]/*' />
        public override bool Equals(Object obj)
        {
            if (obj!=null && (obj is FieldToken)) {
                FieldToken that = (FieldToken) obj;
                return (that.m_fieldTok == m_fieldTok && that.m_class == m_class);
            }
            else
                return false;
        }
    }
}