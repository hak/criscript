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
 * Module   : cilBuiltinObject.cpp
 * File     : Initialize built in obj
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

/***************************************************************************
 *      Methods
 ***************************************************************************/
bool CCilCodeGen::initializeBuiltInObjects()
{
	bool bReturn = true;

	//-----------------------------------------------------------------
	//Initialize system constants
	//-----------------------------------------------------------------

	//RID_CONSTANT_UNDEFINED
	RID rid = m_MetaData.generateStaticFieldRID( OPERAND_UNDEFINED,
											OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
											| OPERAND_FLAG_DONTENUM
											| OPERAND_FLAG_READONLY ) );
	generateSymbolInformation( rid, wstring( STRING_INTERNAL_UNDEFINED ), CG_SYMBOL_FLAG_READONLY );
	assert( rid == RID_CONSTANT_UNDEFINED );

	//RID_CONSTANT_NULL
	rid = m_MetaData.generateStaticFieldRID( OPERAND_NULL,
											OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
											| OPERAND_FLAG_DONTENUM
											| OPERAND_FLAG_READONLY ) );
	generateSymbolInformation( rid, wstring( STRING_INTERNAL_NULL ), CG_SYMBOL_FLAG_READONLY );
	assert( rid == RID_CONSTANT_NULL );

	//RID_CONSTANT_THIS
	rid = m_MetaData.generateStaticFieldRID( OPERAND_OBJECTREF,
											OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
											| OPERAND_FLAG_DONTENUM
											| OPERAND_FLAG_READONLY ) );
	generateSymbolInformation( rid, wstring( STRING_INTERNAL_THIS ), CG_SYMBOL_FLAG_READONLY );
	assert( rid == RID_CONSTANT_THIS );
	
	//RID_CONSTANT_BOOL_TRUE
	rid = m_MetaData.generateStaticFieldRID( OPERAND_BOOLEAN,
											OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
											| OPERAND_FLAG_DONTENUM
											| OPERAND_FLAG_READONLY ) );
	generateSymbolInformation( rid, wstring( STRING_INTERNAL_TRUE ), CG_SYMBOL_FLAG_READONLY );
	assert( rid == RID_CONSTANT_BOOL_TRUE );

	//RID_CONSTANT_BOOL_FALSE
	rid = m_MetaData.generateStaticFieldRID( OPERAND_BOOLEAN,
											OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
											| OPERAND_FLAG_DONTENUM
											| OPERAND_FLAG_READONLY ) );
	generateSymbolInformation( rid, wstring( STRING_INTERNAL_FALSE ), CG_SYMBOL_FLAG_READONLY );
	assert( rid == RID_CONSTANT_BOOL_FALSE );

	//RID_SWITCH_TEMPORARY
	rid = m_MetaData.generateStaticFieldRID( OPERAND_UNDEFINED,
											OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
											| OPERAND_FLAG_DONTENUM ) );
