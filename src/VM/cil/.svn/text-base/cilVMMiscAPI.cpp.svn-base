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
 * File     : cilVMMiscAPI.cpp
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
 * Retrieve VM exit code
 ***************************************************************************/	
int32_t CCilVm::getExitCode()
{
	if( m_status != VM_EXECUTE_DONE )
	{
		return -1;
	}
	
	VMOBJECTREF objSystem =&getPrototypeObject( getSystemObjectRID() );
	CVariable* pvarEnvironment = objSystem->lookupProperty( NAME_BUILTIN_SYSTEM_PROPERTY_ENVIRONMENT);
	CVariable* pvarExitCode = pvarEnvironment->refObject->lookupProperty( NAME_BUILTIN_SYSTEM_PROPERTY_EXITCODE );
	int32_t iRet = pvarExitCode->toInt();
	return iRet;
}


/***************************************************************************
 * VM Helper API body
 ***************************************************************************/	
void MakeWrapperStdVoidP0( CCilVm* const pVm,
			const int32_t iNumArguments,
			vector<CVariable>::iterator pArguments,
			PMETHOD pMethodBody )
{
	((void (*)( void ) )pMethodBody)();
	pVm->pushEvalStackUndefined();

	return;
}

//----------------------
//Void return version
//----------------------
void MakeWrapperMtdVoidP0( CCilVm* const pVm,
			const int32_t iNumArguments,
			vector<CVariable>::iterator pArguments,
			PMETHOD pMethodBody )
{
	//Keep This pointer
	CVariable* pVar = &*pArguments ++;

	((void (*)( CVariable* ) )pMethodBody)( pVar );
	pVm->pushEvalStackUndefined();

	return;
}

} //namespace CRI