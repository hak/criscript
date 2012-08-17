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
 * File     : CCilVmToken.cpp
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
uint8_t CCilVm::getNextBytecode( void )
{
	assert( m_pCurrentInstruction + 1 - (uint8_t*)&m_CurrentILPool.ILPool[0] <= (int32_t)m_CurrentILPool.ILPool.size() );
	uint8_t bIL = *m_pCurrentInstruction;
	m_pCurrentInstruction ++;
	return bIL;
};

RID CCilVm::getNextToken( void )
{
	assert( m_pCurrentInstruction + sizeof( uint32_t ) - (uint8_t*)&m_CurrentILPool.ILPool[0] <= (int32_t)m_CurrentILPool.ILPool.size() );
	uint32_t bToken = *(uint32_t *)m_pCurrentInstruction;
	m_pCurrentInstruction += sizeof( uint32_t );
	return bToken;
};

int32_t CCilVm::getNextInt32( void )
{
	assert( m_pCurrentInstruction + sizeof( int32_t ) - (uint8_t*)&m_CurrentILPool.ILPool[0] <= (int32_t)m_CurrentILPool.ILPool.size() );
	int32_t bToken = *(int32_t *)m_pCurrentInstruction;
	m_pCurrentInstruction += sizeof( int32_t );
	return bToken;
};

int64_t CCilVm::getNextInt64( void )
{
	assert( m_pCurrentInstruction + sizeof( int64_t ) - (uint8_t*)&m_CurrentILPool.ILPool[0] <= (int32_t)m_CurrentILPool.ILPool.size() );
	int64_t bToken = *(int64_t *)m_pCurrentInstruction;
	m_pCurrentInstruction += sizeof( int64_t );
	return bToken;
};

int8_t CCilVm::getNextInt8( void )
{
	assert( m_pCurrentInstruction + sizeof( int8_t ) - (uint8_t*)&m_CurrentILPool.ILPool[0] <= (int32_t)m_CurrentILPool.ILPool.size() );
	int8_t bToken = *(int8_t *)m_pCurrentInstruction;
	m_pCurrentInstruction += sizeof( int8_t );
	return bToken;
};

uint8_t CCilVm::getNextUint8( void )
{
	assert( m_pCurrentInstruction + sizeof( uint8_t ) - (uint8_t*)&m_CurrentILPool.ILPool[0] <= (int32_t)m_CurrentILPool.ILPool.size() );
	uint8_t bToken = *(uint8_t *)m_pCurrentInstruction;
	m_pCurrentInstruction += sizeof( uint8_t );
	return bToken;
};


float CCilVm::getNextFloat( void )
{
	assert( m_pCurrentInstruction + sizeof( float ) - (uint8_t*)&m_CurrentILPool.ILPool[0] <= (int32_t)m_CurrentILPool.ILPool.size() );
	float bToken = *(float *)m_pCurrentInstruction;
	m_pCurrentInstruction += sizeof( float );
	return bToken;
};

double CCilVm::getNextDouble( void )
{
	assert( m_pCurrentInstruction + sizeof( double ) - (uint8_t*)&m_CurrentILPool.ILPool[0] <= (int32_t)m_CurrentILPool.ILPool.size() );
	double bToken = *(double *)m_pCurrentInstruction;
	m_pCurrentInstruction += sizeof( double );
	return bToken;
};


} //namespace CRI