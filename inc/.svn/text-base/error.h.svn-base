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
 * File     : Error.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once

namespace cri {
/***************************************************************************
 *      Error code definitions
 ***************************************************************************/
//http://msdn2.microsoft.com/en-us/library/6bby3x2e.aspx
enum CIL_ERRORS {
	ERROR_FILENOTFOUND				= 01,
	ERROR_INVALIDFILEENCODING		= 02,
	ERROR_INVALIDOPTIONSWITCH		= 03,

	ERROR_CANNOTHAVEBREAK			= 1019,
	ERROR_CANNOTHAVECONTINUE		= 1020,
	ERROR_CONDITIONALCOMPILATION	= 1030,
	ERROR_CANNOTHAVEDEFAULT			= 1027,
	ERROR_EXPECTED_LP				= 1005,	//Left Parenthesis
	ERROR_EXPECTED_RP				= 1006,	//Right Parenthesis
	ERROR_EXPECTED_SLASH			= 1012,
	ERROR_EXPECTED_COLON			= 1003,
	ERROR_EXPECTED_SEMICOLON		= 1004,
	ERROR_EXPECTED_AT				= 1031,
	ERROR_EXPECTED_ATEND			= 1029,
	ERROR_EXPECTED_RSB				= 1007,	//Right square bracket
	ERROR_EXPECTED_LCB				= 1008,	//Left curly bracket
	ERROR_EXPECTED_RCB				= 1009,
	ERROR_EXPECTED_EQU				= 1011,
	ERROR_EXPECTED_CATCH			= 1033,
	ERROR_EXPECTED_CONSTANT			= 1031,
	ERROR_EXPECTED_HEX				= 1023,
	ERROR_EXPECTED_ID				= 1010,
	ERROR_EXPECTED_ID_STRING_NUM	= 1028,
	ERROR_EXPECTED_WHILE			= 1024,
	ERROR_INVALIDCHARACTER			= 1014,
	ERROR_LABELNOTFOUND				= 1026,
	ERROR_LABELALREADYDEFINED		= 1025,
	ERROR_INVALIDRETURN				= 1018,
	ERROR_SYNTAXERROR				= 1002,
	ERROR_INVALIDTHROW				= 1035,
	ERROR_INVALIDCOMMENT			= 1016,
	ERROR_INVALIDSTRING				= 1015,
	ERROR_INVALIDSTRINGCOLON		= 1017,
	ERROR_RESERVEDWORD				= 1018,
	ERROR_IDALREADYDEFINED			= 2000,
	ERROR_STATEMENTNOTALLOWED		= 2001,
	ERROR_FOR_INFINITE				= 2002,
	ERROR_INTERNALIDALREADYDEFINED	= 2003,
	ERROR_PARAMETERALREADYDEFINED	= 2004,
	ERROR_FUNCTIONNAMENOTFOUND		= 2005,
	ERROR_INVALIDSYMBOLCALL			= 2006,
	ERROR_METHODNAMEASSIGNMENTNOTALLOWED = 2007,
	ERROR_ILLEGALCALLTYPE			= 2008,
	ERROR_CANNOTASSIGNCONSTANT		= 2009,
	ERROR_CANNOTASSIGNREADONLY		= 2010,
	ERROR_ONLYONEDEFAULTISALLOWED	= 2011,
	ERROR_CANNOTHAVECASE			= 2012,
	ERROR_NOOBJECTCTOR				= 2013,
	ERROR_FUNCTIONCTORNOTSUPPORTED	= 2014,
	ERROR_WITHONLYACCEPTOBJREF		= 2015,
	ERROR_OBJECTNAMENOTFOUND		= 2016,
	ERROR_BADREGEXLITERALFORMAT		= 2017,
	ERROR_ILLEGALTYPESPECIFIER		= 2018,
	ERROR_DYNAMICEVALNOTSUPPORTED	= 2019,
	
	INFO_CODE_NOEFFECT				= 3000,
	INFO_IDNOTREGISTERED			= 3001,
	INFO_IF_NOEFFECT				= 3002,
	INFO_FOR_NOEFFECT				= 3003,
	INFO_VARIABLENOTASSIGNED		= 3004,
	INFO_DIFFERENTNUMARGS			= 3005,
	INFO_OBJECTINITIALIZORDUPLICATION	= 3006,
	INFO_arrayLiteralTOOLARGE		= 3007,
	INFO_INVOKINGVARIABLE			= 3008,

