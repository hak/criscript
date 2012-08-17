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
 * File     : filestreamlexer.cpp
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
wchar_t CFileStreamLexer::getNextInt8( void )
{
	uint16_t c = 0;
	m_pistream->read( (char*)&c, sizeof( uint16_t ) );
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
	if( m_pistream->eof() == true )
	{
		m_bEndOfStream = true;
		return 0;
	}
	if( c == CODE_LF )
	{
		m_iLineNumber++;
		m_arrayColumns.push_back( m_iColumn - 1 );
		m_arrayLineOffsets.push_back( m_pistream->tellg() );
		m_iColumn = 1;
		m_strCurrentLine.clear();
	}
	else
		m_iColumn++;

	return (wchar_t)c;
}

/***************************************************************************
 * Unget int8_t
 ***************************************************************************/	
void CFileStreamLexer::ungetChar( wchar_t c )
{
	assert( m_pistream );
	assert( !( c & 0xffff0000 ) );
	m_bEndOfStream = false;

	//Reset the stream status
	m_pistream->clear();
#ifdef _WIN32	//USE_SEEKG	
	//Seek to prior character's position
	m_pistream->seekg( -(int32_t)sizeof( uint16_t ), ios_base::cur );
#else	//OSX
	m_pistream->putback( (c & 0xff00 >> 8 ) );
	m_pistream->putback( (c & 0xff ) );
#endif
	
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
wchar_t CFileStreamLexer::fetchNextChar( void )
{
	wchar_t c = getNextInt8();
	ungetChar( c );
	return c;
}

/***************************************************************************
 * getLine string
 ***************************************************************************/	
wstring& CFileStreamLexer::getLineString( const uint32_t iNumLine )
{
	assert(m_pistream );
	assert( m_bInitialized == true );
	assert( m_arrayLineOffsets.size() > iNumLine - 1 || !iNumLine );

	if( !iNumLine ) return m_strCurrentLine;

	//Create currentline info
	m_strCurrentLine.clear();

	//Store current location
	m_pistream->ios::clear();
	int32_t iPos = m_pistream->tellg();

	m_pistream->seekg( m_arrayLineOffsets[ iNumLine - 1 ], ios_base::beg );

	uint16_t c = 0;
	while( !m_pistream->eof() && c != CODE_LF )
	{
		m_pistream->read( (char*)&c, sizeof( uint16_t ) );
		m_strCurrentLine += (wchar_t)c;
	}
	//Restore offset
	m_pistream->seekg( iPos, ios_base::beg );
	return m_strCurrentLine;
}

/***************************************************************************
 * SetFileStream
 ***************************************************************************/	
bool CFileStreamLexer::setStream ( istream *pstream, const wstring* pFilename )
{
	assert( m_mapReservedWord.size() );
	if( m_pistream != NULL ) return false;	//already file is set.
	m_pistream = pstream;
	if( m_pistream->fail() ) return false;	//already file is set.
	m_iLineNumber = 1;
	m_iColumn = 1;
	if( pFilename != NULL )
		m_strFileName = *pFilename;
	else
		m_strFileName = L"";
	m_arrayLineOffsets.push_back( m_pistream->tellg() );

	m_bInitialized = true;

	return true;
}

/***************************************************************************
 * ResetStream
 ***************************************************************************/	
bool CFileStreamLexer::resetStream ()
{
	assert( m_bInitialized == true );
	if( m_pistream == NULL ) return false;
	m_iLineNumber = 1;
	m_iColumn = 1;
	//Seek to start pos
	m_pistream->clear();
	m_pistream->seekg( m_arrayLineOffsets[ 0 ], ios::beg );
	m_arrayLineOffsets.clear();
	m_arrayLineOffsets.push_back( m_pistream->tellg() );
	return true;
}

/***************************************************************************
 * getCurrentLine string
 ***************************************************************************/	
wstring& CFileStreamLexer::getCurrentLineString()
{
	assert( m_bInitialized == true );

	return getLineString( (uint32_t)m_arrayLineOffsets.size() );
}


}
} //namespace CRI
