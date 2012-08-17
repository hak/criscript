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
 * File     : cilRedandantDup.cpp
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
 *      optimizerConditionalBranches
 ***************************************************************************/
int32_t CCilCodeGen::optimizerEliminateRedandantDup(ILFRAGMENT::iterator itIn, ILFRAGMENT& optimizedILPool)
{
	uint32_t iByteCode = getNextByteCode( itIn );
	OPCODE_TABLEENTRY* pOpInfo;
	bool bContinue = true;
	bool bFound = false;
	int32_t iVariableInStack = 2;
	int32_t iNumByteCodes = 0;

	ILFRAGMENT::iterator itStart = itIn;

	switch( iByteCode )
	{
	case CEE_DUP:
		//Check into the stream
		while( bContinue ) {
			iByteCode = getNextByteCode( itIn );
			iNumByteCodes ++;
			switch( iByteCode )
			{
			default:
				pOpInfo = m_OpcodeMapByOpcode[ (OP_CODE)iByteCode ];
				itIn += pOpInfo->OperandParams & OPERAND_PARAMS_SIZEMASK;
				iNumByteCodes += pOpInfo->OperandParams & OPERAND_PARAMS_SIZEMASK;
				
				iVariableInStack += pOpInfo->iPushbehaivor;
				iVariableInStack -= pOpInfo->iPopbehaivor;

				if( pOpInfo->OpFlow != NEXT || iVariableInStack <= 0 )
				{
					bContinue = false;
				}
				break;
			case CEE_EXT_LDARG:
			case CEE_EXT_STARG:
			case CEE_EXT_LDLOC:
			case CEE_EXT_STLOC:
				pOpInfo = m_OpcodeMapByOpcode[ (OP_CODE)iByteCode ];
				itIn += sizeof( int32_t ) * 3;
				iNumByteCodes += sizeof( int32_t ) * 3;
				
				iVariableInStack += pOpInfo->iPushbehaivor;
				iVariableInStack -= pOpInfo->iPopbehaivor;

				if( pOpInfo->OpFlow != NEXT || iVariableInStack <= 0 )
				{
					bContinue = false;
				}
				break;
			case CEE_POP:
				//Got it!!
				if( iVariableInStack == 1 )
					bFound = true;
				bContinue = false;
				break;
			case CEE_LEAVE_TMP:
			case CEE_CODE_LABEL:
			case CEE_DUP:
			case CEE_RET:
				bContinue = false;
				break;
			}
		}
		break;
	default:
		Debug_Fatal( "Illegal parameter" );
	}

	if( bFound )
	{
		//Got optimization case
		iNumByteCodes--;	//-1 = pop
		for( int32_t i = 0; i < iNumByteCodes; ++i )
		{
			optimizedILPool.push_back( *itStart++ );
		}
		assert( *itStart == CEE_POP );
		return iNumByteCodes + 2;	//Push/Pop + bytecode between them
	}
	

	return 0;
}



} //namespace CRI
