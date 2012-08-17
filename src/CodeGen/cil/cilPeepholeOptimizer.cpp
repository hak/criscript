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
 * Module   : Peephole optimizer
 * File     : cilPeepholeOptimizer.cpp
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


/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      initPeepholeOptimizer
 ***************************************************************************/
bool CCilCodeGen::initPeepholeOptimizer()
{
	//Mearge condition check+conditional branches
	m_mapPeepholeOptimizer[ CEE_CEQ ] = optimizerConditionalBranches;
	m_mapPeepholeOptimizer[ CEE_EXT_CNE ] = optimizerConditionalBranches;
	m_mapPeepholeOptimizer[ CEE_CGT ] = optimizerConditionalBranches;
	m_mapPeepholeOptimizer[ CEE_CLT ] = optimizerConditionalBranches;
	m_mapPeepholeOptimizer[ CEE_EXT_CGE ] = optimizerConditionalBranches;
	m_mapPeepholeOptimizer[ CEE_EXT_CLE ] = optimizerConditionalBranches;

	//Remove redandant dup
	m_mapPeepholeOptimizer[ CEE_DUP ] = optimizerEliminateRedandantDup;

	//Optimize ld/inc/st case
	m_mapPeepholeOptimizer[ CEE_LDSFLD ] = optimizerSfldIncrement;
	
	return true;
}

/***************************************************************************
 *      performPeepholeOptimizer
 ***************************************************************************/
bool CCilCodeGen::performPeepholeOptimize( ILFRAGMENTINFO* pILPool )
{
	//early termination
	if( pILPool->ILPool.size() == 0 )
		return true;

	ILFRAGMENTINFO& info = *pILPool;
	ILFRAGMENT optimizedILPool;

	ILFRAGMENT* pSrc = &pILPool->ILPool;
	ILFRAGMENT* pDest = &optimizedILPool;

	int32_t iMax;
	uint8_t cByteCode;
	hash_map< OP_CODE, PEEPHOLEOPTIMIZERCALLBACK >::iterator itCallback;

	uint32_t i;
	bool bContinue = true;

	while( bContinue )
	{
		pDest->clear();
		uint32_t iSize = uint32_t(pSrc->size());

		bContinue = false;

		for( i = 0; i < iSize; )
		{
			OPCODE_TABLEENTRY* pInfo;
			cByteCode = (*pSrc)[ i++ ];
			
			switch( cByteCode )
			{
			case 0xfe:
				cByteCode = (*pSrc)[ i++ ];

				//Invoke call back if available
				itCallback = m_mapPeepholeOptimizer.find( OP_CODE(cByteCode | 0xfe00) );
				if( itCallback != m_mapPeepholeOptimizer.end() )
				{
					if( int32_t iSkip = itCallback->second( pSrc->begin() + i - 2, *pDest ) )
					{
						i += iSkip - 2;
						bContinue = true;
						break;
					}
				}

				switch( cByteCode )
				{
				case CEE_CODE_LABEL & 0xff:
					{
						pDest->push_back( 0xfe );
						pDest->push_back( cByteCode );

						//Skip code label info
						for( int32_t ii = 0; ii < sizeof( int32_t ); ii++ )
						{
							pDest->push_back( (*pSrc)[ i++ ] );
						}
						break;
					}

				case CEE_EXT_LDARG & 0xff:
				case CEE_EXT_STARG & 0xff:
				case CEE_EXT_LDLOC & 0xff:
				case CEE_EXT_STLOC & 0xff:
					{
						pDest->push_back( 0xfe );
						pDest->push_back( cByteCode );

						for( int32_t ii = 0; ii < sizeof( int32_t ) * 3; ii++ )
						{
							pDest->push_back( (*pSrc)[ i++ ] );
						}
						break;
					}
				case CEE_LEAVE_TMP & 0xff:
					{
						pDest->push_back( 0xfe );
						pDest->push_back( cByteCode );

						for( int32_t ii = 0; ii < sizeof( int32_t ) * 2; ii++ )
						{
							pDest->push_back( (*pSrc)[ i++ ] );
						}
						break;
					}
				default:
					pDest->push_back( 0xfe );
					pDest->push_back( cByteCode );
					pInfo = m_OpcodeMapByOpcode[ (OP_CODE)(0xfe00 | cByteCode) ];
					iMax = pInfo->OperandParams & OPERAND_PARAMS_SIZEMASK;
					for( int32_t ii = 0; ii < iMax; ii++ )
					{
						pDest->push_back( (*pSrc)[ i++ ] );
					}
					break;
				}
				break;
			default:
				{
				//Invoke call back if available
				hash_map< OP_CODE, PEEPHOLEOPTIMIZERCALLBACK >::iterator itCallback;
				itCallback = m_mapPeepholeOptimizer.find( OP_CODE(cByteCode) );
				if( itCallback != m_mapPeepholeOptimizer.end() )
				{
					if( int32_t iSkip = itCallback->second( pSrc->begin() + i - 1, *pDest ) )
					{
						i += iSkip - 1;
						bContinue = true;
						break;
					}
				}

				pInfo = m_OpcodeMapByOpcode[ (OP_CODE)cByteCode ];
				pDest->push_back( cByteCode );
				iMax = pInfo->OperandParams & OPERAND_PARAMS_SIZEMASK;
				for( int32_t ii = 0; ii < iMax; ii++ )
				{
					pDest->push_back( (*pSrc)[ i++ ] );
				}
				}
				break;
			}
		}


		if( bContinue )
		{
			ILFRAGMENT* pTmp = pSrc;
			pSrc = pDest;
			pDest = pTmp;
		}
	}

	if( pDest != &info.ILPool )
	{
		info.ILPool.clear();
		info.ILPool = optimizedILPool;
	}

	return true;
}



} //namespace CRI