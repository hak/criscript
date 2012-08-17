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
 * Module   : Expression
 * File     : cilStatementExpression.cpp
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
 *      Expression statement node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::expressionStatement( CCgNodeData* expression )
{
	if( expression == NULL )
	{
		return NULL;
	}
	CCgNodeData* object = new CCgNodeData;
	object->NodeType = CG_NODE_EXPRESSIONNODE;
	object->statementNode = expression;
	object->iLineNumber = m_pLex->getLineNumber();
	return object;
}

/***************************************************************************
//Create parse expression statement node 
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseExpressionStatementNode( CCgNodeData* expression )
{
	assert( expression != NULL );
	assert( expression->NodeType == CG_NODE_EXPRESSIONNODE );
	assert( expression->statementNode != NULL );

	//Do some optimizations work here

	//Check if the node has an effect or not.
	//If no, just show the error and does not parse the tree 

	switch( expression->statementNode->NodeType )
	{
	case CG_NODE_IDENTIFIER:
	case CG_NODE_STRINGLITERAL:
	case CG_NODE_REGEXLITERAL:
	case CG_NODE_INTEGERLITERAL:
	case CG_NODE_FLOATLITERAL:
		//Has no effect
		reportError( INFO_CODE_NOEFFECT,
			expression->statementNode->iFileId,
			expression->statementNode->iLineNumber,
			expression->statementNode->iColumn );
		break;
	default:
		expression->statementNode = parseExpressionNodeTree( expression->statementNode, true );
		break;
	}
	
	if( expression->siblingNode )
	{
		expression->siblingNode = parseStatementNodeTree( expression->siblingNode ); 
	}
	
	delete expression;
	return NULL;
};

} //namespace CRI