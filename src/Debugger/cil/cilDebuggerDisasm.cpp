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
 * Module   : cil Virtual Machine Debugger
 * File     : cilDebugger<NameHere>.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
#include "cilVm.h"
#include "cilDebugger.h"
namespace cri {
	/***************************************************************************
	 *      Variables
	 ***************************************************************************/
	
	/***************************************************************************
	 *      Methods
	 ***************************************************************************/
	/***************************************************************************
	 * Initialize members
	 ***************************************************************************/
	bool CCilDisasm::init ()
	{
		//Initialize OpCodeMap
		int32_t i = 0;
		while ( g_Opecode[ i ].OpCode != CEE_FORCE_DWORD )
		{
			m_OpcodeMapByOpcode[ g_Opecode[ i ].OpCode ] = &g_Opecode[ i ];
			i++;
		}
		m_bInitialized = true;
		m_bDumpByteCode = true;
		m_bDumpByteCodeData = true;
		m_bDumpSymbolInfo = false;
		
		return true;
	};
	
	/***************************************************************************
	 * Disasm data
	 ***************************************************************************/
	void CCilDisasm::dumpDisasm()
	{
		assert( m_bInitialized );
		assert( m_ILStream.ILPool.size() );
		int8_t m_strBuffer[ SIZE_ERRORSTRING ];
		map< uint32_t, uint32_t > mapLabels;
		uint32_t i;
		//First pass
		//Retrieve label data
		uint32_t iByteCode;
		int32_t iOffset;
		int32_t iLabelID = 0;
		pair< uint32_t, uint32_t > pairLabel;
		pair< map< uint32_t, uint32_t >::iterator, bool > result;
		for( i = 0; i < m_ILStream.ILPool.size(); )
		{
			OPCODE_TABLEENTRY* pInfo;
			iByteCode = getCharFromILPool( m_ILStream, i );
			
			switch( iByteCode )
			{
				case CEE_BR:
				case CEE_BRFALSE:
				case CEE_BRTRUE:
				case CEE_BEQ:
				case CEE_BGE:
				case CEE_BGT:
				case CEE_BLE:
				case CEE_BLT:
				case CEE_BNE_UN:
				case CEE_EXT_BNGE:
				case CEE_EXT_BNGT:
				case CEE_EXT_BNLE:
				case CEE_EXT_BNLT:
				case CEE_JMP:
					iOffset = (int32_t)getIntFromILPool( m_ILStream, i );
					pairLabel.first =  i + iOffset;
					pairLabel.second = iLabelID;
					result = mapLabels.insert( pairLabel );
					if( result.second ) iLabelID ++;
					break;
				case CEE_BR_S:
				case CEE_BRFALSE_S:
				case CEE_BRTRUE_S:
				case CEE_BEQ_S:
				case CEE_BGE_S:
				case CEE_BGT_S:
				case CEE_BLE_S:
				case CEE_BLT_S:
				case CEE_BNE_UN_S:
				case CEE_EXT_BNGE_S:
				case CEE_EXT_BNGT_S:
				case CEE_EXT_BNLE_S:
				case CEE_EXT_BNLT_S:
					iOffset = (int32_t)(int8_t)getCharFromILPool( m_ILStream, i );
					pairLabel.first =  i + iOffset;
					pairLabel.second = iLabelID;
					result = mapLabels.insert( pairLabel );
					if( result.second ) iLabelID ++;
					break;
				case 0xfe:
					iByteCode = getCharFromILPool( m_ILStream, i );
					pInfo = m_OpcodeMapByOpcode[ (OP_CODE)(0xfe00 | iByteCode) ];
					i += pInfo->OperandParams & OPERAND_PARAMS_SIZEMASK;
					break;
				default:
					pInfo = m_OpcodeMapByOpcode[ (OP_CODE)iByteCode ];
					i += pInfo->OperandParams & OPERAND_PARAMS_SIZEMASK;
					break;
			}
		}
		
		map< uint32_t, uint32_t >::iterator itMap = mapLabels.begin();
		pair< uint32_t, uint32_t > LabelData;
		map< uint32_t, METHOD_DEF >::iterator ItFunction;
		
		uint32_t iInstructionOffset;
		//Second pass
		for( i = 0; i < m_ILStream.ILPool.size(); )
		{
			OPCODE_TABLEENTRY* pInfo;
			iInstructionOffset = i;
			
			//dump functions
			if( (ItFunction = m_MapMethod.find( i )) != m_MapMethod.end() )
			{
				//dump functions
				wstring strFunc = m_SymbolInfoPool[ ItFunction->second.ridMethod ];
				dumpLabel( strFunc );
			}
			else
			{
				//dump labels
				itMap = mapLabels.find( i );
				if( itMap != mapLabels.end() )
				{
					LabelData = *itMap;
					if( i == LabelData.first )
					{
						dumpTemporaryLabel( LabelData.second );
						itMap ++;
					}
					else
						dumpTab();
				}
				else
					dumpTab();
			}
			
			iByteCode = getCharFromILPool( m_ILStream, i );
			switch( iByteCode )
			{
				case CEE_BR:
				case CEE_BRFALSE:
				case CEE_BRTRUE:
				case CEE_BEQ:
				case CEE_BGE:
				case CEE_BGT:
				case CEE_BLE:
				case CEE_BLT:
				case CEE_BNE_UN:
				case CEE_EXT_BNGE:
				case CEE_EXT_BNGT:
				case CEE_EXT_BNLE:
				case CEE_EXT_BNLT:
				case CEE_JMP:
					pInfo = m_OpcodeMapByOpcode[ (OP_CODE)iByteCode ];
					iOffset = getIntFromILPool( m_ILStream, i );
					iLabelID = mapLabels[ i + iOffset ];
					dumpBranch( pInfo, iLabelID );
					break;
				case CEE_BR_S:
				case CEE_BRFALSE_S:
				case CEE_BRTRUE_S:
				case CEE_BEQ_S:
				case CEE_BGE_S:
				case CEE_BGT_S:
				case CEE_BLE_S:
				case CEE_BLT_S:
				case CEE_BNE_UN_S:
				case CEE_EXT_BNGE_S:
				case CEE_EXT_BNGT_S:
				case CEE_EXT_BNLE_S:
				case CEE_EXT_BNLT_S:
					pInfo = m_OpcodeMapByOpcode[ (OP_CODE)iByteCode ];
					iOffset = (int32_t)(int8_t)getCharFromILPool( m_ILStream, i );
					iLabelID = mapLabels[ i + iOffset ];
					dumpBranch( pInfo, iLabelID );
					break;
				case 0xfe:
					iByteCode = (OP_CODE)(0xfe00 | getCharFromILPool( m_ILStream, i ));
					pInfo = m_OpcodeMapByOpcode[ (OP_CODE)iByteCode ];
					dumpInstruction( pInfo, i );
					i += pInfo->OperandParams & OPERAND_PARAMS_SIZEMASK;
					break;
				default:
					pInfo = m_OpcodeMapByOpcode[ (OP_CODE)iByteCode ];
					dumpInstruction( pInfo, i );
					i += pInfo->OperandParams & OPERAND_PARAMS_SIZEMASK;
					break;
			}
			if( m_bDumpByteCodeData )
			{
				if( iByteCode <= 0xff )
				{
					sprintf_s( SPRINTF_CAST(m_strBuffer), "; %04x: %02x", iInstructionOffset, iByteCode );
				}
				else
				{
					sprintf_s( SPRINTF_CAST(m_strBuffer), "; %04x: %04x", iInstructionOffset, iByteCode );
				}
				cout << m_strBuffer;
				for( uint32_t iIndex = i - (pInfo->OperandParams & OPERAND_PARAMS_SIZEMASK);
					iIndex < i; iIndex ++ )
				{
					uint8_t c = *(uint8_t*)&m_ILStream.ILPool[ iIndex ];
					sprintf_s( SPRINTF_CAST(m_strBuffer), " %02x", c );
					cout << m_strBuffer;
				}
				
			}
			cout << "\n";
			
			
		}
		
	}
	
