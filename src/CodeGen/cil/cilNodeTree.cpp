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
 * Module   : NodeTree parsing
 * File     : cilNodeTree.cpp
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
 * Tree optimization
 ***************************************************************************/
// N + 0 -> N
// N - 0 -> N
// N * 0 -> 0
// N * 1 -> N
// 0 / N -> 0
// N / N -> 1
// N & 0 = 0
// N && 0 = false
// N || 1 = true
// N | -1 = -1
// N ^ N = 0
// Loop strength
	
	
/***************************************************************************
 * Create & a formula node tree
 ***************************************************************************/
CCgNodeData* CCilCodeGen::makeFormulaNode( CG_OPERATIONTYPE iOperation,
										  CCgNodeData* lhs,
										  CCgNodeData* rhs,
										  bool bBlockTraverseLeft,
										  bool bBlockTraverseRight
										  )
{
	CCgNodeData* object = new CCgNodeData;
	object->NodeType = CG_NODE_FORMULANODE;
	object->iOperation = iOperation;
	object->lhsNode = lhs;
	object->rhsNode = rhs;
	object->bBlockTraverseLeft = bBlockTraverseLeft;
	object->bBlockTraverseRight = bBlockTraverseRight;
	return object;
};

/***************************************************************************
 * Create & an arbitral type of a node tree
 ***************************************************************************/
CCgNodeData* CCilCodeGen::makeNode(		CG_NODE_TYPE NodeType,
							CG_OPERATIONTYPE iOperation,
							CCgNodeData* lhs,
							CCgNodeData* rhs,
							CCgNodeData* siblingNode,
							CCgNodeData* initialNode,
							CCgNodeData* statementNode,
							CCgNodeData* conditionNode,
							CCgNodeData* argumentlistNode
							)
{
	CCgNodeData* object = new CCgNodeData;
	object->NodeType = NodeType;
	object->iOperation = iOperation;
	object->lhsNode = lhs;
	object->rhsNode = rhs;
	object->siblingNode = siblingNode;
	object->initialNode = initialNode;
	object->statementNode = statementNode;
	object->conditionNode = conditionNode;
	object->argumentlistNode = argumentlistNode;
	return object;
}

/***************************************************************************
 * Duplicate node tree
 ***************************************************************************/
CCgNodeData* CCilCodeGen::duplicateNode( CCgNodeData* srcNode )
{
	if( srcNode == NULL ) return NULL;

	CCgNodeData* object = new CCgNodeData;

	object->NodeType = srcNode->NodeType;

	if( srcNode->NodeType == CG_NODE_IDENTIFIER )
	{
		object->pIdentifier = new wstring( *srcNode->pIdentifier );
	}
	else
		object->iOperation = srcNode->iOperation;

	if( srcNode->initialNode )
	{
		object->initialNode = duplicateNode( srcNode->initialNode );
	}
	if( srcNode->statementNode )
	{
		object->statementNode = duplicateNode( srcNode->statementNode );
	}
	if( srcNode->conditionNode )
	{
		object->conditionNode = duplicateNode( srcNode->conditionNode );
	}
	if( srcNode->argumentlistNode )
	{
		object->argumentlistNode = duplicateNode( srcNode->argumentlistNode );
	}
	if( srcNode->lhsNode )
	{
		object->lhsNode = duplicateNode( srcNode->lhsNode );
	}
	if( srcNode->rhsNode )
	{
		object->rhsNode = duplicateNode( srcNode->rhsNode );
	}
	if( srcNode->siblingNode )
	{
		object->siblingNode = duplicateNode( srcNode->siblingNode );
	}
		
	return object;
};

