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
 * Module   : cilBuiltinFunctionObjec
 * File     : Defines built in function object
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
METHOD_INITIALIZATION_PARAM CFunctionObject::initParams[] = {
	{
		NAME_BUILTIN_FUNCTION_OBJECT_CTOR,
		METHOD_NATIVE,
		(PMETHOD)&CFunctionObject::ctor,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_FUNCTION_OBJECT_CALL,
		METHOD_NATIVE,
		(PMETHOD)&CFunctionObject::call,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_FUNCTION_OBJECT_CTORWITHRID,
		METHOD_NATIVE,
		(PMETHOD)&CFunctionObject::CreateFunctionObject,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_FUNCTION_OBJECT_APPLY,
		METHOD_NATIVE,
		(PMETHOD)&CFunctionObject::apply,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
};

//Initialized when VM starts
RID CFunctionObject::m_ridObjectName;

/***************************************************************************
 *      Initialize
 ***************************************************************************/
bool CFunctionObject::InitializeCodeGen( ICodeGen& CodeGen )
{
	bool bReturn = true;

	RID ridObject = CodeGen.registerBuiltinObject( NAME_BUILTIN_FUNCTION_OBJECT );
	if( ridObject == RID_NOTDEFINED )
		return bReturn;

	//Initialize Global object ref
	RID rid = CodeGen.getMetaData().generateStaticFieldRID( OPERAND_OBJECTREF,
															OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
															| OPERAND_FLAG_DONTENUM
															| OPERAND_FLAG_READONLY ) );
	CodeGen.generateSymbolInformation( rid, NAME_BUILTIN_FUNCTION_OBJECT, CG_SYMBOL_FLAG_READONLY, ridObject );

	//---------------------------------------------------------------------------
	// Initialize System functions
	//---------------------------------------------------------------------------
	for( int32_t i = 0; i<sizeof( initParams ) / sizeof( initParams[ 0 ] ); ++i )
	{
		//-----------------------------------------------------------------------
		//Register methods
		//-----------------------------------------------------------------------
		RID ridMethod = CodeGen.registerBuiltinMethod(
									wstring( initParams[ i ].pStrSymbol ),
									initParams[ i ].methodType,
									initParams[ i ].ridParam
									);
		if( ridMethod == RID_NOTDEFINED )
			return bReturn;
	}

	return bReturn;
}

/***************************************************************************
 *      Create a prototype object
 ***************************************************************************/
bool CFunctionObject::InitializeVm( CCilVm& vm )
{
	m_ridObjectName = vm.getMetaData().getObjectNameRID( vm.getFunctionObjectRID() );
	assert( m_ridObjectName != RID_NOTDEFINED );

	RID rid = vm.createPrototypeObject( vm.getFunctionObjectRID(),
										m_ridObjectName,
										vm.getObjectRID() );
	assert( rid == vm.getFunctionObjectRID() );

	//---------------------------------------------------------------------------
	//Bind global object
	//---------------------------------------------------------------------------
	CVmObject* pObj = vm.createObject( vm.getFunctionObjectRID() );
	CVariable obj( pObj, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
									| OPERAND_FLAG_DONTENUM
									| OPERAND_FLAG_READONLY ) );

	//Set true as an object value..
	CVariable var( true );
	pObj->setValue( var );

	CG_SYMBOL_INFORMATION info = vm.querySymbolInformation( NAME_BUILTIN_FUNCTION_OBJECT );
	vm.setStaticField( info.rid, obj, true );	//Force over write it


	//Add properties of prototype object here
	//---------------------------------------------------------------------------
	// Bind System functions
	//---------------------------------------------------------------------------
	for( int32_t i = 0; i<sizeof( initParams ) / sizeof( initParams[ 0 ] ); ++i )
	{
		int32_t iParams;
		iParams = (int32_t)RidFromToken( initParams[ i ].ridParam );
		if( isBuiltinRid( initParams[ i ].ridParam ) )
		{
			iParams = RidFromToken( -iParams ) - 1;
		}

		//-----------------------------------------------------------------------
		//Register methods
		//-----------------------------------------------------------------------
		bool b = vm.bindBuiltinMethod(
									wstring( initParams[ i ].pStrSymbol ),
									initParams[ i ].pMethod,
									iParams
									);
		if( !b )
			return false;
	}

	return true;
}

