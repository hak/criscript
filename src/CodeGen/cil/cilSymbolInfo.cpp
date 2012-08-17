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
 * Module   : DebugInfoGenerator
 * File     : cilDebugInfo.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criscript.h"
namespace cri {

/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

	
/***************************************************************************
 * Look up symbol info from debug info pool
 ***************************************************************************/
RID CCilCodeGen::lookUpSymbolInformationInCurrentScope( wstring & strScopedVariableName )
{
	return lookUpSymbolInformationInCurrentScope( strScopedVariableName, NULL );
}

/***************************************************************************
 * Look up symbol info from debug info pool
 ***************************************************************************/
RID CCilCodeGen::lookUpSymbolInformationInCurrentScope( wstring & strScopedVariableName, CG_SYMBOL_INFORMATION**ppInfo )
{
	//Check with statement stack
	if( m_WithStatementLookupStack.size() )
	{
		//Retrieve symbol name
		wstring::reverse_iterator itSymbol = strScopedVariableName.rbegin();
		wstring strSubScopedVariableName = L"";
		for( ; itSymbol != strScopedVariableName.rend(); ++itSymbol )
		{
			if( *itSymbol != STRING_INTERNAL_SCOPEDELIMITER_SINGLE )
			{
				strSubScopedVariableName = *itSymbol + strSubScopedVariableName;
			}
		}

		wstring s;
		//Look up object property
		vector< CCgNodeData* >::reverse_iterator it = m_WithStatementLookupStack.rbegin();
		while( it != m_WithStatementLookupStack.rend() )
		{
			assert( (*it)->NodeType == CG_NODE_IDENTIFIER );
			s = *(*it)->pIdentifier;
			s.append( STRING_INTERNAL_SCOPEDELIMITER );
			s.append( strSubScopedVariableName );
			if( m_SymbolInfoPool.find( s ) != m_SymbolInfoPool.end() )
			{
				if( ppInfo != NULL )
					*ppInfo = &m_SymbolInfoPool[ strScopedVariableName ];
				return m_SymbolInfoPool[ strScopedVariableName ].rid;
			}
			++it;
		}
	}

	if( m_SymbolInfoPool.find( strScopedVariableName ) != m_SymbolInfoPool.end() )
	{
		if( ppInfo != NULL )
			*ppInfo = &m_SymbolInfoPool[ strScopedVariableName ];
		return m_SymbolInfoPool[ strScopedVariableName ].rid;
	}
	return RID_NOTDEFINED;
}

/***************************************************************************
 * Look up symbol info from debug info pool
 ***************************************************************************/
RID CCilCodeGen::lookUpSymbolInformation( wstring & strScopedVariableName )
{
	return lookUpSymbolInformation( strScopedVariableName, NULL );
}



/***************************************************************************
 * Look up symbol info from debug info pool
 ***************************************************************************/
RID CCilCodeGen::lookUpSymbolInformation( wstring & strScopedVariableName, CG_SYMBOL_INFORMATION**ppInfo )
{
	//Check with statement stack
	if( m_WithStatementLookupStack.size() )
	{
		//Retrieve symbol name
		wstring::reverse_iterator itSymbol = strScopedVariableName.rbegin();
		wstring strSubScopedVariableName = L"";
		for( ; itSymbol != strScopedVariableName.rend(); ++itSymbol )
		{
			if( *itSymbol != STRING_INTERNAL_SCOPEDELIMITER_SINGLE )
			{
				strSubScopedVariableName = *itSymbol + strSubScopedVariableName;
			}
		}

		wstring s;
		//Look up object property
		vector< CCgNodeData* >::reverse_iterator it = m_WithStatementLookupStack.rbegin();
		while( it != m_WithStatementLookupStack.rend() )
		{
			assert( (*it)->NodeType == CG_NODE_IDENTIFIER );
			s = *(*it)->pIdentifier;
			s.append( STRING_INTERNAL_SCOPEDELIMITER );
			s.append( strSubScopedVariableName );
			if( m_SymbolInfoPool.find( s ) != m_SymbolInfoPool.end() )
			{
				if( ppInfo != NULL )
					*ppInfo = &m_SymbolInfoPool[ strScopedVariableName ];
				return m_SymbolInfoPool[ strScopedVariableName ].rid;
			}
			++it;
		}

	}

	return lookUpSymbolInformationSub( strScopedVariableName, ppInfo );
}

/***************************************************************************
 * lookUpSymbolInformation helper/ recursively called
 ***************************************************************************/
RID CCilCodeGen::lookUpSymbolInformationSub( wstring & strScopedVariableName, CG_SYMBOL_INFORMATION**ppInfo )
{
	hash_map<wstring, CG_SYMBOL_INFORMATION>::iterator itVariableName;
	itVariableName = m_SymbolInfoPool.find( strScopedVariableName );
	if( itVariableName != m_SymbolInfoPool.end() )
	{
		if( ppInfo != NULL )
			*ppInfo = &itVariableName->second;
		return itVariableName->second.rid;
	}
	//If strScopedVariableName is a multiple scoped name, reduce scope and lookup again
	int32_t iPos = int32_t (strScopedVariableName.rfind( STRING_INTERNAL_SCOPEDELIMITER_SINGLE ) );
	if( iPos != -1 )
	{
		wstring::reverse_iterator it = strScopedVariableName.rbegin() + strScopedVariableName.size() - iPos - 1;
		wstring::reverse_iterator itEnd = strScopedVariableName.rend();
		wstring strSubScopedVariableName = L"";
		if( *it == STRING_INTERNAL_SCOPEDELIMITER_SINGLE )
		{
			wstring::reverse_iterator itSt = it - 1;
			wstring::reverse_iterator itEd = strScopedVariableName.rbegin();
			strSubScopedVariableName.append( &*itSt, itSt - itEd + 1 );

			++it;	//Skip '@'
			//Skip until next delimiter
			for( ; it != itEnd; ++it )
			{
				if( *it == STRING_INTERNAL_SCOPEDELIMITER_SINGLE )
					break;
			}
			if( it != itEnd )
			{
				strSubScopedVariableName = strScopedVariableName.substr(0, (itEnd - it )) + strSubScopedVariableName;
			}
		}
		//scope1@scope2@tmp -> scope1@tmp
		//scope1@tmp -> tmp
		return lookUpSymbolInformationSub( strSubScopedVariableName, ppInfo );
	}
	return RID_NOTDEFINED;
}

/***************************************************************************
 * Generate symbol info
 ***************************************************************************/
void CCilCodeGen::generateSymbolInformation( const RID ridSymbol,
											const wstring & strScopedVariableName,
											const CG_SYMBOL_FLAG Flag,
											const int32_t iIndex )
{
	ILFRAGMENTINFO* pILinfo = getCurrentScopeInformation();
	CG_SYMBOL_INFORMATION info;
	info.rid = ridSymbol;
	info.ridScope = pILinfo->MethodRID;
	info.Flags = Flag;
	info.iIndex = iIndex;
	m_SymbolInfoPool[ strScopedVariableName ] = info;

}

/***************************************************************************
 * Tag the line for a symbol<->Source line matching info
 ***************************************************************************/
uint32_t CCilCodeGen::tagTheLine()
{
	uint32_t iDebugTag;
	//emit codes
	iDebugTag = createNewLabelIDAndInsertTag();

	//Add iDebugTag # to source line/IL matching table

	return iDebugTag;
}



} //namespace CRI