/***************************************************************************
 * Create & parse Statement node tree
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseStatementNodeTree( CCgNodeData* nodetree )
{
	if( nodetree == NULL ) return NULL;

	//Start parsing a statement node tree
	m_bResultInEvalStack = false;


	switch( nodetree->NodeType )
	{
	case CG_NODE_FUNCTIONDECLNODE:
		nodetree = parseFunctionDeclarationNode( nodetree );

		//Pop return value
		emit( CEE_POP );
		break;
	case CG_NODE_IFNODE:
		nodetree = parseIfNode( nodetree );
		break;
	case CG_NODE_FORNODE:
		nodetree = parseForNode( nodetree, false );
		break;
	case CG_NODE_FORINNODE:
		nodetree = parseForInNode( nodetree );
		break;
	case CG_NODE_DOWHILENODE:
		nodetree = parseForNode( nodetree, true );
		break;
	case CG_NODE_FORMULANODE:
		nodetree = parseNodeTreeSub( nodetree, NULL, true );
		break;
	case CG_NODE_EXPRESSIONNODE:
		nodetree = parseExpressionStatementNode( nodetree );
		break;
	case CG_NODE_WITHNODE:
		nodetree = parseWithStatementNode( nodetree );
		break;
	case CG_NODE_RETNODE:
		nodetree = parseRetStatementNode( nodetree );
		break;
	case CG_NODE_THROWNODE:
		nodetree = parseThrowStatementNode( nodetree );
		break;
	case CG_NODE_SWITCHNODE:
		nodetree = parseSwitchStatementNode( nodetree );
		break;
	case CG_NODE_LABELEDNODE:
		nodetree = parseLabeledStatementNode( nodetree );
		break;
	case CG_NODE_BREAKNODE:
		nodetree = parseBreakStatementNode( nodetree );
		break;
	case CG_NODE_CONTINUENODE:
		nodetree = parseContinueStatementNode( nodetree );
		break;
	case CG_NODE_EXCEPTIONHANDLINGNODE:
		nodetree = parseExceptionHandlingNode( nodetree );
		break;
	case CG_NODE_CASECLAUSENODE: 
		nodetree = parseCaseNode( nodetree );
		break;
	case CG_NODE_IDENTIFIER:
	case CG_NODE_STRINGLITERAL:
	case CG_NODE_REGEXLITERAL:
	case CG_NODE_INTEGERLITERAL:
	case CG_NODE_FLOATLITERAL:
	case CG_NODE_BOOLEANLITERAL:
		emitLoadStack( nodetree );
		break;
	case CG_NODE_NULL: 
		break;
	default:
		Debug_Fatal( "Not implemented yet" );
		break;
	}

	if( nodetree && nodetree->siblingNode )
	{
		nodetree->siblingNode = parseStatementNodeTree( nodetree->siblingNode );
	}
	//Do some more optimization work here
	//e.g. loop strength reduction 
	delete nodetree;
	return NULL;

}

/***************************************************************************
 * Create & parse Expression node tree
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseExpressionNodeTree( CCgNodeData* nodetree, bool bAdjustStackEntries )
{
	if( nodetree == NULL ) return NULL;

	//Check the eval stack position
	int32_t iEvalStackPosition = getCurrentILInfo().iNumEvalStackEntries;

	switch( nodetree->NodeType )
	{
	case CG_NODE_IFNODE:
	case CG_NODE_RETNODE:
	case CG_NODE_FORNODE:
	case CG_NODE_EXCEPTIONHANDLINGNODE:
	case CG_NODE_THROWNODE:
	case CG_NODE_WITHNODE:
	case CG_NODE_SWITCHNODE:
	case CG_NODE_CATCHNODE:
	case CG_NODE_FINALLYNODE:
	case CG_NODE_LABELEDNODE:
	case CG_NODE_CASECLAUSENODE: 
		reportError( ERROR_STATEMENTNOTALLOWED,
							nodetree->iFileId,
							nodetree->iLineNumber,
							nodetree->iColumn );
		break;
	case CG_NODE_FUNCTIONDECLNODE:
		//Function decl EXPRESSION
		nodetree = parseFunctionExpressionNode( nodetree );
		break;
	case CG_NODE_FORMULANODE:
		nodetree = parseNodeTreeSub( nodetree, NULL, bAdjustStackEntries );
		break;
	case CG_NODE_IDENTIFIER:
	case CG_NODE_STRINGLITERAL:
	case CG_NODE_REGEXLITERAL:
	case CG_NODE_INTEGERLITERAL:
	case CG_NODE_FLOATLITERAL:
	case CG_NODE_BOOLEANLITERAL:
		emitLoadStack( nodetree );
		break;
	case CG_NODE_EXPRESSIONNODE:
		nodetree->statementNode = parseExpressionNodeTree( nodetree->statementNode, bAdjustStackEntries );
		break;
	case CG_NODE_NULL:
		break;
	default:
		Debug_Fatal( "Not implemented yet" );
		break;
	}

	if( bAdjustStackEntries )
	{
		//Pop the result if something is still on the stack
		iEvalStackPosition = getCurrentILInfo().iNumEvalStackEntries - iEvalStackPosition;
		for( ; iEvalStackPosition > 0; iEvalStackPosition-- )
		{
			//Optimization: If the last dup ops was 2 instruction prior (and in the same scope),
			//Just remove the dup rather than incert CEE_POP
			if( m_iLastILOffset[ 0 ] && m_iLastILOffset[ 1 ] && m_iLastILOffset[ 2 ] )
			{
				ILFRAGMENTINFO& info = getCurrentILInfo();
				//Try optimization
				if( info.ILPool[ m_iLastILOffset[ 1 ] ] == CEE_DUP )
				{
					//Just remove it
					info.ILPool.erase( info.ILPool.begin() + m_iLastILOffset[ 1 ] );
					clearILOffsetCache();
					adjustEvalStack( &info, m_OpcodeMapByOpcode[ CEE_POP ], 0 );
				}
				else if( info.ILPool[ m_iLastILOffset[ 2 ] ] == CEE_DUP )
				{
					//Just remove it
					info.ILPool.erase( info.ILPool.begin() + m_iLastILOffset[ 2 ] );
					clearILOffsetCache();
					adjustEvalStack( &info, m_OpcodeMapByOpcode[ CEE_POP ], 0 );
				}
				else
					emit( CEE_POP );
			}
			else
				emit( CEE_POP );
		}
	}

	if( nodetree->siblingNode != NULL )
	{
		nodetree->siblingNode = parseExpressionNodeTree( nodetree->siblingNode, false );
	}

	//Do some more optimization work here
	//e.g. loop strength reduction 
	delete nodetree;

	return NULL;
}

/***************************************************************************
 * Create & parse Parameter list node tree
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseParameterListNodeTree( CCgNodeData* nodetree, RID RidParamList )
{
	if( nodetree == NULL ) return NULL;

	switch( nodetree->NodeType )
	{
	case CG_NODE_IDENTIFIER:
		{
			if( !isMultiPasses() )
			{
				//Single pass mode
				//
				wstring strScopedVariableName = getCurrentScopeName() + *nodetree->pIdentifier;
				RID ridSrc = lookUpSymbolInformationInCurrentScope( strScopedVariableName );
				if( ridSrc != RID_NOTDEFINED ) {
					//Oops, I'm registerred already.
					reportError( ERROR_PARAMETERALREADYDEFINED, *nodetree->pIdentifier,
								nodetree->iFileId,
								nodetree->iLineNumber,
								nodetree->iColumn );
					delete nodetree;
					return NULL;
				}
				int32_t iIndex = m_MetaData.addParamListEntry( RidParamList, OPERAND_UNDEFINED );
				generateSymbolInformation( RidParamList,
											strScopedVariableName,
											CG_SYMBOL_FLAG_ARG,
											iIndex );
			}

			//Recursively parse Node tree
			if( nodetree->siblingNode )
				nodetree->siblingNode = parseParameterListNodeTree( nodetree->siblingNode,
																		RidParamList );
		}
		break;
	case CG_NODE_TYPEDIDENTIFIER:
		{
			if( !isMultiPasses() )
			{
				//Single pass mode
				//
				wstring strScopedVariableName = getCurrentScopeName() + *nodetree->lhsNode->pIdentifier;
				RID ridSrc = lookUpSymbolInformationInCurrentScope( strScopedVariableName );
				if( ridSrc != RID_NOTDEFINED ) {
					//Oops, I'm registerred already.
					reportError( ERROR_PARAMETERALREADYDEFINED, *nodetree->lhsNode->pIdentifier,
								nodetree->lhsNode->iFileId,
								nodetree->lhsNode->iLineNumber,
								nodetree->lhsNode->iColumn );
					return nodetree;
				}

				CCgNodeData* nodeType = nodetree->rhsNode;
				hash_map< wstring, TYPECONSTRAINT_INFO>::iterator it;

				it = m_MapTypeConstraint.find( *nodeType->pIdentifier );
				if( it == m_MapTypeConstraint.end() )
				{
					reportError( ERROR_ILLEGALTYPESPECIFIER, *nodeType->rhsNode->pIdentifier,
								nodetree->rhsNode->iFileId,
								nodetree->rhsNode->iLineNumber,
								nodetree->rhsNode->iColumn );
					return nodetree;
				}


				int32_t iIndex = m_MetaData.addParamListEntry( RidParamList,
															it->second.type.type,
															OPERAND_FLAG_STRICT,
															it->second.type.ridConstraintType  );
				generateSymbolInformation( RidParamList,
											strScopedVariableName,
											CG_SYMBOL_FLAG_ARG,
											iIndex );
			}

			//Recursively parse Node tree
			if( nodetree->siblingNode )
				nodetree->siblingNode = parseParameterListNodeTree( nodetree->siblingNode,
																		RidParamList );
		}
		break;
	case CG_NODE_RESTARGS:
		{
			//Set temporary variable to notify it has restArgs
			m_bRestArgsInArgList = true;

			if( !isMultiPasses() )
			{
				//Single pass mode
				//
				//Rest arguments (...args)
				//
				assert( nodetree->lhsNode == NULL );	//Should be NULL node
				assert( nodetree->rhsNode != NULL );	//Should be identifier

				wstring strScopedVariableName = getCurrentScopeName();
				
				switch( nodetree->rhsNode->NodeType )
				{
				case CG_NODE_IDENTIFIER:
					strScopedVariableName.append( *nodetree->rhsNode->pIdentifier );
					break;
				case CG_NODE_TYPEDIDENTIFIER:
					strScopedVariableName.append( *nodetree->rhsNode->lhsNode->pIdentifier );
					break;
				default:
					Debug_Fatal( "Illegal Node" );
				}

				RID ridSrc = lookUpSymbolInformationInCurrentScope( strScopedVariableName );
				if( ridSrc != RID_NOTDEFINED ) {
					//Oops, I'm registerred already.
					reportError( ERROR_PARAMETERALREADYDEFINED, *nodetree->lhsNode->pIdentifier,
								nodetree->lhsNode->iFileId,
								nodetree->lhsNode->iLineNumber,
								nodetree->lhsNode->iColumn );
					return nodetree;
				}

				//
				//TODO: note that array constraint is not implemented yet
				//

				int32_t iIndex = m_MetaData.addParamListEntry( RidParamList, OPERAND_UNDEFINED );
				generateSymbolInformation( RidParamList,
											strScopedVariableName,
											CG_SYMBOL_FLAG_ARG,
											iIndex );

				//Should be last node in the arg tree
				if( nodetree->siblingNode )
				{
					Debug_Fatal( "Illega node" );
				}
			}
			//Mark this function has rest arguments
		}
		break;
	case CG_NODE_STRINGLITERAL:
	case CG_NODE_REGEXLITERAL:
	case CG_NODE_INTEGERLITERAL:
	case CG_NODE_FLOATLITERAL:
	case CG_NODE_IFNODE:
	case CG_NODE_RETNODE:
	case CG_NODE_FORMULANODE:
	case CG_NODE_EXCEPTIONHANDLINGNODE:
	case CG_NODE_THROWNODE:
	case CG_NODE_WITHNODE:
	case CG_NODE_SWITCHNODE:
	case CG_NODE_CATCHNODE:
	case CG_NODE_FINALLYNODE:
	case CG_NODE_LABELEDNODE:
	case CG_NODE_CASECLAUSENODE: 
	default:
		Debug_Fatal( "Illegal Node" );
		break;
	}
	//Do some more optimization work here
	//e.g. loop strength reduction 
	delete nodetree;
	return NULL;
}

/***************************************************************************
 * Create & parse Parameter list node tree for pass0
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseParameterListNodeTreePass0( CCgNodeData* nodetree, RID RidParamList )
{
	if( nodetree == NULL ) return NULL;

	switch( nodetree->NodeType )
	{
	case CG_NODE_IDENTIFIER:
		{
			wstring strScopedVariableName = getCurrentScopeName() + *nodetree->pIdentifier;
			RID ridSrc = lookUpSymbolInformationInCurrentScope( strScopedVariableName );
			if( ridSrc != RID_NOTDEFINED ) {
				//Oops, I'm registerred already.
				reportError( ERROR_PARAMETERALREADYDEFINED, *nodetree->pIdentifier,
							nodetree->iFileId,
							nodetree->iLineNumber,
							nodetree->iColumn );
				return nodetree;
			}
			int32_t iIndex = m_MetaData.addParamListEntry( RidParamList, OPERAND_UNDEFINED );
			generateSymbolInformation( RidParamList,
										strScopedVariableName,
										CG_SYMBOL_FLAG_ARG,
										iIndex );

			//Recursively parse Node tree
			if( nodetree->siblingNode )
				nodetree->siblingNode = parseParameterListNodeTreePass0( nodetree->siblingNode,
																		RidParamList );
		}
		break;
	case CG_NODE_TYPEDIDENTIFIER:
		{
			wstring strScopedVariableName = getCurrentScopeName() + *nodetree->lhsNode->pIdentifier;
			RID ridSrc = lookUpSymbolInformationInCurrentScope( strScopedVariableName );
			if( ridSrc != RID_NOTDEFINED ) {
				//Oops, I'm registerred already.
				reportError( ERROR_PARAMETERALREADYDEFINED, *nodetree->lhsNode->pIdentifier,
							nodetree->lhsNode->iFileId,
							nodetree->lhsNode->iLineNumber,
							nodetree->lhsNode->iColumn );
				return nodetree;
			}

			CCgNodeData* nodeType = nodetree->rhsNode;
			hash_map< wstring, TYPECONSTRAINT_INFO>::iterator it;

			it = m_MapTypeConstraint.find( *nodeType->pIdentifier );
			if( it == m_MapTypeConstraint.end() )
			{
				reportError( ERROR_ILLEGALTYPESPECIFIER, *nodeType->rhsNode->pIdentifier,
							nodetree->rhsNode->iFileId,
							nodetree->rhsNode->iLineNumber,
							nodetree->rhsNode->iColumn );
				return nodetree;
			}


			int32_t iIndex = m_MetaData.addParamListEntry( RidParamList,
														it->second.type.type,
														OPERAND_FLAG_STRICT,
														it->second.type.ridConstraintType  );
			generateSymbolInformation( RidParamList,
										strScopedVariableName,
										CG_SYMBOL_FLAG_ARG,
										iIndex );

			//Recursively parse Node tree
			if( nodetree->siblingNode )
				nodetree->siblingNode = parseParameterListNodeTreePass0( nodetree->siblingNode,
																		RidParamList );
		}
		break;
	case CG_NODE_RESTARGS:
		{
			//Rest arguments (...args)
			//

			//Set temporary variable to notify it has restArgs
			m_bRestArgsInArgList = true;

			assert( nodetree->lhsNode == NULL );	//Should be NULL node
			assert( nodetree->rhsNode != NULL );	//Should be identifier

			wstring strScopedVariableName = getCurrentScopeName();
			
			switch( nodetree->rhsNode->NodeType )
			{
			case CG_NODE_IDENTIFIER:
				strScopedVariableName.append( *nodetree->rhsNode->pIdentifier );
				break;
			case CG_NODE_TYPEDIDENTIFIER:
				strScopedVariableName.append( *nodetree->rhsNode->lhsNode->pIdentifier );
				break;
			default:
				Debug_Fatal( "Illegal Node" );
			}

			RID ridSrc = lookUpSymbolInformationInCurrentScope( strScopedVariableName );
			if( ridSrc != RID_NOTDEFINED ) {
				//Oops, I'm registerred already.
				reportError( ERROR_PARAMETERALREADYDEFINED, *nodetree->lhsNode->pIdentifier,
							nodetree->lhsNode->iFileId,
							nodetree->lhsNode->iLineNumber,
							nodetree->lhsNode->iColumn );
				return nodetree;
			}

			//
			//TODO: note that array constraint is not implemented yet
			//

			int32_t iIndex = m_MetaData.addParamListEntry( RidParamList, OPERAND_UNDEFINED );
			generateSymbolInformation( RidParamList,
										strScopedVariableName,
										CG_SYMBOL_FLAG_ARG,
										iIndex );

			//Should be last node in the arg tree
			if( nodetree->siblingNode )
			{
				Debug_Fatal( "Illega node" );
			}
		}
		break;
	case CG_NODE_STRINGLITERAL:
	case CG_NODE_REGEXLITERAL:
	case CG_NODE_INTEGERLITERAL:
	case CG_NODE_FLOATLITERAL:
	case CG_NODE_IFNODE:
	case CG_NODE_RETNODE:
	case CG_NODE_FORMULANODE:
	case CG_NODE_EXCEPTIONHANDLINGNODE:
	case CG_NODE_THROWNODE:
	case CG_NODE_WITHNODE:
	case CG_NODE_SWITCHNODE:
	case CG_NODE_CATCHNODE:
	case CG_NODE_FINALLYNODE:
	case CG_NODE_LABELEDNODE:
	case CG_NODE_CASECLAUSENODE: 
	default:
		Debug_Fatal( "Illegal Node" );
		break;
	}
	return nodetree;
}

/***************************************************************************
//parse & emit IL code
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseNodeTreeSub( CCgNodeData* nodetree,
											CCgNodeData* parentnode,
											bool bStackAdjustment )
{
	assert( nodetree != NULL );
	bool bResultInStack = false;

	//Parsing the tree
	if( nodetree->lhsNode && !nodetree->bBlockTraverseLeft )
	{
		switch( nodetree->lhsNode->NodeType )
		{
		case CG_NODE_FORMULANODE:
		case CG_NODE_IDENTIFIER:
		case CG_NODE_BOOLEANLITERAL:
		case CG_NODE_STRINGLITERAL:
		case CG_NODE_REGEXLITERAL:
		case CG_NODE_INTEGERLITERAL:
		case CG_NODE_FLOATLITERAL:
			//Recursive call
			nodetree->lhsNode = parseNodeTreeSub( nodetree->lhsNode,
													nodetree,
													bStackAdjustment );
			break;
		default:
			break;
		}
	}

	if( nodetree->rhsNode && !nodetree->bBlockTraverseRight )
	{
		switch( nodetree->rhsNode->NodeType )
		{
		case CG_NODE_FORMULANODE:
		case CG_NODE_IDENTIFIER:
		case CG_NODE_BOOLEANLITERAL:
		case CG_NODE_STRINGLITERAL:
		case CG_NODE_REGEXLITERAL:
		case CG_NODE_INTEGERLITERAL:
		case CG_NODE_FLOATLITERAL:
			//Recursive call
			nodetree->rhsNode = parseNodeTreeSub( nodetree->rhsNode,
													nodetree,
													bStackAdjustment );
			break;
		default:
			break;
		}
	}

	//Yes, I'm end of the node now
	switch( nodetree->NodeType )
	{
	case CG_NODE_IFNODE:
		nodetree = parseIfNode( nodetree );
		break;
	case CG_NODE_FORNODE:
		nodetree = parseForNode( nodetree, false );
		break;
	case CG_NODE_FORINNODE:
		nodetree = parseForInNode( nodetree );
		break;
	case CG_NODE_DOWHILENODE:
		nodetree = parseForNode( nodetree, true );
		break;
	case CG_NODE_BREAKNODE:
		nodetree = parseBreakStatementNode( nodetree );
		break;
	case CG_NODE_CONTINUENODE:
		nodetree = parseContinueStatementNode( nodetree );
		break;
	case CG_NODE_FUNCTIONDECLNODE:
		//Function decl STATEMENT
		nodetree = parseFunctionDeclarationNode( nodetree, NULL );

		//Pop return value
//		emit( CEE_POP );
		break;
	case CG_NODE_EXCEPTIONHANDLINGNODE:
		nodetree = parseExceptionHandlingNode( nodetree );
		break;
	case CG_NODE_RETNODE:
		nodetree = parseRetStatementNode( nodetree );
		break;
	case CG_NODE_THROWNODE:
		nodetree = parseThrowStatementNode( nodetree );
		break;
	case CG_NODE_SWITCHNODE:
		nodetree = parseSwitchStatementNode( nodetree );
		break;
	case CG_NODE_LABELEDNODE:
		nodetree = parseLabeledStatementNode( nodetree );
		break;
	case CG_NODE_CASECLAUSENODE: 
		nodetree = parseCaseNode( nodetree );
		break;
	case CG_NODE_EXPRESSIONNODE:
		nodetree = parseExpressionStatementNode( nodetree );
		break;
	case CG_NODE_WITHNODE:
		nodetree = parseWithStatementNode( nodetree );
		break;
	case CG_NODE_FORMULANODE:
		switch( nodetree->iOperation )
		{
		case CG_CALLEXPRESSION:
			nodetree = emitCallExpression( nodetree );
			break;
		case CG_PROPERTYBYID:
			nodetree = emitAccessPropertyById( nodetree, parentnode );
			break;
		case CG_PROPERTYBYEXP:
			nodetree = emitAccessPropertyByExp( nodetree, parentnode );
			break;
		case CG_VARIABLEDECL:
			nodetree = emitVariableDeclaration( nodetree );
			break;
		case CG_TYPEDVARIABLEDECL:
			nodetree = emitTypedVariableDeclaration( nodetree );
			break;
		case CG_VARIABLEASSIGN:
			nodetree = emitVariableAssignment( nodetree );
			break;
		case CG_ADD:
			nodetree = emitAddOperator( nodetree );
			break;
		case CG_SUB:
			nodetree = emitSubtractOperator( nodetree );
			break;
		case CG_MUL:
			nodetree = emitMultiplyOperator( nodetree );
			break;
		case CG_DIV:
			nodetree = emitDivisionOperator( nodetree );
			break;
		case CG_REM:
			nodetree = emitReminderOperator( nodetree );
			break;
		case CG_EQUAL:
			nodetree = emitCmpOperator( nodetree, CEE_CEQ );
			break;
		case CG_NOTEQUAL:
			nodetree = emitCmpOperator( nodetree, CEE_EXT_CNE );
			break;
		case CG_STRICTEQUAL:
			nodetree = emitStrictCmpOperator( nodetree, CEE_CEQ );
			break;
		case CG_STRICTNOTEQUAL:
			nodetree = emitStrictCmpOperator( nodetree, CEE_EXT_CNE );
			break;
		case CG_GREATER:
			nodetree = emitCmpOperator( nodetree, CEE_CGT );
			break;
		case CG_GREATEREQUAL:
			nodetree = emitCmpOperator( nodetree, CEE_EXT_CGE );
			break;
		case CG_LESS:
			nodetree = emitCmpOperator( nodetree, CEE_CLT );
			break;
		case CG_LESSEQUAL:
			nodetree = emitCmpOperator( nodetree, CEE_EXT_CLE );
			break;
		case CG_BITWISEAND:
			nodetree = emitBitwiseAndOperator( nodetree );
			break;
		case CG_BITWISEOR:
			nodetree = emitBitwiseOrOperator( nodetree );
			break;
		case CG_LOGICALAND:
			nodetree = emitLogicalAndOperator( nodetree );
			break;
		case CG_LOGICALOR:
			nodetree = emitLogicalOrOperator( nodetree );
			break;
		case CG_BITWISEXOR:
			nodetree = emitBitwiseXorOperator( nodetree );
			break;
		case CG_SHL:
			nodetree = emitShiftLeftOperator( nodetree );
			break;
		case CG_SHR:
			nodetree = emitShiftRightOperator( nodetree );
			break;
		case CG_SHRUN:
			nodetree = emitShiftRightUnsignedOperator( nodetree );
			break;
		case CG_UNARYPLUS:
			nodetree = emitUnaryPlusOperator( nodetree );
			break;
		case CG_UNARYMINUS:
			nodetree = emitUnaryMinusOperator( nodetree );
			break;
		case CG_UNARYINC:
			nodetree = emitUnaryIncOperator( nodetree, false );
			break;
		case CG_UNARYDEC:
			nodetree = emitUnaryDecOperator( nodetree, false );
			break;
		case CG_UNARYPOSTINC:
			nodetree = emitUnaryIncOperator( nodetree, true );
			bResultInStack = true;
			break;
		case CG_UNARYPOSTDEC:
			nodetree = emitUnaryDecOperator( nodetree, true );
			bResultInStack = true;
			break;
		case CG_UNARYBITWISENOT:
			nodetree = emitUnaryBitwiseNotOperator( nodetree );
			break;
		case CG_UNARYLOGICALNOT:
			nodetree = emitUnaryLogicalNotOperator( nodetree );
			break;
		case CG_UNARYVOID:
			nodetree = emitUnaryVoidOperator( nodetree );
			break;
		case CG_UNARYDELETE:
			nodetree = emitUnaryDeleteOperator( nodetree );
			break;
		case CG_UNARYTYPEOF:
			nodetree = emitUnaryTypeofOperator( nodetree );
			break;
		case CG_objectLiteral:
			nodetree = emitObjectLiteral( nodetree );
			break;
		case CG_arrayLiteral:
			nodetree = emitArrayLiteral( nodetree );
			break;
		case CG_CONDITIONAL:
			nodetree = emitConditionalExpression( nodetree );
			break;
		case CG_NEW:
			nodetree = emitNewOperator( nodetree );
			break;
		case CG_INSTANCEOF:
			nodetree = emitInstanceOfOperator( nodetree );
			break;
		case CG_IN:
			nodetree = emitInOperator( nodetree );
			break;
		case CG_NONE:
			//Do nothing
			break;
		default:
			Debug_Fatal( "Not Implemented yet" );
		}
		break;
	case CG_NODE_IDENTIFIER:
	case CG_NODE_BOOLEANLITERAL:
	case CG_NODE_STRINGLITERAL:
	case CG_NODE_REGEXLITERAL:
	case CG_NODE_INTEGERLITERAL:
	case CG_NODE_FLOATLITERAL:
		emitLoadStack( nodetree );
		break;
	case CG_NODE_NULL:
		break;
	default:
		Debug_Fatal( "Not Implemented yet" );
	}

	// delete tree nodes
	if( nodetree )
	{
		delete nodetree->lhsNode;
		delete nodetree->rhsNode;
		delete nodetree->initialNode;
		delete nodetree->conditionNode;
		delete nodetree->statementNode;
		delete nodetree->argumentlistNode;
		nodetree->lhsNode = NULL;
		nodetree->rhsNode = NULL;
		nodetree->initialNode = NULL;
		nodetree->conditionNode = NULL;
		nodetree->statementNode = NULL;
		nodetree->argumentlistNode = NULL;
 		if( nodetree->NodeType == CG_NODE_IDENTIFIER )
		{
			//Delete string
			delete nodetree->pIdentifier;
		}
		nodetree->NodeType = CG_NODE_NULL;

		if( nodetree->siblingNode != NULL )
		{
			//nodetree->siblingNode = parseNodeTreeSub( nodetree->siblingNode,
			//											NULL,
			//											bStackAdjustment );
			nodetree->siblingNode = parseStatementNodeTree( nodetree->siblingNode );

			delete nodetree->siblingNode;
			nodetree->siblingNode = NULL;
		}
	}

	m_bResultInEvalStack = bResultInStack;
	return nodetree;
}

/***************************************************************************
 * Push a value onto the stack
 ***************************************************************************/
