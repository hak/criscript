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
 * File     : cilVMPrototypeObject.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
#include "cilVM.h"
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Create a prototype object
 ***************************************************************************/
RID CCilVm::createPrototypeObject( RID ridObject, RID ridObjectName, RID ridPrototype )
{
	assert( TypeFromToken( ridObject ) == MDT_TYPEDEF );
	assert( isBuiltinRid( ridObject ) );
	int32_t iIndex = IndexFromBuiltinRid( ridObject );

	CVmObject newobject;
	newobject.setRID( ridObject );
	newobject.setObjectName( ridObjectName );
	newobject.addRef();
	
	//Check if the object RID is registerred already.
	if( m_BuiltinPrototypeObjectTable[ iIndex ].getRID() != RID_NOTDEFINED )
		Debug_Fatal( "An object list with reserved RID already registerred." );

	m_BuiltinPrototypeObjectTable[ iIndex ] = newobject;

	//Finally add prototype entry
	//A object's prototype could be itself
	if( ridPrototype != NULL )
		m_BuiltinPrototypeObjectTable[ iIndex ].setPrototypeObject( &getPrototypeObject( ridPrototype ) );

	return ridObject;
}

/***************************************************************************
 *      Retrieve object reference
 ***************************************************************************/
CVmObject& CCilVm::getPrototypeObject( RID ridObject )
{
	assert( TypeFromToken( ridObject ) == MDT_TYPEDEF );
	assert( isBuiltinRid( ridObject ) );
	int32_t iIndex = IndexFromBuiltinRid( ridObject );
	return 	m_BuiltinPrototypeObjectTable[ iIndex ];

}

} //namespace CRI