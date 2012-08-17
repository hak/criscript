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
 * File     : CCilVmOperatorAdd.cpp
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
 *      Add operator
 *      (1) take value from stack
 *      (2) take value from stack
 *      (3) result(1) + result (2)
 *      (4) push result (3)
 ***************************************************************************/
void CCilVm::addOperator()
{
	assert( getEvalStackSize() >= 2 );
	CVariable& rhs = getEvalStackFirstEntry();
	CVariable& lhs = getEvalStackSecondEntry();

//	bool bValue;

	if( lhs.iOperandType == OPERAND_OBJECTREF
		|| rhs.iOperandType == OPERAND_OBJECTREF)
	{
		OPERAND_TYPE lhsType = lhs.getPrimitiveType( PRIMITIVETYPE_NUMBER );
		OPERAND_TYPE rhsType = rhs.getPrimitiveType( PRIMITIVETYPE_NUMBER );
		if( lhsType == OPERAND_STRING )
		{
			rhsType = rhs.getPrimitiveType( PRIMITIVETYPE_STRING );
		}
		else if( rhsType == OPERAND_STRING )
		{
			lhsType = lhs.getPrimitiveType( PRIMITIVETYPE_STRING );
		}
		
		if( lhsType == OPERAND_OBJECTREF || rhsType == OPERAND_OBJECTREF )
		{
			//if( !lhs.GetConstraintRid() )
			//{
			//	lhsType = lhs.getPrimitiveType( PRIMITIVETYPE_STRING );
			//}
			//if( !rhs.GetConstraintRid() )
			//{
			//	rhsType = rhs.getPrimitiveType( PRIMITIVETYPE_STRING );
			//}

			//lhsType = lhs.getPrimitiveType( PRIMITIVETYPE_STRING );
			//rhsType = rhs.getPrimitiveType( PRIMITIVETYPE_STRING );

			//if( lhsType == OPERAND_OBJECTREF || rhsType == OPERAND_OBJECTREF )
			{
				//Throw TypeError
				popThrowTypeError();
				return;
			}
		}
		
		if( lhsType == OPERAND_STRING || rhsType == OPERAND_STRING )
		{
			rhs.convertToString( this );
			lhs.convertToString( this );
			lhs.pString->append( *rhs.pString );
			popEvalStack();
		}
		else if( lhsType == OPERAND_UNDEFINED || rhsType == OPERAND_UNDEFINED )
		{
			popPushEvalStackUndefined();
		}
		else if( lhsType == OPERAND_NAN || rhsType == OPERAND_NAN )
		{
			popPushEvalStackNaN();
		}
		else if( lhsType == OPERAND_DOUBLE || rhsType == OPERAND_DOUBLE )
		{
			popPushEvalStack( lhs.toDouble() + rhs.toDouble() );
		}
		else if( lhsType == OPERAND_FLOAT || rhsType == OPERAND_FLOAT )
		{
			popPushEvalStack( lhs.toFloat() +  rhs.toFloat() );
		}
		else if( lhsType == OPERAND_INT64 || rhsType == OPERAND_INT64 )
		{
			popPushEvalStack( lhs.toInt64() +  rhs.toInt64() );
		}
		else if( lhsType == OPERAND_INT || rhsType == OPERAND_INT )
		{
			popPushEvalStack( lhs.toInt() +  rhs.toInt() );
		}
		else if( lhsType == OPERAND_BOOLEAN || rhsType == OPERAND_BOOLEAN )
		{
			popPushEvalStack( lhs.toBoolean() + rhs.toBoolean() );
		}
		else
		{
			Debug_Fatal( "Not implemented yet" );
		}
		return;
	} else if( lhs.iOperandType == OPERAND_STRING
		|| rhs.iOperandType == OPERAND_STRING )
	{
		lhs.convertToString( this );
		rhs.convertToString( this );
		lhs.pString->append( *rhs.pString );
		popEvalStack();
		return;
	} else if( lhs.iOperandType == OPERAND_NAN
		|| rhs.iOperandType == OPERAND_NAN )
	{
		popPushEvalStackUndefined();
		return;
	} else if( lhs.iOperandType == OPERAND_UNDEFINED
		|| rhs.iOperandType == OPERAND_UNDEFINED )
	{
		popPushEvalStackNaN();
		return;
	}
	
	switch( lhs.iOperandType )
	{
	case OPERAND_INT:
		switch( rhs.iOperandType )
		{
		case OPERAND_INT:
			popPushEvalStack( int32_t( lhs.iValue + rhs.iValue ) );
			break;
		case OPERAND_INT64:
			popPushEvalStack( int64_t( int64_t(lhs.iValue) + rhs.i64Value ) );
			break;
		case OPERAND_FLOAT:
			popPushEvalStack( float( lhs.iValue ) + rhs.fValue );
			break;
		case OPERAND_DOUBLE:
			popPushEvalStack( double( lhs.iValue ) + rhs.dValue );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			popPushEvalStack( int32_t( lhs.iValue +  rhs.toInt() ) );
			break;
		}
		break;
	case OPERAND_INT64:
		switch( rhs.iOperandType )
		{
		case OPERAND_INT:
			popPushEvalStack( lhs.i64Value + int64_t(rhs.iValue) );
			break;
		case OPERAND_INT64:
			popPushEvalStack( lhs.i64Value + rhs.i64Value );
			break;
		case OPERAND_FLOAT:
			popPushEvalStack( float( lhs.i64Value ) + rhs.fValue );
			break;
		case OPERAND_DOUBLE:
			popPushEvalStack( double( lhs.i64Value ) + rhs.dValue );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			popPushEvalStack( lhs.i64Value + rhs.toInt64() );
			break;
		}
		break;
	case OPERAND_FLOAT:
		switch( rhs.iOperandType )
		{
		case OPERAND_FLOAT:
			popPushEvalStack( lhs.fValue + rhs.fValue );
			break;
		case OPERAND_DOUBLE:
			popPushEvalStack( double(lhs.fValue) + rhs.dValue );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			popPushEvalStack( lhs.fValue + rhs.toFloat() );
			break;
		}
		break;
	case OPERAND_DOUBLE:
		switch( rhs.iOperandType )
		{
		case OPERAND_FLOAT:
			popPushEvalStack( lhs.dValue + double(rhs.fValue) );
			break;
		case OPERAND_DOUBLE:
			popPushEvalStack( lhs.dValue + rhs.dValue );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			popPushEvalStack( lhs.dValue + rhs.toDouble() );
			break;
		}
		break;
		break;
	case OPERAND_UNDEFINED:
	case OPERAND_NAN:
	case OPERAND_OBJECTREF:
	case OPERAND_STRING:
		Debug_Fatal( "Illegal operand" );
		break;
	case OPERAND_BOOLEAN:
		switch( rhs.iOperandType )
		{
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			//if( lhs.bValue )
			//	bValue = true;
			//else
			//	bValue = rhs.toBoolean();
			popPushEvalStack( lhs.toInt() + rhs.toInt() );
			break;
		}
		break;
	case OPERAND_NULL:
		{
		CVariable var = rhs;
		popEvalStack();
		popEvalStack();
		pushEvalStack( var );
		}
		break;
	default:
		Debug_Fatal( "Illegal operand" );
		break;
	}

}

/***************************************************************************
 *      Add operator Helper
 ***************************************************************************/

} //namespace CRI