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
 * Module   : cilBuiltinStringObject
 * File     : Defines built in string object
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

#ifndef BOOST_REGEX_NO_LIB
	#define BOOST_REGEX_NO_LIB (1)
#endif
#ifndef BOOST_NO_EXCEPTIONS
	#define BOOST_NO_EXCEPTIONS (1)
#endif
#include <boost/regex.hpp>

namespace cri {

/***************************************************************************
 *      Definitions
 ***************************************************************************/
struct REPLACE_PARAMS {
	int32_t	iIndex;
	wstring*	pstrMatch;
	CVmObject*	pObj;
};

/***************************************************************************
 *      Variables
 ***************************************************************************/
METHOD_INITIALIZATION_PARAM CStringObject::initParams[] = {
	{
		NAME_BUILTIN_STRING_OBJECT_CALL,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::call,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_STRING_OBJECT_CTOR,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::ctor,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_STRING_OBJECT_FROMCHARCODE,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::fromCharCode,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_STRING_OBJECT_CHARAT,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::charAt,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_STRING_OBJECT_CHARCODEAT,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::charCodeAt,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_STRING_OBJECT_CONCAT,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::concat,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_STRING_OBJECT_INDEXOF,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::indexOf,
		RID_BUILTINPARAM_DEFAULT_TWO
	},
	{
		NAME_BUILTIN_STRING_OBJECT_LASTINDEXOF,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::lastIndexOf,
		RID_BUILTINPARAM_DEFAULT_TWO
	},
	{
		NAME_BUILTIN_STRING_OBJECT_TOUPPERCASE,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::toUpperCase,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_STRING_OBJECT_TOLOWERCASE,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::toLowerCase,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_STRING_OBJECT_TOLOCALEUPPERCASE,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::toUpperCase,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_STRING_OBJECT_TOLOCALELOWERCASE,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::toLowerCase,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_STRING_OBJECT_SUBSTRING,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::substring,
		RID_BUILTINPARAM_DEFAULT_TWO
	},
	{
		NAME_BUILTIN_STRING_OBJECT_SUBSTRING2,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::substring,
		RID_BUILTINPARAM_DEFAULT_TWO
	},
	{
		NAME_BUILTIN_STRING_OBJECT_TOSTRING,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::toString,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_STRING_OBJECT_TOLOCALESTRING,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::toString,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_STRING_OBJECT_MATCH,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::match,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_STRING_OBJECT_SPLIT,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::split,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_STRING_OBJECT_REPLACE,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::replace,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_STRING_OBJECT_SEARCH,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::search,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_STRING_OBJECT_SLICE,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::slice,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_STRING_OBJECT_LOCALECOMPARE,
		METHOD_NATIVE,
		(PMETHOD)&CStringObject::localeCompare,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
};

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Initialize
 ***************************************************************************/
bool CStringObject::InitializeCodeGen( ICodeGen& CodeGen )
{
	bool bReturn = true;

	RID ridObject = CodeGen.registerBuiltinObject( NAME_BUILTIN_STRING_OBJECT );
	if( ridObject == RID_NOTDEFINED )
		return bReturn;

	//Initialize Global object ref
	RID rid = CodeGen.getMetaData().generateStaticFieldRID( OPERAND_OBJECTREF,
															OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
															| OPERAND_FLAG_DONTENUM
															| OPERAND_FLAG_READONLY ) );
	CodeGen.generateSymbolInformation( rid, NAME_BUILTIN_STRING_OBJECT, CG_SYMBOL_FLAG_READONLY, ridObject );

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
bool CStringObject::InitializeVm( CCilVm& vm )
{
	RID rid = vm.createPrototypeObject( vm.getStringObjectRID(),
										vm.getMetaData().getObjectNameRID( vm.getStringObjectRID() ),
										vm.getObjectRID() );

	assert( rid == vm.getStringObjectRID() );

	//setProperty Accessor
	vm.getPrototypeObject( vm.getStringObjectRID() ).setPropertyAccessor( STRING_INTERNAL_LENGTH,
										lengthAccessor );

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
									iParams	);
		if( !b )
			return false;
	}

	//---------------------------------------------------------------------------
	//Bind global object
	//---------------------------------------------------------------------------
	CVmObject* pObj = vm.createObject( vm.getStringObjectRID() );
	CVariable obj( pObj, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
									| OPERAND_FLAG_DONTENUM
									| OPERAND_FLAG_READONLY ) );

	//Set true as an object value..
	CVariable varBool( true );
	pObj->setValue( varBool );

	//Add string property
	//String.fromCharCode
	pObj->addProperty( NAME_BUILTIN_STRING_OBJECT_PROPERTY_FROMCHARCODE,
		*pObj->getPrototypeObject()->lookupProperty( NAME_BUILTIN_STRING_OBJECT_PROPERTY_FROMCHARCODE ) );

	CG_SYMBOL_INFORMATION info = vm.querySymbolInformation( NAME_BUILTIN_STRING_OBJECT );
	vm.setStaticField( info.rid, obj, true );	//Force overwriting
	
	return true;
}

