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
 * Module   : cilBuiltinRegexObject.cpp
 * File     : Defines built in regex object
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"


#ifndef BOOST_REGEX_NO_LIB
	#define BOOST_REGEX_NO_LIB (1)
#endif
#ifndef BOOST_NO_EXCEPTIONS
	#define BOOST_NO_EXCEPTIONS (1)
#endif
#include "boost/regex.hpp"

#include "criScript.h"
#include "cilCodeGen.h"

namespace cri {

/***************************************************************************
 *      Variables
 ***************************************************************************/
METHOD_INITIALIZATION_PARAM CRegexObject::initParams[] = {
	{
		NAME_BUILTIN_REGEX_OBJECT_CTOR,
		METHOD_NATIVE,
		(PMETHOD)&CRegexObject::ctor,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_REGEX_OBJECT_CALL,
		METHOD_NATIVE,
		(PMETHOD)&CRegexObject::call,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_REGEX_OBJECT_EXEC,
		METHOD_NATIVE,
		(PMETHOD)&CRegexObject::exec,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_REGEX_OBJECT_TOSTRING,
		METHOD_NATIVE,
		(PMETHOD)&CRegexObject::toString,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_REGEX_OBJECT_TOLOCALESTRING,
		METHOD_NATIVE,
		(PMETHOD)&CRegexObject::toString,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_REGEX_OBJECT_TEST,
		METHOD_NATIVE,
		(PMETHOD)&CRegexObject::test,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},

};

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Initialize
 ***************************************************************************/
bool CRegexObject::InitializeCodeGen( ICodeGen& CodeGen )
{
	bool bReturn = true;

	RID ridObject = CodeGen.registerBuiltinObject( NAME_BUILTIN_REGEX_OBJECT );
	if( ridObject == RID_NOTDEFINED )
		return bReturn;

	//Initialize Global object ref
	RID rid = CodeGen.getMetaData().generateStaticFieldRID( OPERAND_OBJECTREF,
															OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
															| OPERAND_FLAG_DONTENUM
															| OPERAND_FLAG_READONLY ) );
	CodeGen.generateSymbolInformation( rid, NAME_BUILTIN_REGEX_OBJECT, CG_SYMBOL_FLAG_READONLY, ridObject );

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
bool CRegexObject::InitializeVm( CCilVm& vm )
{
	RID rid = vm.createPrototypeObject( vm.getRegexObjectRID(),
										vm.getMetaData().getObjectNameRID( vm.getRegexObjectRID() ),
										vm.getObjectRID() );
	assert( rid == vm.getRegexObjectRID() );

	//---------------------------------------------------------------------------
	//Bind global object
	//---------------------------------------------------------------------------
	CVmObject* pObj = vm.createObject( vm.getRegexObjectRID() );
	CVariable obj( pObj, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
									| OPERAND_FLAG_DONTENUM
									| OPERAND_FLAG_READONLY ) );

	//Set true as an object value..
	CVariable varBool( true );
	pObj->setValue( varBool );

	CG_SYMBOL_INFORMATION info = vm.querySymbolInformation( NAME_BUILTIN_REGEX_OBJECT );
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

	return true;
}

/***************************************************************************
 *      ctor method
 ***************************************************************************/
