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
 * Module   : cil codegenerater
 * File     : cilobjectLiteral.cpp
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
 * Create constant node leaf
 ***************************************************************************/	
/***************************************************************************
 * Object literal creation
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::objectLiteral( CCgNodeData* node )
{
	//node can be null
	CCgNodeData* Node;
	Node = makeFormulaNode( CG_objectLiteral, node, NULL, true, true );	//Block traversal of both child node

	return Node;
};


/***************************************************************************
 * Property pair initialization
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::propertyNameAndValue( CCgNodeData* lhs, CCgNodeData* rhs)
{
//	assert( lhs != NULL );
	//lhs can be null for array initializer
	assert( rhs != NULL );

	//node can be null
	CCgNodeData* Node;
	Node = makeFormulaNode( CG_PROPERTYNAMEANDVALUE, lhs, rhs, true, true );	//Block traversal of both child node

	return Node;
}

/***************************************************************************
//emit object literal
 ***************************************************************************/
CCgNodeData* CCilCodeGen::emitObjectLiteral( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );

	uint32_t iNumArguments = 0;

	//emit node tree to evalstack
	//StringRid, Expression, String rid, Expression....

	map< RID, bool >mapSanityCheck;
	nodetree->lhsNode = parseobjectLiteralParameterNodeTree( nodetree->lhsNode, iNumArguments, mapSanityCheck );
	mapSanityCheck.clear();

	if( iNumArguments <= MAXBYTE )
		emit( CEE_EXT_STARGLIST_S, (uint8_t)iNumArguments );	//Move to arglist
	else
		emit( CEE_EXT_STARGLIST, (uint32_t)iNumArguments );	//Move to arglist
	emit( CEE_CALL, m_ridMethodLdobjectLiteral );		//Function Object Constructor

	return nodetree;
};

/***************************************************************************
 * parse objectLiteral initialization param tree
  ***************************************************************************/
CCgNodeData* CCilCodeGen::parseobjectLiteralParameterNodeTree( CCgNodeData* nodetree,
															  uint32_t& iNumArguments,
															  map< RID, bool >&mapSanityCheck )
{
	if( nodetree == NULL ) return NULL;
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );
	assert( nodetree->iOperation == CG_PROPERTYNAMEANDVALUE );

	if( nodetree->siblingNode != NULL )
	{
		nodetree->siblingNode = parseobjectLiteralParameterNodeTree( nodetree->siblingNode, iNumArguments, mapSanityCheck );
	}

	RID rid;

	CCgNodeData* currentNode = nodetree->lhsNode;
	switch( currentNode->NodeType )
	{
		case CG_NODE_IDENTIFIER:
		{
			//Generate RID for the string
			CCgNodeData* tmpNode = literalConstant( currentNode->pIdentifier->data() );
			rid = tmpNode->rid;
			delete tmpNode;
			if( mapSanityCheck[ rid ] == true )
			{
				reportError( INFO_OBJECTINITIALIZORDUPLICATION, *currentNode->pIdentifier,
							currentNode->iFileId,
							currentNode->iLineNumber,
							currentNode->iColumn );
			}
			else
			{
				mapSanityCheck[ rid ] = true;
			}
		}
		break;
		case CG_NODE_STRINGLITERAL:
		{
			//String literal
			rid = currentNode->rid;
			if( mapSanityCheck[ rid ] == true )
			{
				reportError( INFO_OBJECTINITIALIZORDUPLICATION, m_MetaData.getStringConstant( rid ),
							currentNode->iFileId,
							currentNode->iLineNumber,
							currentNode->iColumn );
			}
			else
			{
				mapSanityCheck[ rid ] = true;
			}
		}
		break;
		case CG_NODE_INTEGERLITERAL:
		{
			//Generate RID for the string
			wchar_t strBuffer[ 22 ];		//22=max value width of longlong
			swprintf( strBuffer, 22, L"%d",  m_IntegerConstantPool[ currentNode->iIndex ] );
			CCgNodeData* tmpNode = literalConstant( strBuffer );
			rid = tmpNode->rid;
			delete tmpNode;
			if( mapSanityCheck[ rid ] == true )
			{
				reportError( INFO_OBJECTINITIALIZORDUPLICATION, strBuffer,
							currentNode->iFileId,
							currentNode->iLineNumber,
							currentNode->iColumn );
			}
			else
			{
				mapSanityCheck[ rid ] = true;
			}
		}
		break;
		default:
			Debug_Fatal( "Illegal operand" );
			break;
	}
	nodetree->rhsNode = parseExpressionNodeTree( nodetree->rhsNode, false );
	iNumArguments ++;

	emit( CEE_LDC_I4, rid );
	iNumArguments ++;

	delete nodetree;
	return NULL;
}

} //namespace CRI