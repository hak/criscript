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
 * Module   : cilBuiltinSystemObjectGlobal.cpp
 * File     : Defines built in system object
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include <float.h>
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
 *      print function
 *      - output debug string to the console
 ***************************************************************************/
void CSystemObject::print( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );

	if( iNumArguments == 1 )
	{
		pVm->pushEvalStackNaN();
		return;
	}

	//Skip this pointer
	pArguments++;

	CVariable& varString = *pArguments++;

	varString.convertToString( pVm );

	switch( OperandType( varString.iOperandType ) )
	{
	case OPERAND_STRING:
		{
		pVm->appendPrintBuffer( *varString.pString );
		wstring str( L"\n" );
		pVm->appendPrintBuffer( str );
		}
		break;
	default:
		break;
	}

	int32_t iResult = 0;
	pVm->pushEvalStack( (int32_t)iResult );
	return;
}

/***************************************************************************
 *      eval function
 *      - output debug string to the console
 ***************************************************************************/
void CSystemObject::eval( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );

	if( iNumArguments == 1 )
	{
		pVm->pushEvalStackNaN();
		return;
	}

	//Skip this pointer
	pArguments++;
	CVariable& varEval = *pArguments++;

	//TODO: need to throw "typeerror"
	if( OperandType( varEval.iOperandType ) == OPERAND_STRING )
	{
		Debug_Fatal("Dynamic Eval is not implemented yet");
	}

	pVm->pushEvalStack( varEval );
	return;
}

/***************************************************************************
 *      escape function
 ***************************************************************************/
void CSystemObject::escape( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );

	if( iNumArguments == 1 )
	{
		pVm->pushEvalStackNaN();
		return;
	}

	//Skip this pointer
	pArguments++;

	CVariable& varString = *pArguments;
	varString.convertToString( pVm );
	wstring* s = varString.pString;
	wstring* sRet = new wstring( L"" );
	
	//Build up escaped string
	wchar_t strEncode[ 6 ];
	for( wstring::iterator it = s->begin(); it != s->end(); ++ it )
	{
		wchar_t w = *it;
		if( ( w >= L'A' && w <= L'Z' ) ||
			( w >= L'a' && w <= L'z' ) ||
			( w >= L'1' && w <= L'9' ) ||
			w == L'@' ||
			w == L'*' ||
			w == L'_' ||
			w == L'+' ||
			w == L'-' ||
			w == L'.' ||
			w == L'/' )
			sRet->append( w, 1);
		else
		{
			if( w <= 0xff )
			{
				swprintf( strEncode, sizeof( strEncode ) / sizeof( strEncode[ 0 ] ),
					L"%%%02x", w );
			}
			else
			{
				swprintf( strEncode, sizeof( strEncode ) / sizeof( strEncode[ 0 ] ),
					L"%%u%04x", w );
			}
			sRet->append( strEncode );
		}
	}

	CVariable var;
	pVm->pushEvalStackVariable( var );

	pVm->getEvalStackFirstEntry().iOperandType = OPERAND_STRING;
	pVm->getEvalStackFirstEntry().pString = sRet;

	return;
}

/***************************************************************************
 *      unescape function
 ***************************************************************************/
void CSystemObject::unescape( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );

	if( iNumArguments == 1 )
	{
		pVm->pushEvalStackNaN();
		return;
	}

	//Skip this pointer
	pArguments++;

	CVariable& varString = *pArguments;
	varString.convertToString( pVm );
	wstring* s = varString.pString;
	wstring* sRet = new wstring( L"" );
	
	//Build up escaped string
	for( wstring::iterator it = s->begin(); it != s->end(); ++ it )
	{
		wchar_t w = *it;
		if( w != L'%' )
			sRet->append( w, 1);
		else
		{
			++it;
			if( it == s->end() ) break;
			w = 0;

			if( *it == L'u' )
			{
				++it;
				if( it == s->end() ) break;
				w = *it;

				++it;
				if( it == s->end() ) break;
				w = w * 0x10 + *it;

				++it;
				if( it == s->end() ) break;
				w = w * 0x10 + *it;

				++it;
				if( it == s->end() ) break;
				w = w * 0x10 + *it;
			}
			else
			{
				if( it == s->end() ) break;
				w = *it;

				++it;
				if( it == s->end() ) break;
				w = w * 0x10 + *it;
			}

			sRet->append( w, 1 );
		}
	}

	CVariable var;
	pVm->pushEvalStackVariable( var );

	pVm->getEvalStackFirstEntry().iOperandType = OPERAND_STRING;
	pVm->getEvalStackFirstEntry().pString = sRet;

	return;
}

/***************************************************************************
 *      int function
 ***************************************************************************/
void CSystemObject::intCast( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 2 );

	//Skip this
	pArguments++;
	pVm->pushEvalStack( pArguments->toInt() );

	return;
}

/***************************************************************************
 *      int64 function
 ***************************************************************************/
void CSystemObject::int64Cast( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 2 );

	//Skip this
	pArguments++;
	pVm->pushEvalStack( pArguments->toInt64() );

	return;
}

