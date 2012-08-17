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
 * File     : Lexer.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once

#include "error.h"
#include "iCodeGen.h"

//Work around to Doxygen behaivor
//Somehow Doxygen does not parse the descriptions inside a namespace with #include..
namespace cri {
#include "CriScriptparser.hpp"
};

namespace cri {
extern int32_t yychar;

#define AUTOSEMICOLON if( cri::CParser::getparser()->getLexer()->isAutoSemicolonAllowed() )\
		{\
			yychar = ';';\
			cri::CParser::getparser()->getLexer()->duplicateToken();\
		}

struct LEX_WORD_ITEM {
	const wstring sKey;
	const int32_t iTokenValue;
};

struct LEX_WORD_ITEM_EX {
	const wstring sKey;
	const int32_t iTokenValue;
	const int32_t	iValue;
};
const wchar_t CODE_LF = 0xa;
const wchar_t CODE_CR = 0xd;

	//---------------------------------------------------------
	//virtual class for the Lexer
	//---------------------------------------------------------
	/*EN
	 \brief Abstruction class of CRI Script Lexer
	 \ingroup MDL_LIB_COMPILER
	 \par Lexer class should implement this interface
	*/
	class CLexer {
	public:
		//Virtual functions
		/*EN pure virtual function to reset a token stream */
		virtual bool resetStream () = 0;

		/*EN pure virtual function to retrieve current compiling line */
		virtual wstring& getCurrentLineString() = 0;

		/*EN pure virtual function to retrieve a specific line */
		virtual wstring& getLineString( const uint32_t iLineNumber ) = 0;
		//m_bInitialized needs to be set inside setStream() API

		/*EN Initializes the lexer */
		virtual bool init();

		//---------------------------------------------------------
		//ctor, dtor, interfaces
		//---------------------------------------------------------
		/*EN ctor */
		CLexer() :
				m_bAllowAutoSemicolon( 0 ),
				m_AllowRegexLiteral( false ),
				m_bBigEndianSource( false ),
				m_iCurrentTokenLine( 1 ),
				m_iCurrentTokenColumn( 1 ),
				m_bEnableErrorOutput( false ),
				m_bEndOfStream( false ),
				m_strCurrentLine( L"" ),
				m_strErrorInfo( L"" ),
				m_strFileName( L"" ),
				m_strIdentifier( L"" ),
				m_pError( NULL ),
				m_bDuplicateToken( false )
		{
			m_mapReservedWord.clear();
			m_mapKeyword.clear();
			m_mapLiteral.clear();
			m_mapOperator.clear();
		};

		/*EN dtor */
		virtual ~CLexer() {
			m_mapReservedWord.clear();
			m_mapKeyword.clear();
			m_mapLiteral.clear();
			m_mapOperator.clear();
		};

		/*EN Processes a token stream
		\par Token stream need to be attached prior process() call
		\return 0:Success
		*/
		int32_t process( void );

		int32_t getToken( void );
		void duplicateToken();
		bool isAutoSemicolonAllowed();
		void debugRecentlyReduced( const uint32_t iLineNumber )
		{
	#ifdef _DEBUG
			m_iDebugRecentlyReduced = iLineNumber;
	#endif
		}
		void allowRegexLiteral( const bool b ) { m_AllowRegexLiteral = b; }	
		//---------------------------------------------------------
		//Retrieve values
		//---------------------------------------------------------
		wstring& getIdentifier( void )
		{
			assert( m_bInitialized == true );
			return m_strIdentifier;
		}

		int64_t getIntegerTokenValue( void )
		{
			assert( m_bInitialized == true );
			return m_iValue;
		}

		double getDecimalTokenValue( void )
		{
			assert( m_bInitialized == true );
			return m_dValue;
		}
		YYSTYPE getTokenValue( void );

		//---------------------------------------------------------
		//Retrieve information
		//---------------------------------------------------------
		
		/*EN Sets target endianness
		\param b True:The target is Big Endian False: The target is Little Endian
		*/
		void setEndianness( const bool b ) { m_bBigEndianSource = b; }
		/*EN gets target endianness setting
		\return Target endian setting
		*/
		bool getEndianness() { return m_bBigEndianSource; }

		wstring& getCurrentStreamName() { return m_strFileName; }
		void setCurrentStreamName( wstring& str ) { m_strFileName = str; }

		int32_t getLineNumber( void )
		{
			assert( m_bInitialized == true );
			return m_iLineNumber;
		}

		void setLineNumber( const int32_t iLineNumber )
		{
			assert( m_bInitialized == true );
			m_iLineNumber = iLineNumber;
			return;
		}

		int32_t getColumn( void ) {
			assert( m_bInitialized == true );
			return m_iCurrentTokenColumn;
		}
		
		int32_t getLastTokenColumn( void ) {
			assert( m_bInitialized == true );
			return m_iLastTokenColumn;
		}

		int32_t getLastTokenLine( void ) {
			assert( m_bInitialized == true );
			return m_iLastTokenLine;
		}

		void setErrorHandler( CErrorPool * pError )
		{
			m_pError = pError;
			m_bEnableErrorOutput = true;
			return;
		}

		void setErrorOutput( bool bEnable )
		{
			m_bEnableErrorOutput = bEnable;
		}

		wstring& getErrorInfo() { return m_strErrorInfo;}

		bool getTokenDuplication() { return m_bDuplicateToken; }
		void setTokenDuplication( const bool b ) { m_bDuplicateToken = b; return; }

	protected:
		//---------------------------------------------------------
		//Virtual functions
		//---------------------------------------------------------
		virtual void reportError( int32_t iErrorCoed );
		virtual void reportError( int32_t iErrorCoed, const wstring& strErrorInfo );