/***************************************************************************
 *      ctor method
 ***************************************************************************/
void CStringObject::ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	CVariable obj = *pArguments;
	assert( obj.iOperandType == OPERAND_OBJECTREF );

	obj.refObject->setRID( pVm->getStringObjectRID() );

	CVmObject& prototype = pVm->getPrototypeObject( pVm->getStringObjectRID() );
	//Set prototype: String prototype object
	obj.refObject->setPrototypeObject( &prototype );
	
	if( iNumArguments > 1 )
	{
		pArguments ++;
		CVariable& var = *pArguments;
		var.convertToString( pVm );
		obj.refObject->setValue( var );
	}
	else
	{
		CVariable varString( L"" );
		obj.refObject->setValue( varString );
	}

	return;
}

/***************************************************************************
 *      call method
 ***************************************************************************/
void CStringObject::call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	if( iNumArguments >= 1 )
	{
		pArguments++;
		CVariable obj = *pArguments;
		obj.convertToString( pVm );
		
		pVm->pushEvalStackVariable( obj );
	}
	else
		pVm->pushEvalStackString( L"" );
	return;
}

/***************************************************************************
 *      fromCharCode property
 ***************************************************************************/
void CStringObject::fromCharCode( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	pArguments ++;
	CVariable var;
	pVm->pushEvalStackVariable( var );

	wstring *s = new wstring( L"" );
	pVm->getEvalStackFirstEntry().pString = s; 
	pVm->getEvalStackFirstEntry().iOperandType = OPERAND_STRING;

	for( int32_t i = 0; i < iNumArguments - 1; ++ i )
	{
		if( !pArguments->iValue )
			pArguments->iValue = 0xffff;
		wchar_t iChar = pArguments->toInt();
		s->append( (wchar_t*)&iChar, 1 );


		pArguments ++;
	}
	return;
}

/***************************************************************************
 *      charAt property
 ***************************************************************************/
void CStringObject::charAt( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 1 );

	wstring *s = new wstring( L"" );
	pVm->pushEvalStackStringPointer( s );

	CVariable& var = *pArguments;
	int32_t i = 0;

	//If no argument is supplied, return first char of the string
	if( iNumArguments >= 2 )
	{
		pArguments ++;
		i = pArguments->toInt();
	}

	wstring* pStr = var.pString;
	wchar_t wchar;
	if( i >= 0 && i < (int32_t)(pStr)->length() )
	{
		wchar = pStr->at( i );
		s->assign( &wchar, 1 );
	}

	return;
}

/***************************************************************************
 *      charCodeAt property
 ***************************************************************************/
void CStringObject::charCodeAt( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 1 );
	CVariable& var = *pArguments;
	int32_t i = 0;
	uint16_t iResult = 0;

	//If no argument is supplied, return first char of the string
	if( iNumArguments >= 2 )
	{
		pArguments ++;
		i = pArguments->toInt();
	}
	
	if( i >= 0 && i < (int32_t)var.pString->length() )
	{
		iResult = var.pString->data()[ i ];
	}

	pVm->pushEvalStack( (int32_t)iResult );

	return;
}

/***************************************************************************
 *      concat property
 ***************************************************************************/
void CStringObject::concat( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 1 );

	pVm->pushEvalStackVariable( *pArguments );

	pArguments ++;

	CVariable& var = pVm->getEvalStackFirstEntry();
	var.convertToString( pVm );
	
	for( int32_t i = 0; i < iNumArguments - 1; ++ i )
	{
		pArguments->convertToString( pVm );
		var.pString->append( *pArguments->pString );
		pArguments++;
	}
	return;
}

/***************************************************************************
 *      indexOf property
 ***************************************************************************/
void CStringObject::indexOf( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	if( iNumArguments == 1 )
	{
		pVm->pushEvalStack( (int32_t)-1 );
		return;
	}

	CVariable& varThis = *pArguments ++;
	//toString, this
	varThis.convertToString( pVm );

	CVariable& varSearchString = *pArguments ++;
	//toString, srch string
	varSearchString.convertToString( pVm );

	int32_t iIndex;
	int32_t iLength = (int32_t)varThis.pString->length();
	if( !iLength )
	{
		if( !varThis.pString->compare( *varSearchString.pString ) )
			pVm->pushEvalStack( (int32_t)0 );
		else
			pVm->pushEvalStack( (int32_t)-1);
		return;
	}

	if( iNumArguments >= 3 )
	{
		CVariable& varIndex = *pArguments;
		iIndex = varIndex.toInt();
		if( iIndex > iLength )
		{
			pVm->pushEvalStack( (int32_t)-1 );
			return;
		}
		else if( iIndex < 0 )
			iIndex = 0;
	}
	else
		iIndex = 0;


	size_t pos = varThis.pString->find( *varSearchString.pString, iIndex );

	pVm->pushEvalStack( (int32_t)pos );
	return;

}

