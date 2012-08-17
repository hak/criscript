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
 * File     : CCilVmEvalStack.cpp
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

//TODO
//Those eval stack manipurations need to be refactored as a
//Write allocate, pointer based implementation

/***************************************************************************
 *      pushEvalStack
 ***************************************************************************/
void CCilVm::pushEvalStack( const CVariable& operand )
{
	assert( m_pCurrentEvalStack < &m_OperandEvalStack[ MAX_EVALSTACK - 1 ] );
	CVariable& var = *m_pCurrentEvalStack;

#ifdef VM_TRACK_ERASEWHENPOP
	var.copyFast( operand );
//	var.iOperandFlag = OPERAND_FLAG_NONE;
#else
	var = operand;
	var.iOperandFlag = OPERAND_FLAG_NONE;
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      pushEvalStackString
 ***************************************************************************/
void CCilVm::pushEvalStackString( const wchar_t* pStr )
{
	assert( m_pCurrentEvalStack < &m_OperandEvalStack[ MAX_EVALSTACK - 1 ] );
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.iPrameters = OPERAND_STRING;
	var.pString = new wstring( pStr );
#else
	var = CVariable( pStr );
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      pushEvalStackString( wstring * version )
 ***************************************************************************/
void CCilVm::pushEvalStackString( const wstring* str )
{
	assert( m_pCurrentEvalStack < &m_OperandEvalStack[ MAX_EVALSTACK - 1 ] );
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.iPrameters = OPERAND_STRING;
	var.pString = new wstring( *str );
#else
	CVariable operand;
	var = operand;
	var.iOperandType = OPERAND_STRING;
	var.pString = new wstring( *str );
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      pushEvalStackStringPointer
 ***************************************************************************/
void CCilVm::pushEvalStackStringPointer( wstring* pstr )
{
	assert( m_pCurrentEvalStack < &m_OperandEvalStack[ MAX_EVALSTACK - 1 ] );
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.iPrameters = OPERAND_STRING;
	var.pString = pstr;
#else
	CVariable operand;
	var = operand;
	var.iOperandType = OPERAND_STRING;
	var.pString = pstr;
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      pushEvalStack( int32_t version )
 ***************************************************************************/
void CCilVm::pushEvalStack( const int32_t i )
{
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.iPrameters = OPERAND_INT;
	var.iValue = i;
#else
	var = CVariable( i );
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      pushEvalStack( int64_t version )
 ***************************************************************************/
void CCilVm::pushEvalStack( const int64_t i )
{
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.iPrameters = OPERAND_INT64;
	var.i64Value = i;
#else
	var = CVariable( i );
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      pushEvalStack ( boolean )
 ***************************************************************************/
void CCilVm::pushEvalStack( const bool b )
{
	assert( m_pCurrentEvalStack < &m_OperandEvalStack[ MAX_EVALSTACK - 1 ] );
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.iPrameters = OPERAND_BOOLEAN;
	var.bValue = b;
#else
	var = CVariable( b );
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      pushEvalStack( float )
 ***************************************************************************/
void CCilVm::pushEvalStack( const float f )
{
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.iPrameters = OPERAND_FLOAT;
	var.fValue = f;
#else
	var = CVariable( f );
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      pushEvalStack( double )
 ***************************************************************************/
void CCilVm::pushEvalStack( const double d )
{
	assert( m_pCurrentEvalStack < &m_OperandEvalStack[ MAX_EVALSTACK - 1 ] );
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.iPrameters = OPERAND_DOUBLE;
	var.dValue = d;
#else
	var = CVariable( d );
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      pushEvalStackNull
 ***************************************************************************/
void CCilVm::pushEvalStackNull()
{
	assert( m_pCurrentEvalStack < &m_OperandEvalStack[ MAX_EVALSTACK - 1 ] );
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.iPrameters = OPERAND_NULL;
#else
	CVariable operand;
	operand.iOperandType = OPERAND_NULL;
	operand.dValue = NULL;
	var = operand;
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      pushEvalStackUndefined
 ***************************************************************************/
void CCilVm::pushEvalStackUndefined()
{
	assert( m_pCurrentEvalStack < &m_OperandEvalStack[ MAX_EVALSTACK - 1 ] );
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.iPrameters = OPERAND_UNDEFINED;
#else
	CVariable operand;
	operand.iOperandType = OPERAND_UNDEFINED;
	operand.dValue = NULL;
	var = operand;
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      
 ***************************************************************************/
void CCilVm::pushEvalStackNaN()
{
	assert( m_pCurrentEvalStack < &m_OperandEvalStack[ MAX_EVALSTACK - 1 ] );
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.iPrameters = OPERAND_NAN;
#else
	CVariable operand;
	operand.iOperandType = OPERAND_NAN;
	operand.dValue = NULL;
	var = operand;
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      pushEvalStackInfinity
 ***************************************************************************/
void CCilVm::pushEvalStackInfinity()
{
	assert( m_pCurrentEvalStack < &m_OperandEvalStack[ MAX_EVALSTACK - 1 ] );
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.iPrameters = OPERAND_FLOAT;
	var.iValue = 0x7f800000;
#else
	CVariable operand;
	operand.iOperandType = OPERAND_FLOAT;
	operand.iValue = 0x7f800000;
	var = operand;
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      
 ***************************************************************************/
void CCilVm::pushEvalStackObjectRef( VMOBJECTREF refObject )
{
	assert( m_pCurrentEvalStack < &m_OperandEvalStack[ MAX_EVALSTACK - 1 ] );
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.iPrameters = OPERAND_OBJECTREF;
	var.refObject = refObject;
	if( refObject )
		refObject->addRef();
#else
	CVariable operand;
	operand.iOperandType = OPERAND_OBJECTREF;
	operand.refObject = refObject;
	var = operand;
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      pushEvalStackVariable
 ***************************************************************************/
void CCilVm::pushEvalStackVariable( const CVariable& varSrc )
{
	assert( m_pCurrentEvalStack < &m_OperandEvalStack[ MAX_EVALSTACK - 1 ] );
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.copyFast( varSrc );
#else
	var = varSrc;
	var.iOperandFlag = OPERAND_FLAG_NONE;
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      
 ***************************************************************************/
void CCilVm::popEvalStack( void )
{
	assert( m_pCurrentEvalStack > &m_OperandEvalStack[ 0 ]  );
	m_pCurrentEvalStack--;
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.erase();
#endif
}

/***************************************************************************
 *      popEvalStackFast
 ***************************************************************************/
//Used after moveto()
void CCilVm::popEvalStackFast( void )
{
	assert( m_pCurrentEvalStack > &m_OperandEvalStack[ 0 ]  );
	m_pCurrentEvalStack--;
}

/***************************************************************************
 *      isEvalStackEmpty
 ***************************************************************************/
bool CCilVm::isEvalStackEmpty() { return (bool)(m_pCurrentEvalStack != &m_OperandEvalStack[ 0 ]); }

/***************************************************************************
 *      dupEvalStack
 ***************************************************************************/
void CCilVm::dupEvalStack( void )
{
	assert( m_pCurrentEvalStack > &m_OperandEvalStack[ 0 ]  );
	CVariable& var = *m_pCurrentEvalStack;
#ifdef VM_TRACK_ERASEWHENPOP
	var.copyFast( *(m_pCurrentEvalStack - 1) );
#else
	var = *(m_pCurrentEvalStack - 1);
#endif
	updateEvalStackIndex();
}

/***************************************************************************
 *      getEvalStackSize
 ***************************************************************************/
size_t CCilVm::getEvalStackSize()
{
	return (m_pCurrentEvalStack - &m_OperandEvalStack[ 0 ] ) / (&m_OperandEvalStack[ 1 ] - &m_OperandEvalStack[ 0 ] );
}

/***************************************************************************
 *      getEvalStackNewEntry
 ***************************************************************************/
CVariable& CCilVm::getEvalStackNewEntry()
{
	assert( m_pCurrentEvalStack < &m_OperandEvalStack[ MAX_EVALSTACK - 1 ] );
	return *m_pCurrentEvalStack++;
}

/***************************************************************************
 *      getEvalStackFirstEntry
 ***************************************************************************/
CVariable& CCilVm::getEvalStackFirstEntry()
{
	assert( getEvalStackSize() >= 1 );
	return *(m_pCurrentEvalStack - 1);
}

/***************************************************************************
 *      getEvalStackSecondEntry
 ***************************************************************************/
CVariable& CCilVm::getEvalStackSecondEntry()
{
	assert( getEvalStackSize() >= 2 );
	return *(m_pCurrentEvalStack - 2);
}

/***************************************************************************
 *      clearCallStack
 ***************************************************************************/
void CCilVm::clearCallStack()
{
	m_CallStack.clear();
}

/***************************************************************************
 *      clearStaticFields
 ***************************************************************************/
void CCilVm::clearStaticFields()
{
	for( uint32_t i = 0; i < m_iStaticFieldTalbeSize; ++i )
	{
		switch( OperandType( m_pStaticFieldTable[ i ].iOperandType ) )
		{
		case OPERAND_OBJECTREF:
			m_pStaticFieldTable[ i ].i64Value = 0;
			break;
		case OPERAND_STRING:
			delete(  m_pStaticFieldTable[ i ].pString ) ;
			m_pStaticFieldTable[ i ].pString = NULL;
			break;
		default:
			m_pStaticFieldTable[ i ].i64Value = 0;
			break;
		}
	}
}

/***************************************************************************
 *      clearEvalStack
 ***************************************************************************/
void CCilVm::clearEvalStack()
{
	CVariable var ( 0 );
	for( int32_t i = 0; i < MAX_EVALSTACK; ++i )
	{
		m_OperandEvalStack[ i ] = var;
	}
	m_pCurrentEvalStack = &m_OperandEvalStack[ 0 ];
	m_pMaxEvalStack = m_pCurrentEvalStack;

}

/***************************************************************************
 *      clearLocalVariableList
 ***************************************************************************/
void CCilVm::clearLocalVariableList()
{
	CVariable var ( 0 );
	for( int32_t i = 0; i < MAX_LOCAL_VARIABLE; ++i )
	{
		m_LocalVariableList[ i ] = var;
	}
	m_iLocalVariableListIndex = 0;
	m_iMaxLocalVariableListIndex = 0;
}

/***************************************************************************
 *      clearArgList
 ***************************************************************************/
void CCilVm::clearArgList()
{
	CVariable var ( 0 );
	for( int32_t i = 0; i < MAX_ARG_VARIABLE; ++i )
	{
		m_ArgList[ i ] = var;
	}
	m_iArgListIndex = 0;
	m_iMaxArgListIndex = 0;
}

} //namespace CRI