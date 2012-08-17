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

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Strict equal
 ***************************************************************************/
void CSystemObject::StrictEqual( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments == 3 );

	//Skip this pointer
	pArguments++;

	CVariable& varLhs = *pArguments++;
	CVariable& varRhs = *pArguments++;
	
	//Those conversions are out of spec...
	if( OperandType( varLhs.iOperandType ) == OPERAND_OBJECTREF )
		varLhs.convertToPrimitiveType( pVm );
	if( OperandType( varRhs.iOperandType ) == OPERAND_OBJECTREF )
		varRhs.convertToPrimitiveType( pVm );

//	if( OperandType( varLhs.iOperandType ) != OperandType( varRhs.iOperandType ) )
	if( getType( varLhs ) != getType( varRhs ) )
	{
		pVm->pushEvalStack( (int32_t)0 );	//false
		return;
	}

	int32_t iReturn = 0;
	switch( OperandType( varLhs.iOperandType ) )
	{
	case OPERAND_UNDEFINED:
	case OPERAND_NULL:
		iReturn = 1;
		break;
	case OPERAND_NAN:
		iReturn = 0;
		break;
	case OPERAND_OBJECTREF:
		if( varLhs.iValue == varRhs.iValue )
			iReturn = 1;
		break;
	case OPERAND_INT:
	case OPERAND_UNSIGNEDINT:
		switch( OperandType( varRhs.iOperandType ) )
		{
		case OPERAND_FLOAT:
			if( varLhs.toFloat() == varRhs.toFloat() )
				iReturn = 1;
			break;
		case OPERAND_DOUBLE:
			if( varLhs.toDouble() == varRhs.toDouble() )
				iReturn = 1;
			break;
		default:
			if( varLhs.iValue == varRhs.iValue )
				iReturn = 1;
			break;
		}
		break;
	case OPERAND_FLOAT:
		if( varLhs.toFloat() == varRhs.toFloat() )
			iReturn = 1;
		break;
	case OPERAND_DOUBLE:
		if( varLhs.toDouble() == varRhs.toDouble() )
			iReturn = 1;
		break;
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
		if( varLhs.i64Value == varRhs.i64Value )
			iReturn = 1;
		break;
	case OPERAND_STRING:
		if( !varLhs.pString->compare( *varRhs.pString ) )
			iReturn = 1;
		break;
	case OPERAND_BOOLEAN:
		if( varLhs.bValue == varRhs.bValue )
			iReturn = 1;
		break;
	default:
		Debug_Fatal( "Not implemented yet" );
	}

	pVm->pushEvalStack( (int32_t)iReturn );	//false

	return;
}

/***************************************************************************
 *      Strict Not equal
 ***************************************************************************/
void CSystemObject::StrictNotEqual( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments == 3 );

	//Skip this pointer
	pArguments++;

	CVariable& varLhs = *pArguments++;
	CVariable& varRhs = *pArguments++;
	
	//Those conversions are out of spec...
	if( OperandType( varLhs.iOperandType ) == OPERAND_OBJECTREF )
		varLhs.convertToPrimitiveType( pVm );
	if( OperandType( varRhs.iOperandType ) == OPERAND_OBJECTREF )
		varRhs.convertToPrimitiveType( pVm );

//	if( OperandType( varLhs.iOperandType ) != OperandType( varRhs.iOperandType ) )
	if( getType( varLhs ) != getType( varRhs ) )
	{
		pVm->pushEvalStack( (int32_t)1 );	//false
		return;
	}

	int32_t iReturn = 1;
	switch( OperandType( varLhs.iOperandType ) )
	{
	case OPERAND_UNDEFINED:
	case OPERAND_NULL:
		iReturn = 0;
		break;
	case OPERAND_NAN:
		iReturn = 1;
		break;
	case OPERAND_OBJECTREF:
	case OPERAND_INT:
	case OPERAND_UNSIGNEDINT:
	case OPERAND_FLOAT:
		if( varLhs.iValue == varRhs.iValue )
			iReturn = 0;
		break;
	case OPERAND_DOUBLE:
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
		if( varLhs.i64Value == varRhs.i64Value )
			iReturn = 0;
		break;
	case OPERAND_STRING:
		if( !varLhs.pString->compare( *varRhs.pString ) )
			iReturn = 0;
		break;
	case OPERAND_BOOLEAN:
		if( varLhs.bValue == varRhs.bValue )
			iReturn = 0;
		break;
	default:
		Debug_Fatal( "Not implemented yet" );
	}

	pVm->pushEvalStack( (int32_t)iReturn );	//false

	return;
}
/***************************************************************************
 *      Type(x) implementation
 ***************************************************************************/
OPERAND_TYPE CSystemObject::getType( CVariable& var )
{

	switch( OperandType( var.iOperandType ) )
	{
	case OPERAND_BOOLEAN:
		return OPERAND_BOOLEAN;
	case OPERAND_NAN:
	case OPERAND_DOUBLE:
	case OPERAND_FLOAT:
	case OPERAND_INT:
	case OPERAND_UNSIGNEDINT:
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
		return OPERAND_INT;	//Actually 'number'
	case OPERAND_NULL:
//		return OPERAND_OBJECTREF;
		return OPERAND_NULL;
	case OPERAND_UNDEFINED:
		return OPERAND_UNDEFINED;
	case OPERAND_STRING:
		return OPERAND_STRING;
	case OPERAND_OBJECTREF:
		if( var.refObject->getRID() == CCilVm::getStringObjectRID() )
			return OPERAND_STRING;
		else if( var.refObject->getCall() != NULL )
		if( var.refObject->getRID() == CCilVm::getNumberObjectRID() )
			return OPERAND_INT;	//Actually 'number'
		else if( var.refObject->getCall() != NULL )
			return OPERAND_OBJECTREF;	//Actually 'function'
		else
			return OPERAND_OBJECTREF;
		break;
	default:
		return OPERAND_UNDEFINED;
	}
	
	return OPERAND_UNDEFINED;
}

} //namespace CRI