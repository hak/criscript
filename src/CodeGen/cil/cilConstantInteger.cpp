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
 * Module   : cil codegenerater
 * File     : cilConstantInteger.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
namespace cri {
	
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 * Create constant node leaf
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::integerConstant( const int64_t value,
										  	uint32_t iFileId,
											uint32_t iLineNumber,
											uint32_t iColumn )
{
	uint32_t iIndex;
	map< int64_t, uint32_t>::iterator it;

	it = m_mapIntegerConstantPool.find( value );
	if( it != m_mapIntegerConstantPool.end() )
	{
		//found an existing entry
		iIndex = it->second;		
	}
	else
	{
		m_IntegerConstantPool.push_back( value );
		iIndex = (int32_t)m_IntegerConstantPool.size() - 1;
		m_mapIntegerConstantPool[ value ] = iIndex;
	}

	CCgNodeData* object = new CCgNodeData;	//Need to be deleted later
	assert( object != NULL );
	object->NodeType = CG_NODE_INTEGERLITERAL;
	object->iIndex = (int32_t)iIndex;
	object->iFileId = iFileId;
	object->iLineNumber = iLineNumber;
	object->iColumn = iColumn;
	return object;
};

/***************************************************************************
 * emit CIL
 ***************************************************************************/	
void CCilCodeGen::emitLoadInteger( const int64_t value)
{
	if( value > INT_MAX || value < INT_MIN )
	{
		emit( CEE_LDC_I8, value );
		return;
	}
	else if( value > SCHAR_MAX || value < SCHAR_MIN )
	{
		emit( CEE_LDC_I4, (int32_t)value );
		return;
	}
	switch( value )
	{
	case -1:
		emit( CEE_LDC_I4_M1 );
		break;
	case 0:
		emit( CEE_LDC_I4_0 );
		break;
	case 1:
		emit( CEE_LDC_I4_1 );
		break;
	case 2:
		emit( CEE_LDC_I4_2 );
		break;
	case 3:
		emit( CEE_LDC_I4_3 );
		break;
	case 4:
		emit( CEE_LDC_I4_4 );
		break;
	case 5:
		emit( CEE_LDC_I4_5 );
		break;
	case 6:
		emit( CEE_LDC_I4_6 );
		break;
	case 7:
		emit( CEE_LDC_I4_7 );
		break;
	case 8:
		emit( CEE_LDC_I4_8 );
		break;
	default:
		emit( CEE_LDC_I4_S, (int8_t)value );
		break;
	}
	return;
}

} //namespace CRI