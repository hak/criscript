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
 * File     : cilConditionalExpression.cpp
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
 *      conditionalExpression node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::conditionalExpression( CCgNodeData* condition,
											 CCgNodeData* expressiontrue,
											 CCgNodeData* expressionfalse)
{
	assert( condition != NULL );
	assert( expressiontrue != NULL );
	assert( expressionfalse != NULL );
	CCgNodeData* Node;
	Node = MakeConditionalExpressionNode( condition, expressiontrue, expressionfalse );
	return Node;
}

/***************************************************************************
//Create conditionalExpression node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::MakeConditionalExpressionNode( CCgNodeData* condition,
								CCgNodeData* expressiontrue,
								CCgNodeData* expressionfalse )
{
	assert( condition != NULL );
	assert( expressiontrue != NULL );
	assert( expressionfalse != NULL );
	CCgNodeData* object = new CCgNodeData;
	object->NodeType = CG_NODE_FORMULANODE;
	object->iOperation = CG_CONDITIONAL;
	object->conditionNode = condition;
	object->rhsNode = expressiontrue;
	object->lhsNode = expressionfalse;
	object->bBlockTraverseLeft = true;
	object->bBlockTraverseRight = true;
	object->iLineNumber = m_pLex->getLineNumber();
	return object;
};

/***************************************************************************
//Create parse if node 
 ***************************************************************************/
CCgNodeData* CCilCodeGen::emitConditionalExpression( CCgNodeData* conditionalnode )
{
	assert( conditionalnode != NULL );
	assert( conditionalnode->NodeType == CG_NODE_FORMULANODE );
	assert( conditionalnode->conditionNode != NULL );
	assert( conditionalnode->rhsNode != NULL );
	assert( conditionalnode->lhsNode != NULL );
	//Do some optimizations work here
	
	int32_t iLableFalseCase = createNewLabelID();
	int32_t iLableEndIfStatement = createNewLabelID();

	//If(...)
	conditionalnode->conditionNode = parseExpressionNodeTree( conditionalnode->conditionNode, false );
	emit( CEE_BRFALSE, iLableFalseCase );	//Eval & JUMP

	// {true case}
	conditionalnode->rhsNode = parseExpressionNodeTree( conditionalnode->rhsNode, false );
	emit( CEE_BR, iLableEndIfStatement );	//Unconditional JUMP

	// else {false case}
	insertLabelTag( iLableFalseCase );
	conditionalnode->lhsNode = parseExpressionNodeTree( conditionalnode->lhsNode, false );
	insertLabelTag( iLableEndIfStatement );

	//Since the code pass just take 1 route ( push 1 eval stack entry ), it need to be adjusted manually
	adjustEvalStack( -1 );

	//I'm done
	conditionalnode->iOperation = CG_NONE;

	return conditionalnode;
};

} //namespace CRI