void CRegexObject::ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	CVariable& obj = *pArguments;
	pArguments++;
	assert( obj.iOperandType == OPERAND_OBJECTREF );

	//Set prototype: Regex prototype object
	obj.refObject->setPrototypeObject( 
		&pVm->getPrototypeObject( pVm->getRegexObjectRID() ) );

	//Regex
	CVariable* pVarFlags;
	CVariable* pVarRegex;
	switch( iNumArguments )
	{
	case 1:
		pVarRegex = new CVariable( L"^" );
		pVarFlags = new CVariable( L"" );
		break;
	case 2:
		pVarRegex = &*pArguments;
		pVarFlags = new CVariable( L"" );
		break;
	default:
		pVarRegex = &*pArguments;
		pArguments++;
		pVarFlags= &*pArguments;
		break;
	}

	wstring* pStrRegex;
	//Flags
	if( pVarRegex->iOperandType == OPERAND_INT )
	{
		if( TypeFromToken( pVarRegex->iValue ) == MDT_STRING )
		{
			pStrRegex = &pVm->getMetaData().getStringConstant( pVarRegex->iValue );
		}
		else
		{
			pVarRegex = new CVariable( L"^" );
			pStrRegex = pVarRegex->pString;
		}
	}
	else
	{
		pVarRegex->convertToString( pVm );
		pStrRegex = pVarRegex->pString;
	}

	wstring* pStrFlags;
	//Flags
	if( pVarFlags->iOperandType == OPERAND_INT )
	{
		pStrFlags = &pVm->getMetaData().getStringConstant( pVarFlags->iValue );
	}
	else
	{
		pVarFlags->convertToString( pVm );
		pStrFlags = pVarFlags->pString;
	}

	bool bFlag = ( pStrFlags->find( L'g' ) != wstring::npos );
	CVariable varBool( bFlag, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM ) );
	obj.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_GLOBAL,
								varBool );

	bFlag = ( pStrFlags->find( L'i' ) != wstring::npos );
	CVariable varBoolI( bFlag, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM ) );
	obj.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_IGNORECASE,
								varBoolI );

	bFlag = ( pStrFlags->find( L'm' ) != wstring::npos );
	CVariable varBoolM( bFlag, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM ) );
	obj.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_MULTILINE,
								varBoolM );

	CVariable varInt( 0, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM ) );
	obj.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX,
								varInt );

	CVariable varPointer( pStrRegex );
	obj.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_SOURCE, varPointer );
	
	wstring s = wstring( L"/" );
	s.append( *pStrRegex );
	s.append( L"/" );
	s.append( *pStrFlags );
	
	CVariable varString( &s );
	obj.refObject->setValue( varString );
	
	if( iNumArguments <= 1 )
	{
		delete( pVarRegex );
		delete( pVarFlags );
	}
	else if( iNumArguments <= 2 )
	{
		delete( pVarFlags );
	} 
	
	return;
}

/***************************************************************************
 *      call method
 ***************************************************************************/
void CRegexObject::call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	//Skip this
	pArguments++;
	CVariable obj;

	CVmObject* pObj = pVm->createObject( pVm->getRegexObjectRID() );
	obj.setObjectRef( pObj );

	//Regex
	CVariable* pVarFlags;
	CVariable* pVarRegex;
	if( iNumArguments > 1 )
	{
		pVarRegex = &*pArguments;
		pArguments++;
		pVarFlags= &*pArguments;
	}
	else
	{
		pVarRegex = new CVariable( L"" );
		pVarFlags = new CVariable( L"" );
	}
	wstring* pStrRegex;
	//Flags
	if( pVarRegex->iOperandType == OPERAND_INT )
	{
		if( TypeFromToken( pVarRegex->iValue ) == MDT_STRING )
		{
			pStrRegex = &pVm->getMetaData().getStringConstant( pVarRegex->iValue );
		}
		else
		{
			pVarRegex = new CVariable( L"^" );
			pStrRegex = pVarRegex->pString;
		}
	}
	else
	{
		pVarRegex->convertToString( pVm );
		pStrRegex = pVarRegex->pString;
	}

	wstring* pStrFlags;
	//Flags
	if( pVarFlags->iOperandType == OPERAND_INT )
	{
		pStrFlags = &pVm->getMetaData().getStringConstant( pVarFlags->iValue );
	}
	else
	{
		pVarFlags->convertToString( pVm );
		pStrFlags = pVarFlags->pString;
	}

	bool bFlag = ( pStrFlags->find( L'g' ) != wstring::npos );
	CVariable varFlagG( bFlag, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM  ) );
	obj.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_GLOBAL,
								varFlagG );

	bFlag = ( pStrFlags->find( L'i' ) != wstring::npos );
	CVariable varFlagI( bFlag, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM  ) );
	obj.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_IGNORECASE,
							   varFlagI );
	
	bFlag = ( pStrFlags->find( L'm' ) != wstring::npos );
	CVariable varFlagM( bFlag, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM  ) );
	obj.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_MULTILINE,
							   varFlagM );
	
	CVariable varInt( 0, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM ) );
	obj.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX,
								varInt );

	CVariable varPointer( pStrRegex );
	obj.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_SOURCE, varPointer );

	wstring* s = new wstring( L"/" );
	s->append( *pStrRegex );
	s->append( L"/" );
	s->append( *pStrFlags );

	CVariable varString( s );
	obj.refObject->setValue( varString );

	if( iNumArguments <= 1 )
	{
		delete( pVarRegex );
		delete( pVarFlags );
	}
	else if( iNumArguments <= 2 )
	{
		delete( pVarFlags );
	} 

	//Push the ObjectRef to a eval stack
	pVm->pushEvalStackVariable( obj );
	return;
}

