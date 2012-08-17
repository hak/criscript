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
 * File     : cilConstantRegex.cpp
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
CCgNodeData* CCilCodeGen::regexLiteral( const wchar_t* str,
										  	uint32_t iFileId,
											uint32_t iLineNumber,
											uint32_t iColumn )
{
	assert( str != NULL );
	RID ridStringBody;
	RID ridStringFlag;

	//parse Regex literal
	wstring s( str );
	wstring sBody;
	wstring sFlag;
	size_t posDelimiter = s.find_last_of( '/' );
	if( posDelimiter == wstring::npos )
	{
		reportError( ERROR_BADREGEXLITERALFORMAT,
			iFileId,
			iLineNumber,
			iColumn );
		return NULL;
	}
	sBody = s.substr( 0, posDelimiter );
	sFlag = s.substr( posDelimiter + 1, s.length() );

	hash_map< wstring, RID>::iterator it;
	it = m_mapStringConsantPool.find( sBody );
	if( it != m_mapStringConsantPool.end() )
	{
		//found an existing entry
		ridStringBody = it->second;		
	}
	else
	{
		ridStringBody = m_MetaData.generateUserStringConstantRID( sBody );
		m_mapStringConsantPool[ sBody ] = ridStringBody;
	}
	it = m_mapStringConsantPool.find( sFlag );
	if( it != m_mapStringConsantPool.end() )
	{
		//found an existing entry
		ridStringFlag = it->second;		
	}
	else
	{
		ridStringFlag = m_MetaData.generateUserStringConstantRID( sFlag );
		m_mapStringConsantPool[ sFlag ] = ridStringFlag;
	}

	CCgNodeData* object = new CCgNodeData;	//Need to be deleted later
	assert( object != NULL );

	object->NodeType = CG_NODE_REGEXLITERAL;
	object->rid = ridStringBody;
	object->iFileId = iFileId;
	object->iLineNumber = iLineNumber;
	object->iColumn = iColumn;
	object->bBlockTraverseLeft = true;

	CCgNodeData* objectFlag = new CCgNodeData;	//Need to be deleted later
	assert( objectFlag != NULL );

	objectFlag->NodeType = CG_NODE_REGEXLITERAL;
	objectFlag->rid = ridStringFlag;
	objectFlag->iFileId = iFileId;
	objectFlag->iLineNumber = iLineNumber;
	objectFlag->iColumn = iColumn;
	object->lhsNode = objectFlag;

	return object;
};

/***************************************************************************
 * emit Regex literal and regex object
 ***************************************************************************/	
void CCilCodeGen::emitLoadRegexObject(CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->lhsNode != NULL );
	assert( nodetree->NodeType == CG_NODE_REGEXLITERAL );
	assert( nodetree->lhsNode->NodeType == CG_NODE_REGEXLITERAL );

	assert( TypeFromToken( nodetree->rid) == MDT_STRING );
	assert( TypeFromToken( nodetree->lhsNode->rid) == MDT_STRING );
	emit( CEE_LDC_I4, nodetree->lhsNode->rid );
	emit( CEE_LDC_I4, nodetree->rid );
	emit( CEE_EXT_STARGLIST, (uint32_t)2 );
	emit( CEE_NEWOBJ, m_ridRegexObject );

	delete nodetree->lhsNode;
	nodetree->lhsNode = NULL;
}

} //namespace CRI