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
 * Module   : Multi pass compilation management
 * File     : cilMultipassControl.cpp
 * Date     : 10/15/2007
 * Version  : Initial implementation Hak Matsuda
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
 * Generate statement list
 ***************************************************************************/
void CCilCodeGen::updateStatementTree( CCgNodeData* nodeStatement )
{
	assert( m_bMultipass == true );
	if( nodeStatement == NULL )
		return;
	if( m_pRootNode == NULL )
	{
		//Yes, I'm a root node
		m_pRootNode = nodeStatement;
		m_pLastNode = nodeStatement;
	}
	else
	{
		m_pLastNode->SetSibling( nodeStatement );
	}
}

/***************************************************************************
 * Compile statement list
 ***************************************************************************/
void CCilCodeGen::parseStatements()
{
	assert( m_bMultipass == true );

	if( m_pRootNode == NULL ) return;

	//First pass
	//List up a function decl node and construct function name information
	parseStatementsPass0( m_pRootNode );

	//Second pass
	parseStatementNodeTree( m_pRootNode );

	//Insert RET ops
	endOfFile();
	
	return;
}

/***************************************************************************
 * Statement list parsing helper
 ***************************************************************************/
void CCilCodeGen::parseStatementsPass0( CCgNodeData* nodetree )
{
	do
	{
		switch( nodetree->NodeType )
		{
		case	CG_NODE_FUNCTIONDECLNODE:
			parseFunctionDeclarationNodePass0( nodetree );
			break;
		default:
			if( nodetree->lhsNode != NULL )
			{
				parseStatementsPass0( nodetree->lhsNode );
			}
			if( nodetree->rhsNode != NULL )
			{
				parseStatementsPass0( nodetree->rhsNode );
			}
			if( nodetree->statementNode != NULL )
			{
				parseStatementsPass0( nodetree->statementNode );
			}
			//Condition node also can be funcdecl node
			if( nodetree->conditionNode != NULL )
			{
				parseStatementsPass0( nodetree->conditionNode );
			}
			break;
		}

		nodetree = nodetree->siblingNode;
	} while( nodetree != NULL );
}

/***************************************************************************
 * Return multipass flag
 ***************************************************************************/
bool CCilCodeGen::isMultiPasses()
{
	return m_bMultipass;
}

/***************************************************************************
 * Set multipass compilation
 ***************************************************************************/
void CCilCodeGen::setMultiPasses( bool b)
{
	m_bMultipass = b;
}
	

} //namespace CRI