/***************************************************************************
 *      lastIndexOf property
 ***************************************************************************/
void CStringObject::lastIndexOf( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	if( iNumArguments == 1 )
	{
		pVm->pushEvalStack( (int32_t)-1 );
		return;
	}

	CVariable& varThis = *pArguments ++;
	//toString, this
	varThis.convertToString( pVm );

	CVariable& varSearchString = *pArguments ++;
	varSearchString.convertToString( pVm );

	int32_t iIndex;
	int32_t iLength = (int32_t)varThis.pString->length();
	if( !iLength )
	{
		if( !varThis.pString->compare( *varSearchString.pString ) )
			pVm->pushEvalStack( (int32_t)0 );
		else
			pVm->pushEvalStack( (int32_t)-1);
		return;
	}
	if( iNumArguments >= 3 )
	{
		CVariable& varIndex = *pArguments;
		switch( OperandType( varIndex.iOperandType ) )
		{
		case OPERAND_UNDEFINED:
		case OPERAND_OBJECTREF:
			iIndex = iLength;
			break;
		case OPERAND_STRING:
			if( !varIndex.pString->length() )
			{
				iIndex = 0;
				break;
			}
			iIndex = varIndex.toInt();
			if( iIndex == 0 ) iIndex = iLength;
			break;
		default:
			iIndex = varIndex.toInt();
			if( iIndex <= 0 ) iIndex = 0;
			break;
		}
	}
	else
		iIndex = iLength;

	size_t pos = varThis.pString->rfind( *varSearchString.pString, iIndex );

	pVm->pushEvalStack( (int32_t)pos );
	return;

}

/***************************************************************************
 *      toUpperCase property
 ***************************************************************************/
void CStringObject::toUpperCase( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 1 );

	CVariable& varThis = *pArguments ++;
	//toString, this
	varThis.convertToString( pVm );

	pVm->pushEvalStackString( L"" );
	
	wstring* pDest = pVm->getEvalStackFirstEntry().pString;
	pDest->resize( varThis.pString->size() );
	
	std::transform( varThis.pString->begin(),
					varThis.pString->end(),
					pDest->begin(),
					(int(*)(int))std::toupper );
	return;
}

/***************************************************************************
 *      toLowerCase property
 ***************************************************************************/
void CStringObject::toLowerCase( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 1 );

	CVariable& varThis = *pArguments ++;
	//toString, this
	varThis.convertToString( pVm );

	pVm->pushEvalStackString( L"" );

	wstring* pDest = pVm->getEvalStackFirstEntry().pString;
	pDest->resize( varThis.pString->size() );
	
	std::transform( varThis.pString->begin(),
					varThis.pString->end(),
					pDest->begin(),
					(int(*)(int))std::tolower );
	return;
}

/***************************************************************************
 *      substring property
 ***************************************************************************/
void CStringObject::substring( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	CVariable& varThis = *pArguments ++;
	//toString, this
	varThis.convertToString( pVm );

	int32_t iStart = 0;
	int32_t iEnd = (int32_t)varThis.pString->length();
	if( iNumArguments >= 2 )
	{
		CVariable& varStart = *pArguments ++;
		iStart = varStart.toInt();
	}
	if( iNumArguments >= 3 )
	{
		CVariable& varEnd = *pArguments ++;
		switch( OperandType( varEnd.iOperandType ) )
		{
		case OPERAND_UNDEFINED:
			break;
		default:
			iEnd = min( iEnd, varEnd.toInt() );
			break;
		}
	}

	if( iStart < 0 ) iStart = 0;
	if( iEnd < 0 ) iEnd = 0;
	if( iStart > iEnd )
	{
		//Swap
		int32_t iTmp = iStart;
		iStart = iEnd;
		iEnd = iTmp;
	}

	//if( iEnd > (int32_t)varThis.pString->length() )
	//{
	//	pVm->pushEvalStackString( L"" );
	//	return;
	//}

	wstring str = varThis.pString->substr( iStart, (iEnd - iStart) );
	pVm->pushEvalStackString( str.data() );
	
	return;
}

/***************************************************************************
 *      toString property
 ***************************************************************************/
void CStringObject::toString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 1 );

	assert( OperandType( pArguments->iOperandType ) == OPERAND_STRING
			|| OperandType( pArguments->iOperandType ) == OPERAND_OBJECTREF );

	switch( OperandType( pArguments->iOperandType ) )
	{
	case OPERAND_STRING:
		pVm->pushEvalStackString( pArguments->pString );
		break;
	case OPERAND_OBJECTREF:
		{
		CVmObject* pObj = pArguments->refObject;
		pVm->pushEvalStackString( pObj->getValue().toString( pVm ) );
		}
	break;
	default:
		Debug_Fatal( "Not supported obj type" );
	}
	return;
}

