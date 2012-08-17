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
 * File     : cilFunctionDeclaration.cpp
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
 * Function decl
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::functionDeclaration( CCgNodeData* identifier,
												   CCgNodeData* ParameterList,
												   CCgNodeData* FunctionBody
												   )
{
	return makeFunctionDeclarationNode( identifier, ParameterList, FunctionBody );
};

/***************************************************************************
 * TypedFunction decl
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::typedFunctionDeclaration( CCgNodeData* identifier,
												   CCgNodeData* ParameterList,
												   CCgNodeData* FunctionBody,
												   CCgNodeData* ReturnValue )
{
	CCgNodeData* object = makeFunctionDeclarationNode( identifier, ParameterList, FunctionBody );
	object->initialNode = ReturnValue;
	return object;
};

/***************************************************************************
//Create FunctionDeclarationNode node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::makeFunctionDeclarationNode(	CCgNodeData* identifier,
								CCgNodeData* ParameterList,
								CCgNodeData* FunctionBody )
{
	// all node can be null
	CCgNodeData* object = new CCgNodeData;
	object->NodeType = CG_NODE_FUNCTIONDECLNODE;
	object->identifier = identifier;
	object->argumentlistNode = ParameterList;
	object->statementNode = FunctionBody;
	object->iLineNumber = m_pLex->getLineNumber();
	return object;
};

/***************************************************************************
//parse function decl node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseFunctionDeclarationNode( CCgNodeData* nodetree, RID* pridMethod )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FUNCTIONDECLNODE );
	//Sibling node can be anything

	ILFRAGMENTINFO* pILinfo = getCurrentScopeInformation();
	wstring strScopedFunctionName;

	RID ridMethod;
	RID ridArguments;
	RID ridLocalVariables;

	if( isMultiPasses() )
	{
	//---------------------------------------------------------------------------------------------
	//Multipass mode
	//---------------------------------------------------------------------------------------------
		//Pass1 operations
		CCgNodeData* objectptr = nodetree->identifier;

		assert( objectptr != NULL );
		assert( nodetree->identifier->NodeType == CG_NODE_IDENTIFIER );
		strScopedFunctionName = getCurrentScopeName()
				+ *objectptr->pIdentifier;

		CG_SYMBOL_INFORMATION* pInfo;
		if( RID_NOTDEFINED == lookUpSymbolInformationInCurrentScope( strScopedFunctionName, &pInfo ) )
		{
			//error but tries to generate symbol

			ridArguments = m_MetaData.generateParamListRID();
			//Note
			//First entry of the argument list is always "this" poiner
			m_MetaData.addParamListEntry( ridArguments, OPERAND_OBJECTREF );
			
			ridLocalVariables = m_MetaData.generateParamListRID();

			//Note that iNumArgments and iNumLocals are not resolved yet here
			ridMethod = m_MetaData.generateMethodRID( METHOD_CIL, pILinfo->MethodRID, ridArguments, ridLocalVariables );

			//Generate static field variable symbol
			RID rid = m_MetaData.generateStaticFieldRID( OPERAND_OBJECTREF );
			generateSymbolInformation( ridMethod, strScopedFunctionName );
			generateSymbolInformation( rid, strScopedFunctionName + STRING_OBJECTNAME_POSTFIX );

			//Register function properties
			generateSymbolInformation( m_MetaData.generatePropertyRID(), 
				strScopedFunctionName + L"." + NAME_BUILTIN_FUNCTION_PROPERTY_ARGUMENTS,
				CG_SYMBOL_FLAG_PROPERTY );

			//Create a function object and load it to the variable
			emit( CEE_LDC_I4, ridMethod );	//Push the mothod RID to eval stack
			emit( CEE_LDC_I4, rid );	//Push the object RID to eval stack
			emit( CEE_EXT_STARGLIST_S, (uint8_t)2 );	//Move to arglist
			emit( CEE_CALL, m_ridMethodFunctionCtorWithRid );		//Function Object Constructor
//				adjustEvalStack( -1 );	//The constructor does not return value

		}
		else
		{
			ridMethod = pInfo->rid;
		}

		METHOD_DEF* pMethod = &m_MetaData.getMethodTable()[ RidFromToken( ridMethod ) ];
		ridArguments = pMethod->ridArgList;
		ridLocalVariables = pMethod->ridLocalVariableList;

		if( m_bTreatFunctionNameReadOnly )
		{
			if( pridMethod != NULL )
				*pridMethod = ridMethod;
		}
		else
		{
			//Look up static field variable symbol
			wstring s( strScopedFunctionName + STRING_OBJECTNAME_POSTFIX );
			RID rid = lookUpSymbolInformationInCurrentScope( s );
			assert( rid != RID_NOTDEFINED );

			//Create a function object and load it to the variable
			emit( CEE_LDC_I4, ridMethod );	//Push the mothod RID to eval stack
			emit( CEE_LDC_I4, rid );	//Push the object RID to eval stack
			emit( CEE_EXT_STARGLIST_S, (uint8_t)2 );	//Move to arglist
			emit( CEE_CALL, m_ridMethodFunctionCtorWithRid );		//Function Object Constructor
//			adjustEvalStack( -1 );	//The constructor does not return value

			if( pridMethod != NULL )
				*pridMethod = rid;
		}
		//Entering new function scope
		enterNewScope( strScopedFunctionName, ridMethod );

		m_MetaData.getMethodTable()[ RidFromToken( ridMethod )].iILOffset = tagTheLine();

		if( nodetree->argumentlistNode )
		{
			nodetree->argumentlistNode = parseParameterListNodeTree( nodetree->argumentlistNode, ridArguments );
		}

		if( nodetree->statementNode )
		{
			nodetree->statementNode = parseStatementNodeTree( nodetree->statementNode );
		}

		//Check if the function returns something or not
		if( !m_MetaData.getMethodTable()[ RidFromToken( getCurrentILInfo().MethodRID ) ].iNumberOfReturnPass )
		{
			m_MetaData.getMethodTable()[ RidFromToken( getCurrentILInfo().MethodRID ) ].iNumberOfReturnPass ++;
			//Return undefined
			emit( CEE_LDSFLD, RID_CONSTANT_UNDEFINED );
			emit( CEE_RET );
		}

		//Resolve locals and args
		m_MetaData.getMethodTable()[ RidFromToken( ridMethod ) ].iNumLocals =
											(int32_t)m_MetaData.getParamListSize( ridLocalVariables );
		//Leave current function scope. Good bye.
		leaveScope();

	}
	else
	{
	//---------------------------------------------------------------------------------------------
	//Single pass mode
	//---------------------------------------------------------------------------------------------
		ridArguments = m_MetaData.generateParamListRID();
		//Note
		//First entry of the argument list is always "this" poiner
		m_MetaData.addParamListEntry( ridArguments, OPERAND_OBJECTREF );
		
		ridLocalVariables = m_MetaData.generateParamListRID();

		//Note that iNumArgments and iNumLocals are not resolved yet here
		ridMethod = m_MetaData.generateMethodRID( METHOD_CIL, pILinfo->MethodRID, ridArguments, ridLocalVariables );

		CCgNodeData* objectptr = nodetree->identifier;
		if( objectptr != NULL )
		{
			assert( nodetree->identifier->NodeType == CG_NODE_IDENTIFIER );
			strScopedFunctionName = getCurrentScopeName()
						+ *objectptr->pIdentifier;
			if( RID_NOTDEFINED != lookUpSymbolInformationInCurrentScope( strScopedFunctionName ) )
			{
				reportError( ERROR_IDALREADYDEFINED, strScopedFunctionName,
								objectptr->iFileId,
								objectptr->iLineNumber,
								objectptr->iColumn );
				delete nodetree;
				return NULL;
			}

			if( m_bTreatFunctionNameReadOnly )
			{
				//Function name is unique and readonly.
				//So it's safe to generate a symbol as RIDMothod
				//(And when attempt to assgin the variable, it can be caught by the compiler
				generateSymbolInformation( ridMethod, strScopedFunctionName );

				//Register function properties
				generateSymbolInformation( m_MetaData.generatePropertyRID(), 
					strScopedFunctionName + L"." + NAME_BUILTIN_FUNCTION_PROPERTY_ARGUMENTS,
					CG_SYMBOL_FLAG_PROPERTY );

				if( pridMethod != NULL )
					*pridMethod = ridMethod;
			}
			else
			{
				//Generate static field variable symbol
				RID rid = m_MetaData.generateStaticFieldRID( OPERAND_OBJECTREF );
				generateSymbolInformation( ridMethod, strScopedFunctionName );
				generateSymbolInformation( rid, strScopedFunctionName + STRING_OBJECTNAME_POSTFIX );

				//Register function properties
				generateSymbolInformation( m_MetaData.generatePropertyRID(), 
					strScopedFunctionName + L"." + NAME_BUILTIN_FUNCTION_PROPERTY_ARGUMENTS,
					CG_SYMBOL_FLAG_PROPERTY );

				//Create a function object and load it to the variable
				emit( CEE_LDC_I4, ridMethod );	//Push the mothod RID to eval stack
				emit( CEE_LDC_I4, rid );	//Push the object RID to eval stack
				emit( CEE_EXT_STARGLIST_S, (uint8_t)2 );	//Move to arglist
				emit( CEE_CALL, m_ridMethodFunctionCtorWithRid );		//Function Object Constructor
//				adjustEvalStack( -1 );	//The constructor does not return value

				if( pridMethod != NULL )
					*pridMethod = rid;
			}
		}
		else
		{
			//Anonymouse functions
			wchar_t buf[ SIZE_STRING ];
			swprintf( buf, SIZE_STRING,  STRING_INTERNAL_LABEL_PREFIX, ridMethod & MAX_NUM_LABELS );
			strScopedFunctionName = getCurrentScopeName() + wstring( buf );

			if( RID_NOTDEFINED != lookUpSymbolInformationInCurrentScope( strScopedFunctionName ) )
			{
				reportError( ERROR_INTERNALIDALREADYDEFINED, strScopedFunctionName );
				delete nodetree;
				return NULL;
			}

			if( m_bTreatFunctionNameReadOnly )
			{
				//Function name is unique and readonly.
				//So it's safe to generate a symbol as RIDMothod
				//(And when attempt to assgin the variable, it can be caught by the compiler
				generateSymbolInformation( ridMethod, strScopedFunctionName );

				//Register function properties
				generateSymbolInformation( m_MetaData.generatePropertyRID(), 
					strScopedFunctionName + L"." + NAME_BUILTIN_FUNCTION_PROPERTY_ARGUMENTS,
					CG_SYMBOL_FLAG_PROPERTY );

				if( pridMethod != NULL )
					*pridMethod = ridMethod;
			}
			else
			{
				//Generate static field variable symbol
				RID rid = m_MetaData.generateStaticFieldRID( OPERAND_OBJECTREF );
				generateSymbolInformation( ridMethod, strScopedFunctionName );
				generateSymbolInformation( rid, strScopedFunctionName + STRING_OBJECTNAME_POSTFIX );

				//Register function properties
				generateSymbolInformation( m_MetaData.generatePropertyRID(), 
					strScopedFunctionName + L"." + NAME_BUILTIN_FUNCTION_PROPERTY_ARGUMENTS,
					CG_SYMBOL_FLAG_PROPERTY );

				//Create a function object and load it to the variable
				emit( CEE_LDC_I4, ridMethod );	//Push the mothod RID to eval stack
				emit( CEE_LDC_I4, rid );	//Push the object RID to eval stack
				emit( CEE_EXT_STARGLIST_S, (uint8_t)2 );	//Move to arglist
				emit( CEE_CALL, m_ridMethodFunctionCtorWithRid );		//Function Object Constructor
//				adjustEvalStack( -1 );	//The constructor does not return value

				if( pridMethod != NULL )
					*pridMethod = rid;
			}

		}
		//Entering new function scope
		enterNewScope( strScopedFunctionName, ridMethod );

		m_MetaData.getMethodTable()[ RidFromToken( ridMethod )].iILOffset = tagTheLine();

		if( nodetree->argumentlistNode )
		{
			//Clear temporary variable
			m_bRestArgsInArgList = false;
			nodetree->argumentlistNode = parseParameterListNodeTree( nodetree->argumentlistNode, ridArguments );
		}

		if( nodetree->statementNode )
		{
			nodetree->statementNode = parseStatementNodeTree( nodetree->statementNode );
		}

		//Check if the function returns something or not
		if( !m_MetaData.getMethodTable()[ RidFromToken( getCurrentILInfo().MethodRID ) ].iNumberOfReturnPass )
		{
			m_MetaData.getMethodTable()[ RidFromToken( getCurrentILInfo().MethodRID ) ].iNumberOfReturnPass ++;
			//Return undefined
			emit( CEE_LDSFLD, RID_CONSTANT_UNDEFINED );
			emit( CEE_RET );
		}


		//Resolve locals and args
		m_MetaData.getMethodTable()[ RidFromToken( ridMethod ) ].iNumArgments =
											(int32_t)m_MetaData.getParamListSize( ridArguments );
		m_MetaData.getMethodTable()[ RidFromToken( ridMethod ) ].iNumLocals =
											(int32_t)m_MetaData.getParamListSize( ridLocalVariables );
		m_MetaData.getMethodTable()[ RidFromToken( ridMethod ) ].bRestArgments = m_bRestArgsInArgList;


		//Leave current function scope. Good bye.
		leaveScope();

	}
	
	return nodetree;
};

/***************************************************************************
//parse function decl node for multipass 0
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseFunctionDeclarationNodePass0( CCgNodeData* nodetree, RID* pridMethod )
{
	assert( nodetree != NULL );
	assert( nodetree->NodeType == CG_NODE_FUNCTIONDECLNODE );

	CCgNodeData* objectptr = nodetree->identifier;

	ILFRAGMENTINFO* pILinfo = getCurrentScopeInformation();
	RID ridArguments = m_MetaData.generateParamListRID();
	
	//Note
	//First entry of the argument list is always "this" poiner
	m_MetaData.addParamListEntry( ridArguments, OPERAND_OBJECTREF );

	RID ridLocalVariables = m_MetaData.generateParamListRID();
	//Note that iNumArgments and iNumLocals are not resolved yet here
	RID ridMethod = m_MetaData.generateMethodRID( METHOD_CIL, pILinfo->MethodRID, ridArguments, ridLocalVariables );

	wstring strScopedFunctionName;
	if( objectptr != NULL )
	{
		assert( nodetree->identifier->NodeType == CG_NODE_IDENTIFIER );
		strScopedFunctionName = getCurrentScopeName()
					+ *objectptr->pIdentifier;
	}
	else
	{
		//Anonymouse functions
		wchar_t buf[ SIZE_STRING ];
		swprintf( buf, SIZE_STRING,  STRING_INTERNAL_LABEL_PREFIX, ridMethod & MAX_NUM_LABELS );

		//Generate a temporary function name for myself
		CCgNodeData* object = new CCgNodeData;
		assert( object != NULL );
		object->NodeType = CG_NODE_IDENTIFIER;
		object->pIdentifier = new wstring( buf );
		object->iFileId = 0;
		object->iLineNumber = 0;
		object->iColumn = 0;
		nodetree->identifier = object;

		strScopedFunctionName = getCurrentScopeName()
				+ *object->pIdentifier;
	}

	if( RID_NOTDEFINED != lookUpSymbolInformationInCurrentScope( strScopedFunctionName ) )
	{
		reportError( ERROR_IDALREADYDEFINED, strScopedFunctionName,
						objectptr->iFileId,
						objectptr->iLineNumber,
						objectptr->iColumn );
		return nodetree;
	}

	if( m_bTreatFunctionNameReadOnly )
	{
		//Function name is unique and readonly.
		//So it's safe to generate a symbol as RIDMothod
		//(And when attempt to assgin the variable, it can be caught by the compiler
		generateSymbolInformation( ridMethod, strScopedFunctionName );

		//Register function properties
		generateSymbolInformation( m_MetaData.generatePropertyRID(), 
			strScopedFunctionName + L"." + NAME_BUILTIN_FUNCTION_PROPERTY_ARGUMENTS,
			CG_SYMBOL_FLAG_PROPERTY );
		

		if( pridMethod != NULL )
			*pridMethod = ridMethod;
	}
	else
	{
		//Generate static field variable symbol
		RID rid = m_MetaData.generateStaticFieldRID( OPERAND_OBJECTREF );
		generateSymbolInformation( ridMethod, strScopedFunctionName );
		generateSymbolInformation( rid, strScopedFunctionName + STRING_OBJECTNAME_POSTFIX );

		//Register function properties
		generateSymbolInformation( m_MetaData.generatePropertyRID(), 
			strScopedFunctionName + L"." + NAME_BUILTIN_FUNCTION_PROPERTY_ARGUMENTS,
			CG_SYMBOL_FLAG_PROPERTY );

		if( pridMethod != NULL )
			*pridMethod = rid;
	}

	enterNewScope( strScopedFunctionName, ridMethod );

	if( nodetree->argumentlistNode )
	{
		//Clear temporary variable
		m_bRestArgsInArgList = false;
		nodetree->argumentlistNode = parseParameterListNodeTreePass0( nodetree->argumentlistNode, ridArguments );
	}

	if( nodetree->lhsNode != NULL )
	{
		parseStatementsPass0( nodetree->lhsNode );
	}
	if( nodetree->rhsNode != NULL )
	{
		parseStatementsPass0( nodetree->rhsNode );
	}
	if( nodetree->statementNode )
	{
		parseStatementsPass0( nodetree->statementNode );
	}

	//Resolve locals and args
	m_MetaData.getMethodTable()[ RidFromToken( ridMethod ) ].iNumArgments =
										(int32_t)m_MetaData.getParamListSize( ridArguments );
	m_MetaData.getMethodTable()[ RidFromToken( ridMethod ) ].bRestArgments = m_bRestArgsInArgList;

	leaveScope();

	return nodetree;
};

/***************************************************************************
//parse function decl expression node
 ***************************************************************************/
CCgNodeData* CCilCodeGen::parseFunctionExpressionNode( CCgNodeData* nodetree )
{

	RID ridMethod;
	CCgNodeData* node = parseFunctionDeclarationNode( nodetree, &ridMethod );

	//Function object ref is already in the stack
	//emit( CEE_LDSFLD, ridMethod );

	return node;
}



} //namespace CRI