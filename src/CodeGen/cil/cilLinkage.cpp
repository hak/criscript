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
 * Module   : Resolve byte code offset info
 * File     : cilLinkage.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
#include "cilCodeGen.h"
namespace cri {

/***************************************************************************
 *      Variables
 ***************************************************************************/
const int32_t SHORT_JUMP_RANGE_MAX = MAXCHAR;
const int32_t SHORT_JUMP_RANGE_MIN = MINCHAR;

/***************************************************************************
 *      Methods
 ***************************************************************************/

// The link method resolves CIL instruction offset in 3 passes.
// Pass 1:  - Merge all CIL instructions in 1 stream
//			- Remove label instruction and resolve line info
//			- Check branch target offset value and emit suitable branch insturction
// Pass 2:  - Check branch target position and if .s version can be generated
// Pass 3:  - Resolve branch instruction offset value

/***************************************************************************
 *      link binary data
 ***************************************************************************/
bool CCilCodeGen::link ()
{
 	assert( m_ILPool[ 0 ].ILPool.size() != 0 );
	ILFRAGMENTINFO ILResolvedPoolPhase1;
	ILFRAGMENTINFO ILResolvedPoolPhase2;

	//Do the peep hole optimization here
	//conditional check + branch true => conditional branch
	
	bool b = linkPass1( m_ILPool, ILResolvedPoolPhase1 );
	if( b )
	{
		b = linkPass2( ILResolvedPoolPhase1, ILResolvedPoolPhase2 );
		ILResolvedPoolPhase1.ILPool.clear();	//Clear the temp pool
	}
	if( b )
	{
		b = linkFinalPass( ILResolvedPoolPhase2, m_ILResolvedPool );
		ILResolvedPoolPhase2.ILPool.clear();	//Clear the temp pool
	}
	if( b ) resolveMethodRID();

	return b;
}

/***************************************************************************
 *      link pass 1 Eliminate Label info
 ***************************************************************************/
bool CCilCodeGen::linkPass1( V_ILFRAGMENTINFO& vILPool, ILFRAGMENTINFO& ILResolvedPool )
{
	assert( vILPool[ 0 ].ILPool.size() != 0 );
	assert( ILResolvedPool.ILPool.size() == 0 );
	// 1st pass
	// Generate single IL code pool
	V_ILFRAGMENTINFO::iterator CurrentILPool = vILPool.begin();
	ILFRAGMENTINFO* pInfo;
	int32_t iMax;
	uint8_t cByteCode;
	for( ; CurrentILPool != vILPool.end(); ++CurrentILPool )
	{
		pInfo = &*CurrentILPool;

		//Perform PeepHole Optimizer
		//Peep hole optimization is done against each ILPool WITH label info.
		//With the label info, it can determine scopes
		performPeepholeOptimize( pInfo );

		uint32_t i;
		uint32_t iSize = uint32_t(pInfo->ILPool.size());
		for( i = 0; i < iSize; )
		{
			OPCODE_TABLEENTRY* pOpInfo;
			cByteCode = getCharFromILPool( *pInfo, i );
			switch( cByteCode )
			{
			case 0xfe:
				cByteCode = getCharFromILPool( *pInfo, i );
				switch( cByteCode )
				{
				case CEE_CODE_LABEL & 0xff:
					{
					//Skip code label info
					uint32_t iTagID = getIntFromILPool( *pInfo, i );
					
					//It's time to resolve the label
					m_mapLabelID2ILAddress[ iTagID ] = (uint32_t)ILResolvedPool.ILPool.size();
					break;
					}
				case CEE_EXT_LDARG & 0xff:
					{
						RID ridMethod = (RID)getIntFromILPool( *pInfo, i );
						RID ridArglist = (RID)getIntFromILPool( *pInfo, i );
						int32_t iIndex = getIntFromILPool( *pInfo, i );
						resolveLoadArgument( &ILResolvedPool, ridMethod, ridArglist, iIndex );
						break;
					}
				case CEE_EXT_STARG & 0xff:
					{
						RID ridMethod = (RID)getIntFromILPool( *pInfo, i );
						RID ridArglist = (RID)getIntFromILPool( *pInfo, i );
						int32_t iIndex = getIntFromILPool( *pInfo, i );
						resolveStoreArgument( &ILResolvedPool, ridMethod, ridArglist, iIndex );
						break;
					}
				case CEE_EXT_LDLOC & 0xff:
					{
						RID ridMethod = (RID)getIntFromILPool( *pInfo, i );
						RID ridArglist = (RID)getIntFromILPool( *pInfo, i );
						int32_t iIndex = getIntFromILPool( *pInfo, i );
						resolveLoadLocal( &ILResolvedPool, ridMethod, ridArglist, iIndex );
						break;
					}
				case CEE_EXT_STLOC & 0xff:
					{
						RID ridMethod = (RID)getIntFromILPool( *pInfo, i );
						RID ridArglist = (RID)getIntFromILPool( *pInfo, i );
						int32_t iIndex = getIntFromILPool( *pInfo, i );
						resolveStoreLocal( &ILResolvedPool, ridMethod, ridArglist, iIndex );
						break;
					}
				case CEE_LEAVE_TMP & 0xff:
					{
						int32_t iLabelID = getIntFromILPool( *pInfo, i );
						int32_t iNestLevel = getIntFromILPool( *pInfo, i );
						OP_CODE code;
						if( iNestLevel > 0 )
						{
							code = CEE_EXT_LEAVE;
						}
						else
						{
							code = CEE_EXT_FINALLY;
							iNestLevel = - iNestLevel;
						}

						const BRANCH_TARGET_INFO& brtarget = getBranchTargetInformation( iLabelID );
						if( brtarget.iExceptionHandlerNestLevel != iNestLevel )
						{
							//Need to emit several 'leave's
							//Insert leaves.
							for( int32_t i = 0;
								i < iNestLevel - brtarget.iExceptionHandlerNestLevel;
								++ i )
							{
								ILResolvedPool.ILPool.push_back( code );
							}
						}
						break;
					}
				default:
					ILResolvedPool.ILPool.push_back( 0xfe );
					ILResolvedPool.ILPool.push_back( cByteCode );
					pOpInfo = m_OpcodeMapByOpcode[ (OP_CODE)(0xfe00 | cByteCode) ];
					iMax = pOpInfo->OperandParams & OPERAND_PARAMS_SIZEMASK;
					for( int32_t ii = 0; ii < iMax; ii++ )
					{
						ILResolvedPool.ILPool.push_back( getCharFromILPool( *pInfo, i ) );
					}
					break;
				}
				break;
			default:
				{
				pOpInfo = m_OpcodeMapByOpcode[ (OP_CODE)cByteCode ];
				ILResolvedPool.ILPool.push_back( cByteCode );
				iMax = pOpInfo->OperandParams & OPERAND_PARAMS_SIZEMASK;
				for( int32_t ii = 0; ii < iMax; ii++ )
				{
					ILResolvedPool.ILPool.push_back( getCharFromILPool( *pInfo, i ) );
				}
				}
				break;
			}
		}
		//Going to parse new scope (thus function)
	}
	return true;
}

/***************************************************************************
 *      link pass 2: Check Target Distance and emit int8_t version if possible
 ***************************************************************************/
bool CCilCodeGen::linkPass2( ILFRAGMENTINFO& SourceILPool, ILFRAGMENTINFO& ILResolvedPool )
{
	assert( SourceILPool.ILPool.size() != 0 );
	assert( ILResolvedPool.ILPool.size() == 0 );

	int32_t iMax;
	uint8_t cByteCode;
	uint32_t iSize = uint32_t(SourceILPool.ILPool.size());
	for( uint32_t i = 0; i < iSize; )
	{
		OPCODE_TABLEENTRY* pInfo;
		cByteCode = getCharFromILPool( SourceILPool, i );

		switch( cByteCode )
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
			{
				uint32_t iTargetLableID = getIntFromILPool( SourceILPool, i );
				uint32_t iTargetAddress = m_mapLabelID2ILAddress[ iTargetLableID ];
				//i = next instruction's address
				uint32_t iCurrentILOffset = (uint32_t)ILResolvedPool.ILPool.size();
				int32_t iAddressDistance = iTargetAddress - iCurrentILOffset;

				if( iAddressDistance <= SHORT_JUMP_RANGE_MAX && iAddressDistance >= SHORT_JUMP_RANGE_MIN )
				{
					//Generate int16_t version
					//So we can reduce 3 bytes each
					
					//This should be heavy CPU load. May need an optimization
					// -3, amount that is reduced by the optimization
					adjustJumpTargetAddresses( iCurrentILOffset, -3 );	//Change jump target address -3;

					putPinToCurrentAddress( iCurrentILOffset, iTargetLableID );	//Put a address<->taget Label matching info
					//13 is a magic nuber to morph a jump instruction to int16_t version
					ILResolvedPool.ILPool.push_back( cByteCode - ( CEE_BR - CEE_BR_S ) );
					ILResolvedPool.ILPool.push_back( 0 );
				}
				else
				{
					//Generate long version
					ILResolvedPool.ILPool.push_back( cByteCode );

					if( getTargetEndianness() == TARGET_LITTLEENDIAN )
					{
						//Little endian version
						for( int32_t ii = 0; ii < sizeof( uint32_t ); ii++ )
						{
							uint8_t c = iTargetLableID & 0xff;
							iTargetLableID >>= 8;
							ILResolvedPool.ILPool.push_back( c );
						}
					}
					else
					{
						//Big endian version
						for( int32_t ii = sizeof( uint32_t ) - 1; ii >= 0; --ii )
						{
							uint8_t c = (iTargetLableID >> (ii * 8) )& 0xff;
							ILResolvedPool.ILPool.push_back( c );
						}
					}

				}
			}
			break;
		case 0xfe:
			ILResolvedPool.ILPool.push_back( cByteCode );
			cByteCode = getCharFromILPool( SourceILPool, i );
			ILResolvedPool.ILPool.push_back( cByteCode );
			pInfo = m_OpcodeMapByOpcode[ (OP_CODE)(0xfe00 | cByteCode) ];
			iMax = pInfo->OperandParams & OPERAND_PARAMS_SIZEMASK;
			for( int32_t ii = 0; ii < iMax; ii++ )
			{
				ILResolvedPool.ILPool.push_back( getCharFromILPool( SourceILPool, i ) );
			}
			break;
		default:
			ILResolvedPool.ILPool.push_back( cByteCode );
			pInfo = m_OpcodeMapByOpcode[ (OP_CODE)cByteCode ];
			iMax = pInfo->OperandParams & OPERAND_PARAMS_SIZEMASK;
			for( int32_t ii = 0; ii < iMax; ii++ )
			{
				ILResolvedPool.ILPool.push_back( getCharFromILPool( SourceILPool, i ) );
			}
			break;
		}
	}
	return true;
}