/***************************************************************************
 *      match property
 ***************************************************************************/
void CStringObject::match( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 2 );

	CVariable& varThis = *pArguments++;
	//toString, this
	varThis.convertToString( pVm );

	CVariable& varRegex = *pArguments++;

	switch( OperandType( varRegex.iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		{
		RID rid = varRegex.refObject->getRID();
		if( rid == pVm->getRegexObjectRID() )
			break;
		else if( rid == pVm->getStringObjectRID() )
		{
			//String object
			if( !varRegex.refObject->getValue().pString->compare( L"" ) )
			{
				varRegex.refObject->getValue().pString->append( L"^" );
			}
			CVariable varBool( false );
			varRegex.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_GLOBAL, varBool );
			varRegex.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_SOURCE, varRegex.refObject->getValue() );
			break;
		}
		pVm->pushEvalStackNull();
		return;
		}
	case OPERAND_STRING:
		{
		wstring* pVar = varRegex.pString;
		if( !pVar->compare( L"" ) )
		{
			pVar->append( L"^" );
		}
		CVmObject* pTmpObject = pVm->createObject( CCilVm::getObjectRID() );
		CVariable varPointer( pVar );
		CVariable varBool( false );
		pTmpObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_SOURCE, varPointer );
		pTmpObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_GLOBAL, varBool );
		pTmpObject->setValue( varPointer );		//Value will be moved

		varRegex.setObjectRef( pTmpObject );

		break;
		}
	case OPERAND_UNDEFINED:
		pVm->pushEvalStackString( L"" );
		return;
	default:
		pVm->pushEvalStackNull();
		return;
	}

	CVariable& varGlobal = *varRegex.refObject->lookupProperty( NAME_BUILTIN_REGEX_PROPERTY_GLOBAL );
	if( OperandType( varGlobal.iOperandType ) == OPERAND_BOOLEAN && varGlobal.bValue == true )
	{
		//GLOBAL mode

		//Create array object
		CVmObject* pObj = pVm->createObject( pVm->getArrayObjectRID() );
		CVariable var;
		var.setObjectRef( pObj );

		int32_t iResult = 0;
		int32_t iLast = -1;
		wchar_t strIndex[ 16 ];
		
		CVariable varInt( 0 );
		varRegex.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX, varInt );
		while( 1 )
		{
			CVariable varForExecute( varThis );
			CRegexObject::ExecMain( pVm, varRegex, varForExecute );
			if( pVm->getEvalStackFirstEntry().iOperandType == OPERAND_OBJECTREF )
			{

				swprintf( strIndex, 16, L"%d", iResult );
				CVariable& varResult = *pVm->getEvalStackFirstEntry().refObject->lookupProperty( L"0" );
				CVariable& varLastIndex = *varRegex.refObject->lookupProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX );
				if( varLastIndex.iValue != iLast )
				{
					CVariable varString( varResult.pString );
					var.refObject->setProperty( strIndex, varString );
					pVm->popEvalStack();
					iLast = varLastIndex.iValue;
				}
				else
				{
					pVm->popEvalStack();
					break;
				}
				++iResult;
			}
			else
			{
				pVm->popEvalStack();
				if( iLast == -1 )
				{
					//return NULL
					pVm->pushEvalStackNull();
					return;
				}
				break;
			}
		}

		//-2 means, 'prototype' & 'length'
		CVariable varLengthInit( (int32_t)0 );
		var.refObject->setProperty( STRING_INTERNAL_LENGTH, varLengthInit );
		CVariable varLength( (int32_t)var.refObject->getPropertyMap().size() - 2 );
		var.refObject->setProperty( STRING_INTERNAL_LENGTH, varLength );

		pVm->pushEvalStack( var );
	}
	else
	{
		CVariable var( varThis );
		CRegexObject::ExecMain( pVm, varRegex, var );
	}

	return;
}

/***************************************************************************
 *      split property
 ***************************************************************************/