	/***************************************************************************
	 * Set symbol information
	 ***************************************************************************/
	void CCilDisasm::setSymbolInformation( hash_map<wstring, CG_SYMBOL_INFORMATION>& symbolinfo)
	{
		hash_map<wstring, CG_SYMBOL_INFORMATION>::iterator it = symbolinfo.begin();
		
		pair<wstring, CG_SYMBOL_INFORMATION> pair;
		for( ; it != symbolinfo.end(); ++it)
		{
			pair = *it;
			m_SymbolInfoPool[ pair.second.rid ] = pair.first;
			
			if( SymbolTypeFromFrag( pair.second.Flags ) == CG_SYMBOL_FLAG_GLOBAL
			   && pair.second.ridObject )
			{
				//Builtin object handling.
				//They are stored both in static field and RID
				m_SymbolInfoPool[ pair.second.ridObject ] = pair.first;
			}
			
		}
		
	}
	
	/***************************************************************************
	 * Set Method Information
	 ***************************************************************************/
	void CCilDisasm::setMetaData( CMetaData& MetaData )
	{
		m_pMetaData = &MetaData;
		vector< METHOD_DEF >& vecMethod = m_pMetaData->getMethodTable();	
		vector< METHOD_DEF >::iterator it = vecMethod.begin();
		for( ; it != vecMethod.end(); ++ it )
		{
			m_MapMethod[ it->iILOffset ] = *it;
		}
	}
	
