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
 * Module   : cil Virtual Machine Branch ops
 * File     : CCilVmOperatorBlt.cpp
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
#include "cilVMBranchMacro.h"
namespace cri {

/***************************************************************************
 *      Variables
 ***************************************************************************/
#ifdef OP
	#undef OP
#endif
#define OP <
#define OP2 ==


/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Blt operator
 *      (1) take value from stack
 *      (2) take value from stack
 *      (3) copmare (1) OP (2)
 *      (4) If true, take branch
 ***************************************************************************/
void CCilVm::BnltOperator()
{
	assert( getEvalStackSize() >= 2 );
	CVariable& rhs = getEvalStackFirstEntry();
	CVariable& lhs = getEvalStackSecondEntry();

	bool bTrue = false;

#pragma warning( disable:4311 )	//Suppress warning

	CheckBranch( OP, OP2 );

#pragma warning( default:4311 )	//Enable warning
	if( !bTrue ) 
	{
		int32_t* pOffset = (int32_t*)m_pCurrentInstruction;
		m_pCurrentInstruction += *pOffset + LONGBR_ADJUST;
	}
	else
		m_pCurrentInstruction += LONGBR_ADJUST;

	popEvalStack();
	popEvalStack();
}

/***************************************************************************
 *      BltS operator
 *      (1) take value from stack
 *      (2) take value from stack
 *      (3) copmare (1) OP (2)
 *      (4) If true, take branch
 ***************************************************************************/
void CCilVm::BnltShortOperator()
{
	assert( getEvalStackSize() >= 2 );
	CVariable& rhs = getEvalStackFirstEntry();
	CVariable& lhs = getEvalStackSecondEntry();

	bool bTrue = false;

#pragma warning( disable:4311 )	//Suppress warning

	CheckBranch( OP, OP2 );

#pragma warning( default:4311 )	//Enable warning
	if( !bTrue ) 
	{
		int8_t* pOffset = (int8_t*)m_pCurrentInstruction;
		m_pCurrentInstruction += *pOffset + SHORTBR_ADJUST;
	}
	else
		m_pCurrentInstruction += SHORTBR_ADJUST;

	popEvalStack();
	popEvalStack();
}

/***************************************************************************
 *      Branch operator Helper
 ***************************************************************************/

} //namespace CRI