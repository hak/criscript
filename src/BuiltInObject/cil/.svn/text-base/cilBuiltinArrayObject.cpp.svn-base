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
 * Module   : cilBuiltinArrayObject.cpp
 * File     : Defines built in Array object
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
METHOD_INITIALIZATION_PARAM CArrayObject::initParams[] = {
	{
		NAME_BUILTIN_ARRAY_OBJECT_CTOR,
		METHOD_NATIVE,
		(PMETHOD)&CArrayObject::ctor,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_ARRAY_OBJECT_CALL,
		METHOD_NATIVE,
		(PMETHOD)&CSystemObject::LoadarrayLiteral,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_ARRAY_OBJECT_TOSTRING,
		METHOD_NATIVE,
		(PMETHOD)&CArrayObject::toString,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_ARRAY_OBJECT_TOLOCALESTRING,
		METHOD_NATIVE,
		(PMETHOD)&CArrayObject::toString,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_OBJECT_OBJECT_JOIN,
		METHOD_NATIVE,
		(PMETHOD)&CArrayObject::join,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_ARRAY_OBJECT_JOIN,
		METHOD_NATIVE,
		(PMETHOD)&CArrayObject::join,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_ARRAY_OBJECT_SORT,
		METHOD_NATIVE,
		(PMETHOD)&CArrayObject::sort,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_ARRAY_OBJECT_REVERSE,
		METHOD_NATIVE,
		(PMETHOD)&CArrayObject::reverse,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_ARRAY_OBJECT_VALUEOF,
		METHOD_NATIVE,
		(PMETHOD)&CArrayObject::valueOf,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},

};

//Initialized when VM starts

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Initialize
 ***************************************************************************/
bool CArrayObject::InitializeCodeGen( ICodeGen& CodeGen )
{
	bool bReturn = true;

	RID ridObject = CodeGen.registerBuiltinObject( wstring( NAME_BUILTIN_ARRAY_OBJECT ) );
	if( ridObject == RID_NOTDEFINED )
		return bReturn;

	//Initialize Global 'Array' object ref
	RID rid = CodeGen.getMetaData().generateStaticFieldRID( OPERAND_OBJECTREF,
															OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
																			| OPERAND_FLAG_DONTENUM
																			| OPERAND_FLAG_READONLY ) );
	CodeGen.generateSymbolInformation( rid, NAME_BUILTIN_ARRAY_OBJECT, CG_SYMBOL_FLAG_READONLY, ridObject );

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
bool CArrayObject::InitializeVm( CCilVm& vm )
{
	int32_t iIndex = IndexFromBuiltinRid( vm.getArrayObjectRID() );
	RID ridObjectName = vm.getMetaData().getBuiltinObjectTable()[ iIndex ].getObjectName();
	assert( ridObjectName != RID_NOTDEFINED );
	
	RID rid = vm.createPrototypeObject( vm.getArrayObjectRID(),
										ridObjectName,
										vm.getObjectRID() );
	assert( rid == vm.getArrayObjectRID() );

	//---------------------------------------------------------------------------
	//Bind global object
	//---------------------------------------------------------------------------
	CVmObject* pObj = vm.createObject( vm.getArrayObjectRID() );
	CVariable obj( pObj, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
									| OPERAND_FLAG_DONTENUM
									| OPERAND_FLAG_READONLY ) );

	//Set true as an object value..
	CVariable var( true );
	pObj->setValue( var );

	CG_SYMBOL_INFORMATION info = vm.querySymbolInformation( NAME_BUILTIN_ARRAY_OBJECT );
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
 *      Construct Array object
 ***************************************************************************/
