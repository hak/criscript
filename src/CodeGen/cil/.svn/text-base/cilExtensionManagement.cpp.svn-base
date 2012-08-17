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
 * Module   : Check extension
 * File     : cilExtensionManagement.cpp
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
bool CCilCodeGen::checkExtension( EXTENSION_FLAGS flags )
{
	bool bEnabled = true;
	switch( flags )
	{
	case EXTENSION_CRISCRIPT:
		break;
	case EXTENSION_JSCRIPT:
#ifndef ENABLE_EXTENSION_JSCRIPT
		bEnabled = false;
#endif
		break;		
	case EXTENSION_ES4DRAFT:
#ifndef ENABLE_EXTENSION_ES4
		bEnabled = false;
#endif
		break;
	default:
		break;
	}
	return bEnabled;
}

} //namespace CRI