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
 * Module   : With Statement
 *			Currently the VM doest not support dynamic context With Statement
 * File     : cilStatementWith.cpp
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
 *      with statement node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::withStatement( CCgNodeData* identifier, CCgNodeData* statement )
{
	assert( identifier != NULL );
	CCgNodeData* object = new CCgNodeData;
	object->NodeType = CG_NODE_WITHNODE;
	object->conditionNode = identifier;
	object->statementNode = statement;
	object->iLineNumber = m_pLex->getLineNumber();
	return object;
}

/***************************************************************************
//Create parse with statement node 
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseWithStatementNode( CCgNodeData* withnode )
{
	assert( withnode != NULL );
	assert( withnode->NodeType == CG_NODE_WITHNODE );
	assert( withnode->conditionNode != NULL );

	if( withnode->conditionNode->NodeType == CG_NODE_IDENTIFIER )
	{
		CG_SYMBOL_INFORMATION* info = NULL;
		wstring strScopedVariableName = getCurrentScopeName() + *withnode->conditionNode->pIdentifier;
		RID ridSrc = lookUpSymbolInformation( strScopedVariableName, &info );
		if( ridSrc == RID_NOTDEFINED ) {
			//Forward decl is not supported yet.
			//Symbol name not found
			reportError( ERROR_OBJECTNAMENOTFOUND, *withnode->conditionNode->pIdentifier,
							withnode->conditionNode->iFileId,
							withnode->conditionNode->iLineNumber,
							withnode->conditionNode->iColumn );
			return withnode;
		}

		switch( TypeFromToken( ridSrc ) )
		{
		case MDT_TYPEDEF:
			//Only type def is accepted as an argument
			break;
		default:
			reportError( ERROR_WITHONLYACCEPTOBJREF,
							withnode->conditionNode->iFileId,
							withnode->conditionNode->iLineNumber,
							withnode->conditionNode->iColumn );
			break;
		}
	}
	//
	//Push objectref node to the With statement stack
	//
	m_WithStatementLookupStack.push_back( withnode->conditionNode );

	//Do some optimizations work here
	switch( withnode->statementNode->NodeType )
	{
	case CG_NODE_IDENTIFIER:
	case CG_NODE_STRINGLITERAL:
	case CG_NODE_REGEXLITERAL:
	case CG_NODE_INTEGERLITERAL:
	case CG_NODE_FLOATLITERAL:
		//Has no effect
		reportError( INFO_CODE_NOEFFECT,
			withnode->statementNode->iFileId,
			withnode->statementNode->iLineNumber,
			withnode->statementNode->iColumn );
		break;
	default:
		withnode->statementNode = parseStatementNodeTree( withnode->statementNode );
		break;
	}
	
	if( withnode->siblingNode )
	{
		withnode->siblingNode = parseStatementNodeTree( withnode->siblingNode ); 
	}
	
	//
	//Pop objectref node to the With statement stack
	//
	m_WithStatementLookupStack.pop_back();

	delete withnode;
	return NULL;
}

} //namespace CRI