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
 * Module   : JScript lexer for UTF16
 * File     : onmemorylexer.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
#include "lexer.h"
#include "parser.h"
#include "error.h"
namespace cri {
extern "C" {
/***************************************************************************
 *      Variables
 ***************************************************************************/
/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 * FileStreamLexer functions
 ***************************************************************************/	

/***************************************************************************
 * get next int8_t from the file stream
 ***************************************************************************/	
wchar_t CStringLexer::getNextInt8( void )
{
	assert( m_iBufferIndex >= 0 );

	if( m_bEndOfStream )
		return 0;

	wchar_t c = 0;
	c = m_strBuffer.data()[ m_iBufferIndex ];
	m_iBufferIndex ++;
#ifdef CRISCRIPT_BIG_ENDIAN
	//Big endian platform
	if( !m_bBigEndianSource )
	{
		c = c << 8 | ((c >> 8) & 0xff);
	}
#else
	if( m_bBigEndianSource )
	{
		c = c << 8 | ((c >> 8) & 0xff);
	}
#endif

	if( m_iBufferIndex >= m_strBuffer.size() )
	{
		m_bEndOfStream = true;
		return c;
	}
	if( c == CODE_LF )
	{
		m_iLineNumber++;
		m_arrayColumns.push_back( m_iColumn - 1 );
		m_arrayLineOffsets.push_back( m_iBufferIndex );
		m_iColumn = 1;
		m_strCurrentLine.clear();
	}
	else
		m_iColumn++;

	return c;
}

/***************************************************************************
 * Unget int8_t
 ***************************************************************************/	
void CStringLexer::ungetChar( wchar_t c ) {
	assert( m_iBufferIndex > 0 );
	assert( m_iBufferIndex <= m_strBuffer.size() );

//	m_bEndOfStream = false;
	m_iBufferIndex --;

	if( c == CODE_LF )
	{
		//Assumes unget happens only 1 times per line
		assert( m_arrayColumns.size() > 0 );

		m_iLineNumber --;
		m_iColumn = m_arrayColumns.back();
		m_arrayLineOffsets.pop_back();
		m_arrayColumns.pop_back();
	}
	else
		m_iColumn--;
}

/***************************************************************************
 * Just look up next int8_t from the file stream
 ***************************************************************************/	
wchar_t CStringLexer::fetchNextChar( void )
{
	wchar_t c = getNextInt8();
	ungetChar( c );
	return c;
}

/***************************************************************************
 * getLine string
 ***************************************************************************/	
wstring& CStringLexer::getLineString( const uint32_t iNumLine )
{
	assert( m_bInitialized == true );
	assert( m_arrayLineOffsets.size() > iNumLine-1 || !iNumLine );

	if( !iNumLine ) return m_strCurrentLine;

	//Create currentline info
	m_strCurrentLine.clear();

	uint32_t iIndex = m_arrayLineOffsets[ iNumLine-1 ];
	wchar_t c = 0;
	while( iIndex < m_strBuffer.size() && c != CODE_LF )
	{
		c = m_strBuffer.data()[ iIndex++ ];
		m_strCurrentLine += c;
	}
	//Restore offset
	return m_strCurrentLine;
}

/***************************************************************************
 * SetFileStream
 ***************************************************************************/	
bool CStringLexer::setStream ( wstring* pstrBuffer )
{
//	assert( m_mapReservedWord.size() );
//	if( m_strBuffer != NULL ) return false;	//already file is set.
	m_strBuffer = *pstrBuffer;
	m_iLineNumber = 1;
	m_iColumn = 1;
	m_strFileName = L"";

	m_iBufferIndex = 0;
	m_bEndOfStream = 0;
	m_arrayLineOffsets.push_back( m_iBufferIndex );

	m_bInitialized = true;

	return true;
}

/***************************************************************************
 * ResetStream
 ***************************************************************************/	
bool CStringLexer::resetStream ()
{
	assert( m_bInitialized == true );
	if( !m_strBuffer.length() ) return false;
	m_iLineNumber = 1;
	m_iColumn = 1;

	m_iBufferIndex = 0;
	m_bEndOfStream = 0;
	m_arrayLineOffsets.clear();
	m_arrayLineOffsets.push_back( m_iBufferIndex );

	return true;
}

/***************************************************************************
 * getCurrentLine string
 ***************************************************************************/	
wstring& CStringLexer::getCurrentLineString()
{
	assert( m_bInitialized == true );

	return getLineString( (uint32_t)m_arrayLineOffsets.size() );
}


}
} //namespace CRI
