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
 * Module   : unaryVoid Operator
 * File     : cilOperatorunaryVoid.cpp
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
 * New Operation
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::newOperator( CCgNodeData* newExpression, CCgNodeData* Arguments )
{
	assert( newExpression != NULL );

	//rhs can be null
	CCgNodeData* Node;

	// A new node is
	// expression + arg list
	// Since expression node is in lhs but rhs is processed first when parsing tree,
	// it's switching rhs and lhs here.
	Node = makeFormulaNode( CG_NEW, Arguments, newExpression, true, true );

	return Node;
};

/***************************************************************************
 * emit New Operation
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitNewOperator( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );

	uint32_t iNumArgs = 0;
	CCgNodeData* ptr = nodetree->lhsNode;
	//Left hand side = parameter list
	if( ptr != NULL )
		nodetree->lhsNode = emitArgumentList( ptr, iNumArgs );
	else
		emit( CEE_EXT_STARGLIST, (uint32_t)0 );

	ptr = nodetree->rhsNode;
	
	if( ptr->NodeType == CG_NODE_IDENTIFIER )
	{
		CG_SYMBOL_INFORMATION* info = NULL;
		wstring strScopedVariableName = getCurrentScopeName() + *ptr->pIdentifier;
		RID ridSrc = lookUpSymbolInformation( strScopedVariableName, &info );
		if( ridSrc == RID_NOTDEFINED ) {
			//Symbol name not found
			reportError( ERROR_FUNCTIONNAMENOTFOUND, *ptr->pIdentifier,
							ptr->iFileId,
							ptr->iLineNumber,
							ptr->iColumn );
			return nodetree;
		}

		switch( TypeFromToken( ridSrc ) )
		{
		case MDT_FIELDDEF:
			{
				if( TypeFromToken( info->ridObject ) != MDT_TYPEDEF )
				{
					//Symbol name not found
					reportError( INFO_INVOKINGVARIABLE, *ptr->pIdentifier,
								ptr->iFileId,
								ptr->iLineNumber,
								ptr->iColumn );

					emit( CEE_NEWOBJ, ridSrc );
				}
				else
				{
					ridSrc = info->ridObject;
					//They should be Global variable of built in object

					//Note:
					//Function ctor is not supported this version since,
					//1) Security. Can not make sure every on the fly generated code works.
					//2) Lexer does not have multiple instances feature.
					if( ridSrc == m_ridFunctionObject
						&& iNumArgs )
					{
						reportError( ERROR_FUNCTIONCTORNOTSUPPORTED, ptr->iFileId, ptr->iLineNumber, ptr->iColumn );
					}
					CObjectTypeInfo obj = m_MetaData.getBuiltinObjectTable()[ IndexFromBuiltinRid( ridSrc ) ];
					if( obj.getConstructor() != NULL )
					{
						emit( CEE_NEWOBJ, ridSrc );
					}
					else
					{
						reportError( ERROR_NOOBJECTCTOR, *ptr->pIdentifier, ptr->iFileId, ptr->iLineNumber, ptr->iColumn );
					}
				}
				break;
			}
		case MDT_TYPEDEF:
			{
			//Note:
			//Function ctor is not supported this version since,
			//1) Security. Can not make sure every on the fly generated code works.
			//2) Lexer does not have multiple instances feature.
			if( ridSrc == m_ridFunctionObject
				&& iNumArgs )
			{
				reportError( ERROR_FUNCTIONCTORNOTSUPPORTED, ptr->iFileId, ptr->iLineNumber, ptr->iColumn );
			}
			CObjectTypeInfo obj = m_MetaData.getBuiltinObjectTable()[ IndexFromBuiltinRid( ridSrc ) ];
			if( obj.getConstructor() != NULL )
			{
				emit( CEE_NEWOBJ, ridSrc );
			}
			else
			{
				reportError( ERROR_NOOBJECTCTOR, *ptr->pIdentifier, ptr->iFileId, ptr->iLineNumber, ptr->iColumn );
			}
			}
			break;
		case MDT_METHODDEF:
			//Not a builtin object
			emit( CEE_NEWOBJ, ridSrc );	//Will leave an object on the stack
			break;
		default:
			Debug_Fatal( "Not implemented yet" );
		}

	}
	else
	{
		//parse the node as a 'new' parameter node
		nodetree->rhsNode = parseNodeTreeSub( ptr, nodetree, false );
	}

	return nodetree;
}

} //namespace CRI