void CArrayObject::ctor( CCilVm* const pVm,
										   const int32_t iNumArguments, 
										   CVariable* pArguments )
{
	assert( pVm != NULL );
	assert( iNumArguments >= 1 );

	//Initialize this object
	CVariable& var = *pArguments;
	pArguments++;
	assert( OperandType( var.iOperandType ) == OPERAND_OBJECTREF );

	//Set prototype: Array prototype object
	var.refObject->setPrototypeObject( 
		&pVm->getPrototypeObject( pVm->getArrayObjectRID() ) );

	//setProperty Accessor
	var.refObject->setPropertyAccessor( STRING_INTERNAL_LENGTH,
										lengthAccessor );
	//setProperty Accessor
	var.refObject->setPropertyAccessor( STRING_PROPERTY_INTERNAL_ANY,
										anyAccessor );

	if( iNumArguments == 2 )
	{
		//Special case when only 1 argument is given

		int32_t iArrayLength = iNumArguments - 1;
		OPERAND_TYPE type = pArguments->getPrimitiveType();
		if( type != OPERAND_STRING )
		{
			//Argument is a number
			int32_t iSize = pArguments->toInt();
			if( iSize >= MAX_ARRAYLENGTH )
			{
				//Throw TypeError
				pVm->throwException( &wstring( ERRORSTRING_OUTOFMEMORY ),
					&wstring( NAME_BUILTIN_ERROR_OBJECT ),
					ERROR_OUT_OF_MEMORY );

				return;
			}
			else if( iSize < 0 )
			{
				//Throw TypeError
				pVm->throwException( &wstring( ERRORSTRING_ARRAY_LENGTH ),
					&wstring( NAME_BUILTIN_ERROR_OBJECT ),
					ERROR_ARRAY_LENGTH );

				return;
			}
			iArrayLength = iSize;
		}
		else
		{
			//Store initializers to the array
			wchar_t strBuffer[ 22 ];		//22=max value width of longlong
			int32_t i = 0;
			for( i = 0; i < iNumArguments - 1; ++i )
			{
				//Argument: 1) value of the property
				swprintf( strBuffer, 22, L"%d", i );
				var.refObject->setProperty( strBuffer, *pArguments );
				pArguments++;
			}
		}

		//Just set length
		//Set to ".lengh"
		CVariable varLength( (int32_t)iArrayLength,
					  OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM ) );
		var.refObject->setProperty( STRING_PROPERTY_INTERNAL_LENGTH, varLength );
	}
	else
	{
		//Store initializers to the array
		wchar_t strBuffer[ 22 ];		//22=max value width of longlong
		int32_t i = 0;
		for( i = 0; i < iNumArguments - 1; ++i )
		{
			//Argument: 1) value of the property
			swprintf( strBuffer, 22, L"%d", i );
			var.refObject->setProperty( strBuffer, *pArguments );
			pArguments++;
		}

		//Set to ".lengh"
		CVariable varLength( (int32_t)iNumArguments - 1,
					  OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM ) ); 
		var.refObject->setProperty( STRING_PROPERTY_INTERNAL_LENGTH, varLength );
	}

	return;
}

/***************************************************************************
 *      toString property
 ***************************************************************************/
void CArrayObject::toString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 1 );

	if( OperandType( pArguments->iOperandType ) != OPERAND_OBJECTREF
		|| pArguments->refObject->getRID() != pVm->getArrayObjectRID() )
	{
		//Throw TypeError
		pVm->throwException( &wstring( ERRORSTRING_TYPEERROR_ARRAY_TOSTRING ),
			&wstring( NAME_BUILTIN_ERROR_OBJECT ),
			ERROR_TYPEERROR_ARRAY_TOSTRING );

		return;
	}

	//call join with no argument
	join( pVm, 1, pArguments );
	
	return;
}

/***************************************************************************
 *      join property
 ***************************************************************************/
