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
 * Module   : shiftRightUnsigned Operator
 * File     : cilOperatorshiftRightUnsigned.cpp
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
#define OP >>

/***************************************************************************
 *      Methods
 ***************************************************************************/
/***************************************************************************
 * shiftRightUnsigned Operation
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::shiftRightUnsignedOperator( CCgNodeData* lhs, CCgNodeData* rhs )
{
	assert( lhs != NULL );
	if( rhs == NULL ) return lhs;	//fast pass

	//rhs can be null
	CCgNodeData* Node;

	Node = makeFormulaNode( CG_SHRUN, lhs, rhs );

	return Node;
};

/***************************************************************************
 * emit shiftRightUnsigned Operation
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitShiftRightUnsignedOperator( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );

	CCgNodeData* ptr = nodetree->lhsNode;
	if( isValue( ptr ) && isValue( nodetree->rhsNode ) )
	{
		//Special case for both values.
		//Create new node
		if( isFloat( ptr ) )
		{
			uint64_t iLhs = (uint64_t)m_FloatConstantPool[ ptr->iIndex ];
			if ( isFloat( nodetree->rhsNode ) )
			{
				int64_t iRhs = (int64_t)m_FloatConstantPool[ nodetree->rhsNode->iIndex ];
				ptr = emitLoadStack( integerConstant( iLhs OP iRhs ) );
				delete ptr;
				return nodetree;
			}
			else
			{
				int64_t iRhs = (int64_t)m_IntegerConstantPool[ nodetree->rhsNode->iIndex ];
				ptr = emitLoadStack( integerConstant( iLhs OP iRhs ) );
				delete ptr;
				return nodetree;
			}
		}
		else
		{
			uint64_t iLhs = m_IntegerConstantPool[ ptr->iIndex ];
			if ( isFloat( nodetree->rhsNode ) )
			{
				int64_t iRhs = (int64_t)m_FloatConstantPool[ nodetree->rhsNode->iIndex ];
				ptr = emitLoadStack( integerConstant( iLhs OP iRhs ) );
				delete ptr;
				return nodetree;
			}
			else
			{
				int64_t iRhs = m_IntegerConstantPool[ nodetree->rhsNode->iIndex ];
				ptr = emitLoadStack( integerConstant( iLhs OP iRhs ) );
				delete ptr;
				return nodetree;
			}
		}	
	}

	//Left hand side
	emitLoadStack( ptr );

	//Right hand side
	ptr = nodetree->rhsNode;
	emitLoadStack( ptr );

	//shiftRightUnsigned 2 stack entries
	emit( CEE_SHR_UN );

	return nodetree;
}

} //namespace CRI