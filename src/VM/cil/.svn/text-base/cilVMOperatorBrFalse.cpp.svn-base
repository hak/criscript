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
 * File     : CCilVmOperatorBrTrue.cpp
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
 *      Branch False operator
 *      (1) take value from stack
 *      (2) If (1) == 0, take branch
 ***************************************************************************/
void CCilVm::BrFalseOperator()
{
	assert( getEvalStackSize() >= 1 );
	CVariable& lhs = getEvalStackFirstEntry();
	bool bTakeBranch;

	int32_t iOperandType = OperandType( lhs.iOperandType );
	switch( iOperandType )
	{
	case OPERAND_INT:
		bTakeBranch = ( lhs.iValue == 0 );
		break;
	case OPERAND_INT64:
		bTakeBranch = ( lhs.i64Value == 0 );
		break;
	case OPERAND_FLOAT:
		bTakeBranch = ( lhs.fValue == 0 );
		break;
	case OPERAND_DOUBLE:
		bTakeBranch = ( lhs.dValue == 0 );
		break;
	case OPERAND_STRING:
		bTakeBranch = ( lhs.pString->length() == 0 );
		break;
	case OPERAND_OBJECTREF:
		bTakeBranch = ( lhs.refObject == NULL );
		break;
	case OPERAND_UNDEFINED:
	case OPERAND_NAN:
	case OPERAND_NULL:
		bTakeBranch = true;
		break;
	case OPERAND_BOOLEAN:
		bTakeBranch = ( lhs.bValue == false );
		break;
	default:
		Debug_Fatal( "Illegal operand" );
		break;
	}
	if( bTakeBranch ) 
	{
		int32_t* pOffset = (int32_t*)m_pCurrentInstruction;
		m_pCurrentInstruction += *pOffset + LONGBR_ADJUST;
	}
	else
		m_pCurrentInstruction += LONGBR_ADJUST;

	popEvalStack();
}

/***************************************************************************
 *      Branch False operator
 *      (1) take value from stack
 *      (2) If (1) == 0, take branch
 ***************************************************************************/
void CCilVm::BrFalseShortOperator()
{
	assert( getEvalStackSize() >= 1 );
	CVariable& lhs = getEvalStackFirstEntry();
	bool bTakeBranch;

	int32_t iOperandType = OperandType( lhs.iOperandType );
	switch( iOperandType )
	{
	case OPERAND_INT:
		bTakeBranch = ( lhs.iValue == 0 );
		break;
	case OPERAND_INT64:
		bTakeBranch = ( lhs.i64Value == 0 );
		break;
	case OPERAND_FLOAT:
		bTakeBranch = ( lhs.fValue == 0 );
		break;
	case OPERAND_DOUBLE:
		bTakeBranch = ( lhs.dValue == 0 );
		break;
	case OPERAND_STRING:
		bTakeBranch = ( lhs.pString->length() == 0 );
		break;
	case OPERAND_OBJECTREF:
		bTakeBranch = ( lhs.refObject == NULL );
		break;
	case OPERAND_UNDEFINED:
	case OPERAND_NULL:
	case OPERAND_NAN:
		bTakeBranch = true;
		break;
	case OPERAND_BOOLEAN:
		bTakeBranch = ( lhs.bValue == false );
		break;
	default:
		Debug_Fatal( "Illegal operand" );
		break;
	}
	if( bTakeBranch ) 
	{
		int8_t* pOffset = (int8_t*)m_pCurrentInstruction;
		m_pCurrentInstruction += *pOffset + SHORTBR_ADJUST;
	}
	else
		m_pCurrentInstruction += SHORTBR_ADJUST;

	popEvalStack();
}

} //namespace CRI