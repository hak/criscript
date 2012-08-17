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
 * File     : cilFunctionCall.cpp
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
 * callExpression
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::callExpression( CCgNodeData* lhs, CCgNodeData* rhs )
{
	assert( lhs != NULL );
	//rhs can be null
	
	CCgNodeData* Node;

	// A function node is
	// expression + arg list
	//	Arguments should not be parsed until parsed as an argumentlist.
	Node = makeFormulaNode( CG_CALLEXPRESSION, lhs, rhs, true, true );
	Node->iLineNumber = m_pLex->getLineNumber();

	return Node;
}

/***************************************************************************
 * emit call expression
  ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitCallExpression( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FORMULANODE );

	uint32_t iNumArgs = 0;

	CCgNodeData* ptr = nodetree->lhsNode;
	
	switch( ptr->NodeType )
	{
	case CG_NODE_IDENTIFIER:
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
		} else if ( ridSrc == m_ridMethodEval )
		{
			if( parseEvalNodeTree( nodetree ) )
				return nodetree;
		}

		//Check Function('function body') call which is prohibited in CriScript
		if( info->ridObject == m_ridFunctionObject
			&& nodetree->rhsNode != NULL )
		{
			reportError( ERROR_FUNCTIONCTORNOTSUPPORTED, ptr->iFileId, ptr->iLineNumber, ptr->iColumn );
			return nodetree;
		}

		switch( TypeFromToken( ridSrc ) )
		{
		case MDT_METHODDEF:
			{
			if( nodetree->rhsNode != NULL )
				nodetree->rhsNode = emitArgumentList( nodetree->rhsNode, iNumArgs );
			else
				emit( CEE_EXT_STARGLIST, (uint32_t)0 );

			//call function indicated by RID
			emit( CEE_CALL, ridSrc );

			//Sanity check of num args if the callee is determined already
			METHOD_DEF* pMethod = &m_MetaData.getMethod( ridSrc );

			if( pMethod->iNumArgments != iNumArgs + 1 )	//+1 is for this pointer
				reportError( INFO_DIFFERENTNUMARGS,
							ptr->iFileId,
							ptr->iLineNumber,
							ptr->iColumn );
			}
			break;
		case MDT_FIELDDEF:
			{
			if( nodetree->rhsNode != NULL )
				nodetree->rhsNode = emitArgumentList( nodetree->rhsNode, iNumArgs );
			else
				emit( CEE_EXT_STARGLIST, (uint32_t)0 );

			//If the field is OBJREF, it can be invoked.
			int32_t iIndex = RidFromToken( ridSrc );

			OPERAND_TYPE type = m_MetaData.getStaticFieldTypeTable()[ iIndex ].type;
			OPERAND_FLAG flag = m_MetaData.getStaticFieldTypeTable()[ iIndex ].flag;

			if( flag & OPERAND_FLAG_STRICT )
			{
				if( OperandType(type) != OPERAND_OBJECTREF )
				{
					reportError( ERROR_ILLEGALCALLTYPE, *ptr->pIdentifier,
							ptr->iFileId,
							ptr->iLineNumber,
							ptr->iColumn );
				}
			}

			//call function indicated by RID
			emit( CEE_CALL, ridSrc );
			}
			break;
		case MDT_PARAMDEF:
			{
			//If the field is OBJREF, it can be invoked.
			OPERAND_TYPE type;
			OPERAND_FLAG flag;
			if( isBuiltinRid( ridSrc ) )
			{
				vector< OPERAND_TYPE_FLAG >& arglist = m_MetaData.getBuiltinParamListTable()[ IndexFromBuiltinRid( ridSrc ) ];
				type = arglist[ info->iIndex ].type;
				flag = arglist[ info->iIndex ].flag;
			}
			else
			{
				vector< OPERAND_TYPE_FLAG >& arglist = m_MetaData.getParamListTable()[ RidFromToken( ridSrc ) ];
				type = arglist[ info->iIndex ].type;
				flag = arglist[ info->iIndex ].flag;
			}
			if( flag & OPERAND_FLAG_STRICT )
			{
				if( OperandType(type) != OPERAND_OBJECTREF )
				{
					reportError( ERROR_ILLEGALCALLTYPE, *ptr->pIdentifier,
							ptr->iFileId,
							ptr->iLineNumber,
							ptr->iColumn );
				}
			}

			switch( SymbolTypeFromFrag( info->Flags ) )
			{
			case CG_SYMBOL_FLAG_ARG:
				//Or from param list?
				emitLoadArgument( info );
				break;
			case CG_SYMBOL_FLAG_LOCAL:
				//Or From local variable?
				emitLoadLocal( info );
				break;
			default:
				Debug_Fatal( "Not implemented yet" );
				break;
			}

			//right hand side = parameter list
			if( nodetree->rhsNode != NULL )
				nodetree->rhsNode = emitArgumentList( nodetree->rhsNode, iNumArgs );
			else
				emit( CEE_EXT_STARGLIST, (uint32_t)0 );

			//call function indirect
			emit( CEE_EXT_CALLI );
			}
			break;
		default:
			Debug_Fatal( "Not implemented yet" );
		}
		break;
	}
	case CG_NODE_FORMULANODE:
		parseExpressionNodeTree( ptr, false );
		//Should leave something on the stack

		//right hand side = parameter list
		if( nodetree->rhsNode != NULL )
			nodetree->rhsNode = emitArgumentList( nodetree->rhsNode, iNumArgs );
		else
			emit( CEE_EXT_STARGLIST, (uint32_t)0 );

		emit( CEE_EXT_CALLI );
		nodetree->lhsNode = NULL;
		break;
	default:
		Debug_Fatal( "Not implemented yet" );	
		break;
	}

	//I'm done
	nodetree->iOperation = CG_NONE;
	return nodetree;
}

/***************************************************************************
 * parse and emit argument list
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::emitArgumentList( CCgNodeData* nodetree, uint32_t& iNumArgumentparsed )
{
	assert( nodetree != NULL );
	uint32_t iNumArguments = 1;
	CCgNodeData* node = nodetree;
	while( node->siblingNode != NULL )
	{
		node = node->siblingNode;
		iNumArguments ++;
	}

	//Load args into the eval stack
	parseExpressionNodeTree( nodetree, false );

	if( iNumArguments > MAXBYTE )
	{
		emit( CEE_EXT_STARGLIST, (uint32_t)iNumArguments );
	}
	else
	{
		emit( CEE_EXT_STARGLIST_S, (uint8_t)iNumArguments );
	}

	iNumArgumentparsed = iNumArguments;

	return NULL;
}

/***************************************************************************
 * Node parser helpers
 ***************************************************************************/	