	/***************************************************************************
	 * Set ILStream
	 ***************************************************************************/
	void CCilDisasm::setILStream( ILFRAGMENT& ILStream)
	{
		m_ILStream.ILPool = ILStream;
	}
	
	void CCilDisasm::setILStream( const uint8_t* pStream, const size_t size )
	{
		assert( pStream != NULL );		
		m_ILStream.ILPool.clear();
		for( uint32_t i = 0; i < size; ++i )
		{
			//Copy over
			m_ILStream.ILPool.push_back( *pStream++ );
		}
	}
	/***************************************************************************
	 *      Disasm helper
	 ***************************************************************************/
	uint8_t CCilDisasm::getCharFromILPool( ILFRAGMENTINFO& SourceILPool, uint32_t& iIndex )
	{
		uint8_t cRet = SourceILPool.ILPool[ iIndex ];
		iIndex ++;
		return cRet;
	}
	
	uint32_t CCilDisasm::getIntFromILPool( ILFRAGMENTINFO& SourceILPool, uint32_t& iIndex )
	{
		uint32_t iRet = *(uint32_t*)&SourceILPool.ILPool[ iIndex ];
		iIndex += sizeof( uint32_t );
		return iRet;
	}
	
	void CCilDisasm::dumpTemporaryLabel( uint32_t iLabelID )
	{
		//32 chars
		sprintf_s( SPRINTF_CAST(m_strBuffer), "L%08x:                      ", iLabelID );
		cout << m_strBuffer;
	}
	
	void CCilDisasm::dumpLabel( wstring& str )
	{
		//32 chars
		str.append( L":" );
		swprintf_s( m_wstrBuffer, ARRAYSIZE( m_wstrBuffer ), L"%-32s", str.data() );
		wcout << m_wstrBuffer;
	}
	void CCilDisasm::dumpTab( void )
	{
		//32 chars
		cout << "                                ";
	}
	
	void CCilDisasm::dumpBranch( OPCODE_TABLEENTRY* pInfo, uint32_t iLabelID )
	{
		assert( pInfo != NULL );		
		int8_t strBuffer[ 10 ];
		sprintf_s( SPRINTF_CAST(strBuffer), "L%08x", iLabelID );
		sprintf_s( SPRINTF_CAST(m_strBuffer), "%-16s%-44s", pInfo->pName, strBuffer );
		cout << m_strBuffer;
	}
	