/***************************************************************************
 *      Label Address adjustment
 ***************************************************************************/
void CCilCodeGen::adjustJumpTargetAddresses( uint32_t iAddress, int32_t iOffset )
{
	map< uint32_t, uint32_t >::iterator it;
	map< uint32_t, uint32_t >::iterator itEnd = m_mapLabelID2ILAddress.end();
	for( it = m_mapLabelID2ILAddress.begin(); it != itEnd; ++it )
	{
		if( it->second > iAddress ) it->second += iOffset;
	}
	
}			

/***************************************************************************
 *      Put Address<->Label information for int16_t jump
 ***************************************************************************/
void CCilCodeGen::putPinToCurrentAddress( uint32_t iAddress, uint32_t iTargetLabelID )
{
		m_mapAddress2LabelID[ iAddress ] = iTargetLabelID;
}

/***************************************************************************
 *      link final pass: Finalize linkage process
 ***************************************************************************/
bool CCilCodeGen::linkFinalPass( ILFRAGMENTINFO& SourceILPool, ILFRAGMENTINFO& ILResolvedPool )
{
	assert( SourceILPool.ILPool.size() != 0 );
	assert( ILResolvedPool.ILPool.size() == 0 );

	int32_t iMax;
	uint8_t cByteCode;
	uint32_t iSize = uint32_t(SourceILPool.ILPool.size());
	for( uint32_t i = 0; i < iSize; )
	{
		OPCODE_TABLEENTRY* pInfo;
		cByteCode = getCharFromILPool( SourceILPool, i );

		switch( cByteCode )
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
			{
				uint32_t iTargetLableID = getIntFromILPool( SourceILPool, i );
				int32_t iDistanceOffset = m_mapLabelID2ILAddress[ iTargetLableID ] - i;

				//Generate long version
				ILResolvedPool.ILPool.push_back( cByteCode );
	
				if( getTargetEndianness() == TARGET_LITTLEENDIAN )
				{
					//Little endian version
					for( int32_t ii = 0; ii < sizeof( uint32_t ); ii++ )
					{
						uint8_t c = iDistanceOffset & 0xff;
						iDistanceOffset >>= 8;
						ILResolvedPool.ILPool.push_back( c );
					}
				}
				else
				{
					//Big endian version
					for( int32_t ii = sizeof( uint32_t ) - 1; ii >= 0; --ii )
					{
						uint8_t c = (iDistanceOffset >> (ii * 8) )& 0xff;
						ILResolvedPool.ILPool.push_back( c );
					}
				}

			}
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
			{
				uint32_t iTargetLableID = m_mapAddress2LabelID[ i - 1 ];
				i ++;	//Skip dummy
				int32_t iDistanceOffset = m_mapLabelID2ILAddress[ iTargetLableID ] - i;
				//-2 = adjust the pointer to the start of the jump inst.
				assert( iDistanceOffset <= SHORT_JUMP_RANGE_MAX -2 );
				assert( iDistanceOffset >= SHORT_JUMP_RANGE_MIN -2 );

				//Generate int16_t version
				ILResolvedPool.ILPool.push_back( cByteCode );
				uint8_t c = iDistanceOffset & 0xff;
				ILResolvedPool.ILPool.push_back( c );
			}
			break;
		case 0xfe:
			ILResolvedPool.ILPool.push_back( cByteCode );
			cByteCode = getCharFromILPool( SourceILPool, i );
			ILResolvedPool.ILPool.push_back( cByteCode );
			pInfo = m_OpcodeMapByOpcode[ (OP_CODE)(0xfe00 | cByteCode) ];
			iMax = pInfo->OperandParams & OPERAND_PARAMS_SIZEMASK;
			for( int32_t ii = 0; ii < iMax; ii++ )
			{
				ILResolvedPool.ILPool.push_back( SourceILPool.ILPool[ i ] );
			}
			break;
		default:
			ILResolvedPool.ILPool.push_back( cByteCode );
			pInfo = m_OpcodeMapByOpcode[ (OP_CODE)cByteCode ];
			iMax = pInfo->OperandParams & OPERAND_PARAMS_SIZEMASK;
			for( int32_t ii = 0; ii < iMax; ii++ )
			{
				ILResolvedPool.ILPool.push_back( getCharFromILPool( SourceILPool, i ) );
			}
			break;
		}
	}
	return true;
}

