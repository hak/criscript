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
 * Module   : unaryTypeof Operator
 * File     : cilOperatorunaryTypeof.cpp
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
 * unaryTypeof Operation
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::unaryTypeofOperator( CCgNodeData* lhs )
{
	assert( lhs != NULL );

	//rhs can be null
	CCgNodeData* Node;

	Node = makeFormulaNode( CG_UNARYTYPEOF, lhs, NULL );

	return Node;
};

/***************************************************************************
 * emit unaryTypeof Operation
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitUnaryTypeofOperator( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );
	assert( nodetree->lhsNode != NULL );
	assert( nodetree->rhsNode == NULL );

	CCgNodeData* ptr = nodetree->lhsNode;
	emitLoadStack( ptr );

	emit( CEE_EXT_STARGLIST_S, (uint8_t)1 );	//Move to arglist
	emit( CEE_CALL, m_ridMethodTypeOf );

	return nodetree;
}

} //namespace CRI