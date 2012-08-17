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
 * Module   : cilBuiltinObjectObject.cpp
 * File     : Defines built in Object object
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
METHOD_INITIALIZATION_PARAM CObjectObject::initParams[] = {
	{
		NAME_BUILTIN_OBJECT_OBJECT_CTOR,
		METHOD_NATIVE,
		(PMETHOD)&CObjectObject::ctor,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_OBJECT_OBJECT_CALL,
		METHOD_NATIVE,
		(PMETHOD)&CObjectObject::call,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_OBJECT_OBJECT_TOSTRING,
		METHOD_NATIVE,
		(PMETHOD)&CObjectObject::toString,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_OBJECT_OBJECT_TOLOCALESTRING,
		METHOD_NATIVE,
		(PMETHOD)&CObjectObject::toString,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_OBJECT_OBJECT_VALUEOF,
		METHOD_NATIVE,
		(PMETHOD)&CObjectObject::valueOf,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_OBJECT_OBJECT_HASOWNPROPERTY,
		METHOD_NATIVE,
		(PMETHOD)&CObjectObject::hasOwnProperty,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_OBJECT_OBJECT_ISPROTOTYPEOF,
		METHOD_NATIVE,
		(PMETHOD)&CObjectObject::isPrototypeOf,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_OBJECT_OBJECT_PROPERTYISENUMERABLE,
		METHOD_NATIVE,
		(PMETHOD)&CObjectObject::propertyIsEnumerable,
		RID_BUILTINPARAM_DEFAULT_ONE
	},

};

PROPERTY_INITIALIZATION_PARAM CObjectObject::initPropertyParams[] = {
	{
		NAME_BUILTIN_OBJECT_OBJECT_ARGUMENTS,	//'Object.arguments'
		OPERAND_TYPE( OPERAND_SYSTEM_ACCESSOR ),
		OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ),
		NULL
	},
	{
		NAME_BUILTIN_OBJECT_OBJECT_CALLER,	//'Object.caller'
		OPERAND_TYPE( OPERAND_SYSTEM_ACCESSOR ),
		OPERAND_FLAG( OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE ),
		NULL
	},
};

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Initialize
 ***************************************************************************/
bool CObjectObject::InitializeCodeGen( ICodeGen& CodeGen )
{
	bool bReturn = true;
	RID ridObject = CodeGen.registerBuiltinObject( NAME_BUILTIN_OBJECT_OBJECT );
	if( ridObject == RID_NOTDEFINED )
		return bReturn;

	//Initialize Global object ref
	RID rid = CodeGen.getMetaData().generateStaticFieldRID( OPERAND_OBJECTREF,
															OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
															| OPERAND_FLAG_DONTENUM
															| OPERAND_FLAG_READONLY ) );
	CodeGen.generateSymbolInformation( rid, NAME_BUILTIN_OBJECT_OBJECT, CG_SYMBOL_FLAG_READONLY, ridObject );

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
bool CObjectObject::InitializeVm( CCilVm& vm )
{
	RID rid = vm.createPrototypeObject( vm.getObjectRID(),		//Object RID
										vm.getMetaData().getObjectNameRID( vm.getObjectRID() ),
										0 );
	assert( rid == vm.getObjectRID() );

	//---------------------------------------------------------------------------
	//Bind global object
	//---------------------------------------------------------------------------
	CVmObject* pObj = vm.createObject( vm.getObjectRID() );

	CVariable obj( pObj, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
									| OPERAND_FLAG_DONTENUM
									| OPERAND_FLAG_READONLY ) );

	//Set true as an object value..
	CVariable varBool( true );
	pObj->setValue( varBool );

	CG_SYMBOL_INFORMATION info = vm.querySymbolInformation( NAME_BUILTIN_OBJECT_OBJECT );
	vm.setStaticField( info.rid, obj, true );	//Force over write it

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

	//setProperty Accessor
	//Work around

	//Object.arguments
	initPropertyParams[ 0 ].pMethod = argumentAccessor;

	//Object.caller
	initPropertyParams[ 1 ].pMethod = callerAccessor;

	//Bind properties
	for( int32_t i = 0; i < ARRAYSIZE( initPropertyParams ); ++i )
	{
		CVariable* pvar;
		//-----------------------------------------------------------------------
		//Register methods
		//-----------------------------------------------------------------------
		switch( OperandType( initPropertyParams[ i ].operandType ) )
		{
		case OPERAND_FLOAT:
			pvar = new CVariable(  initPropertyParams[ i ].fValue,
									initPropertyParams[ i ].operandFlag );
			break;
		case OPERAND_SYSTEM_ACCESSOR:
			pvar = new CVariable();
			pvar->pPointerInternal = (void*)initPropertyParams[ i ].pMethod;
			pvar->iOperandFlag = initPropertyParams[ i ].operandFlag;
			pvar->iOperandType = OPERAND_SYSTEM_ACCESSOR;
			break;
		default:
			Debug_Fatal( "Not supported type" );
		}
		bool b = vm.bindBuiltinVariable( wstring( initPropertyParams[ i ].pStrSymbol ),
										pvar );
		if( !b )
			return false;
		delete pvar;
	}

	return true;
}

