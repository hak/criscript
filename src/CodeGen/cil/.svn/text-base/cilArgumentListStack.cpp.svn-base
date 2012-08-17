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
 * Module   : Argument list stack manupilation
 * File     : cilArgumentListStack.cpp
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
 *      Push entry size on arglist
 ***************************************************************************/
void CCilCodeGen::pushArgumentList( const uint32_t iParam )
{
	m_vecArgumentListStack.push_back( iParam );
}

/***************************************************************************
 *      emit arglist operation
 ***************************************************************************/
void CCilCodeGen::emitArgumentList( void )
{
	assert( m_vecArgumentListStack.size() );
	uint32_t iParam;
	iParam = m_vecArgumentListStack.back();
	m_vecArgumentListStack.pop_back();

	if( iParam )
	{
		if( iParam > MAXBYTE )
			emitArglist( CEE_EXT_STARGLIST, iParam);
		else
			emitArglist( CEE_EXT_STARGLIST_S, (uint8_t)iParam );
	}
}

} //namespace CRI