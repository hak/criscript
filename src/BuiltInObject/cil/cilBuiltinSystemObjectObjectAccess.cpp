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
 * Module   : cilBuiltinSystemObjectPrototypeAccess
 * File     : system object, prototype access
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
 *      Load prototype object
 ***************************************************************************/
void CSystemObject::LoadPrototype( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments == 2 );

	//Skip this pointer
	pArguments++;
	CMetaData& Metadata = pVm->getMetaData();

	//Argument: 1) object reference
	if( OperandType( pArguments->iOperandType ) != OPERAND_OBJECTREF )
	{
		//Store undefined
		CVariable var;
		pVm->pushEvalStack( var );
		return;
	}
	VMOBJECTREF refObject = pArguments->refObject;
	CVariable var;
	var.setObjectRef( refObject->getPrototypeObject() );
	if( refObject->getPrototypeObject() == NULL )
	{
		pVm->pushEvalStackNull();
	}
	else
		pVm->pushEvalStack( var );

	return;
}

/***************************************************************************
 *      Store prototype
 ***************************************************************************/
void CSystemObject::StorePrototype( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments == 2 );

	//Skip this pointer
	pArguments++;
	CMetaData& Metadata = pVm->getMetaData();

	//Argument: 1) object reference
	if( OperandType( pArguments->iOperandType ) != OPERAND_OBJECTREF )
	{
		//Do nothing
		pVm->popEvalStack();
		return;
	}
	VMOBJECTREF refObject = pArguments->refObject;

	refObject->setPrototypeObject( pVm->getEvalStackFirstEntry().refObject );

	//Store the value to the map, too
	CVariable var;
	var.setObjectRef( pVm->getEvalStackFirstEntry().refObject );
	var.setFlag( OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
													| OPERAND_FLAG_DONTENUM
													| OPERAND_FLAG_READONLY ));
	refObject->setProperty( wstring( STRING_INTERNAL_PROTOTYPE ), var );
	pVm->popEvalStack();

	return;
}

/***************************************************************************
 *      typeof implementation
 ***************************************************************************/
void CSystemObject::Typeof( CCilVm* const pVm,
							const int32_t iNumArguments, 
							CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments == 2 );

	//Skip this pointer
	pArguments++;

	//Argument: 1) object reference
	switch( OperandType( pArguments->iOperandType ) )
	{
	case OPERAND_BOOLEAN:
		pVm->pushEvalStack( CVariable( STRING_INTERNAL_TYPEOF_BOOLEAN ) );
		break;
	case OPERAND_NAN:
	case OPERAND_DOUBLE:
	case OPERAND_FLOAT:
	case OPERAND_INT:
	case OPERAND_UNSIGNEDINT:
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
		pVm->pushEvalStack( CVariable( STRING_INTERNAL_TYPEOF_NUMBER ) );
		break;
	case OPERAND_NULL:
		pVm->pushEvalStack( CVariable( STRING_INTERNAL_TYPEOF_OBJECT ) );
		break;
	case OPERAND_UNDEFINED:
		pVm->pushEvalStack( CVariable( STRING_INTERNAL_UNDEFINED ) );
		break;
	case OPERAND_STRING:
		pVm->pushEvalStack( CVariable( STRING_INTERNAL_TYPEOF_STRING ) );
		break;
	case OPERAND_OBJECTREF:
		if( pArguments->refObject->getRID() == pVm->getNumberObjectRID() )
			pVm->pushEvalStack( CVariable( STRING_INTERNAL_TYPEOF_NUMBER ) );
		else if( pArguments->refObject->getCall() != NULL )
			pVm->pushEvalStack( CVariable( STRING_INTERNAL_TYPEOF_FUNCTION ) );
		else
			pVm->pushEvalStack( CVariable( STRING_INTERNAL_TYPEOF_OBJECT ) );
		break;
	default:
		pVm->pushEvalStack( CVariable( STRING_INTERNAL_UNDEFINED ) );
		break;
	}
	
	return;
}

} //namespace CRI