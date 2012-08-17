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
 * Module   : cilBuiltinSystemObject
 * File     : Defines built in system object
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
METHOD_INITIALIZATION_PARAM CSystemObject::initParams[] = {
	{
		NAME_BUILTIN_SYSTEM_OBJECT_CTOR,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::ctor,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_SYSTEM_LOADPROPERTYBYSTRINGRID,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::LoadPropertyByStringRid,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_SYSTEM_STOREPROPERTYBYSTRINGRID,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::StorePropertyByStringRid,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_SYSTEM_LOADPROPERTYBYEXPRESSION,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::LoadPropertyByExpression,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_SYSTEM_STOREPROPERTYBYEXPRESSION,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::StorePropertyByExpression,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_SYSTEM_LOADobjectLiteral,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::LoadobjectLiteral,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_SYSTEM_LOADarrayLiteral,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::LoadarrayLiteral,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_SYSTEM_STRICTEQUAL,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::StrictEqual,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_SYSTEM_STRICTNOTEQUAL,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::StrictNotEqual,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_SYSTEM_LOADPROTOTYPE,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::LoadPrototype,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_SYSTEM_STOREPROTOTYPE,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::StorePrototype,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_SYSTEM_INSTANCEOF,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::instanceOf,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_SYSTEM_IN,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::In,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_SYSTEM_TYPEOF,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::Typeof,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_SYSTEM_FORIN,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::ForIn,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_SYSTEM_PRINT,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::print,
		RID_BUILTINPARAM_DEFAULT_TWO
	},
	{
		NAME_BUILTIN_SYSTEM_EVAL,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::eval,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_ESCAPE,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::escape,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_UNESCAPE,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::unescape,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_ISNAN,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::isNaN,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_ISFINITE,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::isFinite,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_INT,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::intCast,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_UINT,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::intCast,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_FLOAT,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::floatCast,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_INT64,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::int64Cast,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_DOUBLE,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::doubleCast,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_LONG,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::int64Cast,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_ULONG,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::int64Cast,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_SHORT,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::intCast,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_USHORT,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::intCast,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_CHAR,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::intCast,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_SBYTE,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::intCast,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_BYTE,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::intCast,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_SYSTEM_PARSEINT,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::parseInt,
		RID_BUILTINPARAM_DEFAULT_TWO
	},
	{
		NAME_BUILTIN_SYSTEM_PARSEFLOAT,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::parseFloat,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
};

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Initialize
 ***************************************************************************/
bool CSystemObject::InitializeCodeGen( ICodeGen& CodeGen )
{
	bool bReturn = true;
	CMetaData& Metadata = CodeGen.getMetaData();

	//---------------------------------------------------------------------------
	// Initialize Global object name
	//---------------------------------------------------------------------------
	RID ridObject = CodeGen.registerBuiltinObject( NAME_BUILTIN_SYSTEM_OBJECT );
	if( ridObject == RID_NOTDEFINED )
		return bReturn;

	//---------------------------------------------------------------------------
	// Initialize System functions
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

		//System methods are not a part of object property, so need to have a object to represent it
		//Register built in method object
		RID rid = Metadata.generateStaticFieldRID( OPERAND_OBJECTREF );
		CodeGen.generateSymbolInformation( rid, wstring( initParams[ i ].pStrSymbol ) + STRING_OBJECTNAME_POSTFIX );
	}

	return bReturn;
}

/***************************************************************************
 *      Create a prototype object
 ***************************************************************************/
bool CSystemObject::InitializeVm( CCilVm& vm )
{
	int32_t iIndex = IndexFromBuiltinRid( vm.getSystemObjectRID() );
	RID ridObjectName = vm.getMetaData().getBuiltinObjectTable()[ iIndex ].getObjectName();
	assert( ridObjectName != RID_NOTDEFINED );


	RID rid = vm.createPrototypeObject( vm.getSystemObjectRID(),
										ridObjectName,
										vm.getObjectRID() );

	assert( rid == vm.getSystemObjectRID() );

	VMOBJECTREF objref = vm.createObject( vm.getObjectRID() );
	CVariable var;
	var.setObjectRef( objref );

	//Set true as an object value..
	CVariable varBool( true );
	objref->setValue( varBool );

	//Create ExitCode property
	CVariable varExitCode( 0 );
	objref->setProperty( NAME_BUILTIN_SYSTEM_PROPERTY_EXITCODE, varExitCode );

	//Create Environment property
	//Add properties of prototype object here
	vm.getPrototypeObject( vm.getSystemObjectRID() ).setProperty( NAME_BUILTIN_SYSTEM_PROPERTY_ENVIRONMENT,
																	var );

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
									iParams );
		if( !b )
			return false;
	}

	return true;
}

/***************************************************************************
 *      ctor method
 ***************************************************************************/
void CSystemObject::ctor( CCilVm* const pVm,
							const int32_t iNumArguments, 
							CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );

	CVariable obj = *pArguments;
	assert( obj.iOperandType == OPERAND_OBJECTREF );

	//Set prototype: System prototype object
	obj.refObject->setPrototypeObject( 
		&pVm->getPrototypeObject( pVm->getSystemObjectRID() ) );

	//Initialize this object
	assert( OperandType( pArguments->iOperandType ) == OPERAND_OBJECTREF );
	CVmObject* pObj = pArguments->refObject;
	pObj->addProperty( STRING_INTERNAL_LENGTH, CVariable( 1 ) );
		
	return;
}

} //namespace CRI