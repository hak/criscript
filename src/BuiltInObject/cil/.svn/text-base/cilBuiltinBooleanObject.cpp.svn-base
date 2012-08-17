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
 * Module   : cilBuiltinBooleanObject.cpp
 * File     : Defines built in Boolean object
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
#include "cilCodeGen.h"
namespace cri {

/***************************************************************************
 *      Variables
 ***************************************************************************/
METHOD_INITIALIZATION_PARAM CBooleanObject::initParams[] = {
	{
		NAME_BUILTIN_BOOLEAN_OBJECT_CTOR,
		METHOD_NATIVE,
		(PMETHOD)&CBooleanObject::ctor,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_BOOLEAN_OBJECT_CALL,
		METHOD_NATIVE,
		(PMETHOD)&CBooleanObject::call,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_BOOLEAN_OBJECT_TOSTRING,
		METHOD_NATIVE,
		(PMETHOD)&CBooleanObject::toString,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_BOOLEAN_OBJECT_TOLOCALESTRING,
		METHOD_NATIVE,
		(PMETHOD)&CBooleanObject::toString,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_BOOLEAN_OBJECT_VALUEOF,
		METHOD_NATIVE,
		(PMETHOD)&CBooleanObject::valueOf,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
};

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Initialize
 ***************************************************************************/
bool CBooleanObject::InitializeCodeGen( ICodeGen& CodeGen )
{
	bool bReturn = true;
	
	RID ridObject = CodeGen.registerBuiltinObject( NAME_BUILTIN_BOOLEAN_OBJECT );
	if( ridObject == RID_NOTDEFINED )
		return bReturn;

	//Initialize Global 'Boolean' object ref
	RID rid = CodeGen.getMetaData().generateStaticFieldRID( OPERAND_OBJECTREF,
															OPERAND_FLAG(
																	OPERAND_FLAG_DONTDELETE
																	| OPERAND_FLAG_DONTENUM
																	| OPERAND_FLAG_READONLY ) );
	CodeGen.generateSymbolInformation( rid, NAME_BUILTIN_BOOLEAN_OBJECT, CG_SYMBOL_FLAG_READONLY, ridObject );

	//---------------------------------------------------------------------------
	// Initialize Boolean.call/ctor property
	//---------------------------------------------------------------------------
	for( int32_t i = 0; i<sizeof( initParams ) / sizeof( initParams[ 0 ] ); ++i )
	{
		//-----------------------------------------------------------------------
		//Register methods
		//-----------------------------------------------------------------------
		RID ridMethod = CodeGen.registerBuiltinMethod(
									wstring( initParams[ i ].pStrSymbol ),
									initParams[ i ].methodType,
									initParams[ i ].ridParam
									);
		if( ridMethod == RID_NOTDEFINED )
			return bReturn;
	}

	return bReturn;
}

/***************************************************************************
 *      Create a prototype object
 ***************************************************************************/
bool CBooleanObject::InitializeVm( CCilVm& vm )
{
	RID rid = vm.createPrototypeObject( vm.getBooleanObjectRID(),
										vm.getMetaData().getObjectNameRID( vm.getBooleanObjectRID() ),
										vm.getObjectRID() );

	assert( rid == vm.getBooleanObjectRID() );

	//---------------------------------------------------------------------------
	//Bind global object
	//---------------------------------------------------------------------------
	CVmObject* pObj = vm.createObject( vm.getBooleanObjectRID() );
	CVariable obj( pObj, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
									| OPERAND_FLAG_DONTENUM
									| OPERAND_FLAG_READONLY ) );

	//Set true as an object value..
	CVariable var( true );
	pObj->setValue( var );

	CG_SYMBOL_INFORMATION info = vm.querySymbolInformation( NAME_BUILTIN_BOOLEAN_OBJECT );
	vm.setStaticField( info.rid, obj, true );	//Force over write it

	//Add properties of prototype object here
	//---------------------------------------------------------------------------
	// Bind System functions
	//---------------------------------------------------------------------------
	for( int32_t i = 0; i<sizeof( initParams ) / sizeof( initParams[ 0 ] ); ++i )
	{
		int32_t iParams;
		iParams = (int32_t)RidFromToken( initParams[ i ].ridParam );
		if( isBuiltinRid( initParams[ i ].ridParam ) )
		{
			iParams = RidFromToken( -iParams ) - 1;
		}

		//-----------------------------------------------------------------------
		//Register methods
		//-----------------------------------------------------------------------
		bool b = vm.bindBuiltinMethod(
									wstring( initParams[ i ].pStrSymbol ),
									initParams[ i ].pMethod,
									iParams
									);
		if( !b )
			return false;
	}

	return true;
}

/***************************************************************************
 *      ctor
 ***************************************************************************/
void CBooleanObject::ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );
	CVariable& thisobj = *pArguments;
	assert( OperandType(thisobj.iOperandType) == OPERAND_OBJECTREF );

	thisobj.refObject->setRID( pVm->getBooleanObjectRID() );

	//Set prototype: String prototype object
	thisobj.refObject->setPrototypeObject( 
		&pVm->getPrototypeObject( pVm->getBooleanObjectRID() ) );

	if( iNumArguments > 1 )
	{
		pArguments ++;
		switch( OperandType( pArguments->iOperandType ) )
		{
		case OPERAND_INT:
		case OPERAND_UNSIGNEDINT:
		case OPERAND_INT64:
		case OPERAND_UNSIGNEDINT64:
		case OPERAND_FLOAT:
		case OPERAND_DOUBLE:
		case OPERAND_BOOLEAN:
			thisobj.refObject->setValue( *pArguments );
			break;
		case OPERAND_STRING:
		case OPERAND_UNDEFINED:
		case OPERAND_NULL:
		case OPERAND_OBJECTREF:
			{
			CVariable var( pArguments->toBoolean() );
			thisobj.refObject->setValue( var );
			}
			break;
		default:
			Debug_Fatal( "Not implemented yet" );
			break;
		}
	}
	return;
}

