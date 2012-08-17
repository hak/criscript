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
 * File     : cilVMArgumentList.cpp
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
 *      Store from top of the stack to a Argument List
 ***************************************************************************/
void CCilVm::storeArgument( const int32_t iIndex )
{
	VM_CALLSTACK* stack = getCurrentCallStack( VM_CALLSTACK_CALLSTACK );	//Arglist resides in the last callstack
	assert( stack->iArgumentListStartIndex + iIndex <= int32_t(m_iArgListIndex) );

	if( m_ArgList[ stack->iArgumentListStartIndex + iIndex ].getFlag() & OPERAND_FLAG_READONLY )
	{
		//Read only variable
		popEvalStack();
		return;
	}

	if( m_ArgList[ stack->iArgumentListStartIndex + iIndex ].getFlag() & OPERAND_FLAG_STRICT )
	{
		//Test type constraint
		storeAsRestrictedType( m_ArgList[ stack->iArgumentListStartIndex + iIndex ]  );
		popEvalStack();
		return;
	}

	getEvalStackFirstEntry().moveTo( m_ArgList[ stack->iArgumentListStartIndex + iIndex ] );
	popEvalStackFast();
}

/***************************************************************************
 *      Load from a Argument List to Top of the stack
 ***************************************************************************/
void CCilVm::loadArgument( const int32_t iIndex )
{
	VM_CALLSTACK* stack = getCurrentCallStack( VM_CALLSTACK_CALLSTACK );	//Arglist resides in the last callstack
	assert( stack->iArgumentListStartIndex + iIndex <= int32_t(m_iArgListIndex) );
	pushEvalStack( m_ArgList[ stack->iArgumentListStartIndex + iIndex ] );
}

/***************************************************************************
 *      Push undefined to the arglist
 ***************************************************************************/
void CCilVm::pushArgumentListUndefined()
{
	assert( m_iArgListIndex < MAX_ARG_VARIABLE );
	CVariable operand;
	operand.iOperandType = OPERAND_UNDEFINED;
	operand.iOperandFlag = OPERAND_FLAG_NONE;
	operand.dValue = NULL;
	m_ArgList[ m_iArgListIndex ] = operand;
	updateArgListIndex();
}

/***************************************************************************
 *      Pop undefined to the arglist
 ***************************************************************************/
void CCilVm::popArgumentList()
{
	assert( m_iArgListIndex >= 1 );
#ifdef VM_TRACK_ERASEWHENPOP
	m_ArgList[ m_iArgListIndex ].erase();
#endif
	m_iArgListIndex--;
	//Note that it does not destroy entry
}

/***************************************************************************
 *      get Argument Value (Debugger helper)
 ***************************************************************************/
CVariable& CCilVm::getArgument( const int32_t iIndex )
{
	VM_CALLSTACK* stack = getCurrentCallStack( VM_CALLSTACK_CALLSTACK );	//Arglist resides in the last callstack
	assert( stack->iArgumentListStartIndex + iIndex <= (int32_t)m_iArgListIndex );
	return m_ArgList[ stack->iArgumentListStartIndex + iIndex ];
}

/***************************************************************************
 *      Set Argument Value (Debugger helper)
 ***************************************************************************/
bool CCilVm::setArgument( const int32_t iIndex, const CVariable& value )
{
	VM_CALLSTACK* stack = getCurrentCallStack( VM_CALLSTACK_CALLSTACK );	//Arglist resides in the last callstack
	assert( stack->iArgumentListStartIndex + iIndex <= (int32_t)m_iArgListIndex );
	m_ArgList[ stack->iArgumentListStartIndex + iIndex ] = value;
	return true;
}

/***************************************************************************
 *      VM local variable management
 *		store args from eval stack
 ***************************************************************************/
void CCilVm::storeArgumentListWithThisPointer( const uint32_t iArgsToStore )
{
	assert( m_iArgListIndex < MAX_ARG_VARIABLE );
	assert( getEvalStackSize() >= iArgsToStore );

	//Add param list
	CVariable variable;
	//Reserve this pointer entry
	storeArgumentList( variable );
	
	for( uint32_t i = 0; i < iArgsToStore; i++ )
	{
		//Note that here, the order of the arg list is swapped
		getEvalStackFirstEntry().moveTo( m_ArgList[ m_iArgListIndex ] );
		updateArgListIndex();
		popEvalStackFast();
	}
}

