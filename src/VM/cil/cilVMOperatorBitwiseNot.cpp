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
 * Module   : cil Virtual Machine Arithmetical ops
 * File     : CCilVmOperatorbitwiseNot.cpp
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
 *      bitwise Not operator
 *      (1) take value from stack
 *      (2) ~result(1)
 *      (3) push result (2)
 ***************************************************************************/
void CCilVm::bitwiseNotOperator()
{
	assert( getEvalStackSize() >= 1 );
	CVariable& lhs = getEvalStackFirstEntry();

	int32_t i;
	int64_t i64;

	if( lhs.iOperandType == OPERAND_OBJECTREF )
	{
		OPERAND_TYPE lhsType = lhs.getPrimitiveType( PRIMITIVETYPE_NUMBER );
		
		switch( lhsType )
		{
		case OPERAND_OBJECTREF:
			//Throw TypeError
			popEvalStack();
			pushEvalStackNaN();
			ThrowExceptionFromVm( &wstring( ERRORSTRING_TYPEERROR_STORESTATICFIELD ),
				&wstring( NAME_BUILTIN_ERROR_OBJECT ),
				ERROR_TYPEERROR_STORESTATICFIELD );
			return;
		case OPERAND_UNDEFINED:
			popEvalStack();
			pushEvalStackUndefined();
			return;
		case OPERAND_NAN:
			popEvalStack();
			pushEvalStackNaN();
			return;
		case OPERAND_STRING:
		case OPERAND_DOUBLE:
		case OPERAND_FLOAT:
		case OPERAND_INT:
		case OPERAND_BOOLEAN:
			i = ~lhs.toInt();
			popEvalStack();
			pushEvalStack( i );
			return;
		case OPERAND_INT64:
			i64 = ~lhs.toInt64();
			popEvalStack();
			pushEvalStack( i64 );
			return;
		default:
			Debug_Fatal( "Not implemented yet" );
		}
	}

	switch( lhs.iOperandType )
	{
	case OPERAND_INT:
		lhs.iValue = ~lhs.iValue;
		break;
	case OPERAND_INT64:
		lhs.i64Value = ~lhs.i64Value;
		break;
	case OPERAND_NAN:
		popEvalStack();
		pushEvalStackNaN();
		break;
	case OPERAND_UNDEFINED:
		popEvalStack();
		pushEvalStackUndefined();
		break;
	case OPERAND_STRING:
	case OPERAND_FLOAT:
	case OPERAND_BOOLEAN:
	case OPERAND_DOUBLE:
	case OPERAND_NULL:
		i = ~lhs.toInt();
		popEvalStack();
		pushEvalStack( i );
		break;
	case OPERAND_OBJECTREF:
		Debug_Fatal( "Illegal operand" );
		break;
	default:
		Debug_Fatal( "Illegal operand" );
		break;
	}
}

/***************************************************************************
 *      bitwiseNot operator Helper
 ***************************************************************************/

} //namespace CRI