	ERROR_R_INVALIDARRAYLENGTH		= 5029,
	ERROR_R_INVALIDARRAYLENGTH2		= 5030,
	ERROR_R_EXPECTED_ARRAYOROBJECT	= 5028,
	ERROR_R_EXPECTED_BOOLEAN		= 5010,
	ERROR_R_CANNOTASSIGN_FUNCRESULT	= 5003,
	ERROR_R_CANNOTASSIGN_THIS		= 5000,
	ERROR_R_EXPECTED_DATE			= 5006,
	ERROR_R_EXPECTED_ENUM			= 5015,
	ERROR_R_EXCEPTION_NOTCAUGHT		= 5022,
	ERROR_R_REG_EXPECTED_RP			= 5020,
	ERROR_R_EXPECTED_RSB			= 5019,
	ERROR_R_INVALID_FUNCPROTOTYPE	= 5023,
	ERROR_R_EXPECTED_FUNCTION		= 5002,
	ERROR_R_ILLEGALASSIGNMENT		= 5008,
	ERROR_R_INVALID_CHARACTER		= 5021,
	ERROR_R_EXPECTED_JSOBJECT		= 5014,
	ERROR_R_EXPECTED_NUMBER			= 5001,
	ERROR_R_EXPECTED_OBJECT			= 5007,
	ERROR_R_EXPECTED_OBJECTMEMBER	= 5012,
	ERROR_R_EXPECTED_REGEX			= 5016,
	ERROR_R_EXPECTED_STRING			= 5005,
	ERROR_R_REG_SYNTAXERROR			= 5017,
	ERROR_R_FRACTION_OUTOFRANGE		= 5026,
	ERROR_R_PRECISION_OUTOFRANGE	= 5027,
	ERROR_R_INVALID_URIENCODE		= 5025,
	ERROR_R_INVALID_URICHARACTER	= 5024,
	ERROR_R_UNDEFINED_IDENTIFIER	= 5009,
	ERROR_R_UNEXPECTED_QUANTIFIER	= 5018,
	ERROR_R_EXPECTED_VBARRAY		= 5013,
};

/***************************************************************************
 *      Constants for error check
 ***************************************************************************/

//Size check threshold for array literal
const uint32_t MAX_arrayLiteralSIZE = 0xffff;


/***************************************************************************
 *      Error level definition
 ***************************************************************************/
/*EN Error level settings*/
enum ERROR_LEVEL {
	E_FATAL,			/*EN< Compiler Fatal error */
	E_ERROR,			/*EN< Compile error */
	E_WARNING_L1,		/*EN< Warning level 1 */
	E_WARNING_L2,		/*EN< Warning level 2 */
	E_WARNING_L3,		/*EN< Warning level 3 */
	E_WARNING_L4,		/*EN< Warning level 4 */
	E_INFORMATION,		/*EN< Information */
	NUM_ERROR_LEVELS,
};

/***************************************************************************
 *      Error def structure
 ***************************************************************************/
struct ERROR_DEF {
	int32_t iErrorCode;
	ERROR_LEVEL ErrorLevel;
	bool bNeedSubstring;
	wchar_t* ErrorString;
};

/***************************************************************************
 *      Error information struct
 ***************************************************************************/
struct parserError {
	int32_t iErrorCode;
	int32_t iLine;
	int32_t iColumn;
	map< wstring, int32_t >::iterator itFileName;
	map< wstring, int32_t >::iterator itSrcLine;
	map< wstring, int32_t >::iterator itInfo;
};

const int32_t SIZE_ERRORSTRING = 1024;

/***************************************************************************
 *      Error info handler class
 ***************************************************************************/
	/*EN Class for a Compile Error Handler
	 \ingroup MDL_LIB_COMPILER
	*/
	class CErrorPool {
	public:
		/*EN ctor*/
		CErrorPool();

		/*EN dtor*/
		~CErrorPool();
		
		/*EN Invoked when compile error happens from the parser interface
		\param iErrorCode Error code number
		\param iLineNumber Line number that has the error in the stream
		\param iColumnNumber Column number that has the error in the stream
		\param strFileName File name string
		\param strLine Error line string
		\param strOtherInfo Supplemental information
		*/
		void report( int32_t iErrorCode, int32_t iLineNumber, int32_t iColumnNumber,
					 wstring & strFileName, wstring & strLine, wstring & strOtherInfo );

		/*EN dumps the errors to stdout*/
		void dumpError();

		/*EN Sets the error table
		\param pErrorTable Table of error information
		\param iSize Number of table entries
		*/
		void setErrorTable( const ERROR_DEF pErrorTable[], size_t iSize );

		/*EN Suppresses error line output for Eclipse usage which does not need error line
		\param b true: Suppress error line output False:Output error line
		*/
		void suppressErrorLine( const bool b ) { m_bsuppressErrorLine = b; }

		/*EN Check if there was compile error
		\return True when compile error happened otherwise false.
		*/
		bool hasError( void );

		/*EN Check if there was compile information
		\return True when compile information generated otherwise false.
		*/
		bool hasInformation( void );

	private:
		const ERROR_DEF* m_pErrorTable;
		int32_t m_iNumErrors[ NUM_ERROR_LEVELS ];
		bool m_bsuppressErrorLine;
		map< int32_t,const ERROR_DEF * >* m_pmapErrorCode;

		map< wstring, int32_t > m_mapFileName;
		map< wstring, int32_t > m_mapSrcLine;
		map< wstring, int32_t > m_mapInfo;
		vector< parserError > m_ErrorList;

		bool getErrorLevel( int32_t iErrorCode );
	};

}	//namespace cri

