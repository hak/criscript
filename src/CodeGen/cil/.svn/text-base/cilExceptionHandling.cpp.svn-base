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
 * Module   : cil EH routines
 * File     : cilExceptionHandling.cpp
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
//Create Throw statement node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::throwStatement( CCgNodeData* Expression )
{
	// all node can be null

	if( Expression != NULL )
	{
		CCgNodeData* object = new CCgNodeData;
		object->NodeType = CG_NODE_THROWNODE;
		object->statementNode = Expression;
		return object;
	}
	else
		return NULL;
};

/***************************************************************************
//Create Catch block decl node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::catchBlock( CCgNodeData* identifier,
										CCgNodeData* Body )
{
	assert( identifier != NULL );
	// Body node can be null

//Generate 'catch' node even the body is null
	CCgNodeData* object = new CCgNodeData;
	object->NodeType = CG_NODE_CATCHNODE;
	object->identifier = identifier;
	object->statementNode = Body;
	return object;
};

/***************************************************************************
//Create finally node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::finallyBlock( CCgNodeData* Body )
{
	//Generate 'finally' node even the body is null
	CCgNodeData* object = new CCgNodeData;
	object->NodeType = CG_NODE_FINALLYNODE;
	object->statementNode = Body;
	return object;
};

/***************************************************************************
//Create EH block decl node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::exceptionHandlingClause(	CCgNodeData* bodyNode,
								CCgNodeData* catchNode,
								CCgNodeData* finallyNode )
{
	// all node can be null
	CCgNodeData* object = new CCgNodeData;
	object->NodeType = CG_NODE_EXCEPTIONHANDLINGNODE;
	object->statementNode = bodyNode;
	object->lhsNode = catchNode;
	object->rhsNode = finallyNode;
	object->iLineNumber = m_pLex->getLineNumber();
	return object;
};

/***************************************************************************
//parse EH node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseThrowStatementNode( CCgNodeData* nodetree )
{
	nodetree->statementNode = parseExpressionNodeTree( nodetree->statementNode, false );
	//Jump back to the leave operation
	emit( CEE_THROW );
	return nodetree;
}

/***************************************************************************
//parse EH node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseExceptionHandlingNode( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_EXCEPTIONHANDLINGNODE );

 	ILFRAGMENTINFO ILinfo = *getCurrentScopeInformation();
	METHOD_DEF method = { 0 };
	if( ILinfo.MethodRID )
		method = m_MetaData.getMethod( ILinfo.MethodRID );

	int32_t iLableLeaveOperation = createNewLabelID();
	RID ridTry = 0;
	RID ridCatch = 0;
	RID ridFinally = 0;
	RID ridArguments = 0;
	bool bStoreUseFinallyOps;
	
	//Generate ridCatch if needed
	if( nodetree->lhsNode != NULL && nodetree->statementNode != NULL )
	{
		assert( nodetree->lhsNode->NodeType == CG_NODE_CATCHNODE );
		ridArguments = m_MetaData.generateParamListRID();
		//Note
		//First entry of the argument list is always "this" poiner
		m_MetaData.addParamListEntry( ridArguments, OPERAND_OBJECTREF );

		ridCatch = m_MetaData.generateMethodRID( METHOD_CIL,
											ILinfo.MethodRID,
											ridArguments,
											method.ridLocalVariableList );
	}

	//Generate ridFinally if needed
	if( nodetree->rhsNode != NULL )
	{
		assert( nodetree->rhsNode->NodeType == CG_NODE_FINALLYNODE );
		if( nodetree->statementNode != NULL )
		{
			//Inherit local/arg from the parent
			ridFinally = m_MetaData.generateMethodRID( METHOD_CIL,
												ILinfo.MethodRID,
												method.ridArgList,
												method.ridLocalVariableList );
		}
	}

	//-------------------------------------------------------------------------
	//parse try node first
	//-------------------------------------------------------------------------
	if( nodetree->statementNode || ridFinally != 0 )
	{
		ridTry = m_MetaData.generateEhRID( METHOD_EHCLAUSE,
												ILinfo.MethodRID,
												ridCatch,
												ridFinally );

		wchar_t buf[ SIZE_STRING ];
		swprintf( buf, SIZE_STRING,  STRING_INTERNAL_TRYLABEL_PREFIX, ridTry & MAX_NUM_LABELS );
		wstring strScopedFunctionName = getCurrentScopeName() + wstring( buf );
		
		//Look up the temporary variable and make sure not defined yet
		if( RID_NOTDEFINED != lookUpSymbolInformationInCurrentScope( strScopedFunctionName ) )
		{
			reportError( ERROR_INTERNALIDALREADYDEFINED,
							strScopedFunctionName,
							nodetree->statementNode->iFileId,
							nodetree->statementNode->iLineNumber,
							nodetree->statementNode->iColumn );
			return nodetree;
		}
		generateSymbolInformation( ridTry, strScopedFunctionName );
//		m_MetaData.getMethodTable()[ RidFromToken( ridTry )].iILOffset = tagTheLine();

		//emit entrering a clause code
		emit( CEE_EXT_ENTER, ridTry );
		bStoreUseFinallyOps = m_bUseFinallyOps;
		m_bUseFinallyOps = false;
		m_iCurrentEHNestLevel++;
		
		if( nodetree->statementNode != NULL )
		{
			//emit statement node
			nodetree->statementNode = parseStatementNodeTree( nodetree->statementNode );
		}

		//Tag the label
		//Note that the ILOffset of TRY clause indicate the end of the try clause
		m_MetaData.getMethodTable()[ RidFromToken( ridTry )].iILOffset = tagTheLine();
		insertLabelTag( iLableLeaveOperation );
		//emit leaving a clause code
		emit( CEE_EXT_LEAVE );
		m_iCurrentEHNestLevel--;
		m_bUseFinallyOps = bStoreUseFinallyOps;
	}
		
	//-------------------------------------------------------------------------
	//parse catch node if given
	//-------------------------------------------------------------------------
	//Catch node is useless when no statement node is given
	if( ridCatch )
	{
		wchar_t buf[ SIZE_STRING ];
		swprintf( buf, SIZE_STRING,  STRING_INTERNAL_CATCHLABEL_PREFIX, ridCatch & MAX_NUM_LABELS );
		wstring strScopedFunctionName = getCurrentScopeName() + wstring( buf );

		//Look up the temporary variable and make sure not defined yet
		if( RID_NOTDEFINED != lookUpSymbolInformationInCurrentScope( strScopedFunctionName ) )
		{
			reportError( ERROR_INTERNALIDALREADYDEFINED, strScopedFunctionName,
										nodetree->statementNode->iFileId,
							nodetree->statementNode->iLineNumber,
							nodetree->statementNode->iColumn );
			return nodetree;
		}
		generateSymbolInformation( ridCatch, strScopedFunctionName );

		bStoreUseFinallyOps = m_bUseFinallyOps;
		m_bUseFinallyOps = true;
		m_iCurrentEHNestLevel++;
		//emit catch clause
		enterNewScope( strScopedFunctionName, ILinfo.MethodRID );

		m_MetaData.getMethodTable()[ RidFromToken( ridCatch )].iILOffset = tagTheLine();

		if( nodetree->lhsNode->identifier )
		{
			//Catch node parameters need to be registered here rather than 1st pass
			bool b = isMultiPasses();
			setMultiPasses( false );	//Quick hack
			
			nodetree->lhsNode->identifier = parseParameterListNodeTree( nodetree->lhsNode->identifier, ridArguments );

			setMultiPasses( b );
		}

		if( nodetree->lhsNode->statementNode )
		{
			nodetree->lhsNode->statementNode = parseStatementNodeTree( nodetree->lhsNode->statementNode );
		}
		//Jump back to the leave operation
		emit( CEE_EXT_LEAVE );

		//Usually catch does not return anything

		//Resolve locals and args
		m_MetaData.getMethodTable()[ RidFromToken( ridCatch ) ].iNumArgments =
											(int32_t)m_MetaData.getParamListSize( ridArguments );
		m_MetaData.getMethodTable()[ RidFromToken( ridCatch ) ].iNumLocals = 0;

		leaveScope();
		m_iCurrentEHNestLevel--;
		m_bUseFinallyOps = bStoreUseFinallyOps;
	}

	//-------------------------------------------------------------------------
	//parse finally node if given
	//-------------------------------------------------------------------------
	if( nodetree->rhsNode )
	{
		if( ridTry != NULL )
		{
			wchar_t buf[ SIZE_STRING ];
			swprintf( buf, SIZE_STRING,  STRING_INTERNAL_FINALLYLABEL_PREFIX, ridFinally & MAX_NUM_LABELS );
			wstring strScopedFinallyClauseName = getCurrentScopeName() + wstring( buf );

			//Look up the temporary variable and make sure not defined yet
			if( RID_NOTDEFINED != lookUpSymbolInformationInCurrentScope( strScopedFinallyClauseName ) )
			{
				reportError( ERROR_INTERNALIDALREADYDEFINED, strScopedFinallyClauseName,
							nodetree->statementNode->iFileId,
							nodetree->statementNode->iLineNumber,
							nodetree->statementNode->iColumn );
				return nodetree;
			}
			generateSymbolInformation( ridFinally, strScopedFinallyClauseName );

			//emit Finally clause
			bStoreUseFinallyOps = m_bUseFinallyOps;
			m_bUseFinallyOps = true;
			m_iCurrentEHNestLevel++;
			enterNewScope( strScopedFinallyClauseName, ILinfo.MethodRID );
//			enterNewScope( strScopedFinallyClauseName, ridFinally );

			m_MetaData.getMethodTable()[ RidFromToken( ridFinally )].iILOffset = tagTheLine();

			nodetree->rhsNode->statementNode = parseStatementNodeTree( nodetree->rhsNode->statementNode );

			//Jump back to the leave operation
			emit( CEE_EXT_LEAVE );
//			emit( CEE_BR, iLableLeaveOperation );
			
			//Resolve locals and args
			m_MetaData.getMethodTable()[ RidFromToken( ridFinally ) ].iNumArgments = 0;
			m_MetaData.getMethodTable()[ RidFromToken( ridFinally ) ].iNumLocals = 0;

			leaveScope();
			m_iCurrentEHNestLevel--;
			m_bUseFinallyOps = bStoreUseFinallyOps;
		}
		else
		{
			//Just emit finally node. Done.
			ridFinally = 0;
			nodetree->rhsNode->statementNode = parseStatementNodeTree( nodetree->rhsNode->statementNode );
		}
	}

	return nodetree;
}	

} //namespace CRI