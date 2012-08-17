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
 * Module   : JScript Error handlers
 * File     : Error.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
#include "cilErrorDef_English.h"
namespace cri {
extern "C" {



/***************************************************************************
 * YACC error handler
 ***************************************************************************/	
int32_t yyerror( int8_t * pMessage)
{
	assert( pMessage != NULL );
//	cri::CParser::Getparser()->reportError( ERROR_SYNTAXERROR );
	return 0;
}

/***************************************************************************
 * ctor
  ***************************************************************************/	
CErrorPool::CErrorPool() :	m_pErrorTable( NULL ), 
	m_bsuppressErrorLine( false )
{
	for( int32_t i = 0; i< NUM_ERROR_LEVELS; i++ )
		m_iNumErrors[ i ] = 0;

	m_pmapErrorCode = NULL;
	//Set default error table
	setErrorTable( cri::g_ErrorTable_English, sizeof( cri::g_ErrorTable_English ) 
													 / sizeof( cri::g_ErrorTable_English[ 0 ] ) );

};

/***************************************************************************
 * dtor
 ***************************************************************************/	
CErrorPool::~CErrorPool()
{
	delete m_pmapErrorCode;
};

/***************************************************************************
 *      Report errors
 ***************************************************************************/
void CErrorPool::report( int32_t iErrorCode, int32_t iLineNumber, int32_t iColumnNumber,
				 wstring & strCurrentFileName, wstring & strCurrentLine, wstring & strOtherInfo )
{
	assert( m_pErrorTable != NULL );
	parserError error;
	error.iErrorCode = iErrorCode;
	error.iLine = iLineNumber;
	error.iColumn = iColumnNumber;

	pair< wstring, int32_t > pairInsert(strCurrentFileName, 0);
	pair< map< wstring, int32_t >::iterator, bool > p = m_mapFileName.insert( pairInsert );
	error.itFileName = p.first;

	pairInsert.first = strCurrentLine;
	p = m_mapSrcLine.insert( pairInsert );
	error.itSrcLine = p.first;

	pairInsert.first = strOtherInfo;
	p = m_mapInfo.insert( pairInsert );
	error.itInfo = p.first;

	m_ErrorList.push_back( error );

	//Inc error
	const ERROR_DEF* pError = (*m_pmapErrorCode)[ iErrorCode ];
	m_iNumErrors[ pError->ErrorLevel ] ++;
}

/***************************************************************************
 *      dump errors
 ***************************************************************************/
void CErrorPool::dumpError()
{
	assert( m_pErrorTable != NULL );
	vector< parserError >::iterator it;
	parserError error;
	wstring s;
	const ERROR_DEF* pError;
	for( it = m_ErrorList.begin(); it != m_ErrorList.end(); ++ it )
	{
		error = *it;
		pError = (*m_pmapErrorCode)[ error.iErrorCode ];

		s = (*error.itFileName).first;
		if ( !s.empty() )
			wcout << s.data();
		else
			cout << "unknown source file";
		cout << " (" << error.iLine << ":" << error.iColumn << ") : ";
		switch( pError->ErrorLevel )
		{
		case E_FATAL:
		case E_ERROR:
			cout << "error " << error.iErrorCode << ": ";
			break;
		case E_WARNING_L1:
		case E_WARNING_L2:
		case E_WARNING_L3:
		case E_WARNING_L4:
			cout << "warning " << error.iErrorCode << ": ";
			break;
		case E_INFORMATION:
			cout << "information " << error.iErrorCode << ": ";
			break;
		default:
			break;
		}
		if( pError->bNeedSubstring )
		{
			wchar_t buf[ SIZE_ERRORSTRING ];
			const wchar_t* pString = error.itInfo->first.data();			
			swprintf( buf, SIZE_ERRORSTRING,  pError->ErrorString, pString );
			wcout << buf << L"\n";
				
		}
		else
			wcout << pError->ErrorString << L"\n";
		
		if( !m_bsuppressErrorLine )
		{
			s = (*error.itSrcLine).first;
			if ( !s.empty() )
			{
				wcout << s;
				wcout.clear();
			}
		}

	}

	//Summary
	int32_t iErrors = m_iNumErrors[ E_FATAL ] + m_iNumErrors[ E_ERROR ];
	int32_t iWarnings = m_iNumErrors[ E_WARNING_L1 ] +
					m_iNumErrors[ E_WARNING_L2 ] +
					m_iNumErrors[ E_WARNING_L3 ] +
					m_iNumErrors[ E_WARNING_L4 ];
	int32_t iInfo = 	m_iNumErrors[ E_INFORMATION ];

	cout << "\n--------------------------------------------------------------------\n";
	cout << iErrors << " errors. " << iWarnings << " warnings. " << iInfo << " informations.\n";

}

/***************************************************************************
 *      SetError table
 ***************************************************************************/
void CErrorPool::setErrorTable( const ERROR_DEF pErrorTable[], size_t iSize )
{
	m_pErrorTable = pErrorTable;
	
	if( m_pmapErrorCode ) delete m_pmapErrorCode;
	
	m_pmapErrorCode = new map< int32_t,const ERROR_DEF * >();
	for( uint32_t i = 0; i < iSize; i++ )
	{
		(*m_pmapErrorCode)[ pErrorTable[ i ].iErrorCode ] = &(pErrorTable[ i ]);
	}


}

/***************************************************************************
 *      Check errors
 ***************************************************************************/
bool CErrorPool::hasError( void )
{
	bool bRet = false;
	//Does not include information
	//May be check warning level
	for( int32_t i = 0; i< NUM_ERROR_LEVELS - 1; i++ )
		if( m_iNumErrors[ i ] ) bRet = true;
	return bRet;
}

/***************************************************************************
 *      Check informations
 ***************************************************************************/
bool CErrorPool::hasInformation( void )
{
	bool bRet = false;
	for( int32_t i = 0; i< NUM_ERROR_LEVELS; i++ )
		if( m_iNumErrors[ i ] ) bRet = true;
	return bRet;
}


}	//extern "C"
}	//namespace cri
