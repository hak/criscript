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
 * Module   : cilBuiltinSystemObjectPropertyAccess
 * File     : System object, property accessor
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
 *      Load property by string rid method
 ***************************************************************************/
void CSystemObject::LoadPropertyByStringRid( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments == 3 );

	//Skip this pointer
	pArguments++;

	//Save the variable as current watching variable
	CVariable* pParentObject = &*pArguments;

	CMetaData& Metadata = pVm->getMetaData();
	VMOBJECTREF refObject;
	RID ridString = (pArguments + 1)->iValue;

	//Argument: 1) object reference
	//			2) string RID of the property
	switch( OperandType( pArguments->iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		refObject = pArguments->refObject;
		break;
	case OPERAND_STRING:
		//Check if Object prototype has it
		refObject = &pVm->getPrototypeObject( CCilVm::getStringObjectRID() );
		break;
	case OPERAND_BOOLEAN:
		//Check if Object prototype has it
		refObject = &pVm->getPrototypeObject( CCilVm::getBooleanObjectRID() );
		break;
	case OPERAND_INT:
	case OPERAND_UNSIGNEDINT:
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
	case OPERAND_FLOAT:
	case OPERAND_DOUBLE:
	case OPERAND_NAN:
		//Check if Object prototype has it
		refObject = &pVm->getPrototypeObject( CCilVm::getNumberObjectRID() );
		break;
	default:
		//Check if Object prototype has it
		refObject = &pVm->getPrototypeObject( pVm->getObjectRID() );
		break;
	}

	assert( TypeFromToken( ridString ) == MDT_STRING );

	wstring& str = Metadata.getStringConstant( ridString );
	CVariable* pVar = refObject->lookupProperty( pVm, str, pParentObject );
	if( pVar == NULL )
	{
		if( pVm->getExectipnHandlerNestingLevel() )
		{
			//Throw an exception if somebody is watching it.
			pVm->throwException( &wstring( ERRORSTRING_TYPEERROR_UNDEFINEDPROPERTY ),
				&wstring( NAME_BUILTIN_ERROR_OBJECT ),
				ERROR_TYPEERROR_UNDEFINEDPROPERTY );
		}
		else
		{
			Debug_Warning( "Property not found:" );
	#ifdef _XBOX
			OutputDebugStringW( str.data() );
	#else
		#ifdef __APPLE__
		
		#else
		#ifndef  NDEBUG
				OutputDebugString( str.data() );
		#endif
		#endif
	#endif
			Debug_Warning( "\n" );
			pVm->pushEvalStackUndefined();
		}
	}
	else
	{
		if( OperandType( pVar->iOperandType ) == OPERAND_OBJECTREF )
		{
			pVar->refObject->setParentVariable( *pParentObject );
		}
		pVm->pushEvalStack( *pVar );
	}
	return;
}

/***************************************************************************
 *      Store property by string rid method
 ***************************************************************************/
void CSystemObject::StorePropertyByStringRid( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments == 4 );

	//Skip this pointer
	pArguments++;
	CMetaData& Metadata = pVm->getMetaData();

	//Argument: 1) object reference
	//			2) string RID of the property
	if( OperandType( pArguments->iOperandType ) != OPERAND_OBJECTREF )
	{
		//Do nothing
		return;
	}

	VMOBJECTREF refObject = pArguments->refObject;

	pArguments++;

	RID ridString = pArguments->iValue;
	assert( TypeFromToken( ridString ) == MDT_STRING );

	//Clear parent varialbe setting
	//Those setting will be set when the property is accessed
	pArguments++;
	CVariable& var = *pArguments;
	if( OperandType( var.iOperandType ) == OPERAND_OBJECTREF )
	{
		var.refObject->setParentVariable( CVariable() );
	}

	wstring* pStr = &Metadata.getStringConstant( ridString );

	refObject->setProperty( pVm, *pStr, var );

	//Note that popping from the eval stack

	return;
}

/***************************************************************************
 *      Load property by expression method
 ***************************************************************************/
void CSystemObject::LoadPropertyByExpression( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments == 3 );

	//Skip this pointer
	pArguments++;

	//Save the variable as current watching variable
	CVariable* pParentObject = &*pArguments;
	VMOBJECTREF refObject;

	CVariable& var = *(pArguments + 1);
	//Convert var to a string
	var.convertToString( pVm );
	wstring* str = var.pString;

	//Argument: 1) object reference
	//			2) variable need to be converted to a string
	switch( OperandType( pArguments->iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		refObject = pArguments->refObject;
		break;
	case OPERAND_STRING:
		//Check if Object prototype has it
		refObject = &pVm->getPrototypeObject( CCilVm::getStringObjectRID() );
		break;
	case OPERAND_BOOLEAN:
		//Check if Object prototype has it
		refObject = &pVm->getPrototypeObject( CCilVm::getBooleanObjectRID() );
		break;
	case OPERAND_INT:
	case OPERAND_UNSIGNEDINT:
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
	case OPERAND_FLOAT:
	case OPERAND_DOUBLE:
	case OPERAND_NAN:
		//Check if Object prototype has it
		refObject = &pVm->getPrototypeObject( CCilVm::getNumberObjectRID() );
		break;
	default:
		//Check if Object prototype has it
		refObject = &pVm->getPrototypeObject( pVm->getObjectRID() );
		break;
	}

	CVariable* pVar = refObject->lookupProperty( pVm, *str, pParentObject );
	if( pVar == NULL )
	{
		if( pVm->getExectipnHandlerNestingLevel() )
		{
			//Throw an exception if somebody is watching it.
			pVm->throwException( &wstring( ERRORSTRING_TYPEERROR_UNDEFINEDPROPERTY ),
			&wstring( NAME_BUILTIN_ERROR_OBJECT ),
			ERROR_TYPEERROR_UNDEFINEDPROPERTY );
		}
		else
		{
			Debug_Warning( "Property not found:" );
	#ifdef _XBOX
		//Xbox360
			OutputDebugStringW( str->data() );
	#else
		#ifdef __APPLE__
		//OSX
		#else
		//Win32
		#ifndef  NDEBUG
				OutputDebugString( str->data() );
		#endif
		#endif
	#endif
			Debug_Warning( "\n" );
			pVm->pushEvalStackUndefined();
		}
	}
	else
	{
	if( OperandType( pVar->iOperandType ) == OPERAND_OBJECTREF )
		{
			pVar->refObject->setParentVariable( *pParentObject );
		}
		pVm->pushEvalStack( *pVar );
	}

	return;
}

/***************************************************************************
 *      Store property by expression method
 ***************************************************************************/
void CSystemObject::StorePropertyByExpression( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments == 4 );

	//Skip this pointer
	pArguments++;

	//Argument: 1) object reference
	//			2) string RID of the property
	if( OperandType( pArguments->iOperandType ) != OPERAND_OBJECTREF )
	{
		//Do nothing
		return;
	}
	VMOBJECTREF refObject = pArguments->refObject;
	pArguments++;

	CVariable& var = *pArguments++;
	CVariable& varTarget = *pArguments++;

	//Convert var to a string
	var.convertToString( pVm );
	wstring* pStr = var.pString;


	//Clear parent varialbe setting
	//Those setting will be set when the property is accessed
	if( OperandType( varTarget.iOperandType ) == OPERAND_OBJECTREF )
	{	
		varTarget.refObject->setParentVariable( CVariable() );
	}

	//Set property
	refObject->setProperty( pVm, *pStr, varTarget );

	//Note that popping from the eval stack

	return;
}

} //namespace CRI