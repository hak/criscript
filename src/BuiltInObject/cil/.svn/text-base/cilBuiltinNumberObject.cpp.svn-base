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
 * Module   : cilBuiltinNumberObject
 * File     : Defines built in number object
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include <float.h>
#include "criScript.h"
#include "cilCodeGen.h"
namespace cri {

/***************************************************************************
 *      Variables
 ***************************************************************************/
METHOD_INITIALIZATION_PARAM CNumberObject::initParams[] = {
	{
		NAME_BUILTIN_NUMBER_OBJECT_CTOR,
		METHOD_NATIVE,
		(PMETHOD)&CNumberObject::ctor,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_NUMBER_OBJECT_CALL,
		METHOD_NATIVE,
		(PMETHOD)&CNumberObject::call,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_NUMBER_OBJECT_TOSTRING,
		METHOD_NATIVE,
		(PMETHOD)&CNumberObject::toString,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_NUMBER_OBJECT_TOLOCALESTRING,
		METHOD_NATIVE,
		(PMETHOD)&CNumberObject::toLocaleString,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_NUMBER_OBJECT_VALUEOF,
		METHOD_NATIVE,
		(PMETHOD)&CNumberObject::valueOf,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},

};

PROPERTY_INITIALIZATION_PARAM CNumberObject::initPropertyParams[] = {
	//Note, don't change the order of the structrue since
	//it's initializing the entry later
	{
		NAME_BUILTIN_NUMBER_MAX_VALUE,
		OPERAND_TYPE( OPERAND_INT ),
		OPERAND_FLAG( OPERAND_FLAG_READONLY | OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE),
		float( INT_MAX )
	},
	{
		NAME_BUILTIN_NUMBER_MIN_VALUE,
		OPERAND_TYPE( OPERAND_INT ),
		OPERAND_FLAG( OPERAND_FLAG_READONLY | OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE),
		int32_t( INT_MIN )
	},
	{
		NAME_BUILTIN_NUMBER_NAN,
		OPERAND_TYPE( OPERAND_NAN ),
		OPERAND_FLAG( OPERAND_FLAG_READONLY | OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE),
		0
	},
	{
		NAME_BUILTIN_NUMBER_NEGATIVE_INFINITY,
		OPERAND_TYPE( OPERAND_FLOAT ),
		OPERAND_FLAG( OPERAND_FLAG_READONLY | OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE),
		uint32_t( 0xff800000 )
	},
	{
		NAME_BUILTIN_NUMBER_POSITIVE_INFINITY,
		OPERAND_TYPE( OPERAND_FLOAT ),
		OPERAND_FLAG( OPERAND_FLAG_READONLY | OPERAND_FLAG_DONTENUM | OPERAND_FLAG_DONTDELETE),
		uint32_t( 0x7f800000 )
	},
};

//Static object
CNumberparser* CNumberObject::m_pNumberparser;
	
/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Initialize
 ***************************************************************************/
bool CNumberObject::InitializeCodeGen( ICodeGen& CodeGen )
{
	bool bReturn = true;

	RID ridObject = CodeGen.registerBuiltinObject( NAME_BUILTIN_NUMBER_OBJECT );
	if( ridObject == RID_NOTDEFINED )
		return bReturn;

	//Initialize Global object ref
	RID rid = CodeGen.getMetaData().generateStaticFieldRID( OPERAND_OBJECTREF,
															OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
															| OPERAND_FLAG_DONTENUM
															| OPERAND_FLAG_READONLY ) );
	CodeGen.generateSymbolInformation( rid, NAME_BUILTIN_NUMBER_OBJECT, CG_SYMBOL_FLAG_READONLY, ridObject );

	//---------------------------------------------------------------------------
	// Initialize Number.call/ctor property
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
bool CNumberObject::InitializeVm( CCilVm& vm )
{
	RID rid = vm.createPrototypeObject( vm.getNumberObjectRID(),
										vm.getMetaData().getObjectNameRID( vm.getNumberObjectRID() ),
										vm.getObjectRID() );

	assert( rid == vm.getNumberObjectRID() );

	//---------------------------------------------------------------------------
	//Bind global object
	//---------------------------------------------------------------------------
	CVmObject* pObj = vm.createObject( vm.getNumberObjectRID() );
	CVariable obj( pObj, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
									| OPERAND_FLAG_DONTENUM
									| OPERAND_FLAG_READONLY ) );

	//Set true as an object value..
	CVariable varTrue( true );
	pObj->setValue( varTrue );