/***************************************************************************
 *      call method
 ***************************************************************************/
void CBooleanObject::call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );
	CVariable varRet( 0 );
	pVm->pushEvalStackVariable( varRet );

	if( iNumArguments > 1 )
	{
		pArguments ++;
		CVariable& var = *pArguments;
		switch( OperandType( var.iOperandType ) )
		{
		case OPERAND_INT:
		case OPERAND_UNSIGNEDINT:
		case OPERAND_INT64:
		case OPERAND_UNSIGNEDINT64:
		case OPERAND_FLOAT:
		case OPERAND_DOUBLE:
		case OPERAND_BOOLEAN:
			var.moveTo( pVm->getEvalStackFirstEntry() );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NULL:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
		default:
			Debug_Fatal( "Not implemented yet" );
			break;
		}
	}
	return;
}

/***************************************************************************
 *      toString property
 ***************************************************************************/
void CBooleanObject::toString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 1 );
	bool b;

	switch( OperandType( pArguments->iOperandType ) )
	{
	case OPERAND_INT:
	case OPERAND_UNSIGNEDINT:
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
	case OPERAND_FLOAT:
	case OPERAND_DOUBLE:
	case OPERAND_BOOLEAN:
	case OPERAND_NAN:
		b = pArguments->toBoolean();
		break;
	case OPERAND_OBJECTREF:
		{
		CVmObject* pObj = pArguments->refObject;
		b = pObj->getValue().toBoolean();
		}
		break;
	default:
		Debug_Fatal( "Illegal operand" );
	}
	if( b )
		pVm->pushEvalStackString( STRING_INTERNAL_TRUE );
	else
		pVm->pushEvalStackString( STRING_INTERNAL_FALSE );
	return;
}

/***************************************************************************
 *      valueOf property
 ***************************************************************************/
void CBooleanObject::valueOf( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	Debug_Fatal( "Not implemented yet" );
}

} //namespace CRI