void CArrayObject::join( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	assert( iNumArguments >= 1 );
	CVariable& varThis = *pArguments;

	wstring sSeparator = wstring( STRING_INTERNAL_JOIN_DEFAULT_SEPARATOR );

	//Set up separator string
	if( iNumArguments > 1 )
	{
		//Set new separator
		pArguments ++;
		sSeparator = *pArguments->toString( pVm );
	}

	//Initialize destination variable in the eval stack
	pVm->pushEvalStackUndefined();
	
	CVariable& var = pVm->getEvalStackFirstEntry();
	var.iOperandType = OPERAND_STRING;
	var.pString = new wstring(L"");

	uint32_t iIndex = 0;
	uint32_t iLength = 0;
	hash_map< wstring, CVariable >::iterator itStart = varThis.refObject->getPropertyMap().begin();
	hash_map< wstring, CVariable >::iterator itEnd = varThis.refObject->getPropertyMap().end();
	while( itStart != itEnd )
	{
		bool bDigit = CheckArrayIndex( itStart->first );
		if( bDigit )
			iLength ++;
		itStart++;
	}
	
	wchar_t strIndex[ 16 ];

	while( iLength )
	{
		swprintf( strIndex, 16, L"%d", iIndex );
		CVariable* pvar;
		pvar = varThis.refObject->lookupProperty( strIndex );
		if( iIndex )
			var.pString->append( sSeparator );
		iIndex++;
		if( pvar )
		{
			CVariable varTmp = *pvar;
			
			//To eliminate recursive death
			if( varThis.refObject != varTmp.refObject ||
				varTmp.iOperandType != OPERAND_OBJECTREF )
			{
				varTmp.convertToString( pVm );
				var.pString->append( *varTmp.pString );
			}
			iLength--;
		}
	}
	
	return;
}

/***************************************************************************
 *      sort property
 ***************************************************************************/
RID CArrayObject::g_ridPredicater;
CCilVm* CArrayObject::g_pVm;

void CArrayObject::sort( CCilVm* const pVm,
						const int32_t iNumArguments, 
						CVariable* pArguments )
{
	assert( iNumArguments >= 1 );
	CVariable& varThis = *pArguments;
	
	if( OperandType( pArguments->iOperandType ) != OPERAND_OBJECTREF
		|| pArguments->refObject->getRID() != pVm->getArrayObjectRID() )
	{
		//Throw TypeError
		pVm->throwException( &wstring( ERRORSTRING_TYPEERROR_ARRAY_TOSTRING ),
							&wstring( NAME_BUILTIN_ERROR_OBJECT ),
							ERROR_TYPEERROR_ARRAY_TOSTRING );
						return;
	}
		
	hash_map< wstring, CVariable >& map = varThis.refObject->getPropertyMap();
		
	hash_map< wstring, CVariable >::iterator itBegin = map.begin();
	hash_map< wstring, CVariable >::iterator itErase;
	hash_map< wstring, CVariable >::iterator itEnd = map.end();
		
	vector< CVariable >vecArray;
	vecArray.reserve( map.size() );
		
	//Dup entries to the vector
	while( itBegin != itEnd )
	{
		if( !(itBegin->second.iOperandFlag & OPERAND_FLAG_DONTENUM ) )
		{
			vecArray.push_back( itBegin->second );
			itErase = itBegin;
			++itBegin;
			map.erase( itErase );
		}
		else
			++itBegin;
	}
		
	vector< CVariable >::iterator itVecBegin = vecArray.begin();
	vector< CVariable >::iterator itVecEnd = vecArray.end();
	
	g_pVm = pVm;
	if( iNumArguments >= 2 )
	{
		pArguments++;
		if( pArguments->iOperandType == OPERAND_OBJECTREF
			&& pArguments->refObject->getCall() )
		{
			//Cmp function is given as 1st argument
			g_ridPredicater = pArguments->refObject->getCall();
			std::sort( itVecBegin, itVecEnd, CilPredicater );
		}
		else
		{
			//Fallback to regular sort.
			std::sort( itVecBegin, itVecEnd, Predicater );
		}
	}
	else
	{
		//Sort these inside the vector
		std::sort( itVecBegin, itVecEnd, Predicater );
	}
	wchar_t strIndex[ 16 ];
	uint32_t iIndexSrc = 0;
	uint32_t iIndexDest = 0;
	
	//Restore to the map
	itVecBegin = vecArray.begin();
	itVecEnd = vecArray.end();
	while( itVecBegin != itVecEnd )
	{
		//Skip storing UNDEFINED
		if( vecArray[ iIndexSrc ].iOperandType == OPERAND_UNDEFINED )
		{
			++itVecBegin;
			++iIndexSrc;
			continue;
		}
		swprintf( strIndex, 16, L"%d", iIndexDest );
		map[ strIndex ] = vecArray[ iIndexSrc ];
		
		++iIndexSrc;
		++iIndexDest;
		++itVecBegin;
	}
	
	pVm->pushEvalStack( varThis );
	return;
}
	