void CStringObject::split( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );

	CVariable& varThis = *pArguments++;
	//toString, this
	varThis.convertToString( pVm );

	//Create arrayt object
	assert( pVm->getArrayObjectRID() != RID_NOTDEFINED );
	CVmObject* pObj = pVm->createObject( pVm->getArrayObjectRID() );
	CVariable var;
	var.setObjectRef( pObj );

	if( iNumArguments == 1 )
	{
		//Return the string if no separator is given
		CVariable varInt( (int32_t)1 );
		pObj->setProperty( STRING_INTERNAL_LENGTH, varInt );
		pObj->setProperty( L"0", varThis );
		pVm->pushEvalStackVariable( var );
		return;
	}

	CVariable& varSeparator = *pArguments++;
	uint32_t flags = 0;
	int32_t iLastIndex = 0;

	int32_t iLimit = INT_MAX;
	if( iNumArguments >= 3 )
	{
		switch( OperandType( pArguments->iOperandType ) )
		{
		case OPERAND_NULL:
		case OPERAND_UNDEFINED:
			iLimit = INT_MAX;
			break;
		default:
			iLimit = pArguments->toInt();
			if( iLimit == -1 )
				iLimit = INT_MAX;
			break;
		}
	}

	switch( OperandType( varSeparator.iOperandType ) )
	{
	case OPERAND_NULL:
	case OPERAND_UNDEFINED:
		{
			var.refObject->setProperty( L"0",
									varThis );
			CVariable varInt( 1 );
			var.refObject->setProperty( STRING_INTERNAL_LENGTH, varInt );
			pVm->pushEvalStack( var );
		}
		return;
	case OPERAND_OBJECTREF:
		if( varSeparator.refObject->getRID() == pVm->getRegexObjectRID() )
		{
			CVariable* pvar = varSeparator.refObject->lookupProperty( NAME_BUILTIN_REGEX_PROPERTY_SOURCE );
			if( !pvar->pString->length() )
			{
				varSeparator.refObject->release();
				varSeparator.iOperandType = OPERAND_STRING;
				varSeparator.pString = new wstring( L"" );
			} else if( !pvar->pString->compare( *varThis.pString ) )
			{
				varSeparator.refObject->release();
				varSeparator.iOperandType = OPERAND_STRING;
				varSeparator.pString = new wstring( *varThis.pString );
			}
			else
			{
				CVariable varBool( true );
				varSeparator.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_GLOBAL, varBool );
				break;
			}
		}
	default:
		varSeparator.convertToString( pVm );
		flags |= boost::regex_constants::literal;

		wstring* pVar = varSeparator.pString;
		if( !pVar->compare( L"" ) )
		{
			wchar_t str[ 12 ];
			int32_t i;
			for( i = 0; i < (int32_t)varThis.pString->length() && iLimit--; ++ i )
			{
				swprintf( str, 12, L"%d", i );
				wstring s( &varThis.pString->at( i ), 1 );
				CVariable varString( &s );
				var.refObject->setProperty( str, varString );
			}
			CVariable varInt( (int32_t)i ) ;
			var.refObject->setProperty( STRING_INTERNAL_LENGTH, varInt );
			pVm->pushEvalStack( var );
			return;
		}
		if( !pVar->compare( *varThis.pString ) )
		{
			CVariable varString( L"" );
			CVariable varInt( 2 );
			var.refObject->setProperty( L"0", varString );
			var.refObject->setProperty( L"1", varString );
			var.refObject->setProperty( STRING_INTERNAL_LENGTH, varInt );
			pVm->pushEvalStack( var );
			return;
		}

		CVariable varPointer( pVar );
		CVariable varPointer2( pVar );
		CVariable varTrue( true );
		CVmObject* pTmpObject = pVm->createObject( CCilVm::getObjectRID() );
		pTmpObject->setValue( varPointer );
		pTmpObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_GLOBAL, varTrue );
		pTmpObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_SOURCE, varPointer2 );

		varSeparator.setObjectRef( pTmpObject );

	}

	//GLOBAL mode

	int32_t iResult = 0;
	int32_t iLast = -1;
	wchar_t strIndex[ 16 ];
	
	CVariable varIntIndex( iLastIndex );
	varSeparator.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX, varIntIndex );
	
	while( iLimit > 0 )
	{
		CVariable varTmp( varThis );
		CRegexObject::ExecMain( pVm,
								varSeparator,
								varTmp,
								flags);
		if( pVm->getEvalStackFirstEntry().iOperandType == OPERAND_OBJECTREF )
		{

			swprintf( strIndex, 16, L"%d", iResult );
			CVariable& varResult = *pVm->getEvalStackFirstEntry().refObject->lookupProperty( L"0" );
			CVariable& varLastIndex = *varSeparator.refObject->lookupProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX );
			if( varLastIndex.iValue != iLast )
			{
				int32_t iLength = (int32_t)varResult.pString->length();
				int32_t iStart = max( iLast, 0 );
				int32_t iEnd = varLastIndex.iValue - iStart - iLength;
				wstring s = varThis.pString->substr( iStart, iEnd );
				CVariable varString( &s );
				var.refObject->setProperty( strIndex, varString );
				pVm->popEvalStack();
				iLast = varLastIndex.iValue; //- 1 + iLength;
			}
			else
			{
				pVm->popEvalStack();
				break;
			}
			++iResult;
		}
		else
		{
			pVm->popEvalStack();
			if( iLast == -1 )
			{
				var.refObject->setProperty( L"0",
											varThis );
				CVariable varInt( (int32_t)1 );
				var.refObject->setProperty( STRING_INTERNAL_LENGTH, varInt );
				pVm->pushEvalStack( var );
				return;
			}
			else if( iLast <= (int32_t)varThis.pString->length() )
			{
				swprintf( strIndex, 16, L"%d", iResult );
				wstring s = varThis.pString->substr( iLast, -1 );
				CVariable varString( &s );
				var.refObject->setProperty( strIndex, varString );
			}
			break;
		}
		iLimit --;
	}

	//-2 means, 'prototype' & 'length'
	CVariable varInt( (int32_t)0 );
	var.refObject->setProperty( STRING_INTERNAL_LENGTH, varInt );
	CVariable varInt2( (int32_t)(int32_t)var.refObject->getPropertyMap().size() - 2 );
	var.refObject->setProperty( STRING_INTERNAL_LENGTH, varInt2 );

	pVm->pushEvalStack( var );

	return;
}

