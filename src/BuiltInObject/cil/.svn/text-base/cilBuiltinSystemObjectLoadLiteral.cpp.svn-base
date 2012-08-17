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
 * Module   : cilBuiltinSystemObjectLoadLiteral
 * File     : system object, load literal
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

/***************************************************************************
 *      Load object literal onto the stack
 ***************************************************************************/
void CSystemObject::LoadobjectLiteral( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 2 );
	assert( iNumArguments % 2 == 1 );

	//Skip this pointer
	pArguments++;
	int32_t i = iNumArguments;
	i--;
	CMetaData& Metadata = pVm->getMetaData();

	//Create object object
	CVmObject newobject;
	RID ridObject = pVm->getObjectRID();
	newobject.setRID( ridObject );
	newobject.setObjectName( pVm->getMetaData().getObjectNameRID( ridObject ) );
	newobject.setPrototypeObject( &pVm->getPrototypeObject( ridObject ) );

	pVm->getObjectPool().push_back( newobject );

	CVariable var;
	var.setObjectRef( &pVm->getObjectPool().back() );

	for( ; i > 0; i -= 2 )
	{
		//Argument: 1) value of the property
		//			2) string RID of the property

		RID ridString = pArguments->iValue;
		assert( TypeFromToken( ridString ) == MDT_STRING );
		pArguments++;

		wstring& str = Metadata.getStringConstant( ridString );
		var.refObject->setProperty( str, *pArguments );
		pArguments++;
	}

	//Push the ObjectRef to a eval stack
	pVm->pushEvalStackVariable( var );
	return;
}

/***************************************************************************
 *      Load array literal onto the stack
 ***************************************************************************/
void CSystemObject::LoadarrayLiteral( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	//Array parameter can be null
	assert( iNumArguments >= 1 );

	//Skip this pointer
	pArguments++;

	//Create object object
	CVmObject* pObj = pVm->createObject( pVm->getArrayObjectRID() );
	CVariable var;
	var.setObjectRef( pObj );

	wchar_t strBuffer[ 22 ];		//22=max value width of longlong

	int32_t i = 0;
	for( ; i < iNumArguments - 1; ++i )
	{
		//Argument: 1) value of the property
		swprintf( strBuffer, 22, L"%d", i );
		var.refObject->setProperty( strBuffer, *pArguments );
		pArguments++;
	}

	//Set prototype: Array prototype object
	var.refObject->setPrototypeObject( 
		&pVm->getPrototypeObject( pVm->getArrayObjectRID() ) );

	//Update 'length' property
	CVariable varInt( 0 );
	var.refObject->setProperty( STRING_INTERNAL_LENGTH, varInt );
	//-2 means, 'prototype' & 'length'
	CVariable varLength( (int32_t)var.refObject->getPropertyMap().size() - 2,
						OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM ) ); 
	var.refObject->setProperty( STRING_INTERNAL_LENGTH, varLength );

	//Push the ObjectRef to a eval stack
	pVm->pushEvalStackVariable( var );

	return;
}

} //namespace CRI