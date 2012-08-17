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
 * File     : Lexer.cpp
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
/***************************************************************************
 *      Variables
 ***************************************************************************/
const LEX_WORD_ITEM CLexer::s_KeywordTable[] = {
	{L"break",	TOKEN_BREAK },
	{L"else",	TOKEN_ELSE },
	{L"new",	TOKEN_NEW },
	{L"var",	TOKEN_VAR },
	{L"case",	TOKEN_CASE },
	{L"finally", TOKEN_FINALLY },
	{L"return",	TOKEN_RETURN },
	{L"void",	TOKEN_VOID },
	{L"catch",	TOKEN_CATCH },
	{L"for",	TOKEN_FOR },
	{L"switch",	TOKEN_SWITCH },
	{L"while",	TOKEN_WHILE },
	{L"continue",TOKEN_CONTINUE },
	{L"function",TOKEN_FUNCTION },
	{L"this",	TOKEN_THIS },
	{L"with",	TOKEN_WITH },
	{L"default", TOKEN_DEFAULT },
	{L"if",		TOKEN_IF },
	{L"throw",	TOKEN_THROW },
	{L"delete",	TOKEN_DELETE },
	{L"in",		TOKEN_IN },
	{L"try",	TOKEN_TRY },
	{L"do",		TOKEN_DO },
	{L"instanceof", TOKEN_INSTANCEOF },
	{L"typeof",	TOKEN_TYPEOF },

#ifdef ENABLE_EXTENSION_JSCRIPT
	//Extension for JScript compatibility
	{L"@set",	TOKEN_ATSET },
	{L"@if",	TOKEN_ATIF },
	{L"@elif",	TOKEN_ATELIF },
	{L"@else",	TOKEN_ATELSE },
	{L"@end",	TOKEN_ATEND },
	{L"@cc_on",	TOKEN_ATCC_ON },
	{L"import",	TOKEN_IMPORT },
#endif
};

const LEX_WORD_ITEM_EX CLexer::s_LiteralTable[] = {
	{L"true",	LITERAL_BOOLEAN, true },
	{L"false",	LITERAL_BOOLEAN, false },
	{L"null",	LITERAL_NULL, NULL },
	{L"undefined",LITERAL_UNDEFINED, NULL },
};

const LEX_WORD_ITEM CLexer::s_ReservedWordTable[] = {
	{L"abstract",TOKEN_ABSTRACT },
	{L"enum",	TOKEN_ENUM },
	{L"export",	TOKEN_EXPORT },
	{L"interface",TOKEN_INTERFACE },
	{L"static",	TOKEN_STATIC },
	{L"extend",	TOKEN_EXTENDS },
	{L"super",	TOKEN_SUPER },
	{L"final",	TOKEN_FINAL },
	{L"native",	TOKEN_NATIVE },
	{L"synchronized",TOKEN_SYNCHRONIZED },
	{L"class",	TOKEN_CLASS },
	{L"package",TOKEN_PACKAGE },
	{L"throws",	TOKEN_THROWS },
	{L"const",	TOKEN_CONST },
	{L"goto",	TOKEN_GOTO },
	{L"private",TOKEN_PRIVATE },
	{L"transient",	TOKEN_TRANSIENT },
	{L"debugger",	TOKEN_DEBUGGER },
	{L"implements",	TOKEN_IMPLEMENTS },
	{L"protected",	TOKEN_PROTECTED },
	{L"volatile",TOKEN_VOLATILE },
	{L"public",	TOKEN_PUBLIC },
#ifndef ENABLE_EXTENSION_JSCRIPT
	{L"import",	TOKEN_IMPORT },
#endif
#ifndef ENABLE_EXTENSION_ES4
	{L"boolean",	TOKEN_BOOLEAN },
	{L"byte",	TOKEN_BYTE },
	{L"long",	TOKEN_LONG },
	{L"float",	TOKEN_FLOAT },
	{L"double",	TOKEN_DOUBLE },
	{L"int32_t",TOKEN_INT },
	{L"int16_t",TOKEN_SHORT },
	{L"int8_t",	TOKEN_CHAR },
#endif
};