/***************************************************************************
 *      call method
 ***************************************************************************/
void CObjectObject::ctor( CCilVm* const pVm,
							const int32_t iNumArguments, 
							CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );

	//Initialize this object
	assert( OperandType( pArguments->iOperandType ) == OPERAND_OBJECTREF );
	CVmObject* pObj = pArguments->refObject;

	//Check initialization parameter
	if( iNumArguments >= 2 )
	{
		switch( OperandType( (pArguments+1)->iOperandType ) )
		{
		case OPERAND_BOOLEAN:
			CBooleanObject::ctor( pVm, iNumArguments, pArguments );
			break;
		case OPERAND_INT:
		case OPERAND_INT64:
		case OPERAND_UNSIGNEDINT:
		case OPERAND_UNSIGNEDINT64:
		case OPERAND_FLOAT:
		case OPERAND_DOUBLE:
			CNumberObject::ctor( pVm, iNumArguments, pArguments );
			break;
		case OPERAND_STRING:
			CStringObject::ctor( pVm, iNumArguments, pArguments );
			break;
		case OPERAND_UNDEFINED:
		case OPERAND_NULL:
		default:
			//Create new 'object' object
			pObj = pVm->createObject( pVm->getObjectRID() );
			pObj->addRef();
			break;
		case OPERAND_OBJECTREF:
			//Copy over
			*pObj = *(pArguments+1)->refObject;
			pObj->addRef();
			break;

		}
	}
	else
	{		
		//Set prototype object
		pArguments->refObject->setPrototypeObject( 
			&pVm->getPrototypeObject( pVm->getObjectRID() ) );
	}
	return;
}

/***************************************************************************
 *      call method
 ***************************************************************************/
void CObjectObject::call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	CVariable obj;

	CVmObject* pObj = pVm->createObject( pVm->getObjectRID() );
	obj.setObjectRef( pObj );

	//Push the ObjectRef to a eval stack
	pVm->pushEvalStackVariable( obj );

	vector< CVariable > vec;
	vec.push_back( pVm->getEvalStackFirstEntry() );
	for( int32_t i = 1; i < iNumArguments; ++i )
	{
		pArguments++;
		vec.push_back( *(pArguments) );
	}
	ctor( pVm, iNumArguments, &*vec.begin() );

	return;
}

/***************************************************************************
 *      toString method
 ***************************************************************************/
void CObjectObject::toString( CCilVm* const pVm,
							const int32_t iNumArguments, 
							CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );

	CVariable obj = *pArguments;
	wstring s;
	if( OperandType( pArguments->iOperandType ) != OPERAND_OBJECTREF
		|| obj.refObject->getPrototypeObject()->getRID() == CCilVm::getFunctionObjectRID() )
	{
		s = L"[object Object]";
	}
	else
	{
		RID ridObjectName = obj.refObject->getPrototypeObject()->getObjectName();
		s = L"[object ";
		s.append( pVm->getMetaData().getStringConstant( ridObjectName ) );
		s.append( L"]" );
	}

	pVm->pushEvalStackString( s.data() );

	return;
}

/***************************************************************************
 *      argumentAccessor method
 ***************************************************************************/
CVariable* CObjectObject::argumentAccessor(CCilVm* const pVm,
										   CVmObject* const pObject,
										   const wstring* const pName,
										   CVariable* const var,
										   const bool bSet )
{
	assert( pObject != NULL );
	if( bSet )
	{
		//Getter implementation
		//assert( var == NULL );
		return pVm->generateArgumentsProperty();
	}
	else
	{
		//Setter implementation

		//Do nothing

		return NULL;
	}
}

/***************************************************************************
 *      callerAccessor method
 ***************************************************************************/
CVariable* CObjectObject::callerAccessor(CCilVm* const pVm,
										   CVmObject* const pObject,
										   const wstring* const pName,
										   CVariable* const var,
										   const bool bSet )
{
	assert( pObject != NULL );

	if( bSet )
	{
		//Getter implementation

		//assert( var == NULL );
		return pVm->generateCallerProperty();
	}
	else
	{
		//Setter implementation

		//Do nothing

		return NULL;
	}
}

/***************************************************************************
 *      valueOf property
 ***************************************************************************/
void CObjectObject::valueOf( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	Debug_Fatal( "Not implemented yet" );
}

/***************************************************************************
 *      hasOwnProperty property
 ***************************************************************************/