/***************************************************************************
 *      float function
 ***************************************************************************/
void CSystemObject::floatCast( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 2 );

	//Skip this
	pArguments++;

	CVariable& varFloat = *pArguments;
	pVm->pushEvalStack( varFloat.toFloat() );

	return;
}

/***************************************************************************
 *      double function
 ***************************************************************************/
void CSystemObject::doubleCast( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 2 );

	//Skip this
	pArguments++;
	pVm->pushEvalStack( pArguments->toDouble() );

	return;
}

#pragma warning( disable: 4800 )
/***************************************************************************
 *      isNaN function
 ***************************************************************************/
void CSystemObject::isNaN( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );

	if( iNumArguments == 1 )
	{
		pVm->pushEvalStackVariable( true );
		return;
	}

	pArguments++;
	CVariable& var = *pArguments;
	bool bNan = false;
	OPERAND_TYPE op;

	switch( OperandType( var.iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		op = var.getPrimitiveType( PRIMITIVETYPE_NUMBER );
		switch( op )
		{
		case OPERAND_NAN:
		case OPERAND_OBJECTREF:
		case OPERAND_STRING:
			bNan = true;
			break;
		case OPERAND_FLOAT:
			bNan = !_finite( var.fValue );
			break;
		case OPERAND_DOUBLE:
			bNan = !_finite( var.dValue );
			break;
		case OPERAND_BOOLEAN:
		case OPERAND_INT:
		case OPERAND_UNSIGNEDINT:
		case OPERAND_INT64:
		case OPERAND_UNSIGNEDINT64:
			bNan = false;
			break;
		default:
			Debug_Fatal( "" );
		}
		break;
	case OPERAND_STRING:
	case OPERAND_NAN:
	case OPERAND_UNDEFINED:
		bNan = true;
		break;
	case OPERAND_FLOAT:
		bNan = !_finite( var.fValue );
		break;
	case OPERAND_DOUBLE:
		bNan = !_finite( var.dValue );
		break;
	default:
		break;
	}
	pVm->pushEvalStackVariable( bNan );

	return;
}

/***************************************************************************
 *      isFnite function
 ***************************************************************************/
void CSystemObject::isFinite( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );

	if( iNumArguments == 1 )
	{
		pVm->pushEvalStackNaN();
		return;
	}

	pArguments++;
	CVariable& var = *pArguments;
	bool bFinite = false;
	OPERAND_TYPE op;

	switch( OperandType( var.iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		op = var.getPrimitiveType( PRIMITIVETYPE_NUMBER );
		switch( op )
		{
		case OPERAND_NAN:
		case OPERAND_OBJECTREF:
		case OPERAND_STRING:
			bFinite = true;
			break;
		case OPERAND_FLOAT:
			bFinite = !_finite( var.fValue );
			break;
		case OPERAND_DOUBLE:
			bFinite = !_finite( var.dValue );
			break;
		case OPERAND_BOOLEAN:
		case OPERAND_INT:
		case OPERAND_UNSIGNEDINT:
		case OPERAND_INT64:
		case OPERAND_UNSIGNEDINT64:
			bFinite = false;
			break;
		default:
			Debug_Fatal( "" );
		}
		break;
	case OPERAND_STRING:
	case OPERAND_NAN:
	case OPERAND_UNDEFINED:
		bFinite = true;
		break;
	case OPERAND_FLOAT:
		bFinite = !_finite( var.fValue );
		break;
	case OPERAND_DOUBLE:
		bFinite = !_finite( var.dValue );
		break;
	default:
		break;
	}
	pVm->pushEvalStackVariable( bFinite );

	return;
}

/***************************************************************************
 *      parseInt function
 ***************************************************************************/
void CSystemObject::parseInt( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );

	if( iNumArguments == 1 )
	{
		pVm->pushEvalStackNaN();
		return;
	}

	//Skip this
	pArguments++;

	CVariable& var = *pArguments;
	var.convertToString( pVm );
	CVariable varparsed;
	
	bool b = CNumberObject::parseNumber( varparsed, var.pString );
	
	if( !b )
		pVm->pushEvalStackNaN();
	else
		pVm->pushEvalStack( varparsed );
	
	return;
}

/***************************************************************************
 *      parseFloat function
 ***************************************************************************/
void CSystemObject::parseFloat( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );

	if( iNumArguments == 1 )
	{
		pVm->pushEvalStackNaN();
		return;
	}

	//Skip this
	pArguments++;

	CVariable& var = *pArguments;
	CVariable varparsed;

	switch( var.iOperandType )
	{
	case OPERAND_FLOAT:
	case OPERAND_DOUBLE:
		pVm->pushEvalStack( var );
		break;
	default:
		{
		var.convertToString( pVm );
	
		bool b = CNumberObject::parseNumber( varparsed, var.pString );
	
		if( !b )
			pVm->pushEvalStackNaN();
		else
			pVm->pushEvalStack( varparsed );
		return;
		}
	}
}

} //namespace CRI