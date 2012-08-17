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
//Create Break statement node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::breakStatement( CCgNodeData* identifier )
{
	// all node can be null
	CCgNodeData* object = new CCgNodeData;
	object->NodeType = CG_NODE_BREAKNODE;
	object->identifier = identifier;
	object->iLineNumber = m_pLex->getLineNumber();
	return object;
};

/***************************************************************************
//parse EH node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseBreakStatementNode( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_BREAKNODE );

	if( !m_bInsideSwitchStatement && !m_iInsideIterationStatement )
	{
		reportError( ERROR_CANNOTHAVEBREAK,
							nodetree->iFileId,
							nodetree->iLineNumber,
							nodetree->iColumn );
		return nodetree;		
	}

	if( nodetree->identifier == NULL )
	{
		const BRANCH_TARGET_INFO& brtarget = getBranchTargetInformation( m_iLabelBreak );
		if( brtarget.iExceptionHandlerNestLevel )
		{
			if( brtarget.iExceptionHandlerNestLevel > 0 )
			{
				//Insert leaves.
				for( int32_t i = 0;
					i < m_iCurrentEHNestLevel - brtarget.iExceptionHandlerNestLevel;
					++ i )
				{
					emit( CEE_EXT_LEAVE );
				}
			}
			else
			{
				//May need to insert 'leave' operation later
				emit( CEE_LEAVE_TMP );
				emit( m_iLabelBreak );	//Branch target
				if( m_bUseFinallyOps )
					emit( -m_iCurrentEHNestLevel );	//Current nesting level
				else
					emit( m_iCurrentEHNestLevel );	//Current nesting level
			}
		}
		emit( CEE_BR, m_iLabelBreak );
	}
	else
	{
		CG_SYMBOL_INFORMATION* pInfo;
		wstring strScopedLabelName = getCurrentScopeName() + *nodetree->identifier->pIdentifier;
		if( RID_NOTDEFINED == lookUpSymbolInformationInCurrentScope( strScopedLabelName, &pInfo) )
		{
			reportError( ERROR_LABELNOTFOUND, *nodetree->identifier->pIdentifier,
							nodetree->iFileId,
							nodetree->iLineNumber,
							nodetree->iColumn );
			return nodetree;
		}
		else
		{
			//Break need to jump to NEXT instruction rather than labeled one

			if( pInfo->rid != RID_LABELNAME )
			{
				reportError( ERROR_LABELNOTFOUND, *nodetree->identifier->pIdentifier,
							nodetree->iFileId,
							nodetree->iLineNumber,
							nodetree->iColumn );
				return nodetree;
			}
			const BRANCH_TARGET_INFO& brtarget = getBranchTargetInformation( m_iLabelBreak );
			if( brtarget.iExceptionHandlerNestLevel )
			{
				if( brtarget.iExceptionHandlerNestLevel > 0 )
				{
					//Insert leaves.
					for( int32_t i = 0;
						i < m_iCurrentEHNestLevel - brtarget.iExceptionHandlerNestLevel;
						++ i )
					{
						emit( CEE_EXT_LEAVE );
					}
				}
				else
				{
					//May need to insert 'leave' operation later
					emit( CEE_LEAVE_TMP );
					emit( m_iLabelBreak );	//Branch target
					if( m_bUseFinallyOps )
						emit( -m_iCurrentEHNestLevel );	//Current nesting level
					else
						emit( m_iCurrentEHNestLevel );	//Current nesting level
				}
			}
			//Next label should point next statement
			emit( CEE_BR, pInfo->iIndex + 1 );
		}
	}

	return nodetree;
}

/***************************************************************************
//Create Continue statement node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::continueStatement( CCgNodeData* identifier )
{
	// all node can be null
	CCgNodeData* object = new CCgNodeData;
	object->NodeType = CG_NODE_CONTINUENODE;
	object->identifier = identifier;
	object->iLineNumber = m_pLex->getLineNumber();
	return object;
};

/***************************************************************************
//parse EH node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseContinueStatementNode( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_CONTINUENODE );
	if( !m_iInsideIterationStatement )
	{
		reportError( ERROR_CANNOTHAVECONTINUE,
					nodetree->iFileId,
					nodetree->iLineNumber,
					nodetree->iColumn );
		return nodetree;		
	}

//	if( 1 )
	if( nodetree->identifier == NULL )
	{
		const BRANCH_TARGET_INFO& brtarget = getBranchTargetInformation( m_iLabelBreak );
		if( brtarget.iExceptionHandlerNestLevel )
		{
			if( brtarget.iExceptionHandlerNestLevel > 0 )
			{
				//Insert leaves.
				for( int32_t i = 0;
					i < m_iCurrentEHNestLevel - brtarget.iExceptionHandlerNestLevel;
					++ i )
				{
					emit( CEE_EXT_LEAVE );
				}
			}
			else
			{
				//May need to insert 'leave' operation later
				emit( CEE_LEAVE_TMP );
				emit( m_iLabelBreak );	//Branch target
				if( m_bUseFinallyOps )
					emit( -m_iCurrentEHNestLevel );	//Current nesting level
				else
					emit( m_iCurrentEHNestLevel );	//Current nesting level
			}
		}
		emit( CEE_BR, m_iLabelContinue );
	}
	else
	{
		CG_SYMBOL_INFORMATION* pInfo;
		wstring strScopedLabelName = getCurrentScopeName() + *nodetree->identifier->pIdentifier;
		if( RID_NOTDEFINED == lookUpSymbolInformationInCurrentScope( strScopedLabelName, &pInfo) )
		{
			reportError( ERROR_LABELNOTFOUND, *nodetree->identifier->pIdentifier,
							nodetree->iFileId,
							nodetree->iLineNumber,
							nodetree->iColumn );
			return nodetree;
		}
		else
		{
			if( pInfo->rid != RID_LABELNAME )
			{
				reportError( ERROR_LABELNOTFOUND, *nodetree->identifier->pIdentifier,
							nodetree->iFileId,
							nodetree->iLineNumber,
							nodetree->iColumn );
				return nodetree;
			}
			const BRANCH_TARGET_INFO& brtarget = getBranchTargetInformation( m_iLabelBreak );
			if( brtarget.iExceptionHandlerNestLevel )
			{
				if( brtarget.iExceptionHandlerNestLevel > 0 )
				{
					//Insert leaves.
					for( int32_t i = 0;
						i < m_iCurrentEHNestLevel - brtarget.iExceptionHandlerNestLevel;
						++ i )
					{
						emit( CEE_EXT_LEAVE );
					}
				}
				else
				{
					//May need to insert 'leave' operation later
					emit( CEE_LEAVE_TMP );
					emit( m_iLabelBreak );	//Branch target
					if( m_bUseFinallyOps )
						emit( -m_iCurrentEHNestLevel );	//Current nesting level
					else
						emit( m_iCurrentEHNestLevel );	//Current nesting level
				}
			}
			emit( CEE_BR, pInfo->iIndex );
		}
	}

	return nodetree;
}

} //namespace CRI