/***************************************************************************
 *      replace property
 ***************************************************************************/
void CStringObject::replace( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );

	CVariable& varThis = *pArguments ++;
	//toString, this
	varThis.convertToString( pVm );

	REPLACE_PARAMS param;
	vector< REPLACE_PARAMS > vecReplacement;

	//First argument
	//RegExp or toString
	CVariable& varSearchValue = *pArguments ++;

	if( OperandType( varSearchValue.iOperandType ) != OPERAND_OBJECTREF
		|| varSearchValue.refObject->getRID() != pVm->getRegexObjectRID() )
	{
		//No, non Regex object is given
		//I need to be converted to String
		varSearchValue.convertToString( pVm );
		uint32_t iPos = (uint32_t)varThis.pString->find( *varSearchValue.pString );
		if( wstring::npos != iPos )
		{
			param.iIndex = iPos;
			param.pstrMatch = varSearchValue.pString;
			param.pObj = NULL;
			vecReplacement.push_back( param );
		}
	}
	else
	{
		//Yes, I'm regex object

		CVariable& varGlobal = *varSearchValue.refObject->lookupProperty( NAME_BUILTIN_REGEX_PROPERTY_GLOBAL );
		if( OperandType( varGlobal.iOperandType ) == OPERAND_BOOLEAN && varGlobal.bValue == true )
		{
			//GLOBAL mode

			int32_t iResult = 0;
			int32_t iLast = -1;	
			CVariable varInt( 0 );
			varSearchValue.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX, varInt );
			while( 1 )
			{
				CVariable var( varThis );
				CRegexObject::ExecMain( pVm, varSearchValue, var );
				if( pVm->getEvalStackFirstEntry().iOperandType == OPERAND_OBJECTREF )
				{
					CVariable& varResult = *pVm->getEvalStackFirstEntry().refObject->lookupProperty( L"0" );
					CVariable& varLastIndex = *varSearchValue.refObject->lookupProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX );
					if( varLastIndex.iValue != iLast )
					{
						param.iIndex = varLastIndex.iValue - (int32_t)varResult.pString->length();
						param.pstrMatch = varResult.pString;
						param.pObj = pVm->getEvalStackFirstEntry().refObject;
						vecReplacement.push_back( param );

						pVm->popEvalStack();
						iLast = varLastIndex.iValue;
					}
					else
					{
						pVm->popEvalStack();
						break;
					}
					++iResult;
				}
				else
				{
					pVm->popEvalStack();
					if( iLast == -1 )
					{
						//Not found
						break;
					}
					break;
				}
			}

		}
		else
		{
			CVariable var( varThis );
			CRegexObject::ExecMain( pVm, varSearchValue, var );
			if( pVm->getEvalStackFirstEntry().iOperandType == OPERAND_OBJECTREF )
			{
				CVariable& varResult = *pVm->getEvalStackFirstEntry().refObject->lookupProperty( L"0" );
				CVariable& varLastIndex = *varSearchValue.refObject->lookupProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX );
				param.iIndex = varLastIndex.iValue - (int32_t)varResult.pString->length();
				param.pstrMatch = varResult.pString;
				param.pObj = pVm->getEvalStackFirstEntry().refObject;
				vecReplacement.push_back( param );

				pVm->popEvalStack();
			}
			else
			{
				pVm->popEvalStack();
			}
		}

	}

	//2nd arg
	//Function or String
	CVariable& varReplaceValue = *pArguments ++;

	if( OperandType( varReplaceValue.iOperandType ) != OPERAND_OBJECTREF
		|| varReplaceValue.refObject->getRID() != pVm->getFunctionObjectRID() )
	{
		//Invoke with the string
		varReplaceValue.convertToString( pVm );

		vector< REPLACE_PARAMS >::reverse_iterator it = vecReplacement.rbegin();
		for( ; it != vecReplacement.rend(); ++it )
		{

			REPLACE_PARAMS& param = *it;
			//Generate replacement string if Regex search is done,,,
			//
			wstring str = *varReplaceValue.pString;

			ReplacementHelper( str, &param );

			//Replase
			varThis.pString->replace( max( param.iIndex, 0 ),
										param.pstrMatch->length(),
										str );
		}

		pVm->pushEvalStack( varThis );
	}
	else
	{
		vector< REPLACE_PARAMS >::reverse_iterator it = vecReplacement.rbegin();
		wstring* pStr = varThis.pString;
		vector< CVariable > vecParam;
		RID ridMethod = varReplaceValue.refObject->getCall();

		//varThis would be invalidated due to invokeMethod call
		wstring str = *pStr;
		for( ; it != vecReplacement.rend(); ++it )
		{

			REPLACE_PARAMS& param = *it;
			//Generate replacement string if Regex search is done,,,
			//
			
			vecParam.push_back( CVariable( 0 ) );	//For this pointer
			vecParam.push_back( CVariable( param.pstrMatch ) );
			if( param.pObj != NULL )
			{
				uint32_t iIndex = 1;
				while( 1 )
				{
					wchar_t strIndex[ 3 ];
					swprintf( strIndex, 3, L"%d", iIndex );
					CVariable* pvar = param.pObj->lookupProperty( strIndex );
					if( pvar == NULL )
						break;
					iIndex ++;
					vecParam.push_back( *pvar );
				}
			}

			vecParam.push_back( CVariable( param.iIndex ) );

			//invoke given callback function
			CVariable* pvar = pVm->invokeMethod( ridMethod, 
								(int32_t)vecParam.size(),
								&*vecParam.begin() );

			pvar->convertToString( pVm );

			//Replase
			str.replace( max( param.iIndex, 0 ),
										param.pstrMatch->length(),
										*pvar->pString );

			//Delete the return value of Invoke method.
			delete pvar;
			vecParam.clear();

		}
		pVm->pushEvalStackString( str.data() );
	}

	return;
}

