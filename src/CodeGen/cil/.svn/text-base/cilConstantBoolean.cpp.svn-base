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
 * File     : cilConstantBoolean.cpp
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
 * Create constant node leaf
 ***************************************************************************/	
CCgNodeData* CCilCodeGen::booleanConstant( const bool value,
  										  	uint32_t iFileId,
											uint32_t iLineNumber,
											uint32_t iColumn )

{
	//TODO: need a duplication check
	CCgNodeData* object = new CCgNodeData;	//Need to be deleted later
	assert( object != NULL );
	object->NodeType = CG_NODE_BOOLEANLITERAL;
	object->iIndex = value;
	object->iFileId = iFileId;
	object->iLineNumber = iLineNumber;
	object->iColumn = iColumn;
	return object;
};


/***************************************************************************
 * emit CIL
 ***************************************************************************/	
void CCilCodeGen::emitLoadBoolean( const bool value)
{
	//Create boolean object and return a ref.
	if( value )
	{
		emit( CEE_LDSFLD, RID_CONSTANT_BOOL_TRUE );
	}
	else
	{
		emit( CEE_LDSFLD, RID_CONSTANT_BOOL_FALSE );
	}

	return;
}

} //namespace CRI