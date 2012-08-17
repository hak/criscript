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
 * File     : cilarrayLiteral.cpp
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
/***************************************************************************
 * Add Operation
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::arrayLiteral( CCgNodeData* node )
{
	//node can be null
	CCgNodeData* Node;
	Node = makeFormulaNode( CG_arrayLiteral, node, NULL, true, true );	//Block traversal of both child node

	return Node;
};

/***************************************************************************
//emit object literal
 ***************************************************************************/
CCgNodeData* CCilCodeGen::emitArrayLiteral( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );
	assert( nodetree->rhsNode == NULL );
	
	uint32_t iSizeArray = 0;

	nodetree->lhsNode = parsearrayLiteralParameterNodeTree( nodetree->lhsNode, iSizeArray );
	
	if( iSizeArray > MAX_arrayLiteralSIZE )
	{
		//Sanity check for array size
		reportError( INFO_arrayLiteralTOOLARGE,
			nodetree->lhsNode->iFileId,
			nodetree->lhsNode->iLineNumber,
			nodetree->lhsNode->iColumn
			);
	}

	if( iSizeArray <= MAXBYTE )
		emit( CEE_EXT_STARGLIST_S, (uint8_t)iSizeArray );	//Move to arglist
	else
		emit( CEE_EXT_STARGLIST, (uint32_t)iSizeArray );	//Move to arglist
	emit( CEE_CALL, m_ridMethodLdarrayLiteral );		//Function Object Constructor

	return nodetree;
};

/***************************************************************************
 * parse arrayLiteral initialization param tree
  ***************************************************************************/
CCgNodeData* CCilCodeGen::parsearrayLiteralParameterNodeTree( CCgNodeData* nodetree, uint32_t& iNumArguments )
{
	if( nodetree == NULL ) return NULL;
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );
	assert( nodetree->iOperation == CG_PROPERTYNAMEANDVALUE );

	if( nodetree->siblingNode != NULL )
	{
		nodetree->siblingNode = parsearrayLiteralParameterNodeTree( nodetree->siblingNode, iNumArguments );
	}

	assert( nodetree->lhsNode == NULL );
	nodetree->rhsNode = parseExpressionNodeTree( nodetree->rhsNode, false );
	iNumArguments ++;

	delete nodetree;
	return NULL;
}


} //namespace CRI