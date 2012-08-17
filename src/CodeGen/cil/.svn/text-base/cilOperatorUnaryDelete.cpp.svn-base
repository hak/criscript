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
 * Module   : unaryDelete Operator
 * File     : cilOperatorunaryDelete.cpp
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
 * unaryDelete Operation
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::unaryDeleteOperator( CCgNodeData* lhs )
{
	assert( lhs != NULL );

	//Implementation note
	//Store 'deletion' object to the destination
	//When deletion object is stored, store part would handle it.
	CCgNodeData* rhs = new CCgNodeData;
	assert( rhs != NULL );
	rhs->NodeType = CG_NODE_FORMULANODE;
	
	CCgNodeData* Node;
	Node = makeFormulaNode( CG_UNARYDELETE, lhs, rhs, true, true );
	return Node;
};

/***************************************************************************
 * emit unaryMinus Operation
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitUnaryDeleteOperator( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );
	assert( nodetree->lhsNode != NULL );
	assert( nodetree->rhsNode != NULL );

	emit( CEE_LDSFLD, RID_CONSTANT_DELETION );

	m_bResultInEvalStack = true;
	//Store deletion object to the left hand side
	emitVariableAssignment( nodetree );
	emit( CEE_LDSFLD, RID_CONSTANT_BOOL_TRUE );

	return nodetree;
}

} //namespace CRI