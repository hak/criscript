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
 * File     : CCilVmOperatorbitwiseXor.cpp
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
 *      (3) result(1) ^ result (2)
 *      (4) push result (3)
 ***************************************************************************/
void CCilVm::bitwiseXorOperator()
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
			popPushEvalStack( lhs.toInt() ^ rhs.toInt() );
		}
		else if( lhsType == OPERAND_DOUBLE || rhsType == OPERAND_DOUBLE )
		{
			popPushEvalStack( lhs.toInt() ^ rhs.toInt() );
		}
		else if( lhsType == OPERAND_FLOAT || rhsType == OPERAND_FLOAT )
		{
			popPushEvalStack( lhs.toInt() ^ rhs.toInt() );
		}
		else if( lhsType == OPERAND_INT64 || rhsType == OPERAND_INT64 )
		{
			int64_t i64 = rhs.toInt64();
			popPushEvalStack( lhs.toInt64() ^  i64 );
		}
		else if( lhsType == OPERAND_INT || rhsType == OPERAND_INT )
		{
			int32_t i = rhs.toInt();
			popPushEvalStack( lhs.toInt() ^  i );
		}
		else if( lhsType == OPERAND_BOOLEAN || rhsType == OPERAND_BOOLEAN )
		{
			int32_t i = rhs.toInt();
			popPushEvalStack( lhs.toInt() ^ rhs.toInt() );
		}
		else
		{
			Debug_Fatal( "Not implemented yet" );
		}
		return;
	} else if( lhs.iOperandType == OPERAND_STRING
		|| rhs.iOperandType == OPERAND_STRING )
	{
		popPushEvalStack( lhs.toInt() ^ rhs.toInt() );
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
			popPushEvalStack( int32_t( lhs.iValue ^ i ) );
			break;
		case OPERAND_INT64:
			i64 = rhs.toInt64();
			popPushEvalStack( int64_t( int64_t(lhs.iValue) ^ i64 ) );
			break;
		case OPERAND_FLOAT:
		case OPERAND_DOUBLE:
			popPushEvalStack( lhs.iValue ^ rhs.toInt() );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			i = rhs.toInt();
			popPushEvalStack( int32_t( lhs.iValue ^  i ) );
			break;
		}
		break;
	case OPERAND_INT64:
		switch( rhs.iOperandType )
		{
		case OPERAND_INT:
		case OPERAND_INT64:
			i64 = rhs.toInt64();
			popPushEvalStack( lhs.i64Value ^ i64 );
			break;
		case OPERAND_FLOAT:
		case OPERAND_DOUBLE:
			popPushEvalStack( lhs.i64Value ^ rhs.toInt64() );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
//			popPushEvalStack( lhs.toDouble() %  rhs.toDouble() );
			popPushEvalStack( lhs.i64Value ^ rhs.toInt64() );
			break;
		}
		break;
	case OPERAND_FLOAT:
		switch( rhs.iOperandType )
		{
		case OPERAND_FLOAT:
		case OPERAND_DOUBLE:
			popPushEvalStack( lhs.toInt() ^ rhs.toInt() );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			popPushEvalStack( lhs.toInt() ^ rhs.toInt() );
			break;
		}
		break;
	case OPERAND_DOUBLE:
		switch( rhs.iOperandType )
		{
		case OPERAND_FLOAT:
		case OPERAND_DOUBLE:
			popPushEvalStack( lhs.toInt() ^ rhs.toInt() );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			popPushEvalStack( lhs.toInt() ^ rhs.toInt() );
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
			{
			int32_t i = lhs.toInt() ^ rhs.toInt();
			popPushEvalStack( i );
			}
			break;
		}
		break;
	case OPERAND_NULL:
		popPushEvalStack( lhs.toInt() ^ rhs.toInt() );
		break;
	default:
		Debug_Fatal( "Illegal operand" );
		break;
	}
}

/***************************************************************************
 *      bitwiseXor operator Helper
 ***************************************************************************/

} //namespace CRI