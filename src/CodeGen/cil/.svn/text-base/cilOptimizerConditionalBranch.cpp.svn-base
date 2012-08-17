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
 * File     : cilOptimizerConditionalBranch.cpp
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
int32_t CCilCodeGen::optimizerConditionalBranches(ILFRAGMENT::iterator itIn, ILFRAGMENT& optimizedILPool)
{
	uint32_t iByteCode = getNextByteCode( itIn );

	switch( iByteCode )
	{
	case CEE_CEQ:
		iByteCode = getNextByteCode( itIn );
		if( iByteCode == CEE_BRFALSE )
		{
			//CEE_EQU + CEE_BRFALSE = CEE_BNE_UN

			optimizedILPool.push_back( CEE_BNE_UN );
			for( int32_t ii = 0; ii < sizeof( int32_t ); ii++ )
				optimizedILPool.push_back( *itIn++ );
			return 2 + 1 + sizeof( int32_t );
		}
		else if( iByteCode == CEE_BRTRUE )
		{
			//CEE_EQU + CEE_BRTRUE = CEE_BEQ

			optimizedILPool.push_back( CEE_BEQ );
			for( int32_t ii = 0; ii < sizeof( int32_t ); ii++ )
				optimizedILPool.push_back( *itIn++ );
			return 2 + 1 + sizeof( int32_t );
		}
		break;
	case CEE_EXT_CNE:
		iByteCode = getNextByteCode( itIn );
		if( iByteCode == CEE_BRFALSE )
		{
			//CEE_EXT_NE + CEE_BRFALSE = CEE_BEQ

			optimizedILPool.push_back( CEE_BEQ );
			for( int32_t ii = 0; ii < sizeof( int32_t ); ii++ )
				optimizedILPool.push_back( *itIn++ );
			return 2 + 1 + sizeof( int32_t );
		}
		else if( iByteCode == CEE_BRTRUE )
		{
			//CEE_NE + CEE_BRTRUE = CEE_BNE_UN

			optimizedILPool.push_back( CEE_BNE_UN );
			for( int32_t ii = 0; ii < sizeof( int32_t ); ii++ )
				optimizedILPool.push_back( *itIn++ );
			return 2 + 1 + sizeof( int32_t );
		}
		break;
	case CEE_CGT:
		iByteCode = getNextByteCode( itIn );
		if( iByteCode == CEE_BRFALSE )
		{
			//CEE_GT + CEE_BRFALSE = CEE_EXT_BNGT

			optimizedILPool.push_back( CEE_EXT_BNGT );
			for( int32_t ii = 0; ii < sizeof( int32_t ); ii++ )
				optimizedILPool.push_back( *itIn++ );
			return 2 + 1 + sizeof( int32_t );
		}
		//else if( iByteCode == CEE_BRTRUE )
		//{
		//	//CEE_GT + CEE_BRTRUE = CEE_BGT

		//	optimizedILPool.push_back( CEE_BGT );
		//	for( int32_t ii = 0; ii < sizeof( int32_t ); ii++ )
		//		optimizedILPool.push_back( *itIn++ );
		//	return 2 + 1 + sizeof( int32_t );
		//}
		break;
	case CEE_CLT:
		iByteCode = getNextByteCode( itIn );
		if( iByteCode == CEE_BRFALSE )
		{
			//CEE_LT + CEE_BRFALSE = CEE_EXT_BNLT

			optimizedILPool.push_back( CEE_EXT_BNLT );
			for( int32_t ii = 0; ii < sizeof( int32_t ); ii++ )
				optimizedILPool.push_back( *itIn++ );
			return 2 + 1 + sizeof( int32_t );
		}
		//else if( iByteCode == CEE_BRTRUE )
		//{
		//	//CEE_LT + CEE_BRTRUE = CEE_BLT

		//	optimizedILPool.push_back( CEE_BLT );
		//	for( int32_t ii = 0; ii < sizeof( int32_t ); ii++ )
		//		optimizedILPool.push_back( *itIn++ );
		//	return 2 + 1 + sizeof( int32_t );
		//}
		break;
	case CEE_EXT_CGE:
		iByteCode = getNextByteCode( itIn );
		if( iByteCode == CEE_BRFALSE )
		{
			//CEE_EXT_CGE + CEE_BRFALSE = CEE_EXT_BNGE

			optimizedILPool.push_back( CEE_EXT_BNGE );
			for( int32_t ii = 0; ii < sizeof( int32_t ); ii++ )
				optimizedILPool.push_back( *itIn++ );
			return 2 + 1 + sizeof( int32_t );
		}
		//else if( iByteCode == CEE_BRTRUE )
		//{
		//	//CEE_EXT_CGE + CEE_BRTRUE = CEE_BGE

		//	optimizedILPool.push_back( CEE_BGE );
		//	for( int32_t ii = 0; ii < sizeof( int32_t ); ii++ )
		//		optimizedILPool.push_back( *itIn++ );
		//	return 2 + 1 + sizeof( int32_t );
		//}
		break;
	case CEE_EXT_CLE:
		iByteCode = getNextByteCode( itIn );
		if( iByteCode == CEE_BRFALSE )
		{
			//CEE_EXT_CLE + CEE_BRFALSE = CEE_EXT_BNLE

			optimizedILPool.push_back( CEE_EXT_BNLE );
			for( int32_t ii = 0; ii < sizeof( int32_t ); ii++ )
				optimizedILPool.push_back( *itIn++ );
			return 2 + 1 + sizeof( int32_t );
		}
		//else if( iByteCode == CEE_BRTRUE )
		//{
		//	//CEE_EXT_CLE + CEE_BRTRUE = CEE_BLE

		//	optimizedILPool.push_back( CEE_BLE );
		//	for( int32_t ii = 0; ii < sizeof( int32_t ); ii++ )
		//		optimizedILPool.push_back( *itIn++ );
		//	return 2 + 1 + sizeof( int32_t );
		//}
		break;
	default:
		Debug_Fatal( "Illegal parameter" );
	}
	return 0;
}



} //namespace CRI
