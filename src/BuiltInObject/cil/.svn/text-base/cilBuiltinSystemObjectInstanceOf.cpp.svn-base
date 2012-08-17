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
 * Module   : cilBuiltinSystemObjectInstanceOf.cpp
 * File     : Defines built in system object
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
 *      instanceOf
 ***************************************************************************/
void CSystemObject::instanceOf( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments == 3 );

	//Skip this pointer
	pArguments++;

	CVariable& varRhs = *pArguments++;
	CVariable& varLhs = *pArguments++;

	//TODO: need to throw "typeerror"
	if( OperandType( varLhs.iOperandType ) != OPERAND_OBJECTREF )
	{
		pVm->pushEvalStack( (int32_t)0 );
		return;
	}
	if( OperandType( varRhs.iOperandType ) != OPERAND_OBJECTREF )
	{
		pVm->pushEvalStack( (int32_t)0 );
		return;
	}

	int32_t iResult = 0;
	RID ridOriginalType = varRhs.refObject->getRID();
	RID ridOriginalCallType = varRhs.refObject->getCall();
	CVmObject* pObject =  varLhs.refObject;

	if( ridOriginalType == CCilVm::getFunctionObjectRID() )
	{
		while( pObject != NULL )
		{
			if( pObject->getCall() == ridOriginalCallType )
			{
				iResult = 1;
				break;
			}
			pObject = pObject->getPrototypeObject();
		}
	}
	else
	{
		while( pObject != NULL )
		{
			if( pObject->getRID() == ridOriginalType )
			{
				iResult = 1;
				break;
			}
			pObject = pObject->getPrototypeObject();
		}
	}

	pVm->pushEvalStack( (int32_t)iResult );
	return;
}

/***************************************************************************
 *      In
 ***************************************************************************/
void CSystemObject::In( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments == 3 );

	//Skip this pointer
	pArguments++;

	CVariable& varRhs = *pArguments++;
	CVariable& varLhs = *pArguments++;

	//TODO: need to throw "typeerror"
	assert(  OperandType( varRhs.iOperandType ) == OPERAND_OBJECTREF );

	int32_t iResult = 0;
	CVmObject* pObject =  varRhs.refObject;
	varLhs.convertToString( pVm );
	wstring* pString = varLhs.pString;
	
	CVariable* pVar = pObject->lookupProperty( *pString );
	if( pVar != NULL )
	{
		iResult = 1;
	}
	pVm->pushEvalStack( (int32_t)iResult );

	return;
}

} //namespace CRI