bool CArrayObject::CilPredicater( const CVariable& elem1,
									const CVariable& elem2 )
{
	assert( g_ridPredicater != RID_NULL );
	assert( g_pVm != NULL );
	
	//Invoke predicater
	CVariable vars[ 3 ];
	vars[ 1 ] = elem1;
	vars[ 2 ] = elem2;
	
	CVariable* pResult = g_pVm->invokeMethod( g_ridPredicater,
												3,
												vars );
	bool bReturn = (pResult->toInt() < 0);
	delete( pResult );
	
	return bReturn;
}
	
	
bool CArrayObject::Predicater( const CVariable& elem1,
								const CVariable& elem2 )
{
	assert( g_pVm != NULL );
	if( elem1.iOperandType == elem2.iOperandType )
	{
		switch( elem1.iOperandType )
		{
			case OPERAND_STRING:
				return *elem1.pString < *elem2.pString; 
			case OPERAND_OBJECTREF:
			case OPERAND_UNDEFINED:
			case OPERAND_NULL:
			case OPERAND_NAN:
				return false; //Those not comparable
			case OPERAND_INT:
				return elem1.iValue < elem2.iValue;
			case OPERAND_INT64:
				return elem1.i64Value < elem2.i64Value;
			case OPERAND_FLOAT:
				return elem1.fValue < elem2.fValue;
			case OPERAND_DOUBLE:
				return elem1.dValue < elem2.dValue;
			case OPERAND_BOOLEAN:
				return elem1.bValue < elem2.bValue;
			default:
				assert( "Illegal parameter" );
		}
	}
	else
	{
		switch( elem1.iOperandType )
		{
			case OPERAND_STRING:
				switch( elem2.iOperandType )
				{
					case OPERAND_INT:
					case OPERAND_INT64:
					case OPERAND_FLOAT:
					case OPERAND_DOUBLE:
					case OPERAND_BOOLEAN:
						return *elem1.pString < *(const_cast<CVariable&>(elem2).toString( g_pVm ));
					case OPERAND_OBJECTREF:
					case OPERAND_UNDEFINED:
					case OPERAND_NULL:
					case OPERAND_NAN:
						return false; //Those not comparable
					default:
						assert( "Illegal parameter" );
					}
				case OPERAND_INT:
					switch( elem2.iOperandType )
					{
					case OPERAND_STRING:
						return *(const_cast<CVariable&>(elem1)).toString( g_pVm ) < *elem2.pString; 
					case OPERAND_INT64:
						return elem1.iValue < elem2.i64Value;
					case OPERAND_FLOAT:
						return elem1.iValue < elem2.fValue;
					case OPERAND_DOUBLE:
						return elem1.iValue < elem2.dValue;
					case OPERAND_BOOLEAN:
						return elem1.iValue < static_cast<int32_t>(elem2.bValue);
					case OPERAND_OBJECTREF:
					case OPERAND_UNDEFINED:
					case OPERAND_NULL:
					case OPERAND_NAN:
						return false; //Those not comparable
					default:
						assert( "Illegal parameter" );
					}
				case OPERAND_INT64:
					switch( elem2.iOperandType )
					{
					case OPERAND_STRING:
						return *(const_cast<CVariable&>(elem1)).toString( g_pVm ) < *elem2.pString; 
					case OPERAND_INT:
						return elem1.i64Value < elem2.iValue;
					case OPERAND_FLOAT:
						return elem1.i64Value < elem2.fValue;
					case OPERAND_DOUBLE:
						return elem1.i64Value < elem2.dValue;
					case OPERAND_BOOLEAN:
						return elem1.i64Value < static_cast<int64_t>(elem2.bValue);
					case OPERAND_OBJECTREF:
					case OPERAND_UNDEFINED:
					case OPERAND_NULL:
					case OPERAND_NAN:
						return false; //Those not comparable
					default:
						assert( "Illegal parameter" );
					}
				case OPERAND_FLOAT:
					switch( elem2.iOperandType )
					{
					case OPERAND_STRING:
						return *(const_cast<CVariable&>(elem1)).toString( g_pVm ) < *elem2.pString; 
					case OPERAND_INT:
						return elem1.fValue < elem2.fValue;
					case OPERAND_INT64:
						return elem1.fValue < elem2.fValue;
					case OPERAND_DOUBLE:
						return elem1.fValue < elem2.dValue;
					case OPERAND_BOOLEAN:
						return elem1.fValue < static_cast<float>(elem2.bValue);
					case OPERAND_OBJECTREF:
					case OPERAND_UNDEFINED:
					case OPERAND_NULL:
					case OPERAND_NAN:
						return false; //Those not comparable
					default:
						assert( "Illegal parameter" );
					}
				case OPERAND_DOUBLE:
					switch( elem2.iOperandType )
					{
					case OPERAND_STRING:
						return *(const_cast<CVariable&>(elem1)).toString( g_pVm ) < *elem2.pString; 
					case OPERAND_INT:
						return elem1.dValue < elem2.iValue;
					case OPERAND_INT64:
						return elem1.dValue < elem2.i64Value;
					case OPERAND_FLOAT:
						return elem1.dValue < elem2.fValue;
					case OPERAND_BOOLEAN:
						return elem1.dValue < static_cast<double>(elem2.bValue);
					case OPERAND_OBJECTREF:
					case OPERAND_UNDEFINED:
					case OPERAND_NULL:
					case OPERAND_NAN:
						return false; //Those not comparable
					default:
						assert( "Illegal parameter" );
					}
				case OPERAND_BOOLEAN:
					switch( elem2.iOperandType )
					{
					case OPERAND_STRING:
						return *(const_cast<CVariable&>(elem1)).toString( g_pVm ) < *elem2.pString; 
					case OPERAND_INT:
						return static_cast<int32_t>(elem1.bValue) < elem2.iValue;
					case OPERAND_INT64:
						return static_cast<int64_t>(elem1.bValue) < elem2.i64Value;
					case OPERAND_FLOAT:
						return static_cast<float>(elem1.bValue) < elem2.fValue;
					case OPERAND_DOUBLE:
						return static_cast<double>(elem1.bValue) < elem2.dValue;
					case OPERAND_OBJECTREF:
					case OPERAND_UNDEFINED:
					case OPERAND_NULL:
					case OPERAND_NAN:
						return false; //Those not comparable
					default:
						assert( "Illegal parameter" );
					}
				case OPERAND_OBJECTREF:
				case OPERAND_UNDEFINED:
				case OPERAND_NULL:
				case OPERAND_NAN:
					return false; //Those not comparable
				default:
					assert( "Illegal parameter" );
			}
		}
		
	return false; //Should return false if both the vaules are same
}
	