/***************************************************************************
 *      toString property
 ***************************************************************************/
void CRegexObject::toString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 1 );
	assert( OperandType( pArguments->iOperandType ) == OPERAND_OBJECTREF );

	CVmObject* pObj = pArguments->refObject;
	pVm->pushEvalStackStringPointer( pObj->getValue().toString( pVm ) );

	return;
}

/***************************************************************************
 *      test property
 ***************************************************************************/
void CRegexObject::test( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 1 );

	CVariable& varRegex = *pArguments;
	pArguments ++;

	CVariable& varString = *pArguments;
	varString.convertToString( pVm );

	ExecMain( pVm, varRegex, varString );
	if( OperandType( pVm->getEvalStackFirstEntry().iOperandType ) == OPERAND_NULL )
	{
		pVm->popEvalStack();
		pVm->pushEvalStack( false );
	}
	else
	{
		pVm->popEvalStack();
		pVm->pushEvalStack( true );
	}

	CVariable varInt( 0, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM ) );
	varRegex.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX, varInt );
	return;
}

/***************************************************************************
 *      exec property
 ***************************************************************************/
void CRegexObject::exec( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 1 );

	CVariable& varRegex = *pArguments;
	pArguments ++;

	CVariable& varString = *pArguments;
	varString.convertToString( pVm );

	ExecMain( pVm, varRegex, varString );
	return;
}


