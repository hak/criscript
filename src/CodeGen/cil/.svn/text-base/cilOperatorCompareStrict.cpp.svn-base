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
 * File     : cilOperatorCompareStrict.cpp
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
 * emit Strict Equal Operation
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitStrictCmpOperator( CCgNodeData* nodetree, uint32_t iOpCode )
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
				switch( iOpCode )
				{
				case CEE_CEQ:
					if( dLhs == dRhs ) iIndex = CONSTANT_ONE;
					break;
				case CEE_EXT_CNE:
					if( dLhs != dRhs ) iIndex = CONSTANT_ONE;
					break;
				default:
					Debug_Fatal( "Not implemented yet" );
					break;
				}
			}
			else
			{
				switch( iOpCode )
				{
				case CEE_CEQ:
					//Not equal
					break;
				case CEE_EXT_CNE:
					//Not equal
					iIndex = CONSTANT_ONE;
					break;
				default:
					Debug_Fatal( "Not implemented yet" );
					break;
				}
			}
		}
		else
		{
			int64_t iLhs = m_IntegerConstantPool[ ptr->iIndex ];
			if( isFloat( nodetree->rhsNode ) )
			{
				switch( iOpCode )
				{
				case CEE_CEQ:
					//Not equal
					break;
				case CEE_EXT_CNE:
					//Not equal
					iIndex = CONSTANT_ONE;
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

	emit( CEE_EXT_STARGLIST_S, (uint8_t)2 );	//Move to arglist
	
	switch( iOpCode )
	{
	case CEE_CEQ:
		emit( CEE_CALL, m_ridMethodStrictEqual );
		break;
	case CEE_EXT_CNE:
		emit( CEE_CALL, m_ridMethodStrictNotEqual );
		break;
	default:
		Debug_Fatal( "Not implemented yet" );
		break;
	}

	return nodetree;
}

} //namespace CRI