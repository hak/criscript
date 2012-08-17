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
 * Module   : cil codegenerater
 * File     : cilFunctionVariables.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criscript.h"
#include "cilCodeGen.h"
namespace cri {
	
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 * Argument load
 ***************************************************************************/	
void CCilCodeGen::emitLoadArgument( CG_SYMBOL_INFORMATION* pInfo )
{
	assert( pInfo != NULL );
	RID ridArgList = pInfo->rid;
	int32_t iArgumentIndex = pInfo->iIndex;

	assert( iArgumentIndex <= SHRT_MAX && iArgumentIndex >= SHRT_MIN );
	assert( TypeFromToken( ridArgList ) == MDT_PARAMDEF );

	ILFRAGMENTINFO* infoCurrentScope =  getCurrentScopeInformation();
	//Check if the symbol is in the current scope or other scope.
	if( pInfo->ridScope != infoCurrentScope->MethodRID )
	{
		//The Symbol is in the different scope, need to be resolved later
		emit( CEE_EXT_LDARG, infoCurrentScope->MethodRID, pInfo->rid, iArgumentIndex );
	}
	else
	{
		assert( iArgumentIndex < (int32_t)m_MetaData.getParamListSize( ridArgList ) );
		if( iArgumentIndex == 0 )
		{
			emit( CEE_LDARG_0 );
		}
		else if( iArgumentIndex == 1 )
		{
			emit( CEE_LDARG_1 );
		}
		else if( iArgumentIndex == 2 )
		{
			emit( CEE_LDARG_2 );
		}
		else if( iArgumentIndex == 3 )
		{
			emit( CEE_LDARG_3 );
		}
		else if( iArgumentIndex <= SCHAR_MAX && iArgumentIndex >= SCHAR_MIN )
		{
			emit( CEE_LDARG_S, (int8_t)iArgumentIndex );
		}
		else
		{
			emit( CEE_LDARG, (int16_t)iArgumentIndex );
		}
	}
}

/***************************************************************************
 * Argument store
 ***************************************************************************/	
void CCilCodeGen::emitStoreArgument( CG_SYMBOL_INFORMATION* pInfo )
{
	assert( pInfo != NULL );
	RID ridArgList = pInfo->rid;
	int32_t iArgumentIndex = pInfo->iIndex;

	assert( iArgumentIndex <= SHRT_MAX && iArgumentIndex >= SHRT_MIN );
	assert( TypeFromToken( ridArgList ) == MDT_PARAMDEF );

	ILFRAGMENTINFO* infoCurrentScope =  getCurrentScopeInformation();
	//Check if the symbol is in the current scope or other scope.
	if( pInfo->ridScope != infoCurrentScope->MethodRID )
	{
		//The Symbol is in the different scope, need to be resolved later
		emit( CEE_EXT_STARG, infoCurrentScope->MethodRID, pInfo->rid, iArgumentIndex );
	}
	else
	{
		assert( iArgumentIndex < (int32_t)m_MetaData.getParamListSize( ridArgList ) );
		if( iArgumentIndex <= SCHAR_MAX && iArgumentIndex >= SCHAR_MIN )
		{
			emit( CEE_STARG_S, (int8_t)iArgumentIndex );
		}
		else
		{
			emit( CEE_STARG, (int16_t)iArgumentIndex );
		}
	}
}

/***************************************************************************
 * Local load
 ***************************************************************************/	
void CCilCodeGen::emitLoadLocal( CG_SYMBOL_INFORMATION* pInfo )
{
	assert( pInfo != NULL );
	RID ridArgList = pInfo->rid;
	int32_t iArgumentIndex = pInfo->iIndex;

	assert( iArgumentIndex <= SHRT_MAX && iArgumentIndex >= SHRT_MIN );
	assert( TypeFromToken( ridArgList ) == MDT_PARAMDEF );
	ILFRAGMENTINFO* infoCurrentScope =  getCurrentScopeInformation();
	//Check if the symbol is in the current scope or other scope.
	if( pInfo->ridScope != infoCurrentScope->MethodRID )
	{
		//The Symbol is in the different scope, need to be resolved later
		emit( CEE_EXT_LDLOC, infoCurrentScope->MethodRID, pInfo->rid, iArgumentIndex );
	}
	else
	{
		assert( iArgumentIndex < (int32_t)m_MetaData.getParamListSize( ridArgList ) );
		if( iArgumentIndex == 0 )
		{
			emit( CEE_LDLOC_0 );
		}
		else if( iArgumentIndex == 1 )
		{
			emit( CEE_LDLOC_1 );
		}
		else if( iArgumentIndex == 2 )
		{
			emit( CEE_LDLOC_2 );
		}
		else if( iArgumentIndex == 3 )
		{
			emit( CEE_LDLOC_3 );
		}
		else if( iArgumentIndex <= SCHAR_MAX && iArgumentIndex >= SCHAR_MIN )
		{
			emit( CEE_LDLOC_S, (int8_t)iArgumentIndex );
		}
		else
		{
			emit( CEE_LDLOC, (int16_t)iArgumentIndex );
		}
	}
}

/***************************************************************************
 * Local store
 ***************************************************************************/	
void CCilCodeGen::emitStoreLocal( CG_SYMBOL_INFORMATION* pInfo )
{
	assert( pInfo != NULL );
	RID ridArgList = pInfo->rid;
	int32_t iArgumentIndex = pInfo->iIndex;

	assert( iArgumentIndex <= SHRT_MAX && iArgumentIndex >= SHRT_MIN );
	assert( TypeFromToken( ridArgList ) == MDT_PARAMDEF );
	ILFRAGMENTINFO* infoCurrentScope =  getCurrentScopeInformation();
	//Check if the symbol is in the current scope or other scope.
	if( pInfo->ridScope != infoCurrentScope->MethodRID )
	{
		//The Symbol is in the different scope, need to be resolved later
		emit( CEE_EXT_STLOC, infoCurrentScope->MethodRID, pInfo->rid, iArgumentIndex );
	}
	else
	{
		assert( iArgumentIndex < (int32_t)m_MetaData.getParamListSize( ridArgList ) );
		if( iArgumentIndex == 0 )
		{
			emit( CEE_STLOC_0 );
		}
		else if( iArgumentIndex == 1 )
		{
			emit( CEE_STLOC_1 );
		}
		else if( iArgumentIndex == 2 )
		{
			emit( CEE_STLOC_2 );
		}
		else if( iArgumentIndex == 3 )
		{
			emit( CEE_STLOC_3 );
		}
		else if( iArgumentIndex <= SCHAR_MAX && iArgumentIndex >= SCHAR_MIN )
		{
			emit( CEE_STLOC_S, (int8_t)iArgumentIndex );
		}
		else
		{
			emit( CEE_STLOC, (int16_t)iArgumentIndex );
		}
	}
}

/***************************************************************************
 * Resolve arugument index
 * Invoked while linke process
 ***************************************************************************/	

/***************************************************************************
 * Resolve local variable arugument index
 ***************************************************************************/	
bool CCilCodeGen::resolveLoadArgument( ILFRAGMENTINFO* pilinfo,
										RID ridMethod,
										RID ridArglist,
										uint32_t iIndex )
{
	assert( pilinfo != NULL );
	assert( TypeFromToken( ridMethod ) == MDT_METHODDEF );
	assert( TypeFromToken( ridArglist ) == MDT_PARAMDEF );

	int32_t iArgumentIndex = 0;
	METHOD_DEF* pMethod = &m_MetaData.getMethodTable()[ RidFromToken( ridMethod ) ];
	do
	{
		iArgumentIndex -= pMethod->iNumArgments;
		pMethod = &m_MetaData.getMethodTable()[ RidFromToken( pMethod->ridScope ) ];
	} while( pMethod->ridArgList != ridArglist && pMethod->ridScope != 0);

	iArgumentIndex = iIndex -pMethod->iNumArgments;

	if( pMethod->ridArgList != ridArglist )
		return false;

	if( iArgumentIndex <= SCHAR_MAX && iArgumentIndex >= SCHAR_MIN )
	{
		emit( pilinfo, CEE_LDARG_S, (int8_t)iArgumentIndex );
	}
	else
	{
		emit( pilinfo, CEE_LDARG, (int16_t)iArgumentIndex );
	}
	return true;
}

/***************************************************************************
 * Resolve local variable arugument index
 ***************************************************************************/	
bool CCilCodeGen::resolveStoreArgument( ILFRAGMENTINFO* pilinfo,
										RID ridMethod,
										RID ridArglist,
										uint32_t iIndex )
{
	assert( pilinfo != NULL );
	assert( TypeFromToken( ridMethod ) == MDT_METHODDEF );
	assert( TypeFromToken( ridArglist ) == MDT_PARAMDEF );

	int32_t iArgumentIndex = 0;
	METHOD_DEF* pMethod = &m_MetaData.getMethodTable()[ RidFromToken( ridMethod ) ];
	do
	{
		iArgumentIndex -= pMethod->iNumArgments;
		pMethod = &m_MetaData.getMethodTable()[ RidFromToken( pMethod->ridScope ) ];
	} while( pMethod->ridArgList != ridArglist && pMethod->ridScope != 0);

	iArgumentIndex = iIndex -pMethod->iNumArgments;

	if( pMethod->ridArgList != ridArglist )
	{
		Debug_Fatal( "Illegal Arglist" );
		return false;
	}

	if( iArgumentIndex <= SCHAR_MAX && iArgumentIndex >= SCHAR_MIN )
	{
		emit( pilinfo, CEE_STARG_S, (int8_t)iArgumentIndex );
	}
	else
	{
		emit( pilinfo, CEE_STARG, (int16_t)iArgumentIndex );
	}
	return true;
}

/***************************************************************************
 * Resolve local variable arugument index
 ***************************************************************************/	
bool CCilCodeGen::resolveLoadLocal( ILFRAGMENTINFO* pilinfo,
									RID ridMethod,
									RID ridLocallist,
									uint32_t iIndex )
{
	assert( pilinfo != NULL );
	assert( TypeFromToken( ridMethod ) == MDT_METHODDEF );
	assert( TypeFromToken( ridLocallist ) == MDT_PARAMDEF );

	int32_t iLocalIndex = 0;
	METHOD_DEF* pMethod = &m_MetaData.getMethodTable()[ RidFromToken( ridMethod ) ];
	do
	{
		iLocalIndex -= pMethod->iNumLocals;
		pMethod = &m_MetaData.getMethodTable()[ RidFromToken( pMethod->ridScope ) ];
	} while( pMethod->ridLocalVariableList != ridLocallist && pMethod->ridScope != 0);

	iLocalIndex = iIndex -pMethod->iNumLocals;

	if( pMethod->ridLocalVariableList != ridLocallist )
	{
		Debug_Fatal( "Illegal Arglist" );
		return false;
	}

	if( iLocalIndex <= SCHAR_MAX && iLocalIndex >= SCHAR_MIN )
	{
		emit( pilinfo, CEE_LDLOC_S, (int8_t)iLocalIndex );
	}
	else
	{
		emit( pilinfo, CEE_LDLOC, (int16_t)iLocalIndex );
	}
	return true;
}

/***************************************************************************
 * Resolve local variable arugument index
 ***************************************************************************/	
bool CCilCodeGen::resolveStoreLocal( ILFRAGMENTINFO* pilinfo,
									RID ridMethod,
									RID ridLocallist,
									uint32_t iIndex )
{
	assert( pilinfo != NULL );
	assert( TypeFromToken( ridMethod ) == MDT_METHODDEF );
	assert( TypeFromToken( ridLocallist ) == MDT_PARAMDEF );

	int32_t iLocalIndex = 0;
	METHOD_DEF* pMethod = &m_MetaData.getMethodTable()[ RidFromToken( ridMethod ) ];
	do
	{
		iLocalIndex -= pMethod->iNumLocals;
		pMethod = &m_MetaData.getMethodTable()[ RidFromToken( pMethod->ridScope ) ];
	} while( pMethod->ridLocalVariableList != ridLocallist && pMethod->ridScope != 0);

	iLocalIndex = iIndex -pMethod->iNumLocals;

	if( pMethod->ridLocalVariableList != ridLocallist )
	{
		Debug_Fatal( "Illegal Arglist" );
		return false;
	}

	if( iLocalIndex <= SCHAR_MAX && iLocalIndex >= SCHAR_MIN )
	{
		emit( pilinfo, CEE_STLOC_S, (int8_t)iLocalIndex );
	}
	else
	{
		emit( pilinfo, CEE_STLOC, (int16_t)iLocalIndex );
	}
	return true;
}

} //namespace CRI