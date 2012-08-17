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
 * Module   : cil Virtual Machine
 * File     : cilVMLocalVariable.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criscript.h"
#include "cilVm.h"
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Store from top of the stack to a Local field
 ***************************************************************************/
void CCilVm::storeLocalVariable( const int32_t iIndex )
{
	VM_CALLSTACK* stack = getCurrentCallStack( VM_CALLSTACK_CALLSTACK );
	assert( stack->iLocalVariableStartIndex + iIndex <= (int32_t)m_iLocalVariableListIndex );

	OPERAND_FLAG flag =  m_LocalVariableList[ stack->iLocalVariableStartIndex + iIndex ].getFlag();
	if( flag & OPERAND_FLAG_READONLY )
	{
		//Read only variable
		popEvalStack();
		return;
	}

	if( flag & OPERAND_FLAG_STRICT )
	{
		//Test type constraint
		storeAsRestrictedType( m_LocalVariableList[ stack->iLocalVariableStartIndex + iIndex ]  );
		popEvalStack();
		return;
	}

	getEvalStackFirstEntry().moveTo( m_LocalVariableList[ stack->iLocalVariableStartIndex + iIndex ] );
	popEvalStackFast();
}

/***************************************************************************
 *      Load from a Local field to Top of the stack
 ***************************************************************************/
void CCilVm::loadLocalVariable( const int32_t iIndex )
{
	VM_CALLSTACK* stack = getCurrentCallStack( VM_CALLSTACK_CALLSTACK );
	assert( stack->iLocalVariableStartIndex + iIndex <= (int32_t)m_iLocalVariableListIndex );

	pushEvalStack( m_LocalVariableList[ stack->iLocalVariableStartIndex + iIndex ] );
}


/***************************************************************************
 *      get LocalVariable Value (Debugger helper)
 ***************************************************************************/
CVariable& CCilVm::getLocalVariable( const int32_t iIndex )
{
	assert( iIndex < MAX_LOCAL_VARIABLE );

	VM_CALLSTACK* stack = getCurrentCallStack( VM_CALLSTACK_CALLSTACK );
	assert( stack->iLocalVariableStartIndex + iIndex <= (int32_t)m_iLocalVariableListIndex );
	return m_LocalVariableList[ stack->iLocalVariableStartIndex + iIndex ];
}

/***************************************************************************
 *      Set LocalVariable Value (Debugger helper)
 ***************************************************************************/
bool CCilVm::setLocalVariable( const int32_t iIndex, const CVariable& value )
{
	assert( iIndex < MAX_LOCAL_VARIABLE );

	VM_CALLSTACK* stack = getCurrentCallStack( VM_CALLSTACK_CALLSTACK );
	assert( stack->iLocalVariableStartIndex + iIndex <= (int32_t)m_iLocalVariableListIndex );
	m_LocalVariableList[ stack->iLocalVariableStartIndex + iIndex ] = value;
	return true;
}

/***************************************************************************
 *      VM local variable management
 *		reserve local variable slot for future use
 ***************************************************************************/
void CCilVm::reserveLocalVariables( const RID ridLocalsToReserve )
{
	assert( TypeFromToken( ridLocalsToReserve ) == MDT_PARAMDEF );
	vector< OPERAND_TYPE_FLAG >* pParamList;
	int32_t iIndex;
	if( isBuiltinRid( ridLocalsToReserve ) )
	{
		//Built in methods
		iIndex = IndexFromBuiltinRid( ridLocalsToReserve );
		pParamList = &m_MetaData.getBuiltinParamListTable()[ iIndex ];

	}
	else
	{
		iIndex = RidFromToken( ridLocalsToReserve );
		pParamList = &m_MetaData.getParamListTable()[ iIndex ];
	}

	//Add param list
	uint32_t iSize = (uint32_t)pParamList->size();
	assert( m_iLocalVariableListIndex + iSize < MAX_LOCAL_VARIABLE );

	CVariable variable;
	for( uint32_t i = 0; i < iSize; i++ )
	{
		RID ridConstraint =  (*pParamList)[ i ].ridConstraintType;
		if( ridConstraint )
		{
			//With Object type constraint
			VMOBJECTREF obj = createObject( MDT_TYPEDEF | ridConstraint );
			variable.setObjectRefWithFlags( obj,
				(*pParamList)[ i ].flag,
				MDT_TYPEDEF | ridConstraint );
		}
		else
		{
			variable.iOperandType = (*pParamList)[ i ].type;
			variable.iOperandFlag = (*pParamList)[ i ].flag;
			variable.ridConstraintType = 0;
		}

		m_LocalVariableList[ m_iLocalVariableListIndex ] = variable; 
		updateLocalVariableListIndex();
	}
}

/***************************************************************************
 *      VM local variable management
 *		dispose local variable slot for future use
 ***************************************************************************/
void CCilVm::disposeLocalVariables( const uint32_t iLocalsToDispose )
{
	assert( iLocalsToDispose <= m_iLocalVariableListIndex );

#ifdef VM_TRACK_ERASEWHENPOP
	for( uint32_t i = 0; i < iLocalsToDispose; ++i )
		m_LocalVariableList[ --m_iLocalVariableListIndex ].erase();
#else
	m_iLocalVariableListIndex -= iLocalsToDispose;
#endif
}
	
} //namespace CRI