/***************************************************************************
 *      reverse property
 ***************************************************************************/
void CArrayObject::reverse( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	Debug_Fatal( "Not implemented yet" );
}

/***************************************************************************
 *      valueOf property
 ***************************************************************************/
void CArrayObject::valueOf( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )

{
	Debug_Fatal( "Not implemented yet" );
}

/***************************************************************************
 *      CheckArrayIndex`
 *      array object helper
 ***************************************************************************/
bool CArrayObject::CheckArrayIndex( const wstring& str )
{
	bool b = true;

	//Should less than 99999
	if( str.length() > 5 )
		return false;

	wstring::const_iterator itStrStart = str.begin();
	wstring::const_iterator itStrEnd =  str.end();
	while( itStrStart != itStrEnd )
	{
		if( *itStrStart < '0' ||  *itStrStart > '9' )
		{
			b = false;
			break;
		}
		itStrStart++;
	}

	return b;
}

/***************************************************************************
 *      lengthAccessor
 *      array object helper
 ***************************************************************************/
CVariable* CArrayObject::lengthAccessor(CCilVm* const pVm,
										CVmObject* const pObject,
										const wstring* const pName,
										CVariable* const pVar,
										const bool bSet )
{
	assert( pObject != NULL );
	if( bSet )
	{
		//Getter implementation
		return pObject->lookupProperty( STRING_PROPERTY_INTERNAL_LENGTH );
	}
	else
	{
		//Setter implementation
		int32_t iLength = 0;

		//Check existing length
		CVariable* pCurrentLengthVar = pObject->lookupProperty( STRING_PROPERTY_INTERNAL_LENGTH );

		if( pCurrentLengthVar != NULL  )
		{
			iLength = pCurrentLengthVar->toInt();
		}

		int32_t iGivenIndex = pVar->toInt();

		//Just expanding the array, nothing to do
		CVariable var( iGivenIndex,
					  OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM ) );
		pObject->setProperty( STRING_PROPERTY_INTERNAL_LENGTH, var );

		if( iGivenIndex < iLength )
		{
			//Reducing an array size, need to eliminate some entries
			int32_t iNumToEliminate = iLength - iGivenIndex;

			hash_map< wstring, CVariable >::iterator itStart =
										pObject->getPropertyMap().begin();
			hash_map< wstring, CVariable >::iterator itEnd =
										pObject->getPropertyMap().end();
			while( itStart != itEnd && iNumToEliminate )
			{
				if( itStart->second.iOperandFlag && OPERAND_FLAG_DONTENUM )
				{
					itStart++;
					continue;
				}
				if( CheckArrayIndex( itStart->first ) )
				{
					const wstring& String = itStart->first;
					itStart ++;
					if( _wtoi( String.data() ) >= iGivenIndex )
					{
						pObject->removeProperty( String );
						iNumToEliminate--;
					}
				}
				else
					itStart++;
			}
		}
		return NULL;
	}
}

/***************************************************************************
 *      anyAccessor
 ***************************************************************************/
CVariable* CArrayObject::anyAccessor(CCilVm* const pVm,
										CVmObject* const pObject, 
										const wstring* const pName,
										CVariable* const pVar,
										const bool bSet )
{
	assert( pObject != NULL );
	if( bSet )
	{
		//Getter implementation
		return NULL;
	}
	else
	{
		pObject->setProperty( *pName, *pVar );

		//Setter implementation
		int32_t iLength = 0;

		//Check existing length
		CVariable* pCurrentLengthVar = pObject->lookupProperty( STRING_PROPERTY_INTERNAL_LENGTH );

		if( pCurrentLengthVar != NULL  )
		{
			iLength = pCurrentLengthVar->toInt();
		}

		int32_t iGivenIndex = _wtoi( pName->data() ) + 1;
		
		if( iGivenIndex > iLength )
		{
			CVariable var( iGivenIndex, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM ) ); 
			pObject->setProperty( STRING_PROPERTY_INTERNAL_LENGTH, var );
		}
		return NULL;
	}
}

} //namespace CRI