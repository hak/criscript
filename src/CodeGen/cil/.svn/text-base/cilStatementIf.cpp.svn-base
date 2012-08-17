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
 * Module   : If statement
 * File     : cilStatementIf.cpp
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
 *      If statement node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::ifStatement( CCgNodeData* condition,
											 CCgNodeData* statementtrue,
											 CCgNodeData* statementfalse)
{
	assert( condition != NULL );
	CCgNodeData* Node;
	Node = makeIfNode( condition, statementtrue, statementfalse );
	return Node;
}

/***************************************************************************
//Create IF node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::makeIfNode(	CCgNodeData* condition,
								CCgNodeData* statementtrue,
								CCgNodeData* statementfalse )
{
	assert( condition != NULL );
	CCgNodeData* object = new CCgNodeData;
	object->NodeType = CG_NODE_IFNODE;
	object->conditionNode = condition;
	object->rhsNode = statementtrue;
	object->lhsNode = statementfalse;
	object->bBlockTraverseLeft = true;
	object->bBlockTraverseRight = true;
	object->iLineNumber = m_pLex->getLineNumber();
	return object;
};

/***************************************************************************
//Create parse if node 
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseIfNode( CCgNodeData* ifnode )
{
	assert( ifnode != NULL );
	assert( ifnode->NodeType == CG_NODE_IFNODE );
	//Do some optimizations work here
	
	int32_t iLableFalseCase = createNewLabelID();
	int32_t iLableEndIfStatement = createNewLabelID();

	if( ifnode->rhsNode == NULL && ifnode->lhsNode == NULL )
	{
		//The statement has no effect
		reportError( INFO_IF_NOEFFECT,
					ifnode->iFileId,
					ifnode->iLineNumber,
					ifnode->iColumn );
		return ifnode;
	}

	if( ifnode->conditionNode == NULL )
	{
		m_iInsideIterationStatement++;
		//Just generate false node
		ifnode->lhsNode = parseStatementNodeTree( ifnode->lhsNode );
		m_iInsideIterationStatement--;
	}
	else if ( ifnode->rhsNode == NULL )
	{
		//If(...)
		ifnode->conditionNode = parseExpressionNodeTree( ifnode->conditionNode, false );
		emit( CEE_BRTRUE, iLableEndIfStatement );	//Eval & JUMP

		// else {false case}
		m_iInsideIterationStatement++;
		ifnode->lhsNode = parseStatementNodeTree( ifnode->lhsNode );
		m_iInsideIterationStatement--;
		insertLabelTag( iLableEndIfStatement );
	}
	else if ( ifnode->lhsNode == NULL )
	{
		//If(...)
		ifnode->conditionNode = parseExpressionNodeTree( ifnode->conditionNode, false );
		emit( CEE_BRFALSE, iLableEndIfStatement );	//Eval & JUMP
		// {true case}
		m_iInsideIterationStatement++;
		ifnode->rhsNode = parseStatementNodeTree( ifnode->rhsNode );
		m_iInsideIterationStatement--;
		insertLabelTag( iLableEndIfStatement );
	}
	else
	{
		//If(...)
		ifnode->conditionNode = parseExpressionNodeTree( ifnode->conditionNode, false );
		emit( CEE_BRFALSE, iLableFalseCase );	//Eval & JUMP

		// {true case}
		m_iInsideIterationStatement++;
		ifnode->rhsNode = parseStatementNodeTree( ifnode->rhsNode );
		emit( CEE_BR, iLableEndIfStatement );	//Unconditional JUMP

		// else {false case}
		insertLabelTag( iLableFalseCase );
		ifnode->lhsNode = parseStatementNodeTree( ifnode->lhsNode );
		m_iInsideIterationStatement--;
		insertLabelTag( iLableEndIfStatement );
	}

	return ifnode;
};

} //namespace CRI