	CG_SYMBOL_INFORMATION info = vm.querySymbolInformation( NAME_BUILTIN_NUMBER_OBJECT );
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

	//Work around for the VS behaibor
	initPropertyParams[ 0 ].iValue = INT_MAX;
	initPropertyParams[ 1 ].iValue = INT_MIN;

	initPropertyParams[ 3 ].uiValue = uint32_t( 0xff800000 );
	initPropertyParams[ 4 ].uiValue = uint32_t( 0x7f800000 );

	//Bind properties
	for( int32_t i = 0; i < ARRAYSIZE( initPropertyParams ); ++i )
	{
		CVariable* pvar;
		//-----------------------------------------------------------------------
		//Register methods
		//-----------------------------------------------------------------------
		switch( OperandType( initPropertyParams[ i ].operandType ) )
		{
		case OPERAND_INT:
			pvar = new CVariable(  initPropertyParams[ i ].iValue,
									initPropertyParams[ i ].operandFlag );
			break;
		case OPERAND_NAN:
			pvar = new CVariable(  initPropertyParams[ i ].operandFlag );
			break;
		case OPERAND_FLOAT:
			pvar = new CVariable(  initPropertyParams[ i ].fValue,
									initPropertyParams[ i ].operandFlag );
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

	m_pNumberparser->init();

	return true;
}

/***************************************************************************
 *      ctor
 ***************************************************************************/
void CNumberObject::ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );
	CVariable& thisobj = *pArguments;
	assert( OperandType(thisobj.iOperandType) == OPERAND_OBJECTREF );

	//Initialize RID
	thisobj.refObject->setRID( pVm->getNumberObjectRID() );

	//Set prototype: String prototype object
	thisobj.refObject->setPrototypeObject( 
		&pVm->getPrototypeObject( pVm->getNumberObjectRID() ) );

	if( iNumArguments > 1 )
	{
		pArguments ++;
		CVariable var;
		switch( OperandType( pArguments->iOperandType ) )
		{
		case OPERAND_INT:
		case OPERAND_UNSIGNEDINT:
		case OPERAND_INT64:
		case OPERAND_UNSIGNEDINT64:
		case OPERAND_FLOAT:
		case OPERAND_DOUBLE:
		case OPERAND_NAN:
			thisobj.refObject->setValue( *pArguments );
			break;
		case OPERAND_UNDEFINED:
			var.iOperandType = OPERAND_NAN;
			thisobj.refObject->setValue( var );
			break;
		case OPERAND_NULL:
			var.iOperandType = OPERAND_INT;
			var.iValue = 0;
			thisobj.refObject->setValue( var );
			break;
		case OPERAND_BOOLEAN:
			var.iOperandType = OPERAND_INT;
			var.iValue = (pArguments->bValue == true);
			thisobj.refObject->setValue( var );
			break;
		case OPERAND_STRING:
			parseNumber( var, pArguments->pString );
			thisobj.refObject->setValue( var );
			break;
		case OPERAND_OBJECTREF:
			{
			OPERAND_TYPE type = pArguments->getPrimitiveType( PRIMITIVETYPE_NUMBER );
			switch( type )
			{
			case OPERAND_OBJECTREF:
			case OPERAND_UNDEFINED:
				var.iOperandType = OPERAND_NAN;
				thisobj.refObject->setValue( var );
				break;
			case OPERAND_STRING:
				parseNumber( var, pArguments->refObject->getValue().pString );
				thisobj.refObject->setValue( var );
				break;
			case OPERAND_NULL:
				var.iOperandType = OPERAND_INT;
				var.iValue = 0;
				thisobj.refObject->setValue( var );
				break;
			case OPERAND_BOOLEAN:
				var.iOperandType = OPERAND_INT;
				var.iValue = (pArguments->bValue == true);
				thisobj.refObject->setValue( var );
				break;
			case OPERAND_INT:
			case OPERAND_UNSIGNEDINT:
			case OPERAND_INT64:
			case OPERAND_UNSIGNEDINT64:
			case OPERAND_FLOAT:
			case OPERAND_DOUBLE:
			case OPERAND_NAN:
				thisobj.refObject->setValue( pArguments->refObject->getValue() );
				break;
			default:
				Debug_Fatal( "Not implemented yet" );
			}
			break;
			}
		default:
			Debug_Fatal( "Not implemented yet" );
			break;
		}
	}
	else
	{
		CVariable varInt( 0 );
		thisobj.refObject->setValue( varInt );
	}
	return;
}