void CStringObject::ReplacementHelper( wstring& str, REPLACE_PARAMS* pParam )
{
	wstring strNewString = L"";

	for( uint32_t i = 0; i < str.length(); ++ i )
	{
		wchar_t c = str.at( i );
		if( c != '$' )
			strNewString.push_back( c );
		else
		{
			i ++;
			if( i >= str.length() ) break;
			c = str.at( i );
			switch( c )
			{
			case '$':
				strNewString.append( L'$', 1 );
				break;
			case '&':
				//$& The matched substring.
				strNewString.append( *pParam->pstrMatch );
				break;
			case '`':
				//$Åe The portion of string that precedes the matched substring.
				strNewString.append( str.substr( 0, pParam->iIndex ) );
				break;
			case '\'':
				//$Åf The portion of string that follows the matched substring.
				strNewString.append( str.substr( pParam->iIndex + pParam->pstrMatch->length(), str.length() ) );
				break;
			default:
				//$n The nth capture, where n is a single digit 1-9 and $n is not followed by a decimal
				//digit. If n<=m and the nth capture is undefined, use the empty string instead. If n>m,
				//the result is implementation-defined.
				//$nn The nnth capture, where nn is a two-digit decimal number 01-99. If nn<=m and the
				//nnth capture is undefined, use the empty string instead. If nn>m, the result is
				//implementation-defined.

				if( c >= '1' && c <= '9' )
				{
					wstring sIndex( &c, 1 );
					i ++;
					if( i < str.length() )
					{
						c = str.at( i );
						if( c >= '0' && c <= '9' )
						{
							sIndex.append( &c, 1 );
						}
						else
							-- i;
					}

					if( pParam->pObj == NULL )
						break;
					CVariable* pvar = pParam->pObj->lookupProperty( sIndex );
					if( pvar && pvar->iOperandType == OPERAND_STRING )
						strNewString.append( *pvar->pString );
					else
						break;
				}
				else if( c == '0' )
				{
					i ++;
					if( i < str.length() )
					{
						c = str.at( i );
						if( c < '1' && c > '9' )
							break;
					}

					if( pParam->pObj == NULL )
						break;
					CVariable* pvar = pParam->pObj->lookupProperty( wstring( &c, 1 ) );
					assert( pvar->iOperandType == OPERAND_STRING );
					strNewString.append( *pvar->pString );
				}
				else
				{
					strNewString.append( L"$", 1 );
					strNewString.push_back( c );
				}
				break;
			}
		}
	}
	str = strNewString;

	return;
}



/***************************************************************************
 *      search property
 ***************************************************************************/