void CObjectObject::hasOwnProperty( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );
	CVariable& obj = *pArguments++;
	CVmObject* refObject;

	bool b = false;

	if( iNumArguments >= 2 )
	{
		CVariable& varName =*pArguments;
		varName.convertToString( pVm );
		hash_map< wstring, CVariable >::iterator it;
		switch( obj.iOperandType )
		{
		case OPERAND_OBJECTREF:
			refObject = obj.refObject;
			break;
		case OPERAND_STRING:
			//Check if Object prototype has it
			refObject = &pVm->getPrototypeObject( CCilVm::getStringObjectRID() );
			break;
		case OPERAND_BOOLEAN:
			//Check if Object prototype has it
			refObject = &pVm->getPrototypeObject( CCilVm::getBooleanObjectRID() );
			break;
		case OPERAND_INT:
		case OPERAND_UNSIGNEDINT:
		case OPERAND_INT64:
		case OPERAND_UNSIGNEDINT64:
		case OPERAND_FLOAT:
		case OPERAND_DOUBLE:
		case OPERAND_NAN:
			//Check if Object prototype has it
			refObject = &pVm->getPrototypeObject( CCilVm::getNumberObjectRID() );
			break;
		default:
			//Check if Object prototype has it
			refObject = &pVm->getPrototypeObject( pVm->getObjectRID() );
			break;
		}
		it = refObject->getPropertyMap().find( *varName.pString );
		if( it != refObject->getPropertyMap().end() )
		{
			b = true;
		}
	}

	pVm->pushEvalStack( b );
	return;
}

/***************************************************************************
 *      isPrototypeOf property
 ***************************************************************************/
void CObjectObject::isPrototypeOf( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );
	CVariable& obj = *pArguments++;
	CVmObject* refObject;

	bool b = false;

	if( iNumArguments >= 2 )
	{
		switch( obj.iOperandType )
		{
		case OPERAND_OBJECTREF:
			refObject = obj.refObject;
			break;
		case OPERAND_STRING:
			//Check if Object prototype has it
			refObject = &pVm->getPrototypeObject( CCilVm::getStringObjectRID() );
			break;
		case OPERAND_BOOLEAN:
			//Check if Object prototype has it
			refObject = &pVm->getPrototypeObject( CCilVm::getBooleanObjectRID() );
			break;
		case OPERAND_INT:
		case OPERAND_UNSIGNEDINT:
		case OPERAND_INT64:
		case OPERAND_UNSIGNEDINT64:
		case OPERAND_FLOAT:
		case OPERAND_DOUBLE:
		case OPERAND_NAN:
			//Check if Object prototype has it
			refObject = &pVm->getPrototypeObject( CCilVm::getNumberObjectRID() );
			break;
		default:
			//Check if Object prototype has it
			refObject = &pVm->getPrototypeObject( pVm->getObjectRID() );
			break;
		}

		CVariable& varTarget =*pArguments;
		switch( varTarget.iOperandType )
		{
		case OPERAND_OBJECTREF:
			{
			RID targetRid = varTarget.refObject->getRID();
			RID rid = refObject->getRID();

			while( rid )
			{
				if( targetRid == rid )
				{
					b = true;
					break;
				}
				else
				{
					refObject = refObject->getPrototypeObject();
					if( refObject )
					{
						rid = refObject->getRID();
					}
				}
			}

			}
			break;
		default:
			break;
		}

	}

	pVm->pushEvalStack( b );
		
	return;
}

/***************************************************************************
 *      propertyIsEnumerable property
 ***************************************************************************/
void CObjectObject::propertyIsEnumerable( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );
	CVariable& obj = *pArguments++;
	CVmObject* refObject;

	bool b = false;

	if( iNumArguments >= 2 )
	{
		CVariable& varName =*pArguments;
		varName.convertToString( pVm );
		hash_map< wstring, CVariable >::iterator it;
		switch( obj.iOperandType )
		{
		case OPERAND_OBJECTREF:
			refObject = obj.refObject;
			break;
		case OPERAND_STRING:
			//Check if Object prototype has it
			refObject = &pVm->getPrototypeObject( CCilVm::getStringObjectRID() );
			break;
		case OPERAND_BOOLEAN:
			//Check if Object prototype has it
			refObject = &pVm->getPrototypeObject( CCilVm::getBooleanObjectRID() );
			break;
		case OPERAND_INT:
		case OPERAND_UNSIGNEDINT:
		case OPERAND_INT64:
		case OPERAND_UNSIGNEDINT64:
		case OPERAND_FLOAT:
		case OPERAND_DOUBLE:
		case OPERAND_NAN:
			//Check if Object prototype has it
			refObject = &pVm->getPrototypeObject( CCilVm::getNumberObjectRID() );
			break;
		default:
			//Check if Object prototype has it
			refObject = &pVm->getPrototypeObject( pVm->getObjectRID() );
			break;
		}
		it = refObject->getPropertyMap().find( *varName.pString );
		if( it != refObject->getPropertyMap().end() )
		{
			if( !(it->second.iOperandFlag & OPERAND_FLAG_DONTENUM) )
				b = true;
		}
	}

	pVm->pushEvalStack( b );
	return;
}


} //namespace CRI