		virtual wchar_t getNextInt8( void ) = 0;
		virtual wchar_t fetchNextChar( void ) = 0;
		virtual void ungetChar( const wchar_t c ) = 0;
		virtual void tokenFound( const int32_t iToken, const int32_t iLine, const int32_t iColumm);

		//---------------------------------------------------------
		//Helpers
		//---------------------------------------------------------
		bool isDigit( const wchar_t c );
		bool isHex( const wchar_t c );
		bool isOctal( const wchar_t c );
		bool isOctalStart( const wchar_t c );
		int32_t	 getHexValue( const wchar_t c );
		int32_t	 getDigitValue( const wchar_t c );
		bool isLowerCase( const wchar_t c );
		bool isUpperCase( const wchar_t c );
		bool isWhiteSpace( const wchar_t c );
		bool isAlphabet( const wchar_t c ) { return isLowerCase(c) || isUpperCase(c); };
		bool isIdentifierStart( const wchar_t c );
		bool isIdentifier( const wchar_t c );
		bool isAtIdentifierStart( const wchar_t c );
		bool isLineTerminator( const wchar_t c );
		int32_t processEscapeSequence( wstring &s );
		int32_t processRegExpEscapeSequence( wstring &s );
		void updateLine( const int32_t iLine, const int32_t iColumn );

		//---------------------------------------------------------
		//Automatic semicolon insertion flag
		//---------------------------------------------------------
		bool m_bAllowAutoSemicolon;
		bool m_bDuplicateToken;
		bool m_bBigEndianSource;
		bool m_AllowRegexLiteral;

		//---------------------------------------------------------
		//data members
		//---------------------------------------------------------
		static const LEX_WORD_ITEM s_ReservedWordTable[];
		static const LEX_WORD_ITEM s_KeywordTable[];
		static const LEX_WORD_ITEM_EX s_LiteralTable[];
		static const LEX_WORD_ITEM s_OperatorTable[];
		
		bool m_bInitialized;
		bool m_bEnableErrorOutput;
		bool m_bEndOfStream;
		int32_t m_iLineNumber;
		int32_t	m_iColumn;
		int32_t m_iCurrentToken;
		int32_t m_iCurrentTokenColumn;
		int32_t m_iCurrentTokenLine;
		int32_t m_iLastTokenColumn;
		int32_t m_iLastTokenLine;
		int64_t m_iValue;
		double m_dValue;
		YYSTYPE m_uCurrentValue;
		CErrorPool * m_pError;

	#ifdef _DEBUG
		//Debug variable
		uint32_t m_iDebugRecentlyReduced;
	#endif

		vector<int32_t> m_arrayColumns;
		vector<int32_t> m_arrayLineOffsets;

		hash_map< wstring, int32_t > m_mapKeyword;
		hash_map< wstring, int32_t > m_mapReservedWord;
		hash_map< wstring, int32_t > m_mapLiteral;
		hash_map< wstring, int32_t > m_mapOperator;
		
		//---------------------------------------------------------
		//Get current line information
		//---------------------------------------------------------
		wstring m_strIdentifier;
		wstring m_strCurrentLine;
		wstring m_strErrorInfo;
		wstring m_strFileName;
	};

	/*EN
	 \brief Lexer implementation that works with istream
	 \ingroup MDL_LIB_COMPILER
	*/
	class CFileStreamLexer : public CLexer {
	public:
		//---------------------------------------------------------
		//ctor, dtor, interfaces
		//---------------------------------------------------------
		/*EN ctor */
		CFileStreamLexer()
		{
			m_pistream = NULL;
		};
		/*EN dtor */
		virtual ~CFileStreamLexer() {
		};

		/*EN Sets istream pointer to the lexer
		\par Stream need to be in UTF-16. In future UTF-8 version would be supported
		\param pstream Pointer to istream
		\param pFilename Poinger to a filename of the stream. Can be null.
		\return 0:Success
		*/
		bool setStream( istream *pstream, const wstring* pFilename = NULL );

		/*EN Resets the streamistream pointer to the lexer
		\par Stream needs to be in UTF-16. In future UTF-8 version would be supported
		\return 0:Success
		*/
		bool resetStream ();

		wstring& getCurrentLineString();
		wstring& getLineString( const uint32_t iLineNumber );
	private:
		wchar_t getNextInt8( void );
		wchar_t fetchNextChar( void );
		void ungetChar( const wchar_t c );
		void updateLine( const int32_t iLine, const int32_t iColumn );

		//---------------------------------------------------------
		//data members
		//---------------------------------------------------------
		istream* m_pistream;
	};

	/*EN
	 \brief Lexer implementation that works with wstring
	 \ingroup MDL_LIB_COMPILER
	*/
	class CStringLexer : public CLexer {
	public:
		//---------------------------------------------------------
		//ctor, dtor, interfaces
		//---------------------------------------------------------
		/*EN ctor */
		CStringLexer(): m_strBuffer( L"" ),
						m_iBufferIndex( 0 )
		{
		};
		/*EN dtor */
		virtual ~CStringLexer() {
		};

		bool setStream ( wstring* pString );
		bool resetStream ();

		wstring& getCurrentLineString();
		wstring& getLineString( const uint32_t iLineNumber );
	private:
		wchar_t getNextInt8( void );
		wchar_t fetchNextChar( void );
		void ungetChar( const wchar_t c );
		void updateLine( const int32_t iLine, const int32_t iColumn );

		//---------------------------------------------------------
		//data members
		//---------------------------------------------------------
		uint32_t	m_iBufferIndex;
		wstring m_strBuffer;
	};

}	//cri