//Switch temporary value need to be writable
//											| OPERAND_FLAG_READONLY ) );
	generateSymbolInformation( rid, wstring( STRING_INTERNAL_SWITCHTMP ) );
	assert( rid == RID_SWITCHTMP );

	//RID_CONSTANT_DELETION
	rid = m_MetaData.generateStaticFieldRID( OPERAND_SYSTEM_DELETION,
											OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
											| OPERAND_FLAG_DONTENUM
											| OPERAND_FLAG_READONLY ) );
	generateSymbolInformation( rid, wstring( STRING_INTERNAL_DELETION ), CG_SYMBOL_FLAG_READONLY );
	assert( rid == RID_CONSTANT_DELETION );

	//RID_CONSTANT_NAN
	rid = m_MetaData.generateStaticFieldRID( OPERAND_NAN,
											OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
											| OPERAND_FLAG_DONTENUM
											| OPERAND_FLAG_READONLY ) );
	generateSymbolInformation( rid, wstring( STRING_INTERNAL_NAN ), CG_SYMBOL_FLAG_READONLY );
	assert( rid == RID_CONSTANT_NAN );

	//RID_CONSTANT_INFINITY
	rid = m_MetaData.generateStaticFieldRID( OPERAND_FLOAT,
											OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
											| OPERAND_FLAG_DONTENUM
											| OPERAND_FLAG_READONLY ) );
	generateSymbolInformation( rid, wstring( STRING_INTERNAL_INFINITY ), CG_SYMBOL_FLAG_READONLY );
	assert( rid == RID_CONSTANT_INFINITY );

	//RID_ARGUMETNS_TMP
	rid = m_MetaData.generateStaticFieldRID( OPERAND_OBJECTREF,
											OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
											| OPERAND_FLAG_DONTENUM
											| OPERAND_FLAG_READONLY ) );
	generateSymbolInformation( rid, wstring( STRING_INTERNAL_ARGUMENTSTMP ), CG_SYMBOL_FLAG_READONLY );
	assert( rid == RID_ARGUMENTS_TMP );

	//-----------------------------------------------------------------
	//Initialize parameter object
	//-----------------------------------------------------------------
	RID paramRidList[] = {
		RID_BUILTINPARAM_DEFAULT_ZERO,
		RID_BUILTINPARAM_DEFAULT_ONE,
		RID_BUILTINPARAM_DEFAULT_TWO,
		RID_BUILTINPARAM_DEFAULT_THREE,
		RID_BUILTINPARAM_DEFAULT_FOUR,
		RID_BUILTINPARAM_DEFAULT_FIVE,
		RID_BUILTINPARAM_DEFAULT_SIX,
		RID_BUILTINPARAM_DEFAULT_SEVEN,
		RID_BUILTINPARAM_DEFAULT_EIGHT,
		RID_BUILTINPARAM_DEFAULT_NINE,
	};
	for( int32_t i = 0; i < sizeof( paramRidList ) / sizeof( paramRidList [ 0 ] ); ++ i )
	{
		rid = m_MetaData.reserveBuiltinParamListRID( paramRidList[ i ] );
		assert( rid == paramRidList[ i ] );
		for( int32_t j = 0; j < i; ++j )
			m_MetaData.addParamListEntry( rid, OPERAND_OBJECTREF );
	}

	//-----------------------------------------------------------------
	//Initialize builtin strings
	//-----------------------------------------------------------------
	RID ridLengthString = m_MetaData.generateBuiltinStringConstantRID();
	bReturn = m_MetaData.setBuiltinString( ridLengthString, STRING_INTERNAL_LENGTH );
	m_mapStringConsantPool[ STRING_INTERNAL_LENGTH ] = ridLengthString;


	//-----------------------------------------------------------------
	//Initialize object
	//-----------------------------------------------------------------

	// Initialize object Object
	CObjectObject obj;
	bReturn = obj.InitializeCodeGen( *this );

	// Initialize System Object
	CSystemObject system;
	bReturn = system.InitializeCodeGen( *this );

	// Initialize array Object
	CArrayObject arrayObject;
	bReturn = arrayObject.InitializeCodeGen( *this );

	// Initialize String Object
	CStringObject stringobject;
	bReturn = stringobject.InitializeCodeGen( *this );

	// Initialize Number Object
	CNumberObject numberobject;
	bReturn = numberobject.InitializeCodeGen( *this );

	// Initialize Boolean Object
	CBooleanObject booleanobject;
	bReturn = booleanobject.InitializeCodeGen( *this );

	// Initialize Error Object
	CErrorObject errorobject;
	bReturn = errorobject.InitializeCodeGen( *this );

	// Initialize Date Object
	CDateObject dateobject;
	bReturn = dateobject.InitializeCodeGen( *this );

	// Initialize Function Object
	CFunctionObject function;
	bReturn = function.InitializeCodeGen( *this );

	// Initialize Regex Object
	CRegexObject regex;
	bReturn = regex.InitializeCodeGen( *this );

	// Initialize math Object
	CMathObject math;
	bReturn = math.InitializeCodeGen( *this );

	m_ridRegexObject = getMetaData().queryBuiltinObjectRid( wstring( NAME_BUILTIN_REGEX_OBJECT ) );
	assert( m_ridRegexObject != RID_NOTDEFINED );

	m_ridFunctionObject = getMetaData().queryBuiltinObjectRid( wstring( NAME_BUILTIN_FUNCTION_OBJECT ) );
	assert( m_ridFunctionObject != RID_NOTDEFINED );

	m_ridMethodIn = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_SYSTEM_IN ) );
	assert( m_ridMethodIn != RID_NOTDEFINED );

	m_ridMethodForIn = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_SYSTEM_FORIN ) );
	assert( m_ridMethodForIn != RID_NOTDEFINED );

	m_ridMethodLdPrototype = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_SYSTEM_LOADPROTOTYPE ) );
	assert( m_ridMethodLdPrototype != RID_NOTDEFINED );

	m_ridMethodStPrototype = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_SYSTEM_STOREPROTOTYPE ) );
	assert( m_ridMethodStPrototype != RID_NOTDEFINED );

	m_ridMethodLdProperty = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_SYSTEM_LOADPROPERTYBYSTRINGRID ) );
	assert( m_ridMethodLdProperty != RID_NOTDEFINED );

	m_ridMethodStProperty = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_SYSTEM_STOREPROPERTYBYSTRINGRID ) );
	assert( m_ridMethodStProperty != RID_NOTDEFINED );

	m_ridMethodLdPropertyByExpression = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_SYSTEM_LOADPROPERTYBYEXPRESSION ) );
	assert( m_ridMethodLdPropertyByExpression != RID_NOTDEFINED );

	m_ridMethodStPropertyByExpression = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_SYSTEM_STOREPROPERTYBYEXPRESSION ) );
	assert( m_ridMethodStPropertyByExpression != RID_NOTDEFINED );

	m_ridMethodTypeOf = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_SYSTEM_TYPEOF ) );
	assert( m_ridMethodTypeOf != RID_NOTDEFINED );

	m_ridMethodInstanceOf = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_SYSTEM_INSTANCEOF ) );
	assert( m_ridMethodInstanceOf != RID_NOTDEFINED );

	m_ridMethodStrictEqual = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_SYSTEM_STRICTEQUAL ) );
	assert( m_ridMethodStrictEqual != RID_NOTDEFINED );

	m_ridMethodStrictNotEqual = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_SYSTEM_STRICTNOTEQUAL ) );
	assert( m_ridMethodStrictNotEqual != RID_NOTDEFINED );

	m_ridMethodLdobjectLiteral = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_SYSTEM_LOADobjectLiteral ) );
	assert( m_ridMethodLdobjectLiteral != RID_NOTDEFINED );

	m_ridMethodLdarrayLiteral = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_SYSTEM_LOADarrayLiteral ) );
	assert( m_ridMethodLdarrayLiteral != RID_NOTDEFINED );

	m_ridMethodFunctionCtorWithRid = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_FUNCTION_OBJECT_CTORWITHRID ) );
	assert( m_ridMethodFunctionCtorWithRid != RID_NOTDEFINED );

	m_ridMethodEval = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_SYSTEM_EVAL ) );
	assert( m_ridMethodEval != RID_NOTDEFINED );

	m_ridMethodCall = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_FUNCTION_OBJECT_CALL ) );
	assert( m_ridMethodCall != RID_NOTDEFINED );

	m_ridMethodToNumber = getMetaData().queryBuiltinMethodRid( wstring( NAME_BUILTIN_NUMBER_OBJECT_CALL ) );
	assert( m_ridMethodToNumber != RID_NOTDEFINED );


	return bReturn;
}

/***************************************************************************
 *      BuiltIn object registration
 ***************************************************************************/
RID CCilCodeGen::registerBuiltinObject( const wstring& strQualifiedName )
{
	//Register to object map in Metadata
	RID rid = getMetaData().registerBuiltinObject( strQualifiedName );
	assert( rid != RID_NOTDEFINED );

	//Notify codegenerator the object information
	generateSymbolInformation( rid, strQualifiedName );

	return rid;

}

/***************************************************************************
 *      BuiltIn method registration
 ***************************************************************************/
RID CCilCodeGen::registerBuiltinMethod( const wstring& strQualifiedName,
							const METHOD_TYPE methodType,
							const RID_BUILTIN_PARAM ridParam )
{
	RID ridMethod = getMetaData().registerBuiltinMethod( strQualifiedName,
														methodType,
														ridParam );

	//Notify codegenerator the object information
	generateSymbolInformation( ridMethod, strQualifiedName );

	return ridMethod;
}

} //namespace CRI