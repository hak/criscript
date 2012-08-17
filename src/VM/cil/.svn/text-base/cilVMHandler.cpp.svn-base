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
 * File     : cilVMHandler.cpp
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
VM_HANDLER_CALLBACK CCilVm::registerHandler( VM_HANDLER_TYPE handlertype, VM_HANDLER_CALLBACK pCallBack )
{
	if( handlertype >= VM_HANDLER_MAX )
	{
		Debug_Warning( "Illegal Handler Type" );
		return NULL;
	}
	VM_HANDLER_CALLBACK pCurrentCallBack = m_Handler[ handlertype ];
	m_Handler[ handlertype ] = pCallBack;
	return pCurrentCallBack;

}


} //namespace CRI