/***************************************************************************
 *      Resolve Method RID
 ***************************************************************************/
bool CCilCodeGen::resolveMethodRID( void )
{
	vector<METHOD_DEF>::iterator it = m_MetaData.getMethodTable().begin();
	vector<METHOD_DEF>::iterator itEnd = m_MetaData.getMethodTable().end();

	for( ; it != itEnd; ++it )
	{
		switch( it->MethodType )
		{
		case METHOD_CIL:
		case METHOD_EHCLAUSE:
			it->iILOffset = m_mapLabelID2ILAddress[ (RID)it->iILOffset ];
			break;
		case METHOD_NATIVE:
		case METHOD_WRAPPED:
			break;
		default:
			Debug_Fatal( "Bad method type" );
		}

	}
	return true;

}

/***************************************************************************
 *      link helper
 ***************************************************************************/
uint8_t CCilCodeGen::getCharFromILPool( ILFRAGMENTINFO& SourceILPool, uint32_t& iIndex )
{
	uint8_t cRet = SourceILPool.ILPool[ iIndex++ ];
	return cRet;
}

uint32_t CCilCodeGen::getIntFromILPool( ILFRAGMENTINFO& SourceILPool, uint32_t& iIndex )
{
	uint32_t iRet = *(uint32_t*)&SourceILPool.ILPool[ iIndex ];
	iIndex += sizeof( uint32_t );
	return iRet;
}

} //namespace CRI