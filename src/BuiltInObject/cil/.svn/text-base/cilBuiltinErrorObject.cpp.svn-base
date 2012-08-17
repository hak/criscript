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
 * Module   : cilBuiltinErrorObject.cpp
 * File     : Defines built in Error object
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
METHOD_INITIALIZATION_PARAM CErrorObject::initParams[] = {
	{
		NAME_BUILTIN_ERROR_OBJECT_CTOR,
		METHOD_NATIVE,
		(PMETHOD)&CErrorObject::ctor,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_ERROR_OBJECT_CALL,
		METHOD_NATIVE,
		(PMETHOD)&CErrorObject::call,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
};

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Initialize
 ***************************************************************************/
bool CErrorObject::InitializeCodeGen( ICodeGen& CodeGen )
{
	bool bReturn = true;

	RID ridObject = CodeGen.registerBuiltinObject( NAME_BUILTIN_ERROR_OBJECT );
	if( ridObject == RID_NOTDEFINED )
		return bReturn;

	//Initialize Global 'Error' object ref
	RID rid = CodeGen.getMetaData().generateStaticFieldRID( OPERAND_OBJECTREF,
															OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
															| OPERAND_FLAG_DONTENUM
															| OPERAND_FLAG_READONLY ) );
	CodeGen.generateSymbolInformation( rid, NAME_BUILTIN_ERROR_OBJECT, CG_SYMBOL_FLAG_READONLY, ridObject );

	//---------------------------------------------------------------------------
	// Initialize Error.call/ctor property
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
bool CErrorObject::InitializeVm( CCilVm& vm )
{
	RID rid = vm.createPrototypeObject( vm.getErrorObjectRID(),
										vm.getMetaData().getObjectNameRID( vm.getErrorObjectRID() ),
										vm.getObjectRID() );

	assert( rid == vm.getErrorObjectRID() );

	//---------------------------------------------------------------------------
	//Initialize prototype property
	//---------------------------------------------------------------------------
	CVmObject& protoobj = vm.getPrototypeObject( vm.getErrorObjectRID() );
	CVariable varName( NAME_BUILTIN_ERROR_OBJECT );
	CVariable var( -1 );

	protoobj.setProperty( NAME_BUILTIN_ERROR_PROPERTY_NAME, varName );
	protoobj.setProperty( NAME_BUILTIN_ERROR_PROPERTY_NUMBER, var );

	//---------------------------------------------------------------------------
	//Bind global object
	//---------------------------------------------------------------------------
	CVmObject* pObj = vm.createObject( vm.getErrorObjectRID() );
	CVariable obj( pObj, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
									| OPERAND_FLAG_DONTENUM
									| OPERAND_FLAG_READONLY ) );

	//Set true as an object value..
	CVariable varTrue( true );
	pObj->setValue( varTrue );

	CG_SYMBOL_INFORMATION info = vm.querySymbolInformation( NAME_BUILTIN_ERROR_OBJECT );
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
void CErrorObject::ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	CVariable obj = *pArguments;
	assert( obj.iOperandType == OPERAND_OBJECTREF );

	obj.refObject->setRID( pVm->getErrorObjectRID() );

	//Set prototype: String prototype object
	obj.refObject->setPrototypeObject( 
		&pVm->getPrototypeObject( pVm->getErrorObjectRID() ) );

	if( iNumArguments > 2 )
	{
		pArguments ++;
		CVariable& var = *pArguments;
		var.setFlag( OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ) );
		obj.refObject->setProperty( NAME_BUILTIN_ERROR_PROPERTY_NUMBER, var ) ;
		
		pArguments ++;
		CVariable& varMessage = *pArguments;
		varMessage.convertToString( pVm );
		varMessage.setFlag( OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ) );
		obj.refObject->setProperty( NAME_BUILTIN_ERROR_PROPERTY_MESSAGE, varMessage );
	}
	else if( iNumArguments > 1 )
	{
		pArguments ++;
		CVariable& var = *pArguments;
		var.convertToString( pVm );
		var.setFlag( OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ) );
		obj.refObject->setProperty( NAME_BUILTIN_ERROR_PROPERTY_MESSAGE, var );

		CVariable varError( ERROR_PROPERTY_NUMBER_DEFAULT,
						   OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ) );
		obj.refObject->setProperty( NAME_BUILTIN_ERROR_PROPERTY_NUMBER, varError );
	}
	else
	{
		CVariable varString( L"",
							OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ) );
		CVariable varError( ERROR_PROPERTY_NUMBER_DEFAULT,
						   OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ) );
		obj.refObject->setProperty( NAME_BUILTIN_ERROR_PROPERTY_MESSAGE, varString );
		obj.refObject->setProperty( NAME_BUILTIN_ERROR_PROPERTY_NUMBER, varError );
	}

	CVariable var( L"",
				  OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ) );
	obj.refObject->setProperty( NAME_BUILTIN_ERROR_PROPERTY_NAME, var );

}

/***************************************************************************
 *      call method
 ***************************************************************************/
void CErrorObject::call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	CVariable obj;

	CVmObject* pObj = pVm->createObject( pVm->getErrorObjectRID() );
	obj.setObjectRef( pObj );

	//Set prototype: String prototype object
	obj.refObject->setPrototypeObject( 
		&pVm->getPrototypeObject( pVm->getErrorObjectRID() ) );

	if( iNumArguments > 2 )
	{
		pArguments ++;
		CVariable& var = *pArguments;
		var.setFlag( OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ) );
		obj.refObject->setProperty( NAME_BUILTIN_ERROR_PROPERTY_NUMBER, var ) ;
		
		pArguments ++;
		var = *pArguments;
		var.convertToString( pVm );
		var.setFlag( OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ) );
		obj.refObject->setProperty( NAME_BUILTIN_ERROR_PROPERTY_MESSAGE, var );
	} else if( iNumArguments > 1 )
	{
		pArguments ++;
		CVariable& var = *pArguments;
		var.convertToString( pVm );
		var.setFlag( OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ) );
		obj.refObject->setProperty( NAME_BUILTIN_ERROR_PROPERTY_MESSAGE, var );

		CVariable varError( ERROR_PROPERTY_NUMBER_DEFAULT,
					  OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ) );
		obj.refObject->setProperty( NAME_BUILTIN_ERROR_PROPERTY_NUMBER, varError );
	}
	else
	{
		CVariable varString( L"",
							OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ) );
		CVariable varError( ERROR_PROPERTY_NUMBER_DEFAULT,
						   OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ) );
		obj.refObject->setProperty( NAME_BUILTIN_ERROR_PROPERTY_MESSAGE, varString );
		obj.refObject->setProperty( NAME_BUILTIN_ERROR_PROPERTY_NUMBER, varError );
	}

	CVariable var( L"",
				  OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ) );
	obj.refObject->setProperty( NAME_BUILTIN_ERROR_PROPERTY_NAME, var );

	//Push the ObjectRef to a eval stack
	pVm->pushEvalStackVariable( obj );
	return;
}


} //namespace CRI