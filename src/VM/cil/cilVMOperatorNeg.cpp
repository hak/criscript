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
 * File     : CCilVmOperatorNeg.cpp
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
 *      Neg operator
 *      (1) take value from stack
 *      (2) -result(1)
 *      (3) push result (2)
 ***************************************************************************/
void CCilVm::negOperator()
{
	assert( getEvalStackSize() >= 1 );
	CVariable& lhs = getEvalStackFirstEntry();

	lhs.convertToPrimitiveType( this );

	switch( lhs.iOperandType )
	{
	case OPERAND_INT:
		lhs.iValue = -lhs.iValue;
		break;
	case OPERAND_INT64:
		lhs.i64Value = -lhs.i64Value;
		break;
	case OPERAND_FLOAT:
		lhs.fValue = -lhs.fValue;
		break;
	case OPERAND_DOUBLE:
		lhs.dValue = -lhs.dValue;
		break;
	case OPERAND_BOOLEAN:
		lhs.iOperandType = OPERAND_INT;
		lhs.iValue = -lhs.iValue;
		break;
	case OPERAND_OBJECTREF:
		{
		OPERAND_TYPE type = lhs.getPrimitiveType( PRIMITIVETYPE_NUMBER );
		CVariable& var = lhs.refObject->getValue();
		switch( type )
		{
		case OPERAND_STRING:
			CNumberObject::parseNumber( var, lhs.refObject->getValue().pString );
			lhs.refObject->setValue( var );
			break;
		case OPERAND_BOOLEAN:
			var.iOperandType = OPERAND_INT;
			var.iValue = (var.bValue == true);
			break;
		case OPERAND_INT:
		case OPERAND_UNSIGNEDINT:
			var.iValue = -var.iValue;
			break;
		case OPERAND_INT64:
		case OPERAND_UNSIGNEDINT64:
			var.i64Value = -var.i64Value;
			break;
		case OPERAND_FLOAT:
			var.fValue = -var.fValue;
			break;
		case OPERAND_DOUBLE:
			var.dValue = -var.dValue;
			break;
		case OPERAND_OBJECTREF:
		case OPERAND_UNDEFINED:
		case OPERAND_NULL:
		case OPERAND_NAN:
			//Do nothing
			break;
		default:
			Debug_Fatal( "Not implemented yet" );
		}
		break;
		}
	case OPERAND_STRING:
		{
			bool b = CNumberObject::parseNumber( lhs, lhs.pString );
			if( !b )
				pushEvalStackNaN();
			else
			{
				switch( lhs.iOperandType )
				{
				case OPERAND_NULL:
				case OPERAND_NAN:
				case OPERAND_UNDEFINED:
					//Do nothing
					break;
				case OPERAND_INT:
					lhs.iValue = -lhs.iValue;
					break;
				case OPERAND_INT64:
					lhs.i64Value = -lhs.i64Value;
					break;
				case OPERAND_FLOAT:
					lhs.fValue = -lhs.fValue;
					break;
				case OPERAND_DOUBLE:
					lhs.dValue = -lhs.dValue;
					break;
				case OPERAND_BOOLEAN:
					lhs.iValue = -lhs.iValue;
					break;
				}
			}
		}
	case OPERAND_NULL:
	case OPERAND_NAN:
	case OPERAND_UNDEFINED:
		//Do nothing
		break;
	default:
		popEvalStack();
		pushEvalStackNaN();
		break;
	}

}

/***************************************************************************
 *      Neg operator Helper
 ***************************************************************************/

} //namespace CRI