/***************************************************************************
 *      call method
 ***************************************************************************/
void CNumberObject::call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );
	CVariable varRet( 0 );
	pVm->pushEvalStackVariable( varRet );
	if( iNumArguments > 1 )
	{
		pArguments ++;
		CVariable& var = *pArguments;
		switch( OperandType( var.iOperandType ) )
		{
		case OPERAND_INT:
		case OPERAND_UNSIGNEDINT:
		case OPERAND_INT64:
		case OPERAND_UNSIGNEDINT64:
		case OPERAND_FLOAT:
		case OPERAND_DOUBLE:
		case OPERAND_NAN:
			var.moveTo( pVm->getEvalStackFirstEntry() );
			break;
		case OPERAND_UNDEFINED:
			var.iOperandType = OPERAND_NAN;
			var.moveTo( pVm->getEvalStackFirstEntry() );
			break;
		case OPERAND_NULL:
			var.iOperandType = OPERAND_INT;
			var.iValue = 0;
			var.moveTo( pVm->getEvalStackFirstEntry() );
			break;
		case OPERAND_BOOLEAN:
			var.iOperandType = OPERAND_INT;
			var.iValue = (pArguments->bValue == true);
			var.moveTo( pVm->getEvalStackFirstEntry() );
			break;
		case OPERAND_STRING:
			parseNumber( var, pArguments->pString );
			var.moveTo( pVm->getEvalStackFirstEntry() );
			break;
		case OPERAND_OBJECTREF:
			{
			OPERAND_TYPE type = var.getPrimitiveType( PRIMITIVETYPE_NUMBER );
			switch( type )
			{
			case OPERAND_OBJECTREF:
			case OPERAND_UNDEFINED:
				var.iOperandType = OPERAND_NAN;
				var.moveTo( pVm->getEvalStackFirstEntry() );
				break;
			case OPERAND_STRING:
				parseNumber( var, pArguments->refObject->getValue().pString );
				var.moveTo( pVm->getEvalStackFirstEntry() );
				break;
			case OPERAND_NULL:
				var.iOperandType = OPERAND_INT;
				var.iValue = 0;
				var.moveTo( pVm->getEvalStackFirstEntry() );
				break;
			case OPERAND_BOOLEAN:
				var.iOperandType = OPERAND_INT;
				var.iValue = (pArguments->bValue == true);
				var.moveTo( pVm->getEvalStackFirstEntry() );
				break;
			case OPERAND_INT:
			case OPERAND_UNSIGNEDINT:
			case OPERAND_INT64:
			case OPERAND_UNSIGNEDINT64:
			case OPERAND_FLOAT:
			case OPERAND_DOUBLE:
			case OPERAND_NAN:
				pArguments->refObject->getValue().moveTo( pVm->getEvalStackFirstEntry() );
				break;
			default:
				Debug_Fatal( "Not implemented yet" );
			}
			break;
			}
		default:
			Debug_Fatal( "Not implemented yet" );
			break;
		}
	}
	return;
}

/***************************************************************************
 *      Helper for ctor
 ***************************************************************************/
bool CNumberObject::parseNumber( CVariable& var, const wstring* const pstr )
{
	bool bTokenRetrieved = false;
	bool bError = false;
	int32_t iTokenType = 0;
	int64_t iValue;
	double	dValue;
	bool bNegativeSign = false;

	wstring s = *pstr;
	
	s.append( L" " );
	//Remove proceeding ' '
	while( s.length() && s.at( 0 ) == L' ' )
	{
		s = s.substr( 1, s.length() - 1 );
	}
	//Remove proceeding '+'
	while( s.length() && s.at( 0 ) == L'+' )
	{
		s = s.substr( 1, s.length() - 1 );
	}
	if( s.length() && s.at( 0 ) == L'-' )
	{
		bNegativeSign = true;
		s = s.substr( 1, s.length() - 1 );
	}

	if( !s.length() )
	{
		var.iOperandType = OPERAND_NAN;
		return bTokenRetrieved;
	}

	m_pNumberparser->setStream( &s );
	while( m_pNumberparser->process()
		&& m_pNumberparser->getToken() != TOKEN_EOF
		&& !bError )
	{
		if( bTokenRetrieved )
		{
			//Just one token is allowed
			bTokenRetrieved = false;
			bError = true;
			break;
		}
		bTokenRetrieved = true;
		iTokenType = m_pNumberparser->getToken();
		switch( iTokenType )
		{
		case LITERAL_INTEGER:
			iValue = m_pNumberparser->getIntegerTokenValue() * (bNegativeSign ? -1 : 1 );
			break;
		case LITERAL_DECIMAL:
			dValue = m_pNumberparser->getDecimalTokenValue() * (bNegativeSign ? -1.0 : 1.0 );
			break;
		default:
			//Illegal token string
			bTokenRetrieved = false;
			bError = true;
			break;
		}
	}

	if( bTokenRetrieved )
	{
		if( iTokenType == LITERAL_INTEGER )
		{
			if( iValue < INT_MIN || iValue > INT_MAX )
			{
				var.iOperandType = OPERAND_INT64;
				var.i64Value = iValue;
			}
			else
			{
				var.iOperandType = OPERAND_INT;
				var.iValue = (int32_t)iValue;
			}
		}
		else
		{
			if( dValue > FLT_MAX || dValue < FLT_MIN )
			{
				var.iOperandType = OPERAND_DOUBLE;
				var.dValue = dValue;
			}
			else
			{
				var.iOperandType = OPERAND_FLOAT;
				var.fValue = (float)dValue;
			}
		}
	}
	else
	{
		var.iOperandType = OPERAND_NAN;
	}
	return bTokenRetrieved;
}

