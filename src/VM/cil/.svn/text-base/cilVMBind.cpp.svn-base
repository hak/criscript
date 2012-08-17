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
 * Module   : cil Virtual Machine bind apis
 * File     : cilVMBind.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
#include "cilVM.h"
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/
/***************************************************************************
 *      bindBuiltinMethod
 ***************************************************************************/
bool CCilVm::bindBuiltinMethod( const wstring& strQualifiedName,
							PMETHOD const pMethod,
							const int32_t iLength )
{
	bool bReturn = true;
	RID ridMethod = getMetaData().queryBuiltinMethodRid( strQualifiedName );
	if( ridMethod == RID_NULL )
		return false;

	//Bind the API
	METHOD_DEF& method = getMetaData().getMethod( ridMethod );
	if( method.MethodType != METHOD_NATIVE )
		return false;
	method.lpFunction = pMethod;
	
	if( strQualifiedName.find( STRING_INTERNAL_SCOPEDELIMITER ) != wstring::npos )
	{
		wstring strRoot = strQualifiedName.substr( 0, strQualifiedName.rfind( STRING_INTERNAL_SCOPEDELIMITER ) );
		assert( strRoot.find( STRING_INTERNAL_SCOPEDELIMITER ) == wstring::npos );

		wstring strLeaf = strQualifiedName.substr( strQualifiedName.rfind( STRING_INTERNAL_SCOPEDELIMITER ) + 1,
													strQualifiedName.length() );
		RID ridObject = getMetaData().queryBuiltinObjectRid( strRoot );
		if( ridObject == RID_NOTDEFINED )
			return false;

		//Create property
		getPrototypeObject( ridObject ).setProperty( this,
														strLeaf,
														ridMethod,
														iLength );		
	}
	else
	{
		//Global method
		//e.g. print, eval etc
		//Those one need to have correspond static field to keep properties
		CG_SYMBOL_INFORMATION& info = querySymbolInformation( strQualifiedName + STRING_OBJECTNAME_POSTFIX );
		assert( info.Flags == CG_SYMBOL_FLAG_GLOBAL );

		RID rid = info.rid;
		assert( rid != RID_NOTDEFINED );
		if( rid )
			setStaticFieldFunctionObject( rid, ridMethod );
	}

	return bReturn;
}

/***************************************************************************
 *      BindBuiltinMethodWithWrapper
 ***************************************************************************/
bool CCilVm::bindBuiltinMethod( const wstring& strQualifiedName,
							PMETHOD const pMethod,
							PMETHOD const pMethodBody )
{
	bool bReturn = true;
	RID ridMethod = getMetaData().queryBuiltinMethodRid( strQualifiedName );
	if( ridMethod == RID_NULL )
		return false;

	//Bind the API
	METHOD_DEF& method = getMetaData().getMethod( ridMethod );
	if( method.MethodType != METHOD_WRAPPED )
		return false;
	method.lpFunction = pMethod;
	method.lpFunctionBody = pMethodBody;
	
	if( strQualifiedName.find( STRING_INTERNAL_SCOPEDELIMITER ) != wstring::npos )
	{
		wstring strRoot = strQualifiedName.substr( 0, strQualifiedName.rfind( STRING_INTERNAL_SCOPEDELIMITER ) );
		assert( strRoot.find( STRING_INTERNAL_SCOPEDELIMITER ) == wstring::npos );

		wstring strLeaf = strQualifiedName.substr( strQualifiedName.rfind( STRING_INTERNAL_SCOPEDELIMITER ) + 1,
													strQualifiedName.length() );
		RID ridObject = getMetaData().queryBuiltinObjectRid( strRoot );
		if( ridObject == RID_NOTDEFINED )
			return false;

		//Create property
		getPrototypeObject( ridObject ).setProperty( this,
														strLeaf,
														ridMethod,
														method.iNumArgments );		
	}

	return bReturn;
}

/***************************************************************************
 *      bindBuiltinVariable
 ***************************************************************************/
bool CCilVm::bindBuiltinVariable( const wstring& strQualifiedName,
						CVariable* const pvar )
{
	bool bReturn = true;

	//Bind the var
	
	if( strQualifiedName.find( STRING_INTERNAL_SCOPEDELIMITER ) != wstring::npos )
	{
		wstring strRoot = strQualifiedName.substr( 0, strQualifiedName.rfind( STRING_INTERNAL_SCOPEDELIMITER ) );
		assert( strRoot.find( STRING_INTERNAL_SCOPEDELIMITER ) == wstring::npos );

		wstring strLeaf = strQualifiedName.substr( strQualifiedName.rfind( STRING_INTERNAL_SCOPEDELIMITER ) + 1,
													strQualifiedName.length() );
		RID ridObject = getMetaData().queryBuiltinObjectRid( strRoot );
		if( ridObject == RID_NOTDEFINED )
			return false;

		//Create property
		getPrototypeObject( ridObject ).setProperty( strLeaf,
														*pvar );
	}

	return bReturn;
}

/***************************************************************************
 *   setStaticFieldFunctionObject
 ***************************************************************************/
bool CCilVm::setStaticFieldFunctionObject( const RID rid,
											 const RID ridMethod )						 
{
	assert( TypeFromToken( ridMethod ) == MDT_METHODDEF );

	//Create function object and store it to static field
	CVmObject newobject;
	RID ridObj = getFunctionObjectRID();

	newobject.setRID( ridObj );
	newobject.setObjectName( getMetaData().getObjectNameRID( ridObj ) );
	newobject.setPrototypeObject( &getPrototypeObject( ridObj ) );
	newobject.setConstructorProperty( ridMethod );
	newobject.setCallProperty( ridMethod );

	getObjectPool().push_back( newobject );

	CVariable var;
	var.setObjectRef( &getObjectPool().back() );

	METHOD_DEF& method = getMetaData().getMethod( ridMethod );
	int32_t iLength = method.iNumArgments;
	assert( iLength >= 0 );

	CVariable varInt( iLength, OPERAND_FLAG_DONTENUM );
	var.refObject->setProperty( STRING_INTERNAL_LENGTH,
			varInt );

	//Store the ObjectRef to a static field
	setStaticField( rid, var ); 

	return true;
}


} //namespace CRI