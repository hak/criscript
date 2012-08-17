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
 * Module   : Reminder Operator
 * File     : cilOperatorReminder.cpp
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
#define OP %

/***************************************************************************
 *      Methods
 ***************************************************************************/
/***************************************************************************
 * Reminder Operation
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::reminderOperator( CCgNodeData* lhs, CCgNodeData* rhs )
{
	assert( lhs != NULL );
	if( rhs == NULL ) return lhs;	//fast pass
	//rhs can be null

	return makeFormulaNode( CG_REM, lhs, rhs );;
};

/***************************************************************************
 * emit Reminder Operation
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitReminderOperator( CCgNodeData* nodetree )
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
			double dLhs = m_FloatConstantPool[ ptr->iIndex ];
			if ( isFloat( nodetree->rhsNode ) )
			{
				double dRhs = m_FloatConstantPool[ nodetree->rhsNode->iIndex ];
				ptr = emitLoadStack( floatConstant( fmod( dLhs, dRhs ) ) );
				delete ptr;
				return nodetree;
			}
			else
			{
				double dRhs = (double)m_IntegerConstantPool[ nodetree->rhsNode->iIndex ];
				ptr = emitLoadStack( floatConstant( fmod( dLhs, dRhs ) ) );
				delete ptr;
				return nodetree;
			}
		}
		else
		{
			int64_t iLhs = m_IntegerConstantPool[ ptr->iIndex ];
			if ( isFloat( nodetree->rhsNode ) )
			{
				double dRhs = m_FloatConstantPool[ nodetree->rhsNode->iIndex ];
				ptr = emitLoadStack( floatConstant( fmod( double(iLhs), dRhs ) ) );
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
	
	emitLoadStack( ptr );

	//Right hand side
	ptr = nodetree->rhsNode;
	emitLoadStack( ptr );
	
	//Reminder 2 stack entries
	emit( CEE_REM );

	return nodetree;
}

} //namespace CRI