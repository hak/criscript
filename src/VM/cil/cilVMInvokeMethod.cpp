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
 * Module   : cil, invoke a method with RID
 * File     : cilVMInvokeMethod.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
#include "cilVM.h"
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/
CVariable* CCilVm::invokeMethod( const RID ridMethod, const int32_t iNumArg,
								   CVariable* pArguments )
{
	//Get Method parameters
	CVariable* pThis;
	METHOD_DEF* pMethod;
	int32_t iIndex;

	//NOTE:
	//Currently invokeMethod only support a function invocation in the current execution context.
	//If you need to invoke the function from different context, i.e.Should not destroy current
	//context by Throw etc, need to save/restore current execution context.

	switch( TypeFromToken( ridMethod ) )
	{
	case MDT_METHODDEF:
		pMethod = &m_MetaData.getMethod( ridMethod );
		pThis = NULL;
		break;
	case MDT_FIELDDEF:
		{
		//Retrieve static field value
		iIndex = RidFromToken( ridMethod );
		CVariable& var = m_pStaticFieldTable[ iIndex ];
		assert( OperandType( var.iOperandType ) == OPERAND_OBJECTREF );
		assert( var.refObject != NULL );

		RID ridCall = var.refObject->getCall();
		assert( TypeFromToken( ridCall ) == MDT_METHODDEF );
		pMethod = &m_MetaData.getMethod( ridCall );
		pThis = &var;
		}
		break;
	default:
	//TODO
	//Should throw exception rather than assert
		Debug_Fatal( "Not supported call type" );
	}

	VM_CALLSTACK callstack;
	int32_t evalStackEntries = (int32_t)getEvalStackSize();
	int32_t iCurrentInstructionIndex = (uint32_t)(m_pCurrentInstruction - &m_CurrentILPool.ILPool[0]);
	int32_t iSavedNumGivenArguments = m_iNumGivenArguments;
	
	switch( MethodType( pMethod->MethodType ) )
	{
	case METHOD_NATIVE:
		{
		assert( pMethod->lpFunction != NULL );
		m_iNumGivenArguments = 0;
		((PDEFAULTMETHOD)pMethod->lpFunction)( this,
											iNumArg,
											pArguments );		
		VM_CALLSTACK* pCurrentCallstack = getCurrentCallStack( VM_CALLSTACK_CALLSTACK );
		m_iNumGivenArguments = iSavedNumGivenArguments;
		}
		break;
	case METHOD_WRAPPED:
		{
		assert( pMethod->lpFunction != NULL );
		m_iNumGivenArguments = 0;
		((PWRAPPEDMETHOD)pMethod->lpFunction)( this,
											iNumArg,
											pArguments,
											(PMETHOD)pMethod->lpFunctionBody );		
		VM_CALLSTACK* pCurrentCallstack = getCurrentCallStack( VM_CALLSTACK_CALLSTACK );
		m_iNumGivenArguments = iSavedNumGivenArguments;
		}
		break;
	case METHOD_CIL:
		{

		//Copy arguments over arglist
		for( int32_t i = 0; i < iNumArg; ++i )
		{
			storeArgumentList( *pArguments );
			++pArguments;
		}

		if( !pMethod->bRestArgments )
		{
			//Regular pass

			//Fill in arguments if not supplied enough
			if( iNumArg < (int32_t)pMethod->iNumArgments )
			{
				for( int32_t i = 0; i < pMethod->iNumArgments - iNumArg; i ++ )
					pushArgumentListUndefined();
			} else if( iNumArg > (int32_t)pMethod->iNumArgments )
			{
				for( int32_t i = 0; i <  iNumArg - pMethod->iNumArgments; i ++ )
					popArgumentList();
			}
		}
		else
		{
			//Need to generate restArgs
			//Fill in arguments if not supplied enough
			if( m_iNumGivenArguments < pMethod->iNumArgments )
			{
				for( int32_t i = 0; i < pMethod->iNumArgments - m_iNumGivenArguments; i ++ )
					pushArgumentListUndefined();
			} else
			{
				generateRestArguments( m_iNumGivenArguments - pMethod->iNumArgments + 1 ); 
			}
		}

		callstack.type		= VM_CALLSTACK_CALLSTACK;
		callstack.status	= VM_EHCLAUSE_NORMAL;
		callstack.pInstruction = m_pCurrentInstruction;
		callstack.ridMethod = ridMethod;
		callstack.iLocalVariableStartIndex = getCurrentLocalVariableIndex();
		callstack.iArgumentListStartIndex = getCurrentArgumentListIndex() - pMethod->iNumArgments;
		callstack.iNumArgument = pMethod->iNumArgments;
		callstack.bConstructor = false;
		m_CallStack.push_back( callstack );

		uint8_t* pSavedInstruction = m_pCurrentInstruction;
		m_pCurrentInstruction = &m_CurrentILPool.ILPool[ pMethod->iILOffset ];
		m_iCurrentThisPointerIndex = callstack.iArgumentListStartIndex;

		//Note
		//Arguments are already in the arglist in the last call stack

		//Reserve local variable slot
		reserveLocalVariables( pMethod->ridLocalVariableList );

		//Change callstack root
		m_iCallStackRoot = (int32_t)m_CallStack.size();

		//Keep current status
		VM_STATUS_TYPE status = m_status;
		
		execute();
		
		//Restore call stack root
		m_iCallStackRoot = 1;
		
		//restore execution status
		m_status = status;
		
		//Dispose local variable
		disposeLocalVariables( pMethod->iNumLocals );
		//Dispose arglist
		disposeArgumentList( pMethod->iNumArgments );

		m_CallStack.pop_back();

		//Restore IP
		m_pCurrentInstruction = pSavedInstruction;
		m_iNumGivenArguments = iSavedNumGivenArguments;

		}
		break;
	case METHOD_EHCLAUSE:
	default:
		Debug_Fatal( "Illegal Method type" );
		break;
	}

	CVariable* pVar = new( CVariable );
	evalStackEntries = int32_t( getEvalStackSize() ) - evalStackEntries;
	if( evalStackEntries > 0 )
	{
		getEvalStackFirstEntry().moveTo( *pVar );
		popEvalStackFast();
	}
	return pVar;
}


} //namespace CRI