void CStringObject::search( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 2 );

	CVariable& varThis = *pArguments++;
	//toString, this
	varThis.convertToString( pVm );

	CVariable& varRegex = *pArguments++;
	switch( OperandType( varRegex.iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		if( varRegex.refObject->getRID() == pVm->getRegexObjectRID() )
			break;
		if( varRegex.refObject->getRID() != pVm->getStringObjectRID() )
		{
			pVm->pushEvalStack( -1 );
			return;
		}
		varRegex.convertToString( pVm );
	case OPERAND_STRING:
		{
			wstring* pVar;
			pVar = varRegex.pString;
			if( !pVar->compare( L"" ) )
			{
				pVar->append( L"^" );
			}
			CVmObject* pTmpObject = pVm->createObject( CCilVm::getObjectRID() );
			CVariable varBool( false );
			CVariable varString( pVar );
			CVariable varInt( 0 );
			pTmpObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_GLOBAL, varBool );
			pTmpObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_SOURCE, varString );
			pTmpObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX, varInt );
			pTmpObject->setValue( varString );	//Note that varPointer is cleared

			varRegex.setObjectRef( pTmpObject );
			break;
		}
	case OPERAND_UNDEFINED:
		pVm->pushEvalStack( 0 );
		return;
	default:
		pVm->pushEvalStack( -1 );
		return;
	}

	CVariable& varLastIndex = *varRegex.refObject->lookupProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX );
	int32_t iIndexKept = varLastIndex.iValue;

	//Clear last index flag
	CVariable varZero( 0 );
	varRegex.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX, varZero );

	CVariable var( varThis );
	CRegexObject::ExecMain( pVm, varRegex, var );

	int32_t iIndex = -1;
	if( pVm->getEvalStackFirstEntry().iOperandType == OPERAND_OBJECTREF )
	{
		CVariable& varResult = *pVm->getEvalStackFirstEntry().refObject->lookupProperty( L"0" );
		CVariable& varLastIndex = *varRegex.refObject->lookupProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX );
		iIndexKept = varLastIndex.iValue;
		iIndex = max( varLastIndex.iValue - (int32_t)varResult.pString->length(), 0 );
	}
	pVm->popEvalStack();

	//Restore lastIndex
	CVariable varInt( iIndexKept );
	varRegex.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX, varInt );

	pVm->pushEvalStack( iIndex );

	return;
}

/***************************************************************************
 *      slice property
 ***************************************************************************/
void CStringObject::slice( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 2 );

	CVariable& varThis = *pArguments ++;
	//toString, this
	varThis.convertToString( pVm );
	int32_t iLength = (int32_t)varThis.pString->length();

	CVariable& varStart = *pArguments ++;
	int32_t iStart = varStart.toInt();
	int32_t iEnd;

	if( iNumArguments >= 3 )
	{
		CVariable& varEnd = *pArguments ++;
		if( OperandType( varEnd.iOperandType ) == OPERAND_UNDEFINED)
			iEnd = iLength;
		else
			iEnd = varEnd.toInt();
	}
	else
		iEnd = iLength;

	
	if( iStart < 0 )
		iStart = max( iStart + iLength, 0 );
	else
		iStart = min( iStart, iLength );

	if( iEnd < 0 )
		iEnd = max( iEnd + iLength, 0 );
	else
		iEnd = min( iEnd, iLength );

	wstring str = varThis.pString->substr( iStart,
											max(iEnd - iStart, 0) );
	pVm->pushEvalStackString( str.data() );
	
	return;
}

/***************************************************************************
 *      localeCompare property
 ***************************************************************************/
void CStringObject::localeCompare( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	if( iNumArguments == 1 )
	{
		pVm->pushEvalStack( (int32_t)-1 );
		return;
	}

	CVariable& varThis = *pArguments ++;
	//toString, this
	varThis.convertToString( pVm );

	CVariable& varThat = *pArguments ++;
	//toString, that
	varThat.convertToString( pVm );

	int32_t iRet = varThis.pString->compare( *varThat.pString );
	if( iRet < 0 ) iRet = -1;
	else if( iRet > 0 ) iRet = 1;
	
	pVm->pushEvalStack( iRet );

	return;
}

//Temporary variable for String.length
CVariable CStringObject::m_varLength;

/***************************************************************************
 *      lengthAccessor
 ***************************************************************************/
CVariable* CStringObject::lengthAccessor( CCilVm* const pVm,
											CVmObject* const pObject,
											const wstring* const pName,
											CVariable* const pVar,
											const bool bSet )
{
	assert( pObject != NULL );
	if( bSet )
	{
		//Getter implementation
		if( pVar )
		{
			//should return parent variable's length
			switch( pVar->iOperandType )
			{
			case OPERAND_STRING:
				m_varLength.setInt( (int32_t)pVar->pString->length() );
				break;
			case OPERAND_OBJECTREF:
				m_varLength.setInt( (int32_t)pVar->refObject->getValue().pString->length() );
				break;
			default:
				assert( "Illegal Operand" );
			}
		}
		else
		{
			if( pObject->getValue().pString )
				m_varLength.setInt( (int32_t)pObject->getValue().pString->length() );
			else
				m_varLength.setInt( 0 );
		}
		return &m_varLength;
	}
	else
	{
		//Setter implementation

		//Do nothing

		return NULL;
	}
}

} //namespace CRI