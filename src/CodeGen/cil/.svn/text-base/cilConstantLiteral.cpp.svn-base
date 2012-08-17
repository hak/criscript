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
 * File     : cilConstantLiteral.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criscript.h"
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
CCgNodeData* CCilCodeGen::literalConstant( const wchar_t* str,
										  	uint32_t iFileId,
											uint32_t iLineNumber,
											uint32_t iColumn )
{
	assert( str != NULL );
	RID ridString;
	
	hash_map< wstring, RID>::iterator it;
	it = m_mapStringConsantPool.find( str );
	if( it != m_mapStringConsantPool.end() )
	{
		//found an existing entry
		ridString = it->second;		
	}
	else
	{
		ridString = m_MetaData.generateUserStringConstantRID( str );
		m_mapStringConsantPool[ str ] = ridString;
	}

	CCgNodeData* object = new CCgNodeData;	//Need to be deleted later
	assert( object != NULL );

	object->NodeType = CG_NODE_STRINGLITERAL;
	object->rid = ridString;
	object->iFileId = iFileId;
	object->iLineNumber = iLineNumber;
	object->iColumn = iColumn;
	return object;
};

/***************************************************************************
 * emit String literal and string object
 ***************************************************************************/	
void CCilCodeGen::emitLoadStringObject( const RID ridString )
{
	assert( TypeFromToken(ridString) == MDT_STRING );
	emit( CEE_LDSTR, ridString );
}

} //namespace CRI