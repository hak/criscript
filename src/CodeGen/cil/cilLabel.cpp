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
 * Module   : Create label and resolve these
 * File     : cilLabel.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
#include "cilCodeGen.h"
namespace cri {

/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Create new label without bytecode offset
 ***************************************************************************/	
uint32_t CCilCodeGen::createNewLabelID()
{
	uint32_t iLabelID = m_iNumLabel++;

	//Set up branch target info
	if( m_vecBranchTargetInfoTable.size() <= iLabelID )
	{
		m_vecBranchTargetInfoTable.resize( iLabelID );
	}

	//Store temporary value to the iLabel
	m_vecBranchTargetInfoTable.resize( iLabelID + 1);
	BRANCH_TARGET_INFO brtarget;
	brtarget.iExceptionHandlerNestLevel = -1;
	m_vecBranchTargetInfoTable[ iLabelID ] = brtarget;

	return iLabelID;
}

/***************************************************************************
 *      Create new label with bytecode offset
 ***************************************************************************/	
uint32_t CCilCodeGen::createNewLabelIDAndInsertTag()
{
	uint32_t iLabelID = m_iNumLabel++;
	emit( CEE_CODE_LABEL, iLabelID );

	//Store temporary value to the iLabel
	m_vecBranchTargetInfoTable.resize( iLabelID + 1 );
	//Set up branch target info
	setBranchTargetInformation( iLabelID );

	return iLabelID;
}

/***************************************************************************
 *      Resolve existing label with bytecode offset
 ***************************************************************************/	
void CCilCodeGen::insertLabelTag( uint32_t iLabelID )
{
	assert( iLabelID < m_iNumLabel );
	emit( CEE_CODE_LABEL, iLabelID );

	//Set up branch target info
	setBranchTargetInformation( iLabelID );
}

/***************************************************************************
//setBranchTargetInformation
 ***************************************************************************/
void CCilCodeGen::setBranchTargetInformation( const int32_t iLabelID )
{
	assert( m_vecBranchTargetInfoTable.size() > (uint32_t)iLabelID );

	BRANCH_TARGET_INFO brtarget;
	brtarget.iExceptionHandlerNestLevel = m_iCurrentEHNestLevel;
	m_vecBranchTargetInfoTable[ iLabelID ] = brtarget;

}

/***************************************************************************
//getBranchTargetInformation
 ***************************************************************************/
const BRANCH_TARGET_INFO& CCilCodeGen::getBranchTargetInformation( int32_t iLabelID )
{
	assert( m_vecBranchTargetInfoTable.size() > (uint32_t)iLabelID );

	return m_vecBranchTargetInfoTable[ iLabelID ];
}

/***************************************************************************
//Create Labeled statement node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::labeledStatement( CCgNodeData* identifier, CCgNodeData* Statement )
{
	// all node can be null

	if( Statement != NULL )
	{
		CCgNodeData* object = new CCgNodeData;
		object->NodeType = CG_NODE_LABELEDNODE;
		object->identifier = identifier;
		object->statementNode = Statement;
		object->iLineNumber = m_pLex->getLineNumber();
		return object;
	}
	else
		return NULL;
};

/***************************************************************************
//parse EH node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseLabeledStatementNode( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->identifier != NULL );
	assert( nodetree->identifier->NodeType == CG_NODE_IDENTIFIER );
	assert( nodetree->statementNode != NULL );

	uint32_t iLabelId;
	//The label for next statement, assuming the value is added 1.
	uint32_t iLabelIdNextStatement;
	
	CG_SYMBOL_INFORMATION* pInfo;

	wstring strScopedFunctionName = getCurrentScopeName() + *nodetree->identifier->pIdentifier;
	if( RID_NOTDEFINED != lookUpSymbolInformationInCurrentScope( strScopedFunctionName, &pInfo) )
	{
		if( pInfo->rid == RID_LABELNAME )
			reportError( ERROR_LABELALREADYDEFINED, strScopedFunctionName,
							nodetree->iFileId,
							nodetree->iLineNumber,
							nodetree->iColumn );
		else
			reportError( ERROR_IDALREADYDEFINED, strScopedFunctionName,
							nodetree->iFileId,
							nodetree->iLineNumber,
							nodetree->iColumn );
		return nodetree;
	}

	switch( nodetree->statementNode->NodeType )
	{
	case CG_NODE_FORNODE:
		//Special case handling
		//In this case, the label should be handled as non label continue.
		
		iLabelId = createNewLabelID();
		//The label for next statement, assuming the value is added 1.
		iLabelIdNextStatement = createNewLabelID();

		generateSymbolInformation( RID_LABELNAME, strScopedFunctionName, CG_SYMBOL_FLAG_READONLY, iLabelId );	

		//In this case, iLabelID should be put at the end of the loop
		nodetree->statementNode = parseForNode( nodetree->statementNode, false, iLabelId );
		break;
	case CG_NODE_FORINNODE:
		
		iLabelId = createNewLabelID();
		//The label for next statement, assuming the value is added 1.
		iLabelIdNextStatement = createNewLabelID();

		generateSymbolInformation( RID_LABELNAME, strScopedFunctionName, CG_SYMBOL_FLAG_READONLY, iLabelId );	

		//In this case, iLabelID should be put at the end of the loop
		nodetree->statementNode = parseForInNode( nodetree->statementNode, iLabelId );
		break;
	case CG_NODE_DOWHILENODE:
		//Special case handling
		//In this case, the label should be handled as non label continue.
		
		iLabelId = createNewLabelID();
		//The label for next statement, assuming the value is added 1.
		iLabelIdNextStatement = createNewLabelID();

		generateSymbolInformation( RID_LABELNAME, strScopedFunctionName, CG_SYMBOL_FLAG_READONLY, iLabelId );	

		//In this case, iLabelID should be put at the end of the loop
		nodetree->statementNode = parseForNode( nodetree->statementNode, true, iLabelId );
		break;
	default:
		iLabelId = createNewLabelIDAndInsertTag();
		//The label for next statement, assuming the value is added 1.
		iLabelIdNextStatement = createNewLabelID();

		generateSymbolInformation( RID_LABELNAME, strScopedFunctionName, CG_SYMBOL_FLAG_READONLY, iLabelId );	
		nodetree->statementNode = parseStatementNodeTree( nodetree->statementNode );
		break;
	}

	assert( iLabelIdNextStatement == iLabelId + 1 );
	insertLabelTag( iLabelIdNextStatement );

	return nodetree;
}

} //namespace CRI