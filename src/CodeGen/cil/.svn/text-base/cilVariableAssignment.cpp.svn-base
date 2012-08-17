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
 * File     : cilVariableAssignement.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criscript.h"
#include "cilCodeGen.h"
namespace cri {
	
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 * Variable decl
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::variableAssignment( CCgNodeData* lhs, CCgNodeData* rhs )
{
	assert( lhs != NULL );
	//rhs can be null
//	return makeFormulaNode( CG_VARIABLEASSIGN, lhs, rhs, true, false );	//Block traverse for 4th arg
//	return makeFormulaNode( CG_VARIABLEASSIGN, lhs, rhs, false, false );	//Block traverse for 4th arg
	return makeFormulaNode( CG_VARIABLEASSIGN, lhs, rhs, true, true );	//Block traverse for 4th arg
};

/***************************************************************************
 * emit Variable decl CIL
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitVariableAssignment( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );
	assert( nodetree->lhsNode != NULL );
	//rhs can be null

	if( nodetree->lhsNode->NodeType == CG_NODE_BOOLEANLITERAL ||
		nodetree->lhsNode->NodeType == CG_NODE_FLOATLITERAL ||
		nodetree->lhsNode->NodeType == CG_NODE_INTEGERLITERAL ||
		nodetree->lhsNode->NodeType == CG_NODE_STRINGLITERAL ||
		nodetree->lhsNode->NodeType == CG_NODE_REGEXLITERAL
		)
	{
		//Can not assigned to the constant
		reportError( ERROR_CANNOTASSIGNCONSTANT,
					nodetree->iFileId,
					nodetree->iLineNumber,
					nodetree->iColumn );
		return nodetree;
	}

	switch( nodetree->lhsNode->NodeType )
	{
	case CG_NODE_IDENTIFIER:
		{
		//Error check if the variable is already decleared.
		CCgNodeData* objectptr = nodetree->lhsNode;
		RID ridDest;
		CG_SYMBOL_INFORMATION* pInfo;
		wstring strScopedVariableName = getCurrentScopeName() + *objectptr->pIdentifier;
		ridDest = lookUpSymbolInformation( strScopedVariableName, &pInfo );
		if( ridDest == RID_NOTDEFINED )
		{
			reportError( INFO_IDNOTREGISTERED, strScopedVariableName,
					objectptr->iFileId,
					objectptr->iLineNumber,
					objectptr->iColumn );

			//Check if the destination has restrict_ flag.
			//If so, "cast" source explicitly
			ridDest = m_MetaData.generateStaticFieldRID();
			generateSymbolInformation( ridDest, strScopedVariableName );
			lookUpSymbolInformation( strScopedVariableName, &pInfo );
		}
		if( SymbolAttributeFromFrag( pInfo->Flags ) & CG_SYMBOL_FLAG_READONLY )
		{
			reportError( ERROR_CANNOTASSIGNREADONLY, *objectptr->pIdentifier,
					objectptr->iFileId,
					objectptr->iLineNumber,
					objectptr->iColumn );
			return nodetree;
		}

		//Global scope variable
		if( nodetree->rhsNode == NULL )
		{
			if( !m_bTreatUnitializedValueAsZero )
			{
				//Make sure the variable is not assigned
				reportError( INFO_VARIABLENOTASSIGNED, strScopedVariableName,
					nodetree->iFileId,
					nodetree->iLineNumber,
					nodetree->iColumn );
				emit( CEE_LDSFLD, RID_CONSTANT_UNDEFINED );
			}
			else
			{
				//Treat undefined as '0';
				emit( CEE_LDC_I4_0 );
			}
		}
		else
		{
			CCgNodeData* ptr = nodetree->rhsNode;
			nodetree->rhsNode = parseNodeTreeSub( nodetree->rhsNode, nodetree, false );
		}

		//Need to dup the eval stack top entry for further expression
		//OPTIMIZAITON: remove redundant dup and pop
		if( m_bResultInEvalStack == false )
		{
			emit( CEE_DUP );
		}
		m_bResultInEvalStack = true;

		switch( SymbolTypeFromFrag( pInfo->Flags ) )
		{
		case CG_SYMBOL_FLAG_STATIC:
			if( m_bTreatFunctionNameReadOnly && TypeFromToken( ridDest ) == MDT_METHODDEF )
			{
				reportError( ERROR_METHODNAMEASSIGNMENTNOTALLOWED, strScopedVariableName,
						nodetree->iFileId,
						nodetree->iLineNumber,
						nodetree->iColumn );
				return nodetree;
			}
			emit( CEE_STSFLD, ridDest );
			break;
		case CG_SYMBOL_FLAG_LOCAL:
			emitStoreLocal( pInfo );
			break;
		case CG_SYMBOL_FLAG_ARG:
			emitStoreArgument( pInfo );
			break;
		case CG_SYMBOL_FLAG_PROPERTY:
		case CG_SYMBOL_FLAG_INSTANCE:
		default:
			Debug_Fatal( "Not implemented yet" );
			break;
		}
		}
		break;
	case CG_NODE_FORMULANODE:
		{
		RID ridTemporary = reserveTemporaryVariable( CG_SYMBOL_FLAG_GLOBAL );
		bool bTempValueStored = false;

		if( nodetree->lhsNode->iOperation == CG_PROPERTYBYEXP )
		{
			//Traverse rhs node ( x[ this part ] ) first
			nodetree->lhsNode->rhsNode = parseNodeTreeSub( nodetree->lhsNode->rhsNode,
												nodetree->lhsNode,
												false );
			//Move it to tempvar
			emit( CEE_STSFLD, ridTemporary );
			bTempValueStored = true;
		}

		if( nodetree->rhsNode == NULL )
		{
			if( !m_bTreatUnitializedValueAsZero )
			{
				//Make sure the variable is not assigned
				reportError( INFO_VARIABLENOTASSIGNED, wstring( L"" ),
					nodetree->iFileId,
					nodetree->iLineNumber,
					nodetree->iColumn );
				emit( CEE_LDSFLD, RID_CONSTANT_UNDEFINED );
			}
			else
			{
				//Treat undefined as '0';
				emit( CEE_LDC_I4_0 );
			}
		}
		else
		{
			nodetree->rhsNode = emitLoadStack( nodetree->rhsNode );
		}

		//Need to dup the eval stack top entry for further expression
		//OPTIMIZAITON: remove redundant dup and pop
		if( m_bResultInEvalStack == false )
		{
			emit( CEE_DUP );
		}
		m_bResultInEvalStack = true;

		if( bTempValueStored )
			emit( CEE_LDSFLD, ridTemporary );	//Load it from the temp var

		//Now it's time to emit left hand side expressions
		nodetree->lhsNode = parseNodeTreeSub( nodetree->lhsNode, nodetree, true );

		releaseTemporaryVariable( ridTemporary );
		}

		break;
	case CG_NODE_NULL:
		break;
	default:
		Debug_Fatal( "Not implemented yet" );
		break;
	}

	return nodetree;
}


} //namespace CRI