void CRegexObject::ExecMain( CCilVm* const pVm,
							CVariable& varRegex,
							CVariable& varString,
							uint32_t optionalflags )
{
	wstring* s = varString.pString;

	CVariable& regex = *varRegex.refObject->lookupProperty( NAME_BUILTIN_REGEX_PROPERTY_SOURCE );
	wstring* pregexString;
	switch( regex.iOperandType )
	{
	case OPERAND_INT:
		pregexString = &pVm->getMetaData().getStringConstant( regex.iValue );
		break;
	case OPERAND_STRING:
		pregexString = regex.pString;
		break;
	default:
		Debug_Fatal( "Illegal value type" );
	}

	//Check 'global' flag
	int32_t iLastIndex = 0;
	CVariable* pvarGlobal = varRegex.refObject->lookupProperty( NAME_BUILTIN_REGEX_PROPERTY_GLOBAL );
	if( pvarGlobal
		&&OperandType( pvarGlobal->iOperandType ) == OPERAND_BOOLEAN
		&& pvarGlobal->bValue == true )
	{
		iLastIndex = varRegex.refObject->lookupProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX )->toInt();
		if( iLastIndex >= int32_t( s->size() ) )
		{
			//Return null
			pVm->pushEvalStackNull();
			return;
		}
	}
	boost::regex_constants::syntax_option_type reflags = boost::regex_constants::normal | optionalflags;
	//Check 'ignore case' flag
	CVariable* pvarIgnoreCase = varRegex.refObject->lookupProperty( NAME_BUILTIN_REGEX_PROPERTY_IGNORECASE );
	if( pvarIgnoreCase
		&& OperandType( pvarIgnoreCase->iOperandType ) == OPERAND_BOOLEAN
		&& pvarIgnoreCase->bValue == true )
	{
		reflags |= boost::regex_constants::icase;
	}

	boost::wregex r( pregexString->data(), reflags );
	if( r.empty() )
	{
		//Try work arounds
		//Work around 1,
		//Replace '{' to \U007B
		size_t pos;
		pos = pregexString->find( L'{' );
		while( pos != wstring::npos )
		{
			pregexString = &pregexString->replace( pos, 1, L"\\U007B" );
			pos = pregexString->find( L'{' );
		}

		r.assign( pregexString->data(), reflags );

		if( r.empty() )
		{
			//Return null
			pVm->pushEvalStackNull();
			return;
		}
	}
	boost::wsmatch results;
	boost::match_flag_type flags = boost::match_default;

	wstring::const_iterator itStart = s->begin() + iLastIndex;
	wstring::const_iterator itLast = s->end();


	//Do it!!
	bool b = boost::regex_search( itStart, itLast, results, r, flags );
	if( b )
	{
		CVmObject* pGlobalRegex = &pVm->getPrototypeObject( pVm->getRegexObjectRID() );
		CVariable varProperty;
		int32_t i = 0;

		const wchar_t* tableResultPropertyNames[] = {
			NAME_BUILTIN_REGEX_PROPERTY_RESULT0,
			NAME_BUILTIN_REGEX_PROPERTY_RESULT1,
			NAME_BUILTIN_REGEX_PROPERTY_RESULT2,
			NAME_BUILTIN_REGEX_PROPERTY_RESULT3,
			NAME_BUILTIN_REGEX_PROPERTY_RESULT4,
			NAME_BUILTIN_REGEX_PROPERTY_RESULT5,
			NAME_BUILTIN_REGEX_PROPERTY_RESULT6,
			NAME_BUILTIN_REGEX_PROPERTY_RESULT7,
			NAME_BUILTIN_REGEX_PROPERTY_RESULT8,
			NAME_BUILTIN_REGEX_PROPERTY_RESULT9,
		};

		//Create arrayt object
		assert( pVm->getArrayObjectRID() != RID_NOTDEFINED );
		CVmObject* pObj = pVm->createObject( pVm->getArrayObjectRID() );
		CVariable var;
		var.setObjectRef( pObj );
		
		//Set result properties
		CVariable varInt( (int32_t)results.position( 0 ) );
		var.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_INDEX, varInt );
		var.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_INPUT, varString );
		CVariable varLength( (int32_t)results.size(), OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM ) ); 
		var.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_LENGTH,
			varLength );

		//Store global index
		if( (int32_t)results.position( 0 ) ) iLastIndex += (int32_t)results.position( 0 );
		iLastIndex += (int32_t)results.str( i ).length();
		CVariable varIndex( iLastIndex, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM ) ); 
		varRegex.refObject->setProperty( NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX, varIndex );

		wchar_t strProperty[ SIZE_STRING ];
		for( i = 0; i < (int32_t)results.size(); ++i )
		{
			const wstring& s = results.str( i );
			varProperty.setString( s );
			swprintf( strProperty, SIZE_STRING, L"%d", i );
			var.refObject->setProperty( strProperty, varProperty );

			//Update RegExp.$n property
			if( i < ARRAYSIZE( tableResultPropertyNames ) )
			{
				varProperty.setString( s );
				pGlobalRegex->setProperty( tableResultPropertyNames[ i ], varProperty );
			}

		}
		for( ; i < (int32_t)sizeof( tableResultPropertyNames ) / sizeof( tableResultPropertyNames[ 0 ] ); ++i )
		{
			varProperty.setString( L"" );
			pGlobalRegex->setProperty( tableResultPropertyNames[ i ], varProperty );
			//Does not remove it, just set "" string
			//pGlobalRegex->removeProperty( tableResultPropertyNames[ i ] );
		}
		//Push the ObjectRef to a eval stack
		pVm->pushEvalStackVariable( var );

	}
	else
	{
		//Return null
		pVm->pushEvalStackNull();		
	}

	return;
}

} //namespace CRI

/***************************************************************************
 *      exception handling routine for boost
 ***************************************************************************/
namespace boost
{
#ifdef BOOST_NO_EXCEPTIONS
void throw_exception(std::exception const & e)
{
	Debug_Fatal( "Exception triggerred" );
}
#endif
}


