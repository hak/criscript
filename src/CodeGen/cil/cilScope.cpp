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
 * Module   : Scope management
 * File     : cilScope.cpp
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
 *      isInGlobalScope
 ***************************************************************************/	
bool CCilCodeGen::isInGlobalScope( void )
{
	if( m_ILPool[ m_iCurrentILPoolIndex ].MethodRID == 0 )
		return true;
	if( m_MetaData.getMethod( m_ILPool[ m_iCurrentILPoolIndex ].MethodRID ).ridLocalVariableList == 0 )
		return true;
	return false;
	//return ( m_iCurrentILPoolIndex == 0 );
}

/***************************************************************************
 *      returns current scopename
 ***************************************************************************/	
wstring& CCilCodeGen::getCurrentScopeName( void )
{
	return m_strCurrentScope;
}

/***************************************************************************
 *      returns current scope information
 ***************************************************************************/	
ILFRAGMENTINFO* CCilCodeGen::getCurrentScopeInformation( void )
{
	return &m_ILPool[ m_iCurrentILPoolIndex ];
}

/***************************************************************************
 *      Enter new scope
 ***************************************************************************/	
void CCilCodeGen::enterNewScope( wstring& strScopedFunctionName, RID MethodRID )
{
	if( strScopedFunctionName != L"" )
	{
		m_strCurrentScope = strScopedFunctionName + STRING_INTERNAL_SCOPEDELIMITER;
	}
	else
		m_strCurrentScope = strScopedFunctionName;
	
	ILFRAGMENTINFO ilinfo;
	ilinfo.ILPool.clear();
	m_ILPool.push_back( ilinfo );
	
	V_ILFRAGMENTINFO::iterator it = m_ILPool.end() - 1;
	it->MethodRID = MethodRID;
	it->strScopeName = strScopedFunctionName;
	it->iNumEvalStackEntries = 0;
	
	m_iCurrentILPoolIndex = (uint32_t)m_ILPool.size() - 1;
	m_ILPoolCallStack.push_back( m_iCurrentILPoolIndex );

	clearILOffsetCache();
	
}

/***************************************************************************
 *      Leave current scope
 ***************************************************************************/	
void CCilCodeGen::leaveScope()
{
	assert( m_ILPoolCallStack.size() > 1 );
	m_ILPoolCallStack.pop_back();
	m_iCurrentILPoolIndex = m_ILPoolCallStack.back();

	m_strCurrentScope = m_strCurrentScope.substr( 0, 
		m_strCurrentScope.rfind( STRING_INTERNAL_SCOPEDELIMITER, m_strCurrentScope.length() - 2) + 1 );

	clearILOffsetCache();		
}

} //namespace CRI