/***************************************************************************
 *      toString property
 ***************************************************************************/
void CNumberObject::toString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 1 );

	if( iNumArguments < 2 )
	{
		//Without radix
		switch( OperandType( pArguments->iOperandType ) )
		{
			case OPERAND_INT:
			case OPERAND_UNSIGNEDINT:
			case OPERAND_INT64:
			case OPERAND_UNSIGNEDINT64:
			case OPERAND_FLOAT:
			case OPERAND_DOUBLE:
			case OPERAND_NAN:
				pArguments->convertToString( pVm );
				pVm->pushEvalStack( *pArguments );
				break;
			case OPERAND_OBJECTREF:
				if( pArguments->refObject->getRID() == pVm->getNumberObjectRID() )
				{
					CVmObject* pObj = pArguments->refObject;
					pVm->pushEvalStackStringPointer( pObj->getValue().toString( pVm ) );
					break;
				}
				//else, throw type error
			default:
				//Throw TypeError
				pVm->throwException( &wstring( ERRORSTRING_TYPEERROR_NUMBER_TOSTRING ),
									&wstring( NAME_BUILTIN_ERROR_OBJECT ),
									ERROR_TYPEERROR_NUMBER_TOSTRING );
		}
	}
	else
	{
		int32_t iRadix = (pArguments+1)->toInt();
		if( !iRadix ) iRadix = 10;	//to be fail safe
		
		switch( OperandType( pArguments->iOperandType ) )
		{
			case OPERAND_INT:
			case OPERAND_UNSIGNEDINT:
			case OPERAND_INT64:
			case OPERAND_UNSIGNEDINT64:
			case OPERAND_FLOAT:
			case OPERAND_DOUBLE:
			case OPERAND_NAN:
				pArguments->convertToString( pVm, iRadix );
				pVm->pushEvalStack( *pArguments );
				break;
			case OPERAND_OBJECTREF:
				if( pArguments->refObject->getRID() == pVm->getNumberObjectRID() )
				{
					CVmObject* pObj = pArguments->refObject;
					pVm->pushEvalStackStringPointer( pObj->getValue().toString( pVm, iRadix ) );
					break;
				}
				//else, throw type error
			default:
				//Throw TypeError
				pVm->throwException( &wstring( ERRORSTRING_TYPEERROR_NUMBER_TOSTRING ),
									&wstring( NAME_BUILTIN_ERROR_OBJECT ),
									ERROR_TYPEERROR_NUMBER_TOSTRING );
		}
	}
	return;
}

/***************************************************************************
 *      toLocaleString property
 ***************************************************************************/
void CNumberObject::toLocaleString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 1 );
	
	//Does not respect locale setting for now
	toString( pVm, iNumArguments, pArguments );
	return;
}

/***************************************************************************
 *      valueOf property
 ***************************************************************************/
void CNumberObject::valueOf( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	Debug_Fatal( "Not implemented yet" );
}


/***************************************************************************
 *      CNumberparser
 ***************************************************************************/

/***************************************************************************
 *      CNumberparser::init
 ***************************************************************************/
bool CNumberparser::init()
{
	m_bInitialized = true;
	return true;
}

} //namespace CRI