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
 * File     : cilVariableDeclaration.cpp
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
CCgNodeData* CCilCodeGen::variableDeclaration( CCgNodeData* lhs, CCgNodeData* rhs )
{
	assert( lhs != NULL );
	//rhs can be null

	return makeFormulaNode( CG_VARIABLEDECL, lhs, rhs, true, false );	//Block traverse for 4th arg
};

CCgNodeData* CCilCodeGen::typedVariableDeclaration( CCgNodeData* lhs, CCgNodeData* rhs )
{
	assert( lhs != NULL );
	//rhs can be null

	return makeFormulaNode( CG_TYPEDVARIABLEDECL, lhs, rhs, true, false );	//Block traverse for 4th arg
};

/***************************************************************************
 * emit Variable decl CIL
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitVariableDeclaration( CCgNodeData* nodetree,
												  const OPERAND_TYPE type,
												  const OPERAND_FLAG flag,
												  const RID ridConstraintType )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );
	assert( nodetree->lhsNode != NULL );
	assert( nodetree->lhsNode->NodeType == CG_NODE_IDENTIFIER );
	//rhs can be null

	//Error check if the variable is already decleared.

	CCgNodeData* objectptr = nodetree->lhsNode;

	wstring strScopedVariableName = getCurrentScopeName() + *objectptr->pIdentifier;
	if( RID_NOTDEFINED != lookUpSymbolInformationInCurrentScope( strScopedVariableName ) )
	{
		reportError( ERROR_IDALREADYDEFINED, strScopedVariableName,
					objectptr->iFileId,
					objectptr->iLineNumber,
					objectptr->iColumn );
		return nodetree;

		//Check if the destination has restrict_ flag.
		//If so, "cast" source explicitly
	}

	RID	rid;
	if( isInGlobalScope() )
	{
		//Global scope variable
		rid = m_MetaData.generateStaticFieldRID( type, flag, ridConstraintType );
		generateSymbolInformation( rid, strScopedVariableName );

		if( nodetree->rhsNode == NULL )
		{
			if( !m_bTreatUnitializedValueAsZero )
			{
				//Make sure the variable is not assigned
				reportError( INFO_VARIABLENOTASSIGNED, strScopedVariableName,
								nodetree->lhsNode->iFileId,
								nodetree->lhsNode->iLineNumber,
								nodetree->lhsNode->iColumn );
				emit( CEE_LDSFLD, RID_CONSTANT_UNDEFINED );
				assert( TypeFromToken( rid ) == MDT_FIELDDEF );
				emit( CEE_STSFLD, rid );
			}
			else
			{
				if( m_bTreatFunctionNameReadOnly && TypeFromToken ( rid ) == MDT_METHODDEF )
				{
					reportError( ERROR_METHODNAMEASSIGNMENTNOTALLOWED, strScopedVariableName,
									nodetree->lhsNode->iFileId,
									nodetree->lhsNode->iLineNumber,
									nodetree->lhsNode->iColumn );
					return nodetree;
				}
				//Treat undefined as '0';
				emit( CEE_LDC_I4_0 );
				assert( TypeFromToken( rid ) == MDT_FIELDDEF );
				emit( CEE_STSFLD, rid );
			}
		}
		else
		{
			if( m_bTreatFunctionNameReadOnly && TypeFromToken( rid ) == MDT_METHODDEF )
			{
				reportError( ERROR_METHODNAMEASSIGNMENTNOTALLOWED, strScopedVariableName,
									nodetree->iFileId,
									nodetree->iLineNumber,
									nodetree->iColumn );
				return nodetree;
			}
			CCgNodeData* ptr = nodetree->rhsNode;
			emitLoadStack( ptr );
			assert( TypeFromToken( rid ) == MDT_FIELDDEF );
			emit( CEE_STSFLD, rid );
		}
	}
	else
	{
		//Local scope variable
		RID MethodRid = getCurrentILInfo().MethodRID;
		assert( TypeFromToken( MethodRid ) == MDT_METHODDEF );
		METHOD_DEF method = m_MetaData.getMethodTable()[ RidFromToken( MethodRid ) ];
		int32_t iLocalIndex = m_MetaData.addParamListEntry( method.ridLocalVariableList,
															type, flag, ridConstraintType );
		generateSymbolInformation( method.ridLocalVariableList,
									strScopedVariableName,
									CG_SYMBOL_FLAG_LOCAL,
									iLocalIndex );
		CG_SYMBOL_INFORMATION* info;
		lookUpSymbolInformation( strScopedVariableName, &info );
			
		if( nodetree->rhsNode == NULL )
		{
			if( !m_bTreatUnitializedValueAsZero )
			{
				//Make sure the variable is not assigned
				reportError( INFO_VARIABLENOTASSIGNED, strScopedVariableName,
									nodetree->lhsNode->iFileId,
									nodetree->lhsNode->iLineNumber,
									nodetree->lhsNode->iColumn );
				emit( CEE_LDSFLD, RID_CONSTANT_UNDEFINED );
				emitStoreLocal( info );
			}
			else
			{
				//Treat undefined as '0';
				emit( CEE_LDC_I4_0 );
				emitStoreLocal( info );
			}
		}
		else
		{
			CCgNodeData* ptr = nodetree->rhsNode;
			emitLoadStack( ptr );
			emitStoreLocal( info );
		}
	}

	return nodetree;
}

/***************************************************************************
 * emit Typed Variable decl CIL
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitTypedVariableDeclaration( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );
	assert( nodetree->iOperation == CG_TYPEDVARIABLEDECL );
	assert( nodetree->lhsNode != NULL );
	assert( nodetree->lhsNode->NodeType == CG_NODE_TYPEDIDENTIFIER );
	assert( nodetree->lhsNode->lhsNode != NULL );
	assert( nodetree->lhsNode->lhsNode->NodeType == CG_NODE_IDENTIFIER );
	assert( nodetree->lhsNode->rhsNode != NULL );
	assert( nodetree->lhsNode->rhsNode->NodeType == CG_NODE_IDENTIFIER );

	CCgNodeData* nodeType = nodetree->lhsNode->rhsNode;
	hash_map< wstring, TYPECONSTRAINT_INFO>::iterator it;

	it = m_MapTypeConstraint.find( *nodeType->pIdentifier );
	if( it == m_MapTypeConstraint.end() )
	{
		reportError( ERROR_ILLEGALTYPESPECIFIER, *nodeType->pIdentifier,
					nodetree->lhsNode->lhsNode->iFileId,
					nodetree->lhsNode->lhsNode->iLineNumber,
					nodetree->lhsNode->lhsNode->iColumn );
		return nodetree;
	}
	//if( it->second.type.type == OPERAND_OBJECTREF )
	//{
	//	//Object type constraint
	//	//Retrieve object ID

	//}

	delete( nodeType );
	nodetree->lhsNode = nodetree->lhsNode->lhsNode;
	nodetree =  emitVariableDeclaration( nodetree, it->second.type.type, OPERAND_FLAG_STRICT, it->second.type.ridConstraintType );

	return nodetree;
}

} //namespace CRI