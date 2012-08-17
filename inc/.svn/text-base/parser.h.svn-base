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
 * Module   : JScript parser
 * File     : parser.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once

#include "versions.h"
#include "error.h"
#include "lexer.h"
#include "iCodeGen.h"

using namespace std;

//Work around to Doxygen behaivor
//Somehow Doxygen does not parse the descriptions inside a namespace with #include..
namespace cri {
#include "CriScriptparser.hpp"
};

namespace cri {

	/*EN
	 \brief CRI Script parser class
	 \ingroup MDL_LIB_COMPILER
	 \par Main class for a byte code stream generation.
	*/
	class CParser {
	public:
		/*EN ctor */
		CParser()
		{
			if( m_pParser == NULL )
			{
				m_pParser = this;
				m_pError = NULL;
				m_pLex = NULL;
				m_bInitialized = false;
				m_bPerformLinkage = true;
			}
		};
		/*EN dtor */
		~CParser() {
			m_pParser = NULL;
			terminate();
		};

		/*EN Retrieve Version Number of the parser*/
		const CRISCRIPT_VERSIONS& getVersions(void) { return criscript_version_struct; }

		bool init( CLexer* pCLexer, ICodeGen* pCodeGen, CErrorPool * pError );
								/*EN< Sets misc interfaces to the parser
									\param pCLexer Pointer to a lexer interface
									\param pCodeGen Pointer to a CodeGenerator interface
									\param pError Pointer to a Error pool interface
									\return true when suceeded,otherwise false
									*/
		void terminate() {};
								/*EN< Terminates compilation interface. Does nothing for now.
								*/
		bool SetLinkage( bool b ) {
			if( m_bInitialized )
			{
				m_bPerformLinkage = b;
				return false;
			}
			else
				return true;
		}
								/*EN< Enables/Disables Linkage process.
									\param b Enables/Disables linkage
									\return true: success, false: failuree
								*/
		int32_t process( void );
								/*EN< Processes the attached script stream
									\return 0: success
								*/
		int32_t link( void );
								/*EN< Performs a linkage process on a compliled byte code stream
									\return 0: success
								*/

		ICodeGen* getCodeGen(void) { assert( m_bInitialized ); return m_pCodeGen; }
								/*EN< Retrieves an interface
									\return Pointer to ICodeGen interface set to the parser
								*/
		CLexer* getLexer(void) { assert( m_bInitialized ); return m_pLex; }
								/*EN< Retrieves an interface
									\return Pointer to CLexer interface set to the parser
								*/
		void SetLexer( CLexer* pLexer ) { assert( m_bInitialized ); m_pLex = pLexer; }
								/*EN< Sets CLexer interface
									\param pLexer Pointer to a lexer to be used parsing a stream
								*/

		vector<uint8_t> & getILPool();
								/*EN< Retrieves the IL(intermediate language) pool
									\return Reference of a vector which includes compiled byte code stream
								*/
		hash_map<wstring, CG_SYMBOL_INFORMATION>& getSymbolInformation();
								/*EN< Retrieves the Symbol information for a debugger
									\return Reference of a map which includes debug information
									\todo Meta data and Debug Symbols are now not separated well. Will redesign later.
								*/
		CMetaData& getMetaData();
								/*EN< Retrieves the metadata info
									\return Reference of CMetaData which includes metadata
									\todo Meta data and Debug Symbols are now not separated well. Will redesign later.
								*/

		/*EN get a singleton instance of the parser */
		static CParser* getparser() {
			assert( m_pParser );
			return m_pParser;
		};

		int32_t processToken( void );
		int32_t getToken( void );
		YYSTYPE getTokenValue( void );	
	private:
		static CParser* m_pParser;
		bool m_bInitialized;
		bool m_bPerformLinkage;
		CLexer*		m_pLex;
		CErrorPool*	m_pError;
		ICodeGen*	m_pCodeGen;
	};

// External ref declaration
extern int32_t yyparse ();

}	//namespace cri