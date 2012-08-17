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
 * File     : CCilVmOperatorMultiply.cpp
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
 *      Multiply operator
 *      (1) take value from stack
 *      (2) take value from stack
 *      (3) result(1) * result (2)
 *      (4) push result (3)
 ***************************************************************************/
void CCilVm::multiplyOperator()
{
	assert( getEvalStackSize() >= 2 );
	CVariable& rhs = getEvalStackFirstEntry();
	CVariable& lhs = getEvalStackSecondEntry();

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
			//Throw TypeError
			popThrowTypeError();
			return;
		}
		
		if( lhsType == OPERAND_UNDEFINED || rhsType == OPERAND_UNDEFINED )
		{
			popPushEvalStackUndefined();
			return;
		}
		else if( lhsType == OPERAND_NAN || rhsType == OPERAND_NAN )
		{
			popPushEvalStackNaN();
			return;
		}


		if( lhsType == OPERAND_STRING || rhsType == OPERAND_STRING )
		{
			float fRhs = rhs.toFloat();
			float fLhs = lhs.toFloat();

			popPushEvalStack( fLhs * fRhs );
		}
		else if( lhsType == OPERAND_DOUBLE || rhsType == OPERAND_DOUBLE )
		{
			popPushEvalStack( lhs.toDouble() * rhs.toDouble() );
		}
		else if( lhsType == OPERAND_FLOAT || rhsType == OPERAND_FLOAT )
		{
			popPushEvalStack( lhs.toFloat() * rhs.toFloat() );
		}
		else if( lhsType == OPERAND_INT64 || rhsType == OPERAND_INT64 )
		{
			int64_t i64 = rhs.toInt64();
			popPushEvalStack( lhs.toInt64() *  i64 );
		}
		else if( lhsType == OPERAND_INT || rhsType == OPERAND_INT )
		{
			int32_t i = rhs.toInt();
			popPushEvalStack( lhs.toInt() *  i );
		}
		else if( lhsType == OPERAND_BOOLEAN || rhsType == OPERAND_BOOLEAN )
		{
			int32_t i = rhs.toInt();
			popPushEvalStack( lhs.toInt() * rhs.toInt() );
		}
		else
		{
			Debug_Fatal( "Not implemented yet" );
		}
		return;
	} else if( lhs.iOperandType == OPERAND_STRING
		|| rhs.iOperandType == OPERAND_STRING )
	{
		popPushEvalStack( lhs.toFloat() * rhs.toFloat() );
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
	
	int32_t i;
	int64_t i64;

	switch( lhs.iOperandType )
	{
	case OPERAND_INT:
		switch( rhs.iOperandType )
		{
		case OPERAND_INT:
			i = rhs.toInt();
			popPushEvalStack( int32_t( lhs.iValue * i ) );
			break;
		case OPERAND_INT64:
			i64 = rhs.toInt64();
			popPushEvalStack( int64_t( int64_t(lhs.iValue) * i64 ) );
			break;
		case OPERAND_FLOAT:
			popPushEvalStack( float( lhs.iValue ) * rhs.fValue );
			break;
		case OPERAND_DOUBLE:
			popPushEvalStack( double( lhs.iValue ) * rhs.dValue );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			i = rhs.toInt();
			popPushEvalStack( int32_t( lhs.iValue *  i ) );
			break;
		}
		break;
	case OPERAND_INT64:
		switch( rhs.iOperandType )
		{
		case OPERAND_INT:
		case OPERAND_INT64:
			i64 = rhs.toInt64();
			popPushEvalStack( lhs.i64Value * i64 );
			break;
		case OPERAND_FLOAT:
			popPushEvalStack( float( lhs.i64Value ) * rhs.fValue );
			break;
		case OPERAND_DOUBLE:
			popPushEvalStack( double( lhs.i64Value ) * rhs.dValue );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			popPushEvalStack( lhs.i64Value * rhs.toInt64() );
			break;
		}
		break;
	case OPERAND_FLOAT:
		switch( rhs.iOperandType )
		{
		case OPERAND_FLOAT:
			popPushEvalStack( float(lhs.fValue) * rhs.fValue );
			break;
		case OPERAND_DOUBLE:
			popPushEvalStack( double(lhs.fValue) * rhs.dValue );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			popPushEvalStack( float(lhs.fValue) * rhs.toFloat() );
			break;
		}
		break;
	case OPERAND_DOUBLE:
		switch( rhs.iOperandType )
		{
		case OPERAND_FLOAT:
			popPushEvalStack( lhs.dValue * double(rhs.fValue) );
			break;
		case OPERAND_DOUBLE:
			popPushEvalStack( lhs.dValue * rhs.dValue );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			popPushEvalStack( lhs.dValue * rhs.toDouble() );
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
		case OPERAND_INT:
			popPushEvalStack( int32_t( lhs.toInt() * rhs.iValue ) );
			break;
		case OPERAND_INT64:
			popPushEvalStack( int64_t( lhs.toInt64() * rhs.i64Value ) );
			break;
		case OPERAND_FLOAT:
			popPushEvalStack( float( lhs.toFloat() * rhs.fValue ) );
			break;
		case OPERAND_DOUBLE:
			popPushEvalStack( double( lhs.toDouble() * rhs.dValue ) );
			break;
		default:
			popPushEvalStack( lhs.toInt() * rhs.toInt() );
			break;
		}
		break;
	case OPERAND_NULL:
		popPushEvalStack( 0 );
		break;
	default:
		Debug_Fatal( "Illegal operand" );
		break;
	}

}

/***************************************************************************
 *      Multiply operator Helper
 ***************************************************************************/

} //namespace CRI