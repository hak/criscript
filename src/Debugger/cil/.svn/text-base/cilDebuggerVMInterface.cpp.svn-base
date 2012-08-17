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
 * Module   : cil Virtual Machine Debugger
 * File     : cilDebuggerVMInterface.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
#include "cilVm.h"
#include "cilDebugger.h"
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Attach a VM
 ***************************************************************************/
bool CCilDebugger::attach( CCilVm * pVM )
{
	if( m_pVM != NULL ) detach();
	m_pVM = pVM;
	pVM->attachDebugger( this );
	return true;
}

/***************************************************************************
 *      Detach a VM
 ***************************************************************************/
bool CCilDebugger::detach()
{
	assert( m_pVM != NULL );
	m_pVM = NULL;
	return true;
}

/***************************************************************************
 *      Set symbol info
 ***************************************************************************/
void CCilDebugger::setSymbolInformation( hash_map<wstring, CG_SYMBOL_INFORMATION>& symbolinfo)
{
	m_SymbolInfoPool = symbolinfo;
}


} //namespace CRI