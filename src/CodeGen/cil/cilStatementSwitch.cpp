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
 * Module   : Switch statement
 * File     : cilStatementSwitch.cpp
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
//Create Switch statement node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::switchStatement( CCgNodeData* Expression,
										   CCgNodeData* CaseBlock )
{
	assert( Expression != NULL );
	if( CaseBlock != NULL )
	{
		CCgNodeData* object = new CCgNodeData;
		object->NodeType = CG_NODE_SWITCHNODE;
		object->conditionNode = Expression;
		object->statementNode = CaseBlock;
		return object;
	}
	else
		return NULL;
};

/***************************************************************************
//Create Case block decl node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::caseClause( CCgNodeData* Expression,
										CCgNodeData* Statement )
{
	// all node can be null
	CCgNodeData* object = new CCgNodeData;
	object->NodeType = CG_NODE_CASECLAUSENODE;
	object->conditionNode = Expression;
	object->statementNode = Statement;
	object->iLineNumber = m_pLex->getLineNumber();
	return object;
};

/***************************************************************************
//parse case node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseCaseNode( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_CASECLAUSENODE );
	//
	if( !m_bInsideSwitchStatement )
	{
		reportError( ERROR_CANNOTHAVECASE,
					nodetree->iFileId,
					nodetree->iLineNumber,
					nodetree->iColumn );
		return nodetree;		
	}
	Debug_Fatal( "Not implemented yet" );
	return nodetree;		

}

/***************************************************************************
// parse switch statement node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseSwitchStatementNode( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_SWITCHNODE );
	//nodetree->statementNode = parseExpressionNodeTree( nodetree->statementNode, false );

	int32_t iLableSwitchEnd = createNewLabelID();

	//Save states
	bool bSavedInsideSwitchStatement = m_bInsideSwitchStatement;
	uint32_t iSavedLabelBreak = m_iLabelBreak;

	//Set up variables to allow break/continue node
	m_bInsideSwitchStatement = true;
	m_iLabelBreak = iLableSwitchEnd;

	//emit expression and keep it on a temporary field
	nodetree->conditionNode = parseExpressionNodeTree( nodetree->conditionNode, false );
	emit( CEE_STSFLD, RID_SWITCHTMP );

	//Loop over the cases
	int32_t iNumDefaultNode = 0;
	CCgNodeData* currentNode = nodetree->statementNode;
	CCgNodeData* pDefaultNode;
	while( currentNode != NULL )
	{
		assert( currentNode->NodeType == CG_NODE_CASECLAUSENODE );
		if( currentNode->conditionNode == NULL )
		{
			//I'm default node;
			iNumDefaultNode++;
			pDefaultNode = currentNode;
		}
		else
		{
			emit( CEE_LDSFLD, RID_SWITCHTMP );
			currentNode->conditionNode = parseExpressionNodeTree( currentNode->conditionNode, false );

			currentNode->iIndex = createNewLabelID();
			//Todo
			//Should be ===, not ==?
//			emit( CEE_BEQ, currentNode->iIndex );
			emit( CEE_EXT_STARGLIST_S, (uint8_t)2 );	//Move to arglist
			emit( CEE_CALL, m_ridMethodStrictEqual );
			emit( CEE_BRTRUE, currentNode->iIndex );

		}
		currentNode = currentNode->siblingNode;
	}
	if( iNumDefaultNode > 1 )
	{
		//Ooops
		reportError( ERROR_ONLYONEDEFAULTISALLOWED,
							currentNode->iFileId,
							currentNode->iLineNumber,
							currentNode->iColumn );
	}
	//emit default node jump
	if( iNumDefaultNode )
	{
		pDefaultNode->iIndex = createNewLabelID();
		emit( CEE_BR, pDefaultNode->iIndex );		
	}
	else
	{
		emit( CEE_BR, iLableSwitchEnd );		
	}

	//Store statement
	currentNode = nodetree->statementNode;
	while( currentNode != NULL )
	{
		assert( currentNode->NodeType == CG_NODE_CASECLAUSENODE );
		insertLabelTag( currentNode->iIndex );
		currentNode->statementNode = parseStatementNodeTree( currentNode->statementNode );
		currentNode = currentNode->siblingNode;
	}

	//Restore states
	m_bInsideSwitchStatement = bSavedInsideSwitchStatement;
	m_iLabelBreak =  iSavedLabelBreak;

	insertLabelTag( iLableSwitchEnd );

	//Clear Switch temp value to releae an object ref
	emit( CEE_LDNULL );
	emit( CEE_STSFLD, RID_SWITCHTMP );
	return nodetree;
}

} //namespace CRI