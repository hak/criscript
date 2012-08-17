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
 * Module   : parser main routine
 * File     : parser.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "CRIScript.h"
#include "parser.h"
#include "Lexer.h"
#include "Error.h"

using namespace cri;

namespace cri {
CParser* CParser::m_pParser = NULL;

	/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 * Initializer
 ***************************************************************************/	
bool CParser::init( CLexer* plexer, ICodeGen* pCodeGen, CErrorPool * pError )
{
	assert( plexer != NULL);
	assert( pCodeGen != NULL );
	//pError can be null

	bool bReturn = true;

	//Version string not to be stripped out
    static const int8_t * version = criscript_version;

	m_pLex = plexer;
	m_pError = pError;
	m_pCodeGen = pCodeGen;

	//Initialize lexer;
	if( bReturn ) bReturn = m_pLex->init();
	if( bReturn ) bReturn = m_pCodeGen->init();
	if( bReturn ) m_pLex->setErrorHandler( pError );
	if( bReturn ) m_pCodeGen->setInterfaces( this, m_pLex, pError );

	if( bReturn ) m_bInitialized = true;
	return true;
}

/***************************************************************************
 * parser main Interface
 ***************************************************************************/	
int32_t CParser::process()
{
	assert( m_bInitialized == true );
	assert( m_pLex != NULL );

	bool b;
	int32_t i;
	//m_pLex->resetStream();
	m_pLex->setErrorOutput( true );

	//Set multipass compilation
	m_pCodeGen->setMultiPasses( true );
	i = yyparse();

	if( !i )
	{
		//parse it
		m_pCodeGen->parseStatements();
	}
	else
		return i;
	
	if( m_bPerformLinkage )
	{
		//link phase
		if( !i )
			b = m_pCodeGen->link();
		else
			return i;

		//Verification phase
		if( b )
			b = m_pCodeGen->verify();
		else
			return 1;
		return b;
	}
	else
		return i;
}


/***************************************************************************
 * parser helpers
 ***************************************************************************/	
int32_t CParser::processToken( void )
{
	//Invokes lexer and process token
	assert( m_bInitialized == true );
	assert( m_pLex != NULL );
	return m_pLex->process();
}

int32_t CParser::getToken( void )
{
	assert( m_bInitialized == true );
	assert( m_pLex != NULL );
	return m_pLex->getToken();
}

YYSTYPE CParser::getTokenValue( void )
{
	assert( m_bInitialized == true );
	assert( m_pLex != NULL );
	if( m_pLex->getTokenDuplication()  )
	{
		YYSTYPE y;
		y.iValue = yychar;
		//Automatic semicolon insertion done.
		m_pLex->setTokenDuplication( false );
		return y;
	}

	return m_pLex->getTokenValue();
}

/***************************************************************************
 * Retrieve information when it's done
 ***************************************************************************/	
vector<uint8_t>&  CParser::getILPool()
{
	assert( m_bInitialized == true );
	assert( m_pCodeGen != NULL );
	return m_pCodeGen->getILPool();
}

hash_map<wstring, CG_SYMBOL_INFORMATION>& CParser::getSymbolInformation()
{
	assert( m_bInitialized == true );
	assert( m_pCodeGen != NULL );
	return m_pCodeGen->getSymbolInformation();
}

CMetaData& CParser::getMetaData()
{
	assert( m_bInitialized == true );
	assert( m_pCodeGen != NULL );
	return m_pCodeGen->getMetaData();
}

} //namespace CRI