	/***************************************************************************
	 *      dump instruction operands
	 ***************************************************************************/
	void CCilDisasm::dumpInstruction( OPCODE_TABLEENTRY* pInfo, int32_t iParamIndex )
	{
		assert( pInfo != NULL );
		int8_t strBuffer[ SIZE_ERRORSTRING ];
		switch( pInfo->OperandParams & OPERAND_PARAMS_SIZEMASK )
		{
			case 0:
				strBuffer[ 0 ] = 0;
				break;
			case 1:
			{
				uint8_t c = *(uint8_t*)&m_ILStream.ILPool[ iParamIndex ];
				sprintf_s( SPRINTF_CAST(strBuffer), "%-44d", c );
			}
				break;
			case 2:
			{
				uint16_t s = *(uint16_t*)&m_ILStream.ILPool[ iParamIndex ];
				sprintf_s( SPRINTF_CAST(strBuffer), "%-44d", s );
			}
				break;
			case 4:
				if( pInfo->OpCode == CEE_LDSTR )
				{
					//String literal
					uint32_t i = *(uint32_t*)&m_ILStream.ILPool[ iParamIndex ];
					wstring sLiteral = L"\"";
					wstring s = m_pMetaData->getStringConstant( i );
					sLiteral.append( s );
					sLiteral.append( L"\"" );
					if( sLiteral.length() < SIZE_ERRORSTRING )
						sprintf_s( SPRINTF_CAST(strBuffer), "%-44S", sLiteral.data() );
					else
						sprintf_s( SPRINTF_CAST(strBuffer), "String too long" );
				}
				else
				{
					if( pInfo->OperandParams & OPERAND_PARAMS_FLOAT )
					{
						float f = *(float*)&m_ILStream.ILPool[ iParamIndex ];
						sprintf_s( SPRINTF_CAST(strBuffer), "%-44f", f );
					}
					else if( pInfo->OperandParams & OPERAND_PARAMS_FIELD )
					{
						if( m_SymbolInfoPool.size() )
						{
							uint32_t i = *(uint32_t*)&m_ILStream.ILPool[ iParamIndex ];
							map<uint32_t, wstring>::iterator it = m_SymbolInfoPool.find( i );
							if( it != m_SymbolInfoPool.end() )
							{
								pair< uint32_t, wstring> pair = *it;
								wstring s;
								s = L"'";
								s.append( pair.second );
								s.append( L"'" );
								sprintf_s( SPRINTF_CAST(strBuffer), "%-44S", s.data() );
							}
							else
							{
								sprintf_s( SPRINTF_CAST(strBuffer), "%-44x", i );
							}
						}
						else
						{
							uint32_t i = *(uint32_t*)&m_ILStream.ILPool[ iParamIndex ];
							sprintf_s( SPRINTF_CAST(strBuffer), "%-44x", i );
						}
					}
					else
					{
						uint32_t i = *(uint32_t*)&m_ILStream.ILPool[ iParamIndex ];
						sprintf_s( SPRINTF_CAST(strBuffer), "%-44d", i );
					}
				}
				break;
			case 8:
				if( pInfo->OperandParams & OPERAND_PARAMS_FLOAT )
				{
					double d = *(double*)&m_ILStream.ILPool[ iParamIndex ];
					sprintf_s( SPRINTF_CAST(strBuffer), "%-44f", d );
				}
				else
				{
					int64_t i= *(int64_t*)&m_ILStream.ILPool[ iParamIndex ];
					sprintf_s( SPRINTF_CAST(strBuffer), "%-44d", i );
				}
				break;
			default:
				Debug_Fatal( "Illegal operand size" );
		}
		sprintf_s( SPRINTF_CAST(m_strBuffer), "%-16s%-44s", pInfo->pName, strBuffer );
		cout << m_strBuffer;
	}
	
} //namespace CRI