bool CCilCodeGen::parseEvalNodeTree( CCgNodeData* nodetree )
{
	assert( nodetree != NULL );

	//Special case handling for eval.
	//If eval arg is constant literal, we can avoid dynamic code generation
	if( nodetree->rhsNode != NULL
		&& nodetree->rhsNode->NodeType == CG_NODE_STRINGLITERAL
		&& nodetree->rhsNode->siblingNode == NULL )
	{
		//eval( "codes" );

		//Save lexer 
		CLexer*	pLex = m_pLex;

		//Switch lexer
		m_pParser->SetLexer( m_pStringLexer );
		m_pLex = m_pStringLexer;
		wstring str = getMetaData().getStringConstant( nodetree->rhsNode->rid );
		str += L";;;";
		m_pStringLexer->setStream( &str );
		m_pStringLexer->setLineNumber( nodetree->iLineNumber );

		int32_t i;
		m_pRootNode = NULL;
		m_pLastNode = NULL;
		i = yyparse();
		if( !i && m_pRootNode )
		{
			//parse succeeded

			//Restore lexer
			m_pLex = pLex;
			m_pParser->SetLexer( pLex );

			CCgNodeData* pNode = parseExpressionNodeTree( m_pLastNode, false );
			delete pNode;

			nodetree->iOperation = CG_NONE;
			return true;
		}
		else
		{
//					Debug_Fatal( "");
		}

		//Restore lexer
		m_pLex = pLex;
		m_pParser->SetLexer( pLex );
		
	}
	else
	{
		//reportError( ERROR_DYNAMICEVALNOTSUPPORTED, *ptr->pIdentifier,
		//				ptr->iFileId,
		//				ptr->iLineNumber,
		//				ptr->iColumn );
		//return nodetree;
	}
	return false;
}

} //namespace CRI