/***************************************************************************
 *      ctor
 ***************************************************************************/
void CFunctionObject::ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );
	CVariable& thisobj = *pArguments;
	assert( OperandType(thisobj.iOperandType) == OPERAND_OBJECTREF );

	thisobj.refObject->setRID( pVm->getFunctionObjectRID() );

	//Set prototype: String prototype object
	thisobj.refObject->setPrototypeObject( 
		&pVm->getPrototypeObject( pVm->getFunctionObjectRID() ) );

	return;
}

/***************************************************************************
 *      Create function object method
 ***************************************************************************/
void CFunctionObject::CreateFunctionObject( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 3 );

	//Skip this pointer
	pArguments++;

	//Argument: 1) destination static field RID
	//			2) Method RID
	RID ridDest = pArguments->iValue;
	pArguments++;
	RID ridMethod = pArguments->iValue;
	assert( TypeFromToken( ridDest ) == MDT_FIELDDEF );
	assert( TypeFromToken( ridMethod ) == MDT_METHODDEF );

	//Create function object and store it to static field
	CVmObject obj;
	pVm->getObjectPool().push_back( obj );
	
	CVmObject& newobject = pVm->getObjectPool().back();
	newobject.setRID( pVm->getFunctionObjectRID() );
	newobject.setObjectName( m_ridObjectName );
	newobject.setPrototypeObject( &pVm->getPrototypeObject( pVm->getFunctionObjectRID() ) );
	//newobject.setPrototypeObject( 
	//	&pVm->getPrototypeObject( pVm->getFunctionObjectRID() ),
	//					OPERAND_OBJECTREF,
	//					OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
	//								| OPERAND_FLAG_DONTENUM
	//								| OPERAND_FLAG_READONLY ) );


	newobject.setConstructorProperty( ridMethod );
	newobject.setCallProperty( ridMethod );

	CVariable var;
	var.setObjectRef( &newobject );

	//Store the ObjectRef to a static field
	pVm->setStaticField( ridDest,  var );

	pVm->pushEvalStack( var );

	return;
}

/***************************************************************************
 *      call method
 ***************************************************************************/
void CFunctionObject::call( CCilVm* const pVm,
							const int32_t iNumArguments, 
							CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );
	assert( OperandType( pArguments->iOperandType ) == OPERAND_OBJECTREF );

	CVariable varThis = *(pArguments+1);
	RID ridCall = pArguments->refObject->getCall();

	//Remove this pointer from the arg list
	//pVm->getArgumentList().erase( pArguments );

	for( int32_t i = 0; i < iNumArguments - 1; ++i )
	{
		*pArguments = *(pArguments + 1);
		pArguments++;
	}
	pVm->CallOperator( &varThis, ridCall, false );
	pVm->resetArgumentList();

	return;
}

/***************************************************************************
 *      apply method
 ***************************************************************************/
void CFunctionObject::apply( CCilVm* const pVm,
							const int32_t iNumArguments, 
							CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );

	assert( OperandType( pArguments->iOperandType ) == OPERAND_OBJECTREF );

	CVariable varThis = *pArguments;
	RID ridCall = varThis.refObject->getCall();

	if( iNumArguments >= 2 )
	{
		pArguments++;
		varThis = *pArguments;
	}
	if( iNumArguments >= 3 )
	{
		//Arguments are given as an array variable
		pArguments++;
		
		//Note that varArray is not a reference. Since argument list will be destructed before putting array entries
		CVariable varArray = *pArguments;
		assert( OperandType( varArray.iOperandType ) == OPERAND_OBJECTREF );

		//Dispose arglist
		pVm->disposeArgumentList();

		pVm->storeArgumentList( varThis ); 

		hash_map< wstring, CVariable >::iterator itStart = varArray.refObject->getPropertyMap().begin();
		hash_map< wstring, CVariable >::iterator itEnd = varArray.refObject->getPropertyMap().end();
		while( itStart != itEnd )
		{
			if( !(itStart->second.iOperandFlag & OPERAND_FLAG_DONTENUM ) )
			{
				//Move thes to the argument list
				pVm->storeArgumentList( itStart->second );
			}
			++itStart;
		}
	}

	pVm->CallOperator( &varThis, ridCall );
	pVm->resetArgumentList();
	return;
}

} //namespace CRI