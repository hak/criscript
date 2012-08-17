/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * Copyright (c) 2008 CRI Middleware, Inc. All rights reserved.
 *
 * Use, modification and distribution are subject to the CRI Script Software
 * License, Version 1.0(see accompanying file "CriScript_License_1_0.txt" or
 * copy at www.criscript.com/trac/wiki/CRI%20Script%20Software%20License%201.0 ).
 *
 *
 * Library  : CRIScript
 * Module   : cil Virtual Machine
 * File     : CCilVmObjectCreation.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
#include "cilVm.h"
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      New Object instruction
 *      (1) take RID
 ***************************************************************************/
void CCilVm::CreateNewObject()
{
	RID tokenObject = getNextToken();
	uint32_t iObjectType = TypeFromToken( tokenObject );
	assert( iObjectType == MDT_TYPEDEF
		|| iObjectType == MDT_METHODDEF
		|| iObjectType == MDT_FIELDDEF );

	CVmObject obj;
	uint32_t iIndex;

	if( iObjectType == MDT_FIELDDEF )
	{
		//Retrieve static field value
		iIndex = RidFromToken( tokenObject );
		CVariable& var = m_pStaticFieldTable[ iIndex ];

		switch( OperandType( var.iOperandType ) )
		{
		case OPERAND_OBJECTREF:
			tokenObject =  var.refObject->getRID();
			break;
		default:
			tokenObject = getObjectRID();
			break;
		}
		iObjectType = TypeFromToken( tokenObject );		
	}
	
	switch( iObjectType )
	{
	case MDT_TYPEDEF:
		{
		//Create an object
		CObjectTypeInfo typeinfo;
		if( isBuiltinRid( tokenObject ) )
		{
			iIndex = IndexFromBuiltinRid( tokenObject );
			typeinfo = m_MetaData.getBuiltinObjectTable()[ iIndex ];
		}
		else
		{
			iIndex = RidFromToken( tokenObject );
			typeinfo = m_MetaData.getObjectTable()[ iIndex ];
		}

		//Note that push_back addref (1) to the object
		m_ObjectPool.push_back( obj );

		CVmObject& newobject = m_ObjectPool.back();
		newobject.setRID( typeinfo.getRID() );
		newobject.setConstructorProperty( typeinfo.getConstructor() );
		newobject.setCallProperty( typeinfo.getCall() );
		newobject.setObjectName( typeinfo.getObjectName() );
		newobject.setPrototypeObject( 
			&m_BuiltinPrototypeObjectTable[ IndexFromBuiltinRid( typeinfo.getRID() ) ] );
//		newobject.setPrototypeObject( 
//			&m_BuiltinPrototypeObjectTable[ IndexFromBuiltinRid( getObjectRID() ) ] );
		newobject.setRefCount( VM_REFCOUNT_MANAGED | 1 );
		pushEvalStackObjectRef( &newobject );

		//Invoke ctor
		if( typeinfo.getConstructor() )
			CallOperator( &getEvalStackFirstEntry(), typeinfo.getConstructor(), true );
		}
		break;
	case MDT_METHODDEF:
		{
		METHOD_DEF* pMethod;
		//Create a function object
		if( isBuiltinRid( tokenObject ) )
		{
			iIndex = IndexFromBuiltinRid( tokenObject );
			pMethod = &m_MetaData.getBuiltinMethodTable()[ iIndex ];
		}
		else
		{
			iIndex = RidFromToken( tokenObject );
			pMethod = &m_MetaData.getMethodTable()[ iIndex ];
		}

		//Note that push_back addref (1) to the object
		m_ObjectPool.push_back( obj );

		CVmObject& newobject = m_ObjectPool.back();
		newobject.setRID( pMethod->ridMethod );
		newobject.setConstructorProperty( pMethod->ridMethod );
		newobject.setCallProperty( pMethod->ridMethod );
		newobject.setPrototypeObject( 
			&m_BuiltinPrototypeObjectTable[ IndexFromBuiltinRid( getFunctionObjectRID() ) ],
										OPERAND_OBJECTREF,
										OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
													| OPERAND_FLAG_DONTENUM
													| OPERAND_FLAG_READONLY ) );
		newobject.setRefCount( VM_REFCOUNT_MANAGED | 1 );

		pushEvalStackObjectRef( &newobject );

		//Invoke ctor
		//Could be CIL ctor..
		if( pMethod->ridMethod )
			CallOperator( &getEvalStackFirstEntry(), pMethod->ridMethod, true );
		}
		break;
	default:
		Debug_Fatal( "Not implemented yet" );
		break;
	}


}

/***************************************************************************
 *      New String value (not an object) instruction
 *      (1) take RID
 ***************************************************************************/
void CCilVm::CreateNewStringValue()
{
	RID tokenObject = getNextToken();
	uint32_t iObjectType = TypeFromToken( tokenObject );
	assert( iObjectType == MDT_STRING );

	CVariable var;

	uint32_t iIndex; 
	//Initialize string object
	if( isBuiltinRid( tokenObject ) )
	{
		iIndex = IndexFromBuiltinRid( tokenObject );
		var.setString( m_MetaData.getBuiltinStringConstantPool()[ iIndex ] );
	}
	else
	{
		iIndex = RidFromToken( tokenObject );
		var.setString( m_MetaData.getStringConstantPool()[ iIndex ] );
	}
	pushEvalStackVariable( var );
}

/***************************************************************************
 *      Helper
 *		Create an object and push it to the pool
 ***************************************************************************/
VMOBJECTREF CCilVm::createObject( RID ridObject )
{
	//Create an object and store it to static field
	int32_t iIndex;
	CVmObject obj;
	CObjectTypeInfo typeinfo;
	if( isBuiltinRid( ridObject ) )
	{
		iIndex = IndexFromBuiltinRid( ridObject );
		typeinfo = m_MetaData.getBuiltinObjectTable()[ iIndex ];
	}
	else
	{
		iIndex = RidFromToken( ridObject );
		typeinfo = m_MetaData.getObjectTable()[ iIndex ];
	}

	//Note that push_back addref (1) to the object
	getObjectPool().push_back( obj );
	CVmObject& newobject = m_ObjectPool.back();
	newobject.setRID( typeinfo.getRID() );
	newobject.setConstructorProperty( typeinfo.getConstructor() );
	newobject.setCallProperty( typeinfo.getCall() );
	newobject.setObjectName( typeinfo.getObjectName() );
	newobject.setPrototypeObject( 
		&m_BuiltinPrototypeObjectTable[ IndexFromBuiltinRid( ridObject ) ] );

	return &newobject;
}

/***************************************************************************
 *      Helper
 *		Retrieve an object from the variable
 ***************************************************************************/
VMOBJECTREF CCilVm::getObjectPointer( CVariable& var )
{
	//Argument: 1) object reference
	//			2) string RID of the property
	switch( OperandType( var.iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		return var.refObject;
	case OPERAND_STRING:
		return &getPrototypeObject( m_ridStringObject );
	case OPERAND_BOOLEAN:
		return &getPrototypeObject( m_ridBooleanObject );
	case OPERAND_INT:
	case OPERAND_UNSIGNEDINT:
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
	case OPERAND_FLOAT:
	case OPERAND_DOUBLE:
	case OPERAND_NAN:
		return &getPrototypeObject( m_ridNumberObject );
	default:
		return &getPrototypeObject( m_ridObject );
	}
	

}


} //namespace CRI