const LEX_WORD_ITEM CLexer::s_OperatorTable[] ={
	//4 characters
	{ L">>>=", TOKEN_SHRUASSIGNOP },
	//3 characters
	{ L"!==", TOKEN_STRICTNOTEQUALOP },
	{ L"===", TOKEN_STRICTEQUALOP },
	{ L"<<=", TOKEN_SHLASSIGNOP },
	{ L">>=", TOKEN_SHRASSIGNOP },
	{ L">>>", TOKEN_SHIFTRIGHTUNSIGNED },
#ifdef ENABLE_EXTENSION_ES4
	//ECMA4 defined keywords
	{ L"...", TOKEN_RESTARG_PREFIX },
#endif
	//2 characters
	{ L"++", TOKEN_UNINC },
	{ L"--", TOKEN_UNDEC },
	{ L"&&", TOKEN_LOGAND },
	{ L"||", TOKEN_LOGOR },
	{ L"<<", TOKEN_SHIFTLEFT },
	{ L">>", TOKEN_SHIFTRIGHT },
	{ L"*=", TOKEN_MULASSIGNOP },
	{ L"%=", TOKEN_MODASSIGNOP },
	{ L"+=", TOKEN_ADDASSIGNOP },
	{ L"-=", TOKEN_SUBASSIGNOP },
	{ L"^=", TOKEN_XORASSIGNOP },
	{ L"&=", TOKEN_ANDASSIGNOP },
	{ L"|=", TOKEN_ORASSIGNOP },
	{ L"==", TOKEN_EQUALOP },
	{ L"!=", TOKEN_NOTEQUALOP },
	{ L"<=", TOKEN_LESSEQUAL },
	{ L">=", TOKEN_GREATEREQUAL },
	//1 character
	{ L"{", '{' },	//
//	{ L"}", '}' },	// '}' is handled by automatic semicolon insertion routine
	{ L"(", '(' },	//
	{ L")", ')' },	//
	{ L"[", '[' },	//
	{ L"]", ']' },	//
	{ L".", '.' },	//
	{ L";", ';' },	//
	{ L",", ',' },	//
	{ L"~", '~' },	//
	{ L"?", '?' },	//
	{ L":", ':' },	//
	{ L"*", '*' },	//
	{ L"%", '%' },	//
	{ L"+", '+' },	//
	{ L"-", '-' },	//
	{ L"^", '^' },	//
	{ L"&", '&' },	//
	{ L"|", '|' },	//
	{ L"=", '=' },	//=
	{ L"!", '!' },	//
	{ L"<", TOKEN_LESS },//
	{ L">", TOKEN_GREATER },//
};

/***************************************************************************
 *      Methods
 ***************************************************************************/

extern "C" {
/***************************************************************************
 * Lex routine invoked from YACC
 ***************************************************************************/	
int32_t yylex( )
{
	cri::CParser::getparser()->processToken();
	yylval = cri::CParser::getparser()->getTokenValue();
	return cri::CParser::getparser()->getToken();
}
}

/***************************************************************************
 * Lexer initialization
 ***************************************************************************/	
bool CLexer::init()
{
	//initialize hash_maps
	if( !m_mapKeyword.size() )
	{
		for( int32_t i = 0; i < sizeof( s_KeywordTable ) / sizeof( s_KeywordTable[ 0 ] ); i++ )
		{
			m_mapKeyword[ s_KeywordTable[ i ].sKey ] = s_KeywordTable[ i ].iTokenValue;
		}

		for( int32_t i = 0; i < sizeof( s_ReservedWordTable ) / sizeof( s_ReservedWordTable[ 0 ] ); i++ )
		{
			m_mapReservedWord[ s_ReservedWordTable[ i ].sKey ] = s_ReservedWordTable[ i ].iTokenValue;
		}

		for( int32_t i = 0; i < sizeof( s_OperatorTable ) / sizeof( s_OperatorTable[ 0 ] ); i++ )
		{
			m_mapOperator[ s_OperatorTable[ i ].sKey ] = s_OperatorTable[ i ].iTokenValue;
		}
	
		for( int32_t i = 0; i < sizeof( s_LiteralTable ) / sizeof( s_LiteralTable[ 0 ] ); i++ )
		{
			m_mapLiteral[ s_LiteralTable[ i ].sKey ] = i;
		}
	}

	m_iLastTokenLine = 0;
	m_iLastTokenColumn = 0;
	return true;
}

