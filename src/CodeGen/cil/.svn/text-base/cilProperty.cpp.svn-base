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
 * Module   : cilProperty.cpp
 * File     : Property access code for the object
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
 * PropetyAccess Operation
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::propertyAccessExpressionById( CCgNodeData* lhs, CCgNodeData* rhs )
{
	assert( lhs != NULL );
	assert( rhs != NULL );
	assert( rhs->NodeType == CG_NODE_IDENTIFIER );

	CCgNodeData* Node;
	Node = makeFormulaNode( CG_PROPERTYBYID, lhs, rhs, true, true );
	return Node;
};

/***************************************************************************
 * PropetyAccess Operation
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::propertyAccessExpressionByExp( CCgNodeData* lhs, CCgNodeData* rhs )
{
	assert( lhs != NULL );
	assert( rhs != NULL );

	CCgNodeData* Node;
	Node = makeFormulaNode( CG_PROPERTYBYEXP, lhs, rhs, true, true );
	return Node;
};

/***************************************************************************
 * emit LoadPropertyById Operation
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitAccessPropertyById( CCgNodeData* nodetree, CCgNodeData* parentnode )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );
	assert( nodetree->rhsNode->NodeType == CG_NODE_IDENTIFIER ||
			nodetree->rhsNode->NodeType == CG_NODE_NULL ||
			nodetree->rhsNode->NodeType == CG_NODE_STRINGLITERAL
			);

	bool bLoad = true;

	//TODO: Need a special case handling for a system function like,
	//String.charFrom()

	CCgNodeData* ptr;
	//Right hand side
	//identifier
	ptr = nodetree->rhsNode;
	switch( ptr->NodeType )
	{
	case CG_NODE_IDENTIFIER:
	{
		//Check prototype
		if( nodetree->rhsNode->pIdentifier->compare( STRING_INTERNAL_PROTOTYPE ) == 0 )
		{
			//Left hand side
			ptr = nodetree->lhsNode;
			//Should be Object reference
			emitLoadStack( ptr );
			//Load prototype
			emit( CEE_EXT_STARGLIST_S, (uint8_t)1 );	//Move to arglist
			//Load/Store prototype object
			if( bLoad )
			{
				//Calloperation put a value naturally
				emit( CEE_CALL, m_ridMethodLdPrototype );
			}
			else
			{
				//Calloperation put a value naturally
				emit( CEE_CALL, m_ridMethodStPrototype );	
				adjustEvalStack( -1 );
			}
			//Load/Store prototype object
			return nodetree;
		}

		//Generate RID for the string
		CCgNodeData* tmpNode = literalConstant( ptr->pIdentifier->data() );
		emit( CEE_LDC_I4, tmpNode->rid );
		delete tmpNode;
	}
	break;
	case CG_NODE_NULL:
		//Do nothing
		break;
	default:
	{
		if( m_mapStringConsantPool[ STRING_INTERNAL_PROTOTYPE ] == ptr->rid )
		{
			//Left hand side
			ptr = nodetree->lhsNode;
			//Should be Object reference
			emitLoadStack( ptr );
			//Load prototype
			emit( CEE_EXT_STARGLIST_S, (uint8_t)1 );	//Move to arglist
			if( bLoad )
			{
				//Calloperation put a value naturally
				emit( CEE_CALL, m_ridMethodLdPrototype );
			}
			else
			{
				//Calloperation put a value naturally
				emit( CEE_CALL, m_ridMethodStPrototype );	
				adjustEvalStack( -1 );
			}
			//Load/Store prototype object
			return nodetree;
		}

		//String literal
		emit( CEE_LDC_I4, ptr->rid );
	}
	}

	//Left hand side
	ptr = nodetree->lhsNode;
	//Should be Object reference

	//Recursive call
	nodetree->lhsNode = parseNodeTreeSub( nodetree->lhsNode,
											nodetree,
											false );

	if( parentnode )
	{
		if( parentnode->NodeType == CG_NODE_FORMULANODE &&
			( parentnode->iOperation == CG_VARIABLEASSIGN 
			|| parentnode->iOperation == CG_UNARYDELETE 
			)
			)
		{
			if( parentnode->lhsNode == nodetree )
			{
				//Yes, I'm lefthand side, should store to the property
				bLoad = false;
			}
		}
	}
	
	if( bLoad )
	{
		//Load property with the ID
		emit( CEE_EXT_STARGLIST_S, (uint8_t)2 );	//Move to arglist
		emit( CEE_CALL, m_ridMethodLdProperty );
	}
	else
	{
		//Load property with the ID
		emit( CEE_EXT_STARGLIST_S, (uint8_t)3 );	//Move to arglist
		//Calloperation put a value on the stack
		emit( CEE_CALL, m_ridMethodStProperty );	

		//
		//-1 means, the API does not put the result in the stack.
		//
		adjustEvalStack( -1 );
	}

	return nodetree;
}

/***************************************************************************
 * emit LoadPropertyByExpression Operation
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitAccessPropertyByExp( CCgNodeData* nodetree, CCgNodeData* parentnode )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );

	bool bLoad = true;

	CCgNodeData* ptr;
	//Right hand side
	ptr = nodetree->rhsNode;
	if( ptr->NodeType == CG_NODE_STRINGLITERAL )
	{
		//Short cut case for string literal access
		return emitAccessPropertyById( nodetree, parentnode );
	}

	nodetree->rhsNode = parseNodeTreeSub( ptr,
							nodetree,
							false );

	//Left hand side
	ptr = nodetree->lhsNode;
	
	//Should be Object reference
	nodetree->lhsNode = parseNodeTreeSub( nodetree->lhsNode,
											nodetree,
											false );
	if( parentnode )
	{
		if( parentnode->NodeType == CG_NODE_FORMULANODE &&
			parentnode->iOperation == CG_VARIABLEASSIGN )
		{
			if( parentnode->lhsNode == nodetree )
			{
				//Yes, I'm lefthand side, should store to the property
				bLoad = false;
			}
		}
	}
	
	if( bLoad )
	{
		//Load property with the ID
		emit( CEE_EXT_STARGLIST_S, (uint8_t)2 );	//Move to arglist
		emit( CEE_CALL, m_ridMethodLdPropertyByExpression );
	}
	else
	{
		//Load property with the ID
		emit( CEE_EXT_STARGLIST_S, (uint8_t)3 );	//Move to arglist
		emit( CEE_CALL, m_ridMethodStPropertyByExpression );	
		//
		//-1 means, the API does not put the result in the stack.
		//
		adjustEvalStack( -1 );
	}

	return nodetree;
}

} //namespace CRI