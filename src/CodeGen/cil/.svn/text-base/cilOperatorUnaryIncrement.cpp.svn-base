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
 * Module   : unaryInc/Dec Operator
 * File     : cilOperatorunaryIncrement.cpp
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
 * unaryInc Operation
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::unaryIncOperator( CCgNodeData* lhs, bool bDup )
{
	assert( lhs != NULL );

	//rhs can be null
	CCgNodeData* Node;

	if( bDup )
		Node = makeFormulaNode( CG_UNARYPOSTINC, lhs, NULL );
	else
		Node = makeFormulaNode( CG_UNARYINC, lhs, NULL );

	return Node;
};

/***************************************************************************
 * unaryDec Operation
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::unaryDecOperator( CCgNodeData* lhs, bool bDup  )
{
	assert( lhs != NULL );

	//rhs can be null
	CCgNodeData* Node;

	if( bDup )
		Node = makeFormulaNode( CG_UNARYPOSTDEC, lhs, NULL );
	else
		Node = makeFormulaNode( CG_UNARYDEC, lhs, NULL );

	return Node;
};

/***************************************************************************
 * emit unaryInc Operation
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitUnaryIncOperator( CCgNodeData* nodetree, bool bDup  )
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
			ptr = emitLoadStack( floatConstant( dLhs + 1.f ) );
			delete ptr;
			return nodetree;
		}
		else
		{
			int64_t iLhs = m_IntegerConstantPool[ ptr->iIndex ];
			ptr = emitLoadStack( integerConstant( iLhs + 1 ) );
			delete ptr;
			return nodetree;
		}	
	}

	//Left hand side
	emitLoadStack( ptr );
	if( bDup )
	{
		//Post increment
		emit( CEE_DUP );
		m_bResultInEvalStack = true;
	}
	else
		m_bResultInEvalStack = false;

	emit( CEE_EXT_INC );

	return nodetree;
}

/***************************************************************************
 * emit unaryDec Operation
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitUnaryDecOperator( CCgNodeData* nodetree, bool bDup  )
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
			ptr = emitLoadStack( floatConstant( dLhs - 1.f ) );
			delete ptr;
			return nodetree;
		}
		else
		{
			int64_t iLhs = m_IntegerConstantPool[ ptr->iIndex ];
			ptr = emitLoadStack( integerConstant( iLhs - 1 ) );
			delete ptr;
			return nodetree;
		}	
	}

	//Left hand side
	emitLoadStack( ptr );
	if( bDup )
	{
		//Post decrement
		emit( CEE_DUP );
		m_bResultInEvalStack = true;
	}
	else
		m_bResultInEvalStack = false;

	emit( CEE_EXT_DEC );
//	emit( CEE_LDC_I4_M1 );

	//Add 2 stack entries
//	emit( CEE_ADD );

	return nodetree;
}

} //namespace CRI