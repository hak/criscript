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
 * Module   : cilBuiltinSystemObjectForIn.cpp
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
 *      For In helper
 ***************************************************************************/
void CSystemObject::ForIn( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments == 4 );

	//Skip this pointer
	pArguments++;

	CVariable& varObject = *pArguments++;
	CVariable& varIteratorVar = *pArguments++;
	CVariable& varIteratorVar2 = *pArguments++;

	//TODO: need to throw "typeerror"
	if( OperandType( varObject.iOperandType ) != OPERAND_OBJECTREF )
	{
		CVariable var;
		var.setString( (wstring)L"" );
		pVm->pushEvalStack( var );
		return;
	}
	assert( OperandType( varIteratorVar.iOperandType ) == OPERAND_INT );

	CVmObject* pObject;
	
	CVariable& varIterator = pVm->getStaticField( varIteratorVar.iValue );
	CVariable& varReferredObject = pVm->getStaticField( varIteratorVar2.iValue );


	hash_map< wstring, CVariable >::iterator itProperty;
	if( varIterator.iValue == 0 )
	{
		varReferredObject.setObjectRef( varObject.refObject );
		pObject =  varObject.refObject;
		
		itProperty = pObject->getPropertyMap().begin();
		varIterator.iOperandType = OPERAND_INT;
		hash_map< wstring, CVariable >::iterator* pit = 
				new hash_map< wstring, CVariable >::iterator;
		varIterator.pPointerInternal = pit;
	}
	else
	{
		pObject = varReferredObject.refObject;
		itProperty = *(hash_map< wstring, CVariable >::iterator*)varIterator.pPointerInternal;
	}

	while( itProperty != pObject->getPropertyMap().end() )
	{
		if( OPERAND_FLAG_DONTENUM & itProperty->second.iOperandFlag )
		{
			itProperty++;
		}
		else
			break;
	};

	//
	//Check the end of the enum iteration
	//
	if( itProperty != pObject->getPropertyMap().end() )
	{
		//There is an enummable property
		//Push string value to the stack (property name)
		CVariable var;
		var.setString( (wstring)itProperty->first );
		pVm->pushEvalStack( var );

		//Store iterator
		itProperty++;
		hash_map< wstring, CVariable >::iterator* pIt =
				(hash_map< wstring, CVariable >::iterator*)varIterator.pPointerInternal;
		*pIt = itProperty;
	}
	else
	{
		CVariable var;
		var.setString( (wstring)L"" );
		pVm->pushEvalStack( var );
	}

	return;
}

} //namespace CRI