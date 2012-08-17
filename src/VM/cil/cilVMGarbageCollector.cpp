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
 * File     : cilVMGarbageCollector.cpp
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
void CCilVm::cleanObjectPool()
{
//TODO: This is a quick fix for GPF on XP.
//Need to be revisited and cleaned up later

	int32_t iDeletedObject = 0;
	size_t iSize = getObjectPool().size();
	list< CVmObject >::iterator itSt = getObjectPool().begin();
	list< CVmObject >::iterator itEnd = getObjectPool().end();
	while( itSt != itEnd )
	{
		{
			iDeletedObject++;

			hash_map< wstring, CVariable >::iterator itStart = itSt->getPropertyMap().begin();
			hash_map< wstring, CVariable >::iterator itEnd = itSt->getPropertyMap().end();
			while( itStart != itEnd )
			{
				itStart->second.iOperandType = OPERAND_SYSTEM_DELETION;
				itStart ++;
			}
			
			//Destruct everything
			itSt->getPropertyMap().clear();
			//itSt->GetProtoType().iOperandType = OPERAND_SYSTEM_DELETION;
			itSt->getParentVariable().iOperandType = OPERAND_SYSTEM_DELETION;
			itSt->getValue().iOperandType = OPERAND_SYSTEM_DELETION;

		}
		itSt++;
	}

}

	
/***************************************************************************
 *      Clear refence count ot Zero
 *		helper for kicking off the gc process
 ***************************************************************************/
void CCilVm::clearRefCount()
{
	list< CVmObject >::iterator itSt = getObjectPool().begin();
	list< CVmObject >::iterator itEnd = getObjectPool().end();
	while( itSt != itEnd )
	{
		itSt->clearRefCount();
		itSt ++;
	}
}


} //namespace CRI