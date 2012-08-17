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
 * Module   : unaryVoid Operator
 * File     : cilOperatorunaryVoid.cpp
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
 * unaryVoid Operation
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::unaryVoidOperator( CCgNodeData* lhs )
{
	assert( lhs != NULL );

	//rhs can be null
	CCgNodeData* Node;

	Node = makeFormulaNode( CG_UNARYVOID, lhs, NULL );

	return Node;
};

/***************************************************************************
 * emit unaryVoid Operation
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitUnaryVoidOperator( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );
	assert( nodetree->lhsNode != NULL );
	assert( nodetree->rhsNode == NULL );

	delete nodetree;
	nodetree = identifier( STRING_INTERNAL_UNDEFINED );

	//Put undefined value
	emitLoadStack( nodetree );

	return nodetree;
}

} //namespace CRI