CCgNodeData* CCilCodeGen::emitLoadStack( CCgNodeData* ptr )
{

	if( ptr == NULL )
	{
		Debug_Fatal( "Not implemented yet" );
		return NULL;
	}

	//I need to be the leaf node
	assert( ptr->bBlockTraverseRight == true || ptr->rhsNode == NULL || ptr->NodeType == CG_NODE_FORMULANODE );
	assert( ptr->bBlockTraverseLeft == true || ptr->lhsNode == NULL || ptr->NodeType == CG_NODE_FORMULANODE );

	switch( ptr->NodeType )
	{
	case CG_NODE_BOOLEANLITERAL:
		emitLoadBoolean( ptr->iIndex == 0 ? false : true );
		break;
	case CG_NODE_STRINGLITERAL:
		emitLoadStringObject( ptr->rid );
		break;
	case CG_NODE_REGEXLITERAL:
		emitLoadRegexObject( ptr );
		break;
	case CG_NODE_INTEGERLITERAL:
		emitLoadInteger( m_IntegerConstantPool[ ptr->iIndex ] );
		break;
	case CG_NODE_FLOATLITERAL:
		emitLoadFloat( m_FloatConstantPool[ ptr->iIndex ] );
		break;
	case CG_NODE_IDENTIFIER:		//Load variable
		{
			//From static field?
			wstring strScopedName = getCurrentScopeName() + *ptr->pIdentifier;
			CG_SYMBOL_INFORMATION* info = NULL;
			RID ridSrc = lookUpSymbolInformation( strScopedName, &info );
			if( ridSrc == RID_NOTDEFINED ) {
				//Oops, I'm not registerred yet.
				//In This case, global field is created automatically
				ridSrc = m_MetaData.generateStaticFieldRID();
				generateSymbolInformation( ridSrc, strScopedName );
				reportError( INFO_IDNOTREGISTERED, *ptr->pIdentifier,
							ptr->iFileId,
							ptr->iLineNumber,
							ptr->iColumn );
				lookUpSymbolInformation( strScopedName, &info );
			}

			switch( SymbolTypeFromFrag( info->Flags ) )
			{
			case CG_SYMBOL_FLAG_GLOBAL:
			//case CG_SYMBOL_FLAG_STATIC:	//Same value of 'global'
				switch( TypeFromToken( ridSrc ) )
				{
				case MDT_FIELDDEF:
					if( ridSrc == RID_CONSTANT_THIS )
					{
						//Special case for 'this' pointer
						emit( CEE_LDARG_0 );
					}
					else
					{
						emit( CEE_LDSFLD, ridSrc );
					}
					break;
				case MDT_METHODDEF:
					{
						//Generate static field variable symbol
						wstring s( strScopedName + STRING_OBJECTNAME_POSTFIX );
						RID ridObj = lookUpSymbolInformation( s, &info );
						assert( ridObj != RID_NOTDEFINED );
						emit( CEE_LDSFLD, ridObj );
					}
					break;
				case MDT_TYPEDEF:
					emit( CEE_EXT_STARGLIST, (uint32_t)0 );
					emit( CEE_NEWOBJ, ridSrc );
					break;
				default:
					Debug_Fatal( "Not implemented yet" );
				}
				break;
			case CG_SYMBOL_FLAG_ARG:
				//Or from param list?
				emitLoadArgument( info );
				break;
			case CG_SYMBOL_FLAG_LOCAL:
				//Or From local variable?
				emitLoadLocal( info );
				break;
			case CG_SYMBOL_FLAG_PROPERTY:
				{
				//Load a variable from a property

				//Generate RID for the string
				CCgNodeData* tmpNode = literalConstant( ptr->pIdentifier->data() );
				emit( CEE_LDC_I4, tmpNode->rid );
				delete tmpNode;

				//this pointer
				emit( CEE_LDARG_0 );

				//Load property with the ID
				emit( CEE_EXT_STARGLIST_S, (uint8_t)2 );	//Move to arglist
				emit( CEE_CALL, m_ridMethodLdProperty );
				}
				break;
			case CG_SYMBOL_FLAG_INSTANCE:
				//Or from instance field?
				Debug_Fatal( "Not implemented yet" );
				break;
			default:
				Debug_Fatal( "Not implemented yet" );
				break;
			}
		}
		break;
	case CG_NODE_FUNCTIONDECLNODE:
		//Function decl EXPRESSION
		ptr = parseFunctionExpressionNode( ptr );
		break;
	case CG_NODE_FORMULANODE:
		switch( ptr->iOperation )
		{
		case CG_CALLEXPRESSION:
			ptr = emitCallExpression( ptr );
			break;
		case CG_PROPERTYBYID:
			ptr = emitAccessPropertyById( ptr, NULL );
			break;
		case CG_PROPERTYBYEXP:
			ptr = emitAccessPropertyByExp( ptr, NULL );
			break;
		case CG_VARIABLEDECL:
			ptr = emitVariableDeclaration( ptr );
			break;
		case CG_TYPEDVARIABLEDECL:
			ptr = emitTypedVariableDeclaration( ptr );
			break;
		case CG_VARIABLEASSIGN:
			ptr = emitVariableAssignment( ptr );
			break;
		case CG_ADD:
			ptr = emitAddOperator( ptr );
			break;
		case CG_SUB:
			ptr = emitSubtractOperator( ptr );
			break;
		case CG_MUL:
			ptr = emitMultiplyOperator( ptr );
			break;
		case CG_DIV:
			ptr = emitDivisionOperator( ptr );
			break;
		case CG_REM:
			ptr = emitReminderOperator( ptr );
			break;
		case CG_EQUAL:
			ptr = emitCmpOperator( ptr, CEE_CEQ );
			break;
		case CG_NOTEQUAL:
			ptr = emitCmpOperator( ptr, CEE_EXT_CNE );
			break;
		case CG_STRICTEQUAL:
			ptr = emitStrictCmpOperator( ptr, CEE_CEQ );
			break;
		case CG_STRICTNOTEQUAL:
			ptr = emitStrictCmpOperator( ptr, CEE_EXT_CNE );
			break;
		case CG_GREATER:
			ptr = emitCmpOperator( ptr, CEE_CGT );
			break;
		case CG_GREATEREQUAL:
			ptr = emitCmpOperator( ptr, CEE_EXT_CGE );
			break;
		case CG_LESS:
			ptr = emitCmpOperator( ptr, CEE_CLT );
			break;
		case CG_LESSEQUAL:
			ptr = emitCmpOperator( ptr, CEE_EXT_CLE );
			break;
		case CG_BITWISEAND:
			ptr = emitBitwiseAndOperator( ptr );
			break;
		case CG_BITWISEOR:
			ptr = emitBitwiseOrOperator( ptr );
			break;
		case CG_LOGICALAND:
			ptr = emitLogicalAndOperator( ptr );
			break;
		case CG_LOGICALOR:
			ptr = emitLogicalOrOperator( ptr );
			break;
		case CG_BITWISEXOR:
			ptr = emitBitwiseXorOperator( ptr );
			break;
		case CG_SHL:
			ptr = emitShiftLeftOperator( ptr );
			break;
		case CG_SHR:
			ptr = emitShiftRightOperator( ptr );
			break;
		case CG_SHRUN:
			ptr = emitShiftRightUnsignedOperator( ptr );
			break;
		case CG_UNARYPLUS:
			ptr = emitUnaryPlusOperator( ptr );
			break;
		case CG_UNARYMINUS:
			ptr = emitUnaryMinusOperator( ptr );
			break;
		case CG_UNARYINC:
			ptr = emitUnaryIncOperator( ptr, false );
			break;
		case CG_UNARYDEC:
			ptr = emitUnaryDecOperator( ptr, false );
			break;
		case CG_UNARYPOSTINC:
			ptr = emitUnaryIncOperator( ptr, true );
			break;
		case CG_UNARYPOSTDEC:
			ptr = emitUnaryDecOperator( ptr, true );
			break;
		case CG_UNARYBITWISENOT:
			ptr = emitUnaryBitwiseNotOperator( ptr );
			break;
		case CG_UNARYLOGICALNOT:
			ptr = emitUnaryLogicalNotOperator( ptr );
			break;
		case CG_UNARYVOID:
			ptr = emitUnaryVoidOperator( ptr );
			break;
		case CG_UNARYDELETE:
			ptr = emitUnaryDeleteOperator( ptr );
			break;
		case CG_UNARYTYPEOF:
			ptr = emitUnaryTypeofOperator( ptr );
			break;
		case CG_objectLiteral:
			ptr = emitObjectLiteral( ptr );
			break;
		case CG_arrayLiteral:
			ptr = emitArrayLiteral( ptr );
			break;
		case CG_CONDITIONAL:
			ptr = emitConditionalExpression( ptr );
			break;
		case CG_NEW:
			ptr = emitNewOperator( ptr );
			break;
		case CG_INSTANCEOF:
			ptr = emitInstanceOfOperator( ptr );
			break;
		case CG_IN:
			ptr = emitInOperator( ptr );
			break;
		default:
			//if( ptr->lhsNode )
			//	ptr->lhsNode = parseNodeTreeSub( ptr->lhsNode, ptr, false );
			//if( ptr->rhsNode )
			//	ptr->rhsNode = parseNodeTreeSub( ptr->rhsNode, ptr, false );
			assert( ptr->lhsNode == NULL );
			assert( ptr->rhsNode == NULL );
			break;
		}
		break;
	case CG_NODE_NULL:
		break;
	default:
		Debug_Fatal( "Not implemented yet" );
		break;
	}
	return ptr;
}

} //namespace CRI