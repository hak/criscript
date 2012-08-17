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
 * Module   : <Module descriptions here>
 * File     : <ModuleName here>
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
 * set target endian setting
 ***************************************************************************/
void CCilCodeGen::setTargetEndianness( TARGET_ENDIANNESS targetEndian )
{
	m_targetEndianness = targetEndian;
}


/***************************************************************************
 * retrieve target endian setting
 ***************************************************************************/
TARGET_ENDIANNESS CCilCodeGen::getTargetEndianness()
{
	return m_targetEndianness;
}

/***************************************************************************
 * retrieve current platform's endian setting
 ***************************************************************************/
TARGET_ENDIANNESS CCilCodeGen::getCurrentEndianness()
{
	return m_currentEndianness;
}

/***************************************************************************
 * check current platform's endian setting
 ***************************************************************************/
void CCilCodeGen::checkCurrentEndianness( void )
{
	uint32_t iTest = 0xff000000;
	if( *(uint8_t*)&iTest == 0xff )
	{
		m_currentEndianness = TARGET_BIGENDIAN;
	}
	else
	{
		m_currentEndianness = TARGET_LITTLEENDIAN;
	}

	return;
}

} //namespace CRI