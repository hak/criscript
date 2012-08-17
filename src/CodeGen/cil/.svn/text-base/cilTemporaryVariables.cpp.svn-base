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
 * Module   : Temporary variable management
 * File     : cilTemporaryVariables.cpp
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
 * Reserve a temporary variable with specific type
 ***************************************************************************/	
RID CCilCodeGen::reserveTemporaryVariable( CG_SYMBOL_FLAG VariableTypeToReserve )
{
	RID ridTempVar;
	multimap< CG_SYMBOL_FLAG, RID>::iterator itTempVariable;
	itTempVariable = m_mapFreeTemporaryVariables.find( VariableTypeToReserve );
	if( itTempVariable != m_mapFreeTemporaryVariables.end() )
	{
		//Found free temp var
		ridTempVar = itTempVariable->second;
		m_mapReservedTemporaryVariables[ ridTempVar ] = itTempVariable->first;
		m_mapFreeTemporaryVariables.erase( itTempVariable );
	}
	else
	{
		wchar_t strVariable[ 9 ];
		wstring s = STRING_TEMPVARIABLENAME_PREFIX;
		swprintf( strVariable, sizeof( strVariable ) / sizeof strVariable[ 0 ],
					L"%08x", m_iNumTmpVariable++ );
		s.append( strVariable );
		//Create new temp var
		switch ( VariableTypeToReserve )
		{
		case CG_SYMBOL_FLAG_GLOBAL:
			//Global scope variable
			ridTempVar = m_MetaData.generateStaticFieldRID();
			generateSymbolInformation( ridTempVar, s );
			m_mapReservedTemporaryVariables[ ridTempVar ] = VariableTypeToReserve;
			break;
		case CG_SYMBOL_FLAG_LOCAL:
		default:
			Debug_Fatal( "Not implemented yet" );
			break;
		}
	}
	return ridTempVar;

}

/***************************************************************************
 * Release reserved temp variable
 ***************************************************************************/	
void CCilCodeGen::releaseTemporaryVariable( RID ridToRelease )
{
	map< RID, CG_SYMBOL_FLAG >::iterator itTempVariable;
	itTempVariable = m_mapReservedTemporaryVariables.find( ridToRelease );
	assert( itTempVariable != m_mapReservedTemporaryVariables.end() );
	m_mapFreeTemporaryVariables.insert( pair< CG_SYMBOL_FLAG, RID > (itTempVariable->second, ridToRelease ) );
	m_mapReservedTemporaryVariables.erase( ridToRelease );

}


} //namespace CRI