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
 * File     : CCilVmOperatorShrUn.cpp
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
 *      Shr.Un operator
 *      (1) take value from stack
 *      (2) take value from stack
 *      (3) result(2) >> result (1) (unsigned)
 *      (4) push result (3)
 ***************************************************************************/
void CCilVm::ShrUnOperator()
{
	assert( getEvalStackSize() >= 2 );
	CVariable& shiftCount = getEvalStackFirstEntry();
	CVariable& shiftTarget = getEvalStackSecondEntry();

	union {
		int32_t iValue;
		int64_t i64Value;
	};

	shiftCount.iOperandType = (OPERAND_TYPE)shiftCount.iOperandType;
	shiftTarget.iOperandType = (OPERAND_TYPE)shiftTarget.iOperandType;
	switch( shiftCount.iOperandType )
	{
	case OPERAND_INT:
		switch( shiftTarget.iOperandType )
		{
		case OPERAND_INT:
			{
				uint32_t iTmpValue = shiftTarget.iValue;
				iValue = (int32_t)(iTmpValue >> shiftCount.iValue);
				popEvalStack();
				popEvalStack();
				pushEvalStack( iValue );
			}
			break;
		case OPERAND_INT64:
			{
				uint64_t iTmpValue = shiftTarget.i64Value;
				i64Value = (int64_t)iTmpValue >> shiftCount.iValue;
				popEvalStack();
				popEvalStack();
				pushEvalStack( i64Value );
			}
			break;
		default:
			Debug_Fatal( "Illegal operand" );
			break;
		}
		break;
	default:
		Debug_Fatal( "Illegal operand" );
		break;
	}

}

} //namespace CRI