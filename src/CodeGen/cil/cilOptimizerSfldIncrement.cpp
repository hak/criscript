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
 * File     : cilOptimizerSfldIncrement.cpp
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
int32_t CCilCodeGen::optimizerSfldIncrement(ILFRAGMENT::iterator itIn, ILFRAGMENT& optimizedILPool)
{
	uint32_t iByteCode = getNextByteCode( itIn );
	bool bFound = false;

	RID ridSrc;
	RID ridDest;

	ILFRAGMENT::iterator itOp = itIn;
	
	switch( iByteCode )
	{
	case CEE_LDSFLD:
		ridSrc = getIntOperand( itIn );
		iByteCode = getNextByteCode( itIn );
		if( iByteCode != CEE_EXT_INC )
			break;
		iByteCode = getNextByteCode( itIn );
		if( iByteCode != CEE_STSFLD )
			break;
		ridDest = getIntOperand( itIn );
		if( ridSrc == ridDest )
			bFound = true;
		break;
	default:
		Debug_Fatal( "Illegal parameter" );
	}

	if( bFound )
	{
		//Got optimization case
		optimizedILPool.push_back( CEE_EXT_INCSFLD );

		for( int32_t i = 0; i < sizeof( int32_t ); ++i )
		{
			optimizedILPool.push_back( *itOp++ );
		}
		return 5 + 1 + 5;	//ldsfld + inc + stsfld
	}

	return 0;
}



} //namespace CRI
