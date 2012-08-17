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
 * File     : cilFunctionRet.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criscript.h"
#include "cilCodeGen.h"
namespace cri {
	
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/
/***************************************************************************
 * Ret statement
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::returnStatement( CCgNodeData* returnNode )
{
	//returnNode can be null
	
	CCgNodeData* Node;

	Node = makeNode( CG_NODE_RETNODE, (CG_OPERATIONTYPE)0, returnNode, NULL );
	//Block traversing the return value node
	Node->bBlockTraverseLeft = true;
	
	return Node;
}

/***************************************************************************
 * emit call expression
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::parseRetStatementNode( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_RETNODE );
	assert( nodetree->rhsNode == NULL );

	if( nodetree->lhsNode != NULL )
	{
		//Generate Initial node
		nodetree->lhsNode = parseExpressionNodeTree( nodetree->lhsNode, false );
	}
	else
	{
		//Push Undefined to the Eval stack
		emit( CEE_LDSFLD, RID_CONSTANT_UNDEFINED );
	}
	emit( CEE_RET );
	//Increment return pass counter
	m_MetaData.getMethodTable()[ RidFromToken( getCurrentILInfo().MethodRID ) ].iNumberOfReturnPass ++;

	return nodetree;
}


} //namespace CRI