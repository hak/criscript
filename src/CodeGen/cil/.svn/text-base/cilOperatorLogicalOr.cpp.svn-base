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
 * Module   : bitwiseAnd Operator
 * File     : cilOperatorlogicalOr.cpp
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
#define OP ||

/***************************************************************************
 *      Methods
 ***************************************************************************/
/***************************************************************************
 * logicalAnd Operation
 * ECMA262 11.11
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::logicalOrOperator( CCgNodeData* lhs, CCgNodeData* rhs )
{
	assert( lhs != NULL );
	assert( rhs != NULL );

	CCgNodeData* Node;

	Node = makeFormulaNode( CG_LOGICALOR, lhs, rhs, true, true );

	return Node;
};

/***************************************************************************
 * emit logicalAnd Operation
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitLogicalOrOperator( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );
	CCgNodeData* ptr = nodetree->lhsNode;
	if( isValue( ptr ) && isValue( nodetree->rhsNode ) )
	{
		//Special case when both nodes are value.
		//Create new node
		if( isFloat( ptr ) )
		{
			double fLhs = m_FloatConstantPool[ ptr->iIndex ];
			if( fLhs )
			{
				ptr = emitLoadStack( floatConstant( fLhs ) );
				delete ptr;
				return nodetree;
			}
			else
			{
				if ( isFloat( nodetree->rhsNode ) )
				{
					double fRhs = m_FloatConstantPool[ nodetree->rhsNode->iIndex ];
					ptr = emitLoadStack( floatConstant( fRhs ) );
					delete ptr;
					return nodetree;
				}
				else
				{
					int64_t iRhs = (int64_t)m_IntegerConstantPool[ nodetree->rhsNode->iIndex ];
					ptr = emitLoadStack( integerConstant( iRhs ) );
					delete ptr;
					return nodetree;
				}
			}
		}
		else
		{
			int64_t iLhs = m_IntegerConstantPool[ ptr->iIndex ];
			if( iLhs )
			{
				ptr = emitLoadStack( integerConstant( iLhs ) );
				delete ptr;
				return nodetree;
			}
			else
			{
				if ( isFloat( nodetree->rhsNode ) )
				{
					double fRhs = m_FloatConstantPool[ nodetree->rhsNode->iIndex ];
					ptr = emitLoadStack( floatConstant( fRhs ) );
					delete ptr;
					return nodetree;
				}
				else
				{
					int64_t iRhs = (int64_t)m_IntegerConstantPool[ nodetree->rhsNode->iIndex ];
					ptr = emitLoadStack( integerConstant( iRhs ) );
					delete ptr;
					return nodetree;
				}
			}
		}	
	}

	//LHS
	if( isValue( ptr ) )
	{
		if( isFloat( ptr ) )
		{
			double dLhs = m_FloatConstantPool[ ptr->iIndex ];
			if( dLhs != 0 )
			{
				//True
				//LHS should be 0
				ptr = emitLoadStack( floatConstant( dLhs ) );
				delete ptr;
				return nodetree;
			}
		}
		else
		{
			int64_t iLhs = m_IntegerConstantPool[ ptr->iIndex ];
			if( iLhs != 0 )
			{
				//True
				//LHS should be 0
				ptr = emitLoadStack( integerConstant( iLhs ) );
				delete ptr;
				return nodetree;
			}
		}

		//Just emit only right hand side
		nodetree->rhsNode = parseExpressionNodeTree( nodetree->rhsNode, false );
		return nodetree;
	}

	uint32_t iLabelFalse = createNewLabelID();
	//Left hand side
	nodetree->lhsNode = parseExpressionNodeTree( nodetree->lhsNode, false );

	ILFRAGMENTINFO& info = getCurrentILInfo();
	uint32_t iByteCode = info.ILPool[ m_iLastILOffset[ 1 ] ]; 
	if( iByteCode == 0xfe ) iByteCode = 0xfe00 | info.ILPool[ m_iLastILOffset[ 1 ] + 1 ]; 

	if( !m_iLabelEndLoop )
	{
		//Optimization.
		//If last operation is a logical condition check, no needs to check it again.
		if( iByteCode == CEE_EXT_CLE
			|| iByteCode == CEE_EXT_CGE
			|| iByteCode == CEE_CLT
			|| iByteCode == CEE_CGT
			|| iByteCode == CEE_EXT_CNE
			|| iByteCode == CEE_CEQ )
		{
			//Regular pass
			emit( CEE_DUP );
			emit( CEE_BRTRUE, iLabelFalse );	//Pop 1

			emit( CEE_POP );
		}
		else
		{
			//Regular pass
			emit( CEE_DUP );
			emit( CEE_EXT_CONV_B );
			emit( CEE_LDC_I4_0 );
			emit( CEE_EXT_CNE );
			emit( CEE_BRTRUE, iLabelFalse );	//Pop 1

			emit( CEE_POP );
		}

		//Right hand side
		nodetree->rhsNode = parseExpressionNodeTree( nodetree->rhsNode, false );

		insertLabelTag( iLabelFalse );
	}
	else
	{
		//Optimization.
		//If last operation is a logical condition check, no needs to check it again.
		if( iByteCode == CEE_EXT_CLE
			|| iByteCode == CEE_EXT_CGE
			|| iByteCode == CEE_CLT
			|| iByteCode == CEE_CGT
			|| iByteCode == CEE_EXT_CNE
			|| iByteCode == CEE_CEQ )
		{
			//Can jump to end of the loop
			emit( CEE_BRFALSE, m_iLabelEndLoop );	//Pop 1
		}
		else
		{
			//Can jump to end of the loop
			emit( CEE_EXT_CONV_B );
			emit( CEE_LDC_I4_0 );
			emit( CEE_EXT_CNE );
			emit( CEE_BRFALSE, m_iLabelEndLoop );	//Pop 1
		}
		//Right hand side
		//m_iLabelEndLoop = 0;
		nodetree->rhsNode = parseExpressionNodeTree( nodetree->rhsNode, false );
	}

	return nodetree;
}

} //namespace CRI