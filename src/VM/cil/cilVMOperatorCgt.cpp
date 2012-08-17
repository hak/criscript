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
 * File     : CCilVmOperatorCgt.cpp
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
#define OP >
#define OP2 !=


/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Cgt operator
 *      (1) take value from stack
 *      (2) take value from stack
 *      (3) copmare (1) OP (2)
 *      (4) If true, take branch
 ***************************************************************************/
void CCilVm::CgtOperator()
{
	assert( getEvalStackSize() >= 2 );
	CVariable& rhs = getEvalStackFirstEntry();
	CVariable& lhs = getEvalStackSecondEntry();
	bool bTrue = false;

#pragma warning( disable:4311 )	//Suppress warning

	CheckBranch( OP, OP2 );

#pragma warning( default:4311 )	//Enable warning

	popEvalStack();
	popEvalStack();

	pushEvalStack( bTrue );
}

} //namespace CRI