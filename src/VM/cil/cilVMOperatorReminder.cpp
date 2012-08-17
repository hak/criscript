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
 * File     : CCilVmOperatorReminder.cpp
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
 *      Reminder operator
 *      (1) take value from stack
 *      (2) take value from stack
 *      (3) result(1) % result (2)
 *      (4) push result (3)
 ***************************************************************************/
void CCilVm::reminderOperator()
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

			if( !fLhs )
			{
				popPushEvalStack( 0 );
				return;
			} else if( !fRhs )
			{
				popPushEvalStackInfinity();
				return;
			}
			popPushEvalStack( (float)fmod( fLhs, fRhs ) );
		}
		else if( lhsType == OPERAND_DOUBLE || rhsType == OPERAND_DOUBLE )
		{
			popPushEvalStack( fmod( lhs.toDouble(), rhs.toDouble() ) );
		}
		else if( lhsType == OPERAND_FLOAT || rhsType == OPERAND_FLOAT )
		{
			popPushEvalStack( (float)fmod( lhs.toFloat(), rhs.toFloat() ) );
		}
		else if( lhsType == OPERAND_INT64 || rhsType == OPERAND_INT64 )
		{
			int64_t i64 = rhs.toInt64();
			if( !i64 )
			{
				popPushEvalStackInfinity();
				return;
			}
			popPushEvalStack( lhs.toInt64() %  i64 );
		}
		else if( lhsType == OPERAND_INT || rhsType == OPERAND_INT )
		{
			int32_t i = rhs.toInt();
			if( !i )
			{
				popPushEvalStackInfinity();
				return;
			}
			popPushEvalStack( lhs.toInt() %  i );
		}
		else if( lhsType == OPERAND_BOOLEAN || rhsType == OPERAND_BOOLEAN )
		{
			int32_t i = rhs.toInt();
			if( !i )
			{
				popPushEvalStackInfinity();
				return;
			}
			popPushEvalStack( lhs.toInt() % rhs.toInt() );
		}
		else
		{
			Debug_Fatal( "Not implemented yet" );
		}
		return;
	} else if( lhs.iOperandType == OPERAND_STRING
		|| rhs.iOperandType == OPERAND_STRING )
	{
		popPushEvalStack( (float)fmod( lhs.toFloat(), rhs.toFloat() ) );
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
	
	//
	// Check divided by 0 case
	//
	if( !lhs.i64Value )
	{
		popEvalStack();
		popEvalStack();
		pushEvalStack( 0 );
		return;
	} else if( !rhs.i64Value )
	{
		popEvalStack();
		popEvalStack();
		pushEvalStackInfinity();
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
			if( !i )
			{
				popPushEvalStackInfinity();
				return;
			}
			popPushEvalStack( int32_t( lhs.iValue % i ) );
			break;
		case OPERAND_INT64:
			i64 = rhs.toInt64();
			if( !i64 )
			{
				popPushEvalStackInfinity();
				return;
			}
			popPushEvalStack( int64_t( int64_t(lhs.iValue) % i64 ) );
			break;
		case OPERAND_FLOAT:
			if( !_finite( rhs.fValue ) )
				popPushEvalStack( lhs.iValue );
			else
				popPushEvalStack( (float)fmod( float( lhs.iValue ), rhs.fValue ) );
			break;
		case OPERAND_DOUBLE:
			if( !_finite( rhs.dValue ) )
				popPushEvalStack( lhs.iValue );
			else
				popPushEvalStack( fmod( double( lhs.iValue ), rhs.dValue ) );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			i = rhs.toInt();
			if( !i )
			{
				popPushEvalStackInfinity();
				return;
			}
			popPushEvalStack( int32_t( lhs.iValue %  i ) );
			break;
		}
		break;
	case OPERAND_INT64:
		switch( rhs.iOperandType )
		{
		case OPERAND_INT:
		case OPERAND_INT64:
			i64 = rhs.toInt64();
			if( !i64 )
			{
				popPushEvalStackInfinity();
				return;
			}
			popPushEvalStack( lhs.i64Value % i64 );
			break;
		case OPERAND_FLOAT:
			if( !_finite( rhs.fValue ) )
				popPushEvalStack( lhs.i64Value );
			else
				popPushEvalStack( (float)fmod( float( lhs.i64Value ), rhs.fValue ) );
			break;
		case OPERAND_DOUBLE:
			if( !_finite( rhs.dValue ) )
				popPushEvalStack( lhs.i64Value );
			else
				popPushEvalStack( fmod( double( lhs.i64Value ), rhs.dValue ) );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			popPushEvalStack( lhs.i64Value % rhs.toInt64() );
			break;
		}
		break;
	case OPERAND_FLOAT:
		switch( rhs.iOperandType )
		{
		case OPERAND_FLOAT:
			if( !_finite( rhs.fValue ) )
				popPushEvalStack( lhs.fValue );
			else
				popPushEvalStack( (float)fmod( lhs.fValue, rhs.fValue ) );
			break;
		case OPERAND_DOUBLE:
			if( !_finite( rhs.dValue ) )
				popPushEvalStack( lhs.fValue );
			else
				popPushEvalStack( fmod( double(lhs.fValue), rhs.dValue ) );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			{
			float f = rhs.toFloat();
			if( !_finite( f ) )
				popPushEvalStack( lhs.fValue );
			else
				popPushEvalStack( (float)fmod( lhs.fValue, f ) );
			}
			break;
		}
		break;
	case OPERAND_DOUBLE:
		switch( rhs.iOperandType )
		{
		case OPERAND_FLOAT:
			if( !_finite( rhs.fValue ) )
				popPushEvalStack( lhs.dValue );
			else
				popPushEvalStack( fmod( lhs.dValue, double(rhs.fValue) ) );
			break;
		case OPERAND_DOUBLE:
			if( !_finite( rhs.dValue ) )
				popPushEvalStack( lhs.dValue );
			else
				popPushEvalStack( fmod( lhs.dValue, rhs.dValue ) );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NAN:
		case OPERAND_STRING:
		case OPERAND_OBJECTREF:
			Debug_Fatal( "Illegal operand" );
			break;
		default:
			{
			double d = rhs.toDouble();
			if( !_finite( rhs.fValue ) )
				popPushEvalStack( lhs.dValue );
			else
				popPushEvalStack( fmod( lhs.dValue, d ) );
			break;
			}
		}
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
			float f = rhs.toFloat();
			if( !_finite( f ) )
				popPushEvalStack( bool( lhs.toFloat() != 0 ) );
			else
				popPushEvalStack( bool( fmod(lhs.toFloat(), f ) != 0) );
			}
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
 *      Reminder operator Helper
 ***************************************************************************/

} //namespace CRI