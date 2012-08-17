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
 * File     : cilIdentifier.cpp
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
 * Create constant node leaf
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::identifier( const wchar_t* string,
									 const uint32_t iFileId,
									 const uint32_t iLineNumber,
									 const uint32_t iColumn )
{
	assert( string != NULL );

	CCgNodeData* object = new CCgNodeData;
	assert( object != NULL );
	object->NodeType = CG_NODE_IDENTIFIER;
	object->pIdentifier = new wstring( string );
	object->iFileId = iFileId;
	object->iLineNumber = iLineNumber;
	object->iColumn = iColumn;
	return object;
};


} //namespace CRI