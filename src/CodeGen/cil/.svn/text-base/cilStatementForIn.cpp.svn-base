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
 * Module   : For statement
 * File     : cilStatementForIn.cpp
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
 *      ForIn statement node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::forInStatement( CCgNodeData* variable,
											 CCgNodeData* obj,
											 CCgNodeData* statement )
{
	assert( variable != NULL );
	assert( obj != NULL );
	assert( statement != NULL );
	CCgNodeData* object = new CCgNodeData;
	object->NodeType = CG_NODE_FORINNODE;
	object->initialNode = variable;
	object->conditionNode = obj;
	object->statementNode = statement;
	object->iLineNumber = m_pLex->getLineNumber();
	return object;
}

/***************************************************************************
//Create parse ForIn node 
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseForInNode( CCgNodeData* fornode, int32_t iGivenLoopEndLabelID )
{
	assert( fornode != NULL );
	assert( fornode->NodeType == CG_NODE_FORINNODE );
	assert( fornode->initialNode != NULL );
	assert( fornode->conditionNode != NULL );
	assert( fornode->statementNode != NULL );
	//Do some optimizations work here
	
	RID ridTemporary = reserveTemporaryVariable( CG_SYMBOL_FLAG_GLOBAL );
	RID ridTemporary2 = reserveTemporaryVariable( CG_SYMBOL_FLAG_GLOBAL );
	emit( CEE_LDC_I4_0 );	//Clear tmp Variable		
	emit( CEE_STSFLD, ridTemporary );
	emit( CEE_LDC_I4_0 );	//Clear tmp2 Variable		
	emit( CEE_STSFLD, ridTemporary2 );
	
	int32_t iLableLoopStart;
	int32_t iLableLoopEnd;
	if ( iGivenLoopEndLabelID )
		iLableLoopEnd = iGivenLoopEndLabelID;
	else
		iLableLoopEnd = createNewLabelID();

	int32_t iLableEndForStatement = createNewLabelID();

	//Save states
	uint32_t iSavedLabelBreak = m_iLabelBreak;
	uint32_t iSavedLabelContinue = m_iLabelContinue;

	iLableLoopStart = createNewLabelIDAndInsertTag();
	emit( CEE_LDC_I4, ridTemporary );	//Iterator temporary
	emit( CEE_LDC_I4, ridTemporary2 );	//Iterator temporary

	//Generate object ref node
	fornode->conditionNode = parseExpressionNodeTree( fornode->conditionNode, false );
	//Update variable and put a flag on the stack
	emit( CEE_EXT_STARGLIST_S, (uint8_t)3 );	//Move to arglist
	emit( CEE_CALL, m_ridMethodForIn );
	//Returns a STRING (Property name)
	emit( CEE_DUP );
	
	//Generate variable node
	fornode->initialNode = parseExpressionNodeTree( fornode->initialNode, true );		

	emit( CEE_EXT_CONV_B );
	emit( CEE_BRFALSE, iLableEndForStatement );

	if( fornode->statementNode != NULL )
	{
		//Set up variables to allow break/continue node
		m_iInsideIterationStatement++;
		m_iLabelBreak = iLableEndForStatement;
		m_iLabelContinue = iLableLoopEnd;

		//Generate statement body
		fornode->statementNode = parseStatementNodeTree( fornode->statementNode );

		//Restore states
		m_iInsideIterationStatement--;
		m_iLabelBreak =  iSavedLabelBreak;
		m_iLabelContinue =  iSavedLabelContinue;
	}

	insertLabelTag( iLableLoopEnd );

	emit( CEE_BR, iLableLoopStart );
	insertLabelTag( iLableEndForStatement );

	//Release temporary variable
	emit( CEE_LDC_I4_0 );	//Clear Variable		
	emit( CEE_STSFLD, ridTemporary );
	emit( CEE_LDC_I4_0 );	//Clear Variable		
	emit( CEE_STSFLD, ridTemporary2 );
	releaseTemporaryVariable( ridTemporary );
	releaseTemporaryVariable( ridTemporary2 );
	return fornode;
};

} //namespace CRI