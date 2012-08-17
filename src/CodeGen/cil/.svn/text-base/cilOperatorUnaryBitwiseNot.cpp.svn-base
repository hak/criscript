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
 * Module   : unarybitwiseNot Operator
 * File     : cilOperatorunarybitwiseNot.cpp
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
#ifdef OP
	#undef OP
#endif
#define OP ~

/***************************************************************************
 *      Methods
 ***************************************************************************/
/***************************************************************************
 * unarybitwiseNot Operation
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::unaryBitwiseNotOperator( CCgNodeData* lhs )
{
	assert( lhs != NULL );

	//rhs can be null
	CCgNodeData* Node;

	Node = makeFormulaNode( CG_UNARYBITWISENOT, lhs, NULL );

	return Node;
};

/***************************************************************************
 * emit unarybitwiseNot Operation
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitUnaryBitwiseNotOperator( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );
	assert( nodetree->lhsNode != NULL );
	assert( nodetree->rhsNode == NULL );
	
	CCgNodeData* ptr = nodetree->lhsNode;
	if( isValue( ptr ) )
	{
		//Special case for both values.
		//Create new node
		if( isFloat( ptr ) )
		{
			double dLhs = m_FloatConstantPool[ ptr->iIndex ];
			ptr = emitLoadStack( integerConstant( OP (int64_t)dLhs ) );
			delete ptr;
			return nodetree;
		}
		else
		{
			int64_t iLhs = m_IntegerConstantPool[ ptr->iIndex ];
			ptr = emitLoadStack( integerConstant( OP iLhs ) );
			delete ptr;
			return nodetree;
		}	
	}

	//Left hand side
	emitLoadStack( ptr );

	//unarybitwiseNot to stack entries
	emit( CEE_NOT );

	return nodetree;
}

} //namespace CRI