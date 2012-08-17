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
 * Module   : cil Virtual Machine
 * File     : CCilVmILPool.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "cilVm.h"
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Initialize string pool
 ***************************************************************************/
bool CCilVm::setILPool( const vector<uint8_t> & vecIL)
{
	if( m_status != VM_EXECUTE_INITIALIZING ) return false;

	ILFRAGMENTINFO ilInfo;
	ilInfo.ILPool = vecIL;
	m_CurrentILPool = ilInfo;

	m_pCurrentInstruction = &m_CurrentILPool.ILPool[ 0 ];

	return true;
}

bool CCilVm::setILPool( const uint8_t* pILPool, const size_t size )
{
	if( m_status != VM_EXECUTE_INITIALIZING ) return false;
	
	ILFRAGMENTINFO ilInfo;
	m_CurrentILPool = ilInfo;
	
	for( uint32_t i = 0; i < size; ++i )
		m_CurrentILPool.ILPool.push_back( *pILPool++ );
	
	m_pCurrentInstruction = &m_CurrentILPool.ILPool[ 0 ];
	
	return true;
}

} //namespace CRI