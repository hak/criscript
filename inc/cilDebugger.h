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
 * Module   : JScript Virtual Machine Debug interface
 * File     : cilDebugger.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once

#include "IDebugger.h"
#include "cilVm.h"

using namespace std;
namespace cri {

	/*EN
	 \brief Debugger class of CRI Script.
	 Many of features are not implenented yet.
	 \ingroup MDL_LIB_DEBUGGER
	 \par Debugger class should implement this interface
	*/
	class CCilDebugger: public IDebugger {
	public:
		CCilDebugger()
		{
			init();
		};
		~CCilDebugger() {
			terminate();
		};
		bool init() { return true; };
		void terminate() {};

		void execute();
		int32_t putBreakPoint();
		void removeBreakPoint( int32_t i);
		int32_t stepOver();
		int32_t stepInto();

		bool attach( CCilVm * pVM );
		bool detach();
		void setSymbolInformation( hash_map<wstring, CG_SYMBOL_INFORMATION>& symbolinfo );
		void setMetaData( CMetaData& MetaData ) { m_pMetaData = &MetaData; }

		//----------------------------------------------------------------------
		//Debug functions
		//----------------------------------------------------------------------
		void debugDumpStats( void );
		void debugDumpStaticFields( uint32_t );
		void debugDumpObjects( uint32_t );
	private:
		CCilVm* m_pVM;
		CMetaData* m_pMetaData;										//Meta data entries
		hash_map<wstring, CG_SYMBOL_INFORMATION> m_SymbolInfoPool;		//Symbol information

	};

	/*EN
	 \brief Disassembler implemantation of CRI Script bytecode stream
	 \ingroup MDL_LIB_DEBUGGER
	*/
	class CCilDisasm {
	public:
		CCilDisasm()
		{
			m_bInitialized = false;
			init();
		}
		~CCilDisasm() {
			terminate();
		}
		bool init();
		void terminate() {};

		void dumpDisasm();
		void setMetaData( CMetaData& MetaData );
		void setSymbolInformation( hash_map<wstring, CG_SYMBOL_INFORMATION>& symbolinfo);
		void setDumpByteCode( bool b ) { m_bDumpByteCode = b;}
		void setDumpByteCodeData( bool b ) { m_bDumpByteCodeData = b;}
		void setDumpSymbolInfo( bool b ) { m_bDumpSymbolInfo = b;}
		void setILStream( ILFRAGMENT& ILStream);
		void setILStream( const uint8_t* pStream, const size_t size );

	private:
		uint8_t getCharFromILPool( ILFRAGMENTINFO& SourceILPool, uint32_t& iIndex );
		uint32_t getIntFromILPool( ILFRAGMENTINFO& SourceILPool, uint32_t& iIndex );

		void dumpTemporaryLabel( uint32_t iLabelID );
		void dumpLabel( wstring& string );
		void dumpTab( void );
		void dumpBranch( OPCODE_TABLEENTRY* pInfo, uint32_t iLabelID );
		void dumpInstruction( OPCODE_TABLEENTRY* pInfo, int32_t iParamIndex );


		//---------------------------------------------------------------------------
		//OpCodeMap
		//---------------------------------------------------------------------------
		map< OP_CODE, OPCODE_TABLEENTRY*> m_OpcodeMapByOpcode;
		ILFRAGMENTINFO m_ILStream;

		//Disasm related info tables
		CMetaData* m_pMetaData;								//Meta data entries
		map<uint32_t, wstring> m_SymbolInfoPool;		//Symbol information
		map<uint32_t, METHOD_DEF > m_MapMethod;					//Method data entries
		bool m_bInitialized;
		bool m_bDumpByteCode;
		bool m_bDumpByteCodeData;
		bool m_bDumpSymbolInfo;
		char m_strBuffer[ SIZE_ERRORSTRING ];
		wchar_t m_wstrBuffer[ SIZE_ERRORSTRING ];

	};

}	//namespace cri