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
 * Module   : For statement
 * File     : cilStatementFor.cpp
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
 *      For statement node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::forStatement( CCgNodeData* initial,
											 CCgNodeData* condition,
											 CCgNodeData* loopend,
											 CCgNodeData* statement,
											 bool bDoWhile )
{
	//All can be null for FOR
	CCgNodeData* Node;
	Node = MakeForNode( initial, condition, loopend, statement, bDoWhile);
	return Node;
}

/***************************************************************************
//Create For node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::MakeForNode(	CCgNodeData* initial,
								CCgNodeData* condition,
								CCgNodeData* loopend,
								CCgNodeData* statement,
								bool bDoWhile )
{
	CCgNodeData* object = new CCgNodeData;
	if( bDoWhile )
		object->NodeType = CG_NODE_DOWHILENODE;
	else
		object->NodeType = CG_NODE_FORNODE;
	object->initialNode = initial;
	object->conditionNode = condition;
	object->loopendNode = loopend;
	object->statementNode = statement;
	object->iLineNumber = m_pLex->getLineNumber();
	return object;
};

/***************************************************************************
//Create parse For node 
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseForNode( CCgNodeData* fornode,
									   bool bDoWhile,
									   int32_t iGivenLoopEndLabelID )
{							
	assert( fornode != NULL );
	assert( fornode->NodeType == CG_NODE_FORNODE || fornode->NodeType == CG_NODE_DOWHILENODE);
	//Do some optimizations work here
	

	if( fornode->conditionNode == NULL && fornode->statementNode == NULL && fornode->loopendNode == NULL )
	{
		//The statement has no effect
		reportError( ERROR_FOR_INFINITE,
							fornode->iFileId,
							fornode->iLineNumber,
							fornode->iColumn );
		delete fornode;
		return NULL;
	}

	if( fornode->initialNode != NULL )
	{
		//Generate Initial node
		fornode->initialNode = parseExpressionNodeTree( fornode->initialNode, true );
	}

	int32_t iLableLoopStart = createNewLabelIDAndInsertTag();
	int32_t iLableLoopEnd;
	if ( iGivenLoopEndLabelID )
		iLableLoopEnd = iGivenLoopEndLabelID;
	else
		iLableLoopEnd = createNewLabelID();

	int32_t iLableEndForStatement = createNewLabelID();

	//Save states
	uint32_t iSavedLabelBreak = m_iLabelBreak;
	uint32_t iSavedLabelContinue = m_iLabelContinue;
	

	if( bDoWhile )
	{
		if( fornode->statementNode != NULL )
		{
			//Set up variables to allow break/continue node
			m_iInsideIterationStatement++;
			m_iLabelBreak = iLableEndForStatement;
			m_iLabelContinue = iLableLoopEnd;

			//Generate statement body
			fornode->statementNode = parseStatementNodeTree( fornode->statementNode );
			
			//Restore states
			m_iInsideIterationStatement--;
			m_iLabelBreak =  iSavedLabelBreak;
			m_iLabelContinue =  iSavedLabelContinue;
		}

		insertLabelTag( iLableLoopEnd );

		if( fornode->conditionNode != NULL )
		{
			//Generate condition check code
			fornode->conditionNode = parseExpressionNodeTree( fornode->conditionNode, false );
			emit( CEE_BRFALSE, iLableEndForStatement );
		}
	}
	else
	{
		if( fornode->conditionNode != NULL )
		{
			//Generate condition check code
			m_iLabelEndLoop = iLableEndForStatement;
			fornode->conditionNode = parseExpressionNodeTree( fornode->conditionNode, false );
			emit( CEE_BRFALSE, iLableEndForStatement );
			m_iLabelEndLoop = 0;
		}

		if( fornode->statementNode != NULL )
		{
			//Set up variables to allow break/continue node
			m_iInsideIterationStatement++;
			m_iLabelBreak = iLableEndForStatement;
			m_iLabelContinue = iLableLoopEnd;

			//Generate statement body
			fornode->statementNode = parseStatementNodeTree( fornode->statementNode );

			//Restore states
			m_iInsideIterationStatement--;
			m_iLabelBreak =  iSavedLabelBreak;
			m_iLabelContinue =  iSavedLabelContinue;
		}
		insertLabelTag( iLableLoopEnd );
	}


	if( fornode->loopendNode != NULL )
	{
		//Generate loopend
		fornode->loopendNode = parseExpressionNodeTree( fornode->loopendNode, true );
	}

	emit( CEE_BR, iLableLoopStart );
	insertLabelTag( iLableEndForStatement );

	return fornode;
};

} //namespace CRI