/***************************************************************************
 * Lexer main
 ***************************************************************************/	
int32_t CLexer::process( void )
{
	assert( m_bInitialized == true );

	wchar_t c;
	wstring s = L"";

	m_bAllowAutoSemicolon = false;
	if( m_bDuplicateToken )
	{
		//Automatic semicolon insertion done.

		m_bDuplicateToken = false;
		return 1;
	}

	if( m_bEndOfStream )
	{
		m_bAllowAutoSemicolon = true;
		AUTOSEMICOLON;
		tokenFound( TOKEN_EOF, m_iColumn, m_iLineNumber );
		
		return 1;
	}
	if( m_AllowRegexLiteral )
	{
		int32_t iCurrentColumn = m_iColumn;
		int32_t iCurrentLine = m_iLineNumber;
		//Regex literal
		m_AllowRegexLiteral = false;
		do
		{
			c = getNextInt8();
			if( c == '\\' )
				processRegExpEscapeSequence( s ); 
			else
				s += c;
		} while( c != '/' && c != CODE_LF && c );
		if( c != '/' )
		{
			//Syntax error!!
			ungetChar( c );
			reportError( ERROR_INVALIDSTRINGCOLON );
		}
		if( !c )
		{
			m_bAllowAutoSemicolon = true;
			AUTOSEMICOLON;
			tokenFound( TOKEN_EOF, iCurrentLine, iCurrentColumn );
			return 1;
		}

		//Get flags
		do
		{
			c = getNextInt8();
			if( !isIdentifier( c ) )
			{
				ungetChar( c );	//Unget it
				break;
			}
			s += c;
		} while( c );
		if( !c )
		{
			m_bAllowAutoSemicolon = true;
			AUTOSEMICOLON;
			tokenFound( TOKEN_EOF, iCurrentLine, iCurrentColumn );
			return 1;
		}

		m_strIdentifier = s;
		tokenFound( LITERAL_REGULAR_EXPRESSION, iCurrentLine, iCurrentColumn );
		m_uCurrentValue.pString = m_strIdentifier.data();
		return 1;
	}
	while( 1 )
	{
		int32_t iCurrentColumn = m_iColumn;
		int32_t iCurrentLine = m_iLineNumber;

		c = getNextInt8();
		if( !c )
		{
			m_bAllowAutoSemicolon = true;
			AUTOSEMICOLON;
			tokenFound( TOKEN_EOF, iCurrentLine, iCurrentColumn );
			return 1;
		}

		if( isLineTerminator( c ) )	//skip LF or CR
		{
			m_bAllowAutoSemicolon = true;
			continue;
		}
	
		if( isWhiteSpace( c ) )
			continue;	//Skip white spaces

		//Automatic semicolon insertion handling
		// Refer ECMA 262 7.9
		//--------------------------------------------------------------------
		if( c == '}' )
		{
			//Do automatic selicolon insertion
			m_bAllowAutoSemicolon = true;
			tokenFound( '}', m_iLineNumber, m_iColumn );	//Automatic semicolon insertion
			return 1;
		}

		bool bIsAtIdentifier = isAtIdentifierStart( c );
		//identifier or KWD
		if( isIdentifierStart( c ) || bIsAtIdentifier )
		{
			do
			{
				s.append( 1, c );
				c = getNextInt8();
				if( !isIdentifier( c ) )
				{
					ungetChar( c );	//Unget it
					break;
				}
			} while( c );

			if( !c )
			{
				m_bAllowAutoSemicolon = true;
				AUTOSEMICOLON;
				tokenFound( TOKEN_EOF, iCurrentLine, iCurrentColumn );
				return 1;
			}

			//Got word
			wstring sToken = s;
			
			//Note: a keyword is a case sensitive!!
			//std::transform( sToken.begin(), sToken.end(), sToken.begin(), std::tolower );

			//Look for keyword table
			hash_map< wstring, int32_t >::iterator it;
			it = m_mapKeyword.find( sToken );
			if( it != m_mapKeyword.end() )
			{
				pair< wstring, int32_t > value = *it;
				tokenFound( value.second, iCurrentLine, iCurrentColumn );
				m_strIdentifier = sToken;
				m_uCurrentValue.iValue = value.second;
				return 1;
			}
			//Error check if not using Rsvd wd
			it = m_mapReservedWord.find( sToken );
			if( it != m_mapReservedWord.end() )
			{
				//ERROR!!
				//Reserved word is used

				//Update line information
				m_iCurrentTokenLine = iCurrentLine;
				m_iCurrentTokenColumn = iCurrentColumn;
				updateLine( iCurrentLine, iCurrentColumn );
				//Actually report an error 
				reportError( ERROR_RESERVEDWORD, sToken );
				return 1;
			}

			//Look for literal word table
			it = m_mapLiteral.find( sToken );
			if( it != m_mapLiteral.end() )
			{
				pair< wstring, int32_t > value = *it;
				tokenFound( s_LiteralTable[ value.second ].iTokenValue, iCurrentLine, iCurrentColumn );
				m_strIdentifier = s;
				m_uCurrentValue.iValue = s_LiteralTable[ value.second ].iValue;
				return 1;
			}

			if( !bIsAtIdentifier )
			{
				//It's identifier!!
				tokenFound( IDENTIFIER_NAME, iCurrentLine, iCurrentColumn );
				m_strIdentifier = s;
				m_uCurrentValue.pString = m_strIdentifier.data();
			}
			else
			{
				//@identifier
				tokenFound( ATIDENTIFIER_NAME, iCurrentLine, iCurrentColumn );
				m_strIdentifier = s;
				m_uCurrentValue.pString = m_strIdentifier.data();
			}
			return 1;
		}
		//
		//Constant
		bool bDecimalLiteral = false;
		if( c == '.' )
		{
			wchar_t cNext;
			cNext = getNextInt8();
			if( !cNext )
			{
				m_bAllowAutoSemicolon = true;
				AUTOSEMICOLON;
				tokenFound( TOKEN_EOF, iCurrentLine, iCurrentColumn );
				return 1;
			}
			if( isDigit( cNext ) )
			{
				c = '.';	// ".5" style
				bDecimalLiteral = true;
			}
			ungetChar( cNext );
		}
		if( isDigit( c ) || bDecimalLiteral )
		{
			int64_t i = 0;
			if( c == '0' )
			{
				//Hex
				c = getNextInt8();
				if( c == 'x' || c == 'X' )
				{
					
					while( ( c = getNextInt8() ) && isHex( c ))
					{
						i *=16;
						i += getHexValue( c );
					}
					if( !c )
					{
						m_bAllowAutoSemicolon = true;
						AUTOSEMICOLON;
						tokenFound( TOKEN_EOF, iCurrentLine, iCurrentColumn );
						return 1;
					}
					ungetChar( c );
					tokenFound( LITERAL_INTEGER, iCurrentLine, iCurrentColumn );
					m_iValue = i;
					m_uCurrentValue.iValue = m_iValue;
					//Check overflow
					// set value
					return 1;
				}
				else if( !c )
				{
					m_bAllowAutoSemicolon = true;
					AUTOSEMICOLON;
					tokenFound( TOKEN_EOF, iCurrentLine, iCurrentColumn );
					return 1;
				}
			}

			//Decimal
			while( isDigit( c ))
			{
				i *=10;
				i += getDigitValue( c );
				c = getNextInt8();
			}

			if( c != '.' && c != 'E' && c != 'e')
			{
				ungetChar( c );
				tokenFound( LITERAL_INTEGER, iCurrentLine, iCurrentColumn );
				//Check overflow
				// set value
				m_iValue = i;
				m_uCurrentValue.iValue = m_iValue;
				return 1;
			}
			else if( !c )
			{
				m_bAllowAutoSemicolon = true;
				AUTOSEMICOLON;
				tokenFound( TOKEN_EOF, iCurrentLine, iCurrentColumn );
				return 1;
			}

			//Get float value
			double d = (double)i;

			if( c == '.' )
			{
				double dMultiply = 0.1;
				//Get fraction part
				while( ( c = getNextInt8() ) && isDigit( c ))
				{
					d += getDigitValue( c ) * dMultiply;
					dMultiply /= 10.f;
				}
				if( !c )
				{
					m_bAllowAutoSemicolon = true;
					AUTOSEMICOLON;
					tokenFound( TOKEN_EOF, iCurrentLine, iCurrentColumn );
					return 1;
				}
			}
			if( c == 'e' || c == 'E' )
			{
				//exponent value
				//e10
				//e+10
				//e-10

				int32_t iExponent = 0;
				int32_t iExponentSign = 1;

				c = getNextInt8();
				if( c == '+' )
					c = getNextInt8();
				else if( c == '-' )
				{
					iExponentSign = -1;
					c = getNextInt8();
				}

				while( isDigit( c ))
				{
					iExponent = iExponent * 10 + getDigitValue( c );
					c = getNextInt8();
				}
				if( !c )
				{
					m_bAllowAutoSemicolon = true;
					AUTOSEMICOLON;
					tokenFound( TOKEN_EOF, iCurrentLine, iCurrentColumn );
					return 1;
				}

				iExponent *= iExponentSign;
				double dExpornent = pow( 10.0, iExponent );
				d *= dExpornent;

			}
	
			ungetChar( c );

			//Check overflow
			// set value
			tokenFound( LITERAL_DECIMAL, iCurrentLine, iCurrentColumn );
			m_dValue = d;
			m_uCurrentValue.dValue = d;
			return 1;
		}

		//String
		if( c == '"' )	// string literal
		{
			do
			{
				c = getNextInt8();
				if( c == '\\' )
					processEscapeSequence( s ); 
				else if( c != '"' )
					s += c;

			} while( c != '"' && c != CODE_LF && c );
			if( c != '"' )
			{
				//Syntax error!!
				ungetChar( c );
				reportError( ERROR_INVALIDSTRING );

			}
			if( !c )
			{
				m_bAllowAutoSemicolon = true;
				AUTOSEMICOLON;
				tokenFound( TOKEN_EOF, iCurrentLine, iCurrentColumn );
				return 1;
			}
			m_strIdentifier = s;
			tokenFound( LITERAL_STRING, iCurrentLine, iCurrentColumn );
			m_uCurrentValue.pString = m_strIdentifier.data();
			return 1;

		}
		if( c == '\'' )	// string literal
		{
			do
			{
				c = getNextInt8();
				if( c == '\\' )
					processEscapeSequence( s ); 
				else if( c != '\'' )
					s += c;
			} while( c != '\'' && c != CODE_LF && c );
			if( c != '\'' )
			{
				//Syntax error!!
				ungetChar( c );
				reportError( ERROR_INVALIDSTRINGCOLON );
			}
			if( !c )
			{
				m_bAllowAutoSemicolon = true;
				AUTOSEMICOLON;
				tokenFound( TOKEN_EOF, iCurrentLine, iCurrentColumn );
				return 1;
			}
			m_strIdentifier = s;
			tokenFound( LITERAL_STRING, iCurrentLine, iCurrentColumn );
			m_uCurrentValue.pString = m_strIdentifier.data();
			return 1;

		}
		//Special case for comments
		if( c == '/' )
		{
			c = getNextInt8();
			if (c == '=')
			{
				tokenFound( TOKEN_DIVASSIGNOP, iCurrentLine, iCurrentColumn );
				return 1;
			}
			else if( c == '/' )
			{
				while( (c = getNextInt8() ) && !isLineTerminator( c ) )
				{
					//Skip comments
				}
				ungetChar( c );	//Unget it
				return process();							//Recursive call
			}
			else if( c == '*' )								//C style comment
			{
				while( c = getNextInt8() )
				{
					if( c == '*' )
					{
						c = getNextInt8();
						if( c == '/' || !c )
						{
							break;
						}
						else
						{
							ungetChar( c );
						}
					}
				}
				if( !c )
				{
					reportError( ERROR_INVALIDCOMMENT );
					m_bAllowAutoSemicolon = true;
					AUTOSEMICOLON;
					tokenFound( TOKEN_EOF, iCurrentLine, iCurrentColumn );
					return 0;
				}

				return process();
			}
			else if( !c )
			{
				m_bAllowAutoSemicolon = true;
				AUTOSEMICOLON;
				tokenFound( TOKEN_EOF, iCurrentLine, iCurrentColumn );
				return 1;
			}
			tokenFound( '/', iCurrentLine, iCurrentColumn );
			m_strIdentifier = L"/";
			ungetChar( c );	//Unget it
			return 1;
		}

		s = c;
		int32_t iMaxLength = (int32_t)s_OperatorTable[ 0 ].sKey.length();
		int32_t iTokenLength;
		for( iTokenLength = 1; iTokenLength < iMaxLength; iTokenLength++)
		{
			c = getNextInt8();
			if( !c ) break;
			s += c;
		}

		hash_map< wstring, int32_t >::iterator it;
		for( int32_t j = (int32_t)s.length() - 1; j >= 0; --j )
		{
			it = m_mapOperator.find( s );
			if( it != m_mapOperator.end() )
			{
				pair< wstring, int32_t > value = *it;

				tokenFound( value.second, iCurrentLine, iCurrentColumn );
				m_strIdentifier = s;
				return 1;
			}
			ungetChar( s[ j ] );	//Unget it
			s = s.substr( 0, s.size() - 1 );
		}
		if( !c )
		{
			m_bAllowAutoSemicolon = true;
			AUTOSEMICOLON;
			tokenFound( TOKEN_EOF, iCurrentLine, iCurrentColumn );
			return 1;
		}

		//Illegal character!!
		reportError( ERROR_INVALIDCHARACTER );
		return 0;
	}

	return 1;
}

/***************************************************************************
 * Lexer helpers
 ***************************************************************************/	

/***************************************************************************
 * get retrieved token
 ***************************************************************************/	
int32_t CLexer::getToken( void )
{
	assert( m_bInitialized == true );
	return m_iCurrentToken;
}

/***************************************************************************
 * get token's value
 ***************************************************************************/	
YYSTYPE CLexer::getTokenValue( void )
{
	assert( m_bInitialized == true );
	return m_uCurrentValue;
}

bool CLexer::isWhiteSpace( const wchar_t c )
{
	//USP?
	switch( c )
	{
	case ' ':
	case '\t':	// tab
	case '\v':	// vertical tab
	case '\f':	// form feed
	case 0x00a0:// No break space
		return true;
	default:
		return false;
	}
}

bool CLexer::isDigit( const wchar_t c )
{
	if( c >= '0' && c <= '9' )
		return true;
	return false;
}

bool CLexer::isHex( const wchar_t c )
{

	if( c >= '0' && c <='9' || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f')
		return true;
	return false;
}

bool CLexer::isOctal( const wchar_t c )
{

	if( c >= '0' && c <='7' )
		return true;
	return false;
}

bool CLexer::isOctalStart( const wchar_t c )
{
	if( c >= '0' && c <='3' )
		return true;
	return false;
}

int32_t CLexer::getHexValue( const wchar_t c )
{
	if( c >= '0' && c <='9' )
		return c - '0';
	if ( c >= 'A' && c <= 'F' )
		return c - 'A' + 10;
	if ( c >= 'a' && c <= 'f' )
		return c - 'a' + 10;
	return 0;
}

int32_t CLexer::getDigitValue( const wchar_t c )
{
	if( c >= '0' && c <='9' )
		return c - '0';
	return 0;
}

bool CLexer::isLowerCase( const wchar_t c )
{
	if( c >= 'a' && c <= 'z' )
		return true;
	return false;
}

bool CLexer::isUpperCase( const wchar_t c )
{
	if( c >= 'A' && c <= 'Z' )
		return true;
	return false;
}

bool CLexer::isIdentifierStart( const wchar_t c )
{
	if( isAlphabet( c ) || c == '$' || c== '_' || c== '\\' )
		return true;
	return false;
}

//JScript compatibility extension
bool CLexer::isAtIdentifierStart( const wchar_t c )
{
	if( c == '@' )
		return true;
	return false;
}

bool CLexer::isIdentifier( const wchar_t c )
{
	if( isAlphabet( c ) || c == '$' || c=='_' || isDigit( c ) )
		return true;
	return false;
}

bool CLexer::isLineTerminator( const wchar_t c )
{
	switch( c )
	{
	case 0xa:		//LF
	case 0xd:		//CR
	case 0x2028:	//LS
	case 0x2029:	//PS
		return true;
	default:
		return false;
	}
	return false;
}

/***************************************************************************
 * Process unicode escape sequences
 ***************************************************************************/	
//\b \u0008 backspace <BS>
//\t \u0009 horizontal tab <HT>
//\n \u000A line feed (new line) <LF>
//\v \u000B vertical tab <VT>
//\f \u000C form feed <FF>
//\r \u000D carriage return <CR>
//\" \u0022 double quote "
//\' \u0027 single quote '
//\\ \u005C backslash //

int32_t CLexer::processEscapeSequence( wstring &s )
{
	assert( m_pError != NULL );

	wchar_t c = getNextInt8();
	int32_t j = 0;
	switch ( c )
	{
	case 'b':
		s += 0x8;
		break;
	case 't':
		s += 0x9;
		break;
	case 'n':
		s += 0xa;
		break;
	case 'v':
		s += 0xb;
		break;
	case 'f':
		s += 0xc;
		break;
	case 'r':
		s += 0xd;
		break;
	case '"':
		s += 0x22;
		break;
	case '\'':
		s += 0x27;
		break;
	case '\\':
		s += 0x5c;
		break;
	case 'u':
		for( int32_t i = 0; i < 4; i++ )
		{
			c = getNextInt8();
			if( c && isHex( c ) )
			{
				j *= 16;
				j += getHexValue( c );
			}
			else
			{
				//error!!
				reportError( E_FATAL );
			}
		}
		if( !j ) j = -1;
		s += j;
		break;
	case 'x':
		for( int32_t i = 0; i < 2; i++ )
		{
			c = getNextInt8();
			if( c && isHex( c ) )
			{
				j *= 16;
				j += getHexValue( c );
			}
			else
			{
				//error!!
				reportError( E_FATAL );
			}
		}
		if( !j ) j = -1;
		s += j;
		break;
	default:
		if( isOctalStart( c ) )
		{
			//Octal literal?
			j = c;
			for( int32_t i = 0; i < 2; i++ )
			{
				c = getNextInt8();
				if( isOctal( c ) )
				{
					j *= 8;
					j += c;
				}
				else
				{
					ungetChar( c );
					break;
				}
			}
			if( !j ) j = -1;
			s += j;
			break;
		}
		else
		{
			//s += '\\';
			s += c;
		}
		break;
	}
	return 1;
}

/***************************************************************************
 * Process unicode escape sequences for RegExp
 ***************************************************************************/	
//\t \u0009 horizontal tab <HT>
//\n \u000A line feed (new line) <LF>
//\v \u000B vertical tab <VT>
//\f \u000C form feed <FF>
//\r \u000D carriage return <CR>

int32_t CLexer::processRegExpEscapeSequence( wstring &s )
{
	assert( m_pError != NULL );

	wchar_t c = getNextInt8();
	int32_t j = 0;
	switch ( c )
	{
	case 't':
		s += 0x9;
		break;
	case 'n':
		s += 0xa;
		break;
	case 'v':
		s += 0xb;
		break;
	case 'f':
		s += 0xc;
		break;
	case 'r':
		s += 0xd;
		break;
	case 'u':
		for( int32_t i = 0; i < 4; i++ )
		{
			c = getNextInt8();
			if( c && isHex( c ) )
			{
				j *= 16;
				j += getHexValue( c );
			}
			else
			{
				//error!!
				reportError( E_FATAL );
			}
		}
		if( !j ) j = -1;
		s += j;
		break;
	case 'x':
		for( int32_t i = 0; i < 2; i++ )
		{
			c = getNextInt8();
			if( c && isHex( c ) )
			{
				j *= 16;
				j += getHexValue( c );
			}
			else
			{
				//error!!
				reportError( E_FATAL );
			}
		}
		if( !j ) j = -1;
		s += j;
		break;
	default:
		if( isOctalStart( c ) )
		{
			//Octal literal?
			wchar_t cNext = fetchNextChar();
			if( !isOctal( cNext ) )
			{
				//Not an octal literal
				s += '\\';
				s += c;
				//Backward referrer
				break;
			}

			j = c;
			for( int32_t i = 0; i < 2; i++ )
			{
				c = getNextInt8();
				if( isOctal( c ) )
				{
					j *= 8;
					j += c;
				}
				else
				{
					ungetChar( c );
					break;
				}
			}
			if( !j ) j = -1;
			s += j;
			break;
		}
		else
		{
			s += '\\';
			s += c;
		}
		break;
	}
	return 1;
}

/***************************************************************************
 * Update line information
 ***************************************************************************/	
void CLexer::updateLine( const int32_t iLine, const int32_t iColumn )
{
	m_iLastTokenLine = m_iCurrentTokenLine;
	m_iLastTokenColumn = m_iCurrentTokenColumn;
	m_iCurrentTokenLine = iLine;
	m_iCurrentTokenColumn = iColumn;
	m_uCurrentValue.iFileId = 0;
	m_uCurrentValue.iLineNumber = iLine;
	m_uCurrentValue.iColumn = iColumn;
}

/***************************************************************************
 * Mark as a token founded
 ***************************************************************************/	
void CLexer::tokenFound( const int32_t iToken, const int32_t iLine, const int32_t iColumn )
{
	m_iCurrentToken = iToken;
	updateLine( iLine, iColumn );
}


/***************************************************************************
 * Report an error if the error handler is set
 ***************************************************************************/	
void CLexer::reportError( int32_t iErrorCode )
{
	assert( m_bInitialized == true );
	if( m_pError && m_bEnableErrorOutput )
	{
		m_pError->report( iErrorCode,
							getLastTokenLine(),
							getLastTokenColumn(),
							getCurrentStreamName(),
							getLineString( getLastTokenLine() ),
							getErrorInfo() );
	}
}

void CLexer::reportError( int32_t iErrorCode, const wstring& strErrorInfo )
{
	assert( m_bInitialized == true );
	if( m_pError && m_bEnableErrorOutput )
	{
		m_pError->report( iErrorCode,
							getLastTokenLine(),
							getLastTokenColumn(),
							getCurrentStreamName(),
							getLineString( getLastTokenLine() ),
							(wstring &)strErrorInfo );
	}
}

/***************************************************************************
 * isAutoSemicolonAllowed
 ***************************************************************************/	
bool CLexer::isAutoSemicolonAllowed()
{
	return m_bAllowAutoSemicolon;
}

void CLexer::duplicateToken()
{
	m_bDuplicateToken = true;
	return;
}

} //namespace CRI