void CCilVm::storeArgumentListWithoutThisPointer( const uint32_t iArgsToStore )
{
	assert( m_iArgListIndex < MAX_ARG_VARIABLE );
	assert( getEvalStackSize() >= iArgsToStore );

	//Add param list
	//Reserve this pointer entry
	storeArgumentList( m_ArgList[ m_iCurrentThisPointerIndex ] ); 

	CVariable var;
	for( uint32_t i = 0; i < iArgsToStore; i++ )
	{
		//Note that here, the order of the arg list is swapped
		getEvalStackFirstEntry().moveTo( m_ArgList[ m_iArgListIndex ] );
		updateArgListIndex();
		popEvalStackFast();
	}
}

void CCilVm::storeArgumentList( const CVariable& var )
{
	assert( m_iArgListIndex < MAX_ARG_VARIABLE );
	m_ArgList[ m_iArgListIndex ] = var; 
	updateArgListIndex();
}

/***************************************************************************
 *      VM arg list management
 *		dispose arg list slot for future use
 ***************************************************************************/
void CCilVm::disposeArgumentList( const uint32_t iArgsToDispose )
{
	assert( iArgsToDispose <= m_iArgListIndex );

#ifdef VM_TRACK_ERASEWHENPOP
	for( uint32_t i = 0; i < iArgsToDispose; ++i )
		m_ArgList[ m_iArgListIndex-- ].erase();
#else
	m_iArgListIndex -= iArgsToDispose;
#endif

	//the Argument list should keep at least 1 entry for the this pointer
	assert( m_iArgListIndex >= 1 );
}

/***************************************************************************
 *      VM arg list management
 *		dispose arg list slot for future use
 ***************************************************************************/
void CCilVm::disposeArgumentList()
{
	assert( m_iNumGivenArguments <= (int32_t)m_iArgListIndex );

#ifdef VM_TRACK_ERASEWHENPOP
	for( int32_t i = 0; i < m_iNumGivenArguments; ++i )
		m_ArgList[ --m_iArgListIndex ].erase();
#else
	m_iArgListIndex -= m_iNumGivenArguments;
#endif
	m_iNumGivenArguments = 0;

	//the Argument list should keep at least 1 entry for the this pointer
	assert( m_iArgListIndex >= 1 );
}

/***************************************************************************
 *      VM arg list management
 *		reset argument list count
 ***************************************************************************/
void CCilVm::resetArgumentList()
{
	m_iNumGivenArguments = 0;
}

/***************************************************************************
 *      VM arg list management
 *		generateArgumentsProperty
 ***************************************************************************/
CVariable* CCilVm::generateArgumentsProperty()
{
	wchar_t strBuffer[ 22 ];		//22=max value width of longlong
	VM_CALLSTACK* pStack = getCurrentCallStack( VM_CALLSTACK_CALLSTACK );
	int32_t iNumArguments = min( pStack->iNumGivenArgument, pStack->iNumArgument );

	//
	//Need to return a value even no arguments are given
	
	//the object looks like indicading current executing context
	CVmObject* pObj = createObject( m_ridArrayObject );
	CVariable obj( pObj, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
									| OPERAND_FLAG_DONTENUM
									| OPERAND_FLAG_READONLY ) );

	pObj->addProperty( STRING_INTERNAL_LENGTH, (int32_t)iNumArguments - 1 );
	for( int32_t i = 0; i < (int32_t)iNumArguments - 1; ++i )
	{
		//Argument: 1) value of the property
		swprintf( strBuffer, 22, L"%d", i );
		//Does not store 'this' value
		pObj->setProperty( strBuffer, m_ArgList[ pStack->iArgumentListStartIndex + i + 1 ] );
	}

	//Set callee property
	VMOBJECTREF pCallee = createObject( m_ridFunctionObject );
	pCallee->setCallProperty( pStack->ridMethod );
	
	CVariable varPointer( pCallee );
	pObj->setProperty( NAME_BUILTIN_FUNCTION_PROPERTY_CALLEE, varPointer );

	//Set caller proeperty
	//Get 2nd level call stack
	pStack = getCurrentCallStack( VM_CALLSTACK_CALLSTACK, 2 );
	if( pStack != NULL )
	{
		iNumArguments = min( pStack->iNumGivenArgument, pStack->iNumArgument );

		//
		//Need to return a value even no arguments are given
		
		//the object looks like indicading current executing context
		CVmObject* pObjCaller = createObject( m_ridArrayObject );
		CVariable obj( pObjCaller, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
										| OPERAND_FLAG_DONTENUM
										| OPERAND_FLAG_READONLY ) );

		pObjCaller->addProperty( STRING_INTERNAL_LENGTH, (int32_t)iNumArguments - 1 );
		for( int32_t i = 0; i < (int32_t)iNumArguments - 1; ++i )
		{
			//Argument: 1) value of the property
			swprintf( strBuffer, 22, L"%d", i );
			//Does not store 'this' value
			pObjCaller->setProperty( strBuffer, m_ArgList[ pStack->iArgumentListStartIndex + i + 1 ] );
		}

		//Set callee property
		VMOBJECTREF pCallee = createObject( m_ridFunctionObject );
		pCallee->setCallProperty( pStack->ridMethod );
		CVariable varPointer( pCallee );
		pObjCaller->setProperty( NAME_BUILTIN_FUNCTION_PROPERTY_CALLEE, varPointer );

		CVariable varPointerObj( pObjCaller );
		pObj->setProperty( NAME_BUILTIN_FUNCTION_PROPERTY_CALLER, varPointerObj );
	}


	setStaticField( RID_ARGUMENTS_TMP, obj, true );

	return &getStaticField( RID_ARGUMENTS_TMP );
}

/***************************************************************************
 *      VM arg list management
 *		generateRestArguments
 ***************************************************************************/
void CCilVm::generateRestArguments( int32_t iNumArgsToStore )
{
	wchar_t strBuffer[ 22 ];		//22=max value width of longlong
	assert( m_iArgListIndex >= uint32_t(iNumArgsToStore) );
	//
	//Need to return a value even no arguments are given
	
	//the object looks like indicading current executing context
	CVmObject* pObj = createObject( m_ridArrayObject );
	CVariable obj( pObj, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
									| OPERAND_FLAG_DONTENUM
									| OPERAND_FLAG_READONLY ) );

	pObj->addProperty( STRING_INTERNAL_LENGTH, (int32_t)iNumArgsToStore );
	if( iNumArgsToStore )
		for( int32_t i = iNumArgsToStore - 1; i >= 0; --i )
		{
			//Argument: 1) value of the property
			swprintf( strBuffer, 22, L"%d", i );
			//Does not store 'this' value
			pObj->setProperty( strBuffer, m_ArgList[ m_iArgListIndex - 1 ] );
			popArgumentList();
		}
	else
		popArgumentList();	//Just pop_back and push array

	m_ArgList[ m_iArgListIndex ] = obj;
	m_ArgList[ m_iArgListIndex ].iOperandFlag = OPERAND_FLAG_NONE;
	updateArgListIndex();
	return;
}

/***************************************************************************
 *      VM arg list management
 *		generateCallerProperty
 ***************************************************************************/
CVariable* CCilVm::generateCallerProperty()
{
	VM_CALLSTACK* pStack = getCurrentCallStack( VM_CALLSTACK_CALLSTACK, 2);
	//
	//Need to return a value even no arguments are given
	
	//the object looks like indicading current executing context

	//Set caller proeperty
	if( pStack != NULL )
	{
		VMOBJECTREF pCaller = createObject( m_ridFunctionObject );
		CVariable obj( pCaller, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
										| OPERAND_FLAG_DONTENUM
										| OPERAND_FLAG_READONLY ) );
		setStaticField( RID_ARGUMENTS_TMP, obj, true );
	}
	else
		setStaticField( RID_ARGUMENTS_TMP, CVariable(), true );


	return &getStaticField( RID_ARGUMENTS_TMP );
}

} //namespace CRI