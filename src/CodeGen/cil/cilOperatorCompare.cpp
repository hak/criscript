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
 * Module   : Equal Operator
 * File     : cilOperatorCompare.cpp
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
 * Equal Operation
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::cmpOperator( CCgNodeData* lhs, CCgNodeData* rhs, CG_OPERATIONTYPE iOpCode )
{
	assert( lhs != NULL );
	assert( rhs != NULL );
	CCgNodeData* Node = makeFormulaNode( iOpCode, lhs, rhs );
	return Node;
};

/***************************************************************************
 * emit Equal Operation
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitCmpOperator( CCgNodeData* nodetree, uint32_t iOpCode )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );

	CCgNodeData* ptr = nodetree->lhsNode;
	if( isValue( ptr ) && isValue( nodetree->rhsNode ) )
	{
		//Special case for both values.
		int32_t iIndex  =  CONSTANT_ZERO;
		if( isFloat( ptr ) )
		{
			double dLhs = m_FloatConstantPool[ ptr->iIndex ];
			double dRhs;
			if( isFloat( nodetree->rhsNode ) )
			{
				dRhs = m_FloatConstantPool[ nodetree->rhsNode->iIndex ];
			}
			else
			{
				dRhs = (double)m_IntegerConstantPool[ nodetree->rhsNode->iIndex ];
			}
			switch( iOpCode )
			{
			case CEE_CEQ:
				if( dLhs == dRhs ) iIndex = CONSTANT_ONE;
				break;
			case CEE_EXT_CNE:
				if( dLhs != dRhs ) iIndex = CONSTANT_ONE;
				break;
			case CEE_CGT:
				if( dLhs > dRhs ) iIndex = CONSTANT_ONE;
				break;
			case CEE_EXT_CGE:
				if( dLhs >= dRhs ) iIndex = CONSTANT_ONE;
				break;
			case CEE_CLT:
				if( dLhs < dRhs ) iIndex = CONSTANT_ONE;
				break;
			case CEE_EXT_CLE:
				if( dLhs <= dRhs ) iIndex = CONSTANT_ONE;
				break;
			default:
				Debug_Fatal( "Not implemented yet" );
				break;
			}
		}
		else
		{
			int64_t iLhs = m_IntegerConstantPool[ ptr->iIndex ];
			if( isFloat( nodetree->rhsNode ) )
			{
				double dRhs = m_FloatConstantPool[ nodetree->rhsNode->iIndex ];
				switch( iOpCode )
				{
				case CEE_CEQ:
					if( double(iLhs) == dRhs ) iIndex = CONSTANT_ONE;
					break;
				case CEE_EXT_CNE:
					if( double(iLhs) != dRhs ) iIndex = CONSTANT_ONE;
					break;
				case CEE_CGT:
					if( double(iLhs) > dRhs ) iIndex = CONSTANT_ONE;
					break;
				case CEE_EXT_CGE:
					if( double(iLhs) >= dRhs ) iIndex = CONSTANT_ONE;
					break;
				case CEE_CLT:
					if( double(iLhs) < dRhs ) iIndex = CONSTANT_ONE;
					break;
				case CEE_EXT_CLE:
					if( double(iLhs) <= dRhs ) iIndex = CONSTANT_ONE;
					break;
				default:
					Debug_Fatal( "Not implemented yet" );
					break;
				}
			}
			else
			{
				int64_t iRhs = (int64_t)m_IntegerConstantPool[ nodetree->rhsNode->iIndex ];
				switch( iOpCode )
				{
				case CEE_CEQ:
					if( iLhs == iRhs ) iIndex = CONSTANT_ONE;
					break;
				case CEE_EXT_CNE:
					if( iLhs != iRhs ) iIndex = CONSTANT_ONE;
					break;
				case CEE_CGT:
					if( iLhs > iRhs ) iIndex = CONSTANT_ONE;
					break;
				case CEE_EXT_CGE:
					if( iLhs >= iRhs ) iIndex = CONSTANT_ONE;
					break;
				case CEE_CLT:
					if( iLhs < iRhs ) iIndex = CONSTANT_ONE;
					break;
				case CEE_EXT_CLE:
					if( iLhs <= iRhs ) iIndex = CONSTANT_ONE;
					break;
				default:
					Debug_Fatal( "Not implemented yet" );
					break;
				}
			}
		}
		if( iIndex == CONSTANT_ONE )
			emit( CEE_LDSFLD, RID_CONSTANT_BOOL_TRUE );
		else
			emit( CEE_LDSFLD, RID_CONSTANT_BOOL_FALSE );

//		emitLoadInteger( m_IntegerConstantPool[ iIndex ] );
		delete nodetree;
		return NULL;
	}
	
	emitLoadStack( ptr );

	//Right hand side
	ptr = nodetree->rhsNode;
	emitLoadStack( ptr );

	//Compare 2 stack entries
	emit( (OP_CODE)iOpCode );

	return nodetree;
}

} //namespace CRI