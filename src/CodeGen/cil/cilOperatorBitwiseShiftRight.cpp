/****************************************************************************
 *
 * CRI Middleware SDK
 *
 * CopyRight (c) 2005-2008 CRI Middleware Co., Ltd.
 *
 *
 *
 * Library  : CRIScript
 * Module   : shiftRight Operator
 * File     : cilOperatorshiftRight.cpp
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
 * shiftRight Operation
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::shiftRightOperator( CCgNodeData* lhs, CCgNodeData* rhs )
{
	assert( lhs != NULL );
	if( rhs == NULL ) return lhs;	//fast pass

	//rhs can be null
	CCgNodeData* Node;

	Node = makeFormulaNode( CG_SHR, lhs, rhs );

	return Node;
};

/***************************************************************************
 * emit shiftRight Operation
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitShiftRightOperator( CCgNodeData* nodetree )
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
			int64_t iLhs = (int64_t)m_FloatConstantPool[ ptr->iIndex ];
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
			int64_t iLhs = m_IntegerConstantPool[ ptr->iIndex ];
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

	//Right hand side
	emitLoadStack( ptr );

	//Right hand side
	ptr = nodetree->rhsNode;
	emitLoadStack( ptr );

	//shiftRight 2 stack entries
	emit( CEE_SHR );

	return nodetree;
}

} //namespace CRI