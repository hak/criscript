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
 * File     : cilConstantFloat.cpp
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
 * Create float constant node leaf
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::floatConstant( const double value,
										  	uint32_t iFileId,
											uint32_t iLineNumber,
											uint32_t iColumn )
{
	uint32_t iIndex;
	map< double, uint32_t>::iterator it;
	it = m_mapFloatConstantPool.find( value );
	if( it != m_mapFloatConstantPool.end() )
	{
		//found an existing entry
		iIndex = it->second;		
	}
	else
	{
		m_FloatConstantPool.push_back( value );
		iIndex = (int32_t)m_FloatConstantPool.size() - 1;
		m_mapFloatConstantPool[ value ] = iIndex;
	}

	CCgNodeData* object = new CCgNodeData;	//Need to be deleted later
	assert( object != NULL );
	object->NodeType = CG_NODE_FLOATLITERAL;
	object->iIndex = (int32_t)iIndex;
	object->iFileId = iFileId;
	object->iLineNumber = iLineNumber;
	object->iColumn = iColumn;
	return object;
};


/***************************************************************************
 * emit float CIL
 ***************************************************************************/
void CCilCodeGen::emitLoadFloat( const double value)
{
	//TODO
	//Need better math to determine double/float
	if( value > FLT_MAX || value < FLT_MIN )
		emit( CEE_LDC_R8, value );
	else
		emit( CEE_LDC_R4, (float)value );
	
}

} //namespace CRI