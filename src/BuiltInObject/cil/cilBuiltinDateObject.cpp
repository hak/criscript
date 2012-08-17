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
 * Module   : cilBuiltinDateObject.cpp
 * File     : Defines builtin Date object
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include <time.h>
#include "criScript.h"
#include "cilCodeGen.h"

namespace cri {

/***************************************************************************
 *      Variables
 ***************************************************************************/
METHOD_INITIALIZATION_PARAM CDateObject::initParams[] = {
	{
		NAME_BUILTIN_DATE_OBJECT_CTOR,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::ctor,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_DATE_OBJECT_CALL,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::call,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_DATE_OBJECT_SETTIME,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::setTime,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_DATE_OBJECT_GETTIMEZONEOFFSET,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getTimezoneOffset,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_DATE_OBJECT_TOSTRING,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::toString,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_DATE_OBJECT_TOLOCALESTRING,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::toString,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_DATE_OBJECT_UTC,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::UTC,
		RID_BUILTINPARAM_DEFAULT_SEVEN
	},
	{
		NAME_BUILTIN_DATE_OBJECT_PARSE,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::parse,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_DATE_OBJECT_GETDATE,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getDate,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_DATE_OBJECT_GETTIME,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getTime,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},

	//Not implemented yet
	{
		NAME_BUILTIN_DATE_OBJECT_GETDAY,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getYear,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_DATE_OBJECT_GETHOURS,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getYear,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_DATE_OBJECT_GETMINUTES,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getYear,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_DATE_OBJECT_GETMONTH,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getYear,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_DATE_OBJECT_GETSECONDS,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getYear,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_DATE_OBJECT_GETYEAR,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getYear,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_DATE_OBJECT_SETDAY,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getYear,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
	{
		NAME_BUILTIN_DATE_OBJECT_SETHOURS,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getYear,
		RID_BUILTINPARAM_DEFAULT_FOUR
	},
	{
		NAME_BUILTIN_DATE_OBJECT_SETMINUTES,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getYear,
		RID_BUILTINPARAM_DEFAULT_THREE
	},
	{
		NAME_BUILTIN_DATE_OBJECT_SETMONTH,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getYear,
		RID_BUILTINPARAM_DEFAULT_TWO
	},
	{
		NAME_BUILTIN_DATE_OBJECT_SETSECONDS,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getYear,
		RID_BUILTINPARAM_DEFAULT_TWO
	},
	{
		NAME_BUILTIN_DATE_OBJECT_SETYEAR,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getYear,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_DATE_OBJECT_SETDATE,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::getYear,
		RID_BUILTINPARAM_DEFAULT_ONE
	},
	{
		NAME_BUILTIN_DATE_OBJECT_TOGMTSTRING,
		METHOD_NATIVE,
		(PMETHOD)&CDateObject::toString,
		RID_BUILTINPARAM_DEFAULT_ZERO
	},
};

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Initialize
 ***************************************************************************/
bool CDateObject::InitializeCodeGen( ICodeGen& CodeGen )
{
	bool bReturn = true;

	RID ridObject = CodeGen.registerBuiltinObject( NAME_BUILTIN_DATE_OBJECT );
	if( ridObject == RID_NOTDEFINED )
		return bReturn;

	//Initialize Global 'Date' object ref
	RID rid = CodeGen.getMetaData().generateStaticFieldRID( OPERAND_OBJECTREF,
															OPERAND_FLAG(
																	OPERAND_FLAG_DONTDELETE
																	| OPERAND_FLAG_DONTENUM
																	| OPERAND_FLAG_READONLY ) );
	CodeGen.generateSymbolInformation( rid, NAME_BUILTIN_DATE_OBJECT, CG_SYMBOL_FLAG_READONLY, ridObject );

	//---------------------------------------------------------------------------
	// Initialize Date.call/ctor property
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
bool CDateObject::InitializeVm( CCilVm& vm )
{
	RID rid = vm.createPrototypeObject( vm.getDateObjectRID(),
										vm.getMetaData().getObjectNameRID( vm.getDateObjectRID() ),
										vm.getObjectRID() );

	assert( rid == vm.getDateObjectRID() );

	//---------------------------------------------------------------------------
	//Bind global object
	//---------------------------------------------------------------------------
	CVmObject* pObj = vm.createObject( vm.getDateObjectRID() );
	CVariable obj( pObj, OPERAND_FLAG( OPERAND_FLAG_DONTDELETE
									| OPERAND_FLAG_DONTENUM
									| OPERAND_FLAG_READONLY ) );

	//Set true as an object value..
	CVariable var( true );
	pObj->setValue( var );

	CG_SYMBOL_INFORMATION info = vm.querySymbolInformation( NAME_BUILTIN_DATE_OBJECT );
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
 *      ctor
 ***************************************************************************/
void CDateObject::ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );
	CVariable& thisobj = *pArguments;
	assert( OperandType(thisobj.iOperandType) == OPERAND_OBJECTREF );

	//Set prototype: String prototype object
	thisobj.refObject->setPrototypeObject( 
		&pVm->getPrototypeObject( pVm->getDateObjectRID() ) );

	int32_t iData[] = { 0x7b2, 0, 1, 0, 0, 0, 0 };

	if( iNumArguments == 2 )
	{
		CVariable& varDate = *( pArguments + 1 );

		switch( OPERAND_TYPE( varDate.iOperandType ) )
		{
		case OPERAND_STRING:
		{
			//String version
			//parse the arg
			parse( pVm, iNumArguments, pArguments );

			if( pVm->getEvalStackFirstEntry().iOperandType == OPERAND_NAN )
			{
				//parse failure
				pVm->popEvalStack();
				//Throw TypeError
				pVm->throwException( &wstring( ERRORSTRING_TYPEERROR_STORESTATICFIELD ),
					&wstring( NAME_BUILTIN_ERROR_OBJECT ),
					ERROR_TYPEERROR_STORESTATICFIELD );
				return;
			}
			
			//init property
			CDateObject* pDateObject = new CDateObject;
			//Initialize vars
			pDateObject->setTimeZone( L"PST" );
			pDateObject->m_iTime = iData[ 3 ] * msPerHour
								+ iData[ 4 ] * msPerHour
								+ iData[ 5 ] * msPerSecond
								+ iData[ 6 ];
			pDateObject->m_iTime += TimeFromYearMonth( iData[ 0 ], iData[ 1 ] );
			pDateObject->m_iTime += iData[ 2 ] - 1;

			//Store date_time object to the property
			CVariable varObject;
			varObject.pPointerInternal = pDateObject;
			varObject.iOperandType = OPERAND_INT;
			varObject.iOperandFlag = OPERAND_FLAG( OPERAND_FLAG_DONTDELETE |
												OPERAND_FLAG_DONTENUM |
												OPERAND_FLAG_READONLY );
			thisobj.refObject->setProperty( NAME_BUILTIN_DATE_PROPERTY_DATA1, varObject );

			//Create arg list
			CVariable& var = pVm->getEvalStackFirstEntry();
			CVariable vars[ 2 ];
			vars[ 0 ] = thisobj;
			vars[ 1 ] = var;

			//Set time
			setTime( pVm, 2, vars );

			//Set number value to the object
			CVariable varInt( 0 );
			thisobj.refObject->setValue( varInt );

			pVm->popEvalStack();
			pVm->popEvalStack();
			return;
		}
		case OPERAND_OBJECTREF:
			{
			//init property
			CDateObject* pDateObject = new CDateObject;
			//Initialize vars
			pDateObject->setTimeZone( L"PST" );
			//Convert to int64
			pDateObject->m_iTime = varDate.toInt64();
			
			CVariable varObject;
			varObject.pPointerInternal = pDateObject;
			varObject.iOperandType = OPERAND_INT;
			varObject.iOperandFlag = OPERAND_FLAG( OPERAND_FLAG_DONTDELETE |
													OPERAND_FLAG_DONTENUM |
													OPERAND_FLAG_READONLY );
			//Store date_time object to the property
			thisobj.refObject->setProperty( NAME_BUILTIN_DATE_PROPERTY_DATA1,
										varObject );

			//Create arg list
			CVariable& var = pVm->getEvalStackFirstEntry();
			CVariable vars[ 2 ];
			vars[ 0 ] = thisobj;
			vars[ 1 ] = var;

			//Set time
			setTime( pVm, 2, vars );

			//Set number value to the object
			thisobj.refObject->setValue( varDate );

			pVm->popEvalStack();
			return;
			}
		default:
			{
			//init property
			CDateObject* pDateObject = new CDateObject;
			//Initialize vars
			pDateObject->setTimeZone( L"PST" );
			pDateObject->m_iTime = varDate.i64Value;
			
			CVariable varObject;
			varObject.pPointerInternal = pDateObject;
			varObject.iOperandType = OPERAND_INT;
			varObject.iOperandFlag = OPERAND_FLAG( OPERAND_FLAG_DONTDELETE |
													OPERAND_FLAG_DONTENUM |
													OPERAND_FLAG_READONLY );
			//Store date_time object to the property
			thisobj.refObject->setProperty( NAME_BUILTIN_DATE_PROPERTY_DATA1,
										varObject );

			//Create arg list
			CVariable& var = pVm->getEvalStackFirstEntry();
			CVariable vars[ 2 ];
			vars[ 0 ] = thisobj;
			vars[ 1 ] = var;

			//Set time
			setTime( pVm, 2, vars );

			//Set number value to the object
			thisobj.refObject->setValue( varDate );

			pVm->popEvalStack();
			return;
			}
		}
	}
	int32_t iArgumentNumber = iNumArguments - 1;
	if( iArgumentNumber )
	{
		pArguments ++;
		CVariable& varDate = *pArguments;

		int32_t i = 0;
		while( i < ARRAYSIZE( iData ) && iArgumentNumber )
		{
			varDate = *pArguments;
			pArguments++;

			iData[ i ] = varDate.toInt(); 
			i ++;
			iArgumentNumber--;
		}
	}

	CDateObject* pDateObject = new CDateObject;
	//Initialize vars
	pDateObject->setTimeZone( L"PST" );
	
	pDateObject->m_iTime = iData[ 3 ] * msPerHour + iData[ 4 ] * msPerHour + iData[ 5 ] * msPerSecond + iData[ 6 ];
	pDateObject->m_iTime += TimeFromYearMonth( iData[ 0 ], iData[ 1 ] );
	pDateObject->m_iTime += (iData[ 2 ] - 1 ) * msPerDay;
	
	//TO UTC
	pDateObject->m_iTime += pDateObject->m_LocalTZA;	//Time zone adjuestment

	//Store date_time object to the property
	CVariable varObject;
	varObject.pPointerInternal = pDateObject;
	varObject.iOperandType = OPERAND_INT;
	varObject.iOperandFlag = OPERAND_FLAG( OPERAND_FLAG_DONTDELETE |
											OPERAND_FLAG_DONTENUM |
											OPERAND_FLAG_READONLY );
	thisobj.refObject->setProperty( NAME_BUILTIN_DATE_PROPERTY_DATA1,
								varObject );
	return;
}

/***************************************************************************
 *      call method
 ***************************************************************************/
void CDateObject::call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );
	CVariable obj;

	//Push the ObjectRef to a eval stack
	pVm->pushEvalStackString( L"Current time:)" );
	return;
}

/***************************************************************************
 *      toString
 ***************************************************************************/
void CDateObject::toString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );
	CVariable& thisobj = *pArguments;

	if( OperandType( pArguments->iOperandType ) != OPERAND_OBJECTREF
		|| pArguments->refObject->getRID() != pVm->getDateObjectRID() )
	{
		//Throw TypeError
		pVm->throwException( &wstring( ERRORSTRING_TYPEERROR_DATE_TOSTRING ),
			&wstring( NAME_BUILTIN_ERROR_OBJECT ),
			ERROR_TYPEERROR_DATE_TOSTRING );
		return;
	}

	//toString
	//Retrieve info
	CVariable* pvar = thisobj.refObject->lookupProperty( NAME_BUILTIN_DATE_PROPERTY_DATA1 );
	if( !pvar || OperandType( pvar->iOperandType ) != OPERAND_INT )
	{
		pVm->pushEvalStackString( L"" );
		return;
	}
	
	CDateObject* pDate = (CDateObject*)pvar->pPointerInternal;

	int64_t iTime = pDate->m_iTime;
	
	//From UTC to local time
	iTime += pDate->m_LocalTZA;	//Time zone adjuestment

	int32_t iWeekDay = CDateObject::WeekDay( iTime );
	wchar_t strDate[ 32 ];
/*
 //To keep compatibility between GNU and VS
 swprintf( strDate, 32, L"%s %s %d %02d:%02d:%02d %s %d",
						strWeekday[ iWeekDay ],					//Weekday
						strMonth[ CDateObject::MonthFromTime( iTime ) ],	//Month
						CDateObject::DateFromTime( iTime ),		//Date
						CDateObject::HourFromTime( iTime ),		//Hour
						CDateObject::MinFromTime( iTime ),		//Min
						CDateObject::SecFromTime( iTime ),		//Sec
						TimeZoneSettings[ pDate->m_currentTimeZoneIndex ].strTimeZone,	//Timezone
						CDateObject::YearFromTime( iTime )		//Year
						);
*/
	wstring* pStr = new wstring( strWeekday[ iWeekDay ] );
	pStr->append( L" " );
	pStr->append( strMonth[ CDateObject::MonthFromTime( iTime ) ] );
	pStr->append( L" " );
	swprintf( strDate, 32, L"%d %02d:%02d:%02d ",
			 CDateObject::DateFromTime( iTime ),		//Date
			 CDateObject::HourFromTime( iTime ),		//Hour
			 CDateObject::MinFromTime( iTime ),		//Min
			 CDateObject::SecFromTime( iTime )		//Sec
			 );
	pStr->append( strDate );
	pStr->append( TimeZoneSettings[ pDate->m_currentTimeZoneIndex ].strTimeZone );
	swprintf( strDate, 32, L" %d",
			 CDateObject::YearFromTime( iTime )		//Year
			 );
	pStr->append( strDate );

	//pVm->pushEvalStackString( strDate );
	pVm->pushEvalStackStringPointer( pStr );

	return;
}

/***************************************************************************
 *      parse
 ***************************************************************************/
void CDateObject::parse( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );
	//Skip this
	pArguments ++;
	
	CVariable& varString = *pArguments;
	varString.convertToString( pVm );

	//parse
	int32_t iData[] = { 0x7b2, 0, 1, 0, 0, 0, 0 };

	//wchar_t *strTZ;
	int32_t i = swscanf_s( varString.pString->data(), L"%d/%d/%d %d:%d:%d",
				&iData[ 1 ],
				&iData[ 2 ],
				&iData[ 0 ],
				&iData[ 3 ],
				&iData[ 4 ],
				&iData[ 5 ]);
	if( i != 6 )
	{
		//parse failure
		//Push to the time value to the stack
		pVm->pushEvalStackNaN();
		return;
	}

	//Month is 0-11
	iData[ 1 ] --;	//Data range adjustment

	//Calc time value
	int64_t iTime;

	iTime = iData[ 3 ] * msPerHour + iData[ 4 ] * msPerHour + iData[ 5 ] * msPerSecond + iData[ 6 ];
	iTime += iData[ 2 ] * msPerDay;
	iTime += TimeFromYearMonth( iData[ 0 ], iData[ 1 ] );

	//Push to the time value to the stack
	pVm->pushEvalStack( iTime );

	return;
}

/***************************************************************************
 *      setTime
 ***************************************************************************/
void CDateObject::setTime( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );
	CVariable& thisobj = *pArguments;
	assert( OperandType(thisobj.iOperandType) == OPERAND_OBJECTREF );

	int64_t iTime = 0;
	if( iNumArguments > 1 )
	{
		pArguments ++;
		CVariable& varDate = *pArguments;
		iTime = varDate.toInt64();

		//Store date_time object to the property		
		CVariable* pvar = thisobj.refObject->lookupProperty( NAME_BUILTIN_DATE_PROPERTY_DATA1 );
		assert( pvar != NULL );
		CDateObject* pDateObject = (CDateObject*)pvar->pPointerInternal;
		//Initialize vars

		pDateObject->m_iTime = iTime;

		CVariable varObject;
		varObject.pPointerInternal = pDateObject;
		varObject.iOperandType = OPERAND_INT;
		varObject.iOperandFlag = OPERAND_FLAG( OPERAND_FLAG_DONTDELETE |
											OPERAND_FLAG_DONTENUM |
											OPERAND_FLAG_READONLY );

		//Store date_time object to the property
		thisobj.refObject->setProperty( NAME_BUILTIN_DATE_PROPERTY_DATA1,
									varObject );

	}

	//Push to the time value to the stack
	pVm->pushEvalStack( iTime );
	return;
}

/***************************************************************************
 *      getTimezoneOffset
 ***************************************************************************/
void CDateObject::getTimezoneOffset( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );
	CVariable& thisobj = *pArguments;
	assert( OperandType(thisobj.iOperandType) == OPERAND_OBJECTREF );

	CVariable* pvar = thisobj.refObject->lookupProperty( NAME_BUILTIN_DATE_PROPERTY_DATA1 );
	assert( pvar != NULL );
	CDateObject* pDate = (CDateObject*)pvar->pPointerInternal;
	
	pVm->pushEvalStack( -pDate->m_LocalTZA / msPerMinute );
	return;
}

/***************************************************************************
 *      getTime
 ***************************************************************************/
void CDateObject::getTime( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );
	CVariable& thisobj = *pArguments;
	assert( OperandType(thisobj.iOperandType) == OPERAND_OBJECTREF );

	//Store date_time object to the property
	CVariable* pvar = thisobj.refObject->lookupProperty( NAME_BUILTIN_DATE_PROPERTY_DATA1 );
	assert( pvar != NULL );
	CDateObject* pDate = (CDateObject*)pvar->pPointerInternal;

	int64_t iTime = pDate->m_iTime;
	
	//Pur time value onto the stack
	pVm->pushEvalStack( iTime );
	return;
}

/***************************************************************************
 *      getDate
 ***************************************************************************/
void CDateObject::getDate( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );
	CVariable& thisobj = *pArguments;
	assert( OperandType(thisobj.iOperandType) == OPERAND_OBJECTREF );

	//Store date_time object to the property
	CVariable* pvar = thisobj.refObject->lookupProperty( NAME_BUILTIN_DATE_PROPERTY_DATA1 );
	assert( pvar != NULL );
	CDateObject* pDate = (CDateObject*)pvar->pPointerInternal;

	int64_t iTime = pDate->m_iTime;
	
	//From UTC to local time
	iTime += pDate->m_LocalTZA;	//Time zone adjuestment
	pVm->pushEvalStack( CDateObject::DateFromTime( iTime ) );
	return;
}

/***************************************************************************
 *      getYear
 ***************************************************************************/
void CDateObject::getYear( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	Debug_Fatal( "Not implemented yet" );
	return;
}
/***************************************************************************
 *      UTC
 ***************************************************************************/
void CDateObject::UTC( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments )
{
	assert( iNumArguments >= 1 );

	CVariable& thisobj = *pArguments;
	assert( OperandType(thisobj.iOperandType) == OPERAND_OBJECTREF );

	int32_t iData[] = { 0x7b2, 0, 1, 0, 0, 0, 0 };
	
	int32_t iArgumentNumber = iNumArguments - 1;
	if( iArgumentNumber )
	{
		pArguments ++;

		CVariable& varDate = *pArguments;
		if( OPERAND_TYPE( varDate.iOperandType ) == OPERAND_STRING )
		{
			Debug_Fatal( "Not supported yet" );
		}
		int32_t i = 0;
		while( i < ARRAYSIZE( iData ) && iArgumentNumber )
		{
			varDate = *pArguments;
			pArguments++;

			iData[ i ] = varDate.toInt(); 
			i ++;
			iArgumentNumber--;
		}
	}

	//Adjust value
	if( iData[ 0 ] <= 99 )
		iData[ 0 ] += 1900;

	//Make day
	//Calc time value
	int64_t iTime;

	iTime = iData[ 3 ] * msPerHour + iData[ 4 ] * msPerHour + iData[ 5 ] * msPerSecond + iData[ 6 ];
	iTime += (iData[ 2 ] - 1 )* msPerDay;
	iTime += TimeFromYearMonth( iData[ 0 ], iData[ 1 ] );

	//Push to the time value to the stack
	pVm->pushEvalStack( iTime );

	return;
}

/***************************************************************************
 *      Helpers
 ***************************************************************************/
int64_t CDateObject::Day( int64_t t )
{
	int64_t day = ( t - msPerDay + 1 ) / msPerDay;
	return day;
}

int64_t CDateObject::TimeWithinDay( int64_t t )
{
	return t % msPerDay;
}

//DaysInYear(y) = 365 if (y modulo 4) ÅÇ 0
//= 366 if (y modulo 4) = 0 and (y modulo 100) ÅÇ 0
//= 365 if (y modulo 100) = 0 and (y modulo 400) ÅÇ 0
//= 366 if (y modulo 400) = 0
int32_t CDateObject::DaysInYear( int32_t y )
{
	if( y % 4 ) return 365;
	else if( ( y % 4 == 0 ) && ( y % 100 ) ) return 366;
	else if( ( y % 100 == 0 ) && ( y % 400 ) ) return 365;
	else if( y % 400 == 0 ) return 366;

	return 365;
}

int32_t CDateObject::DayFromYear( int32_t y )
{
	return 365*(y-1970)+((y-1969)/4)-((y-1901)/100)+((y-1601)/400);
}

//TimeFromYear(y) =msPerDay Å~ DayFromYear(y)
int64_t CDateObject::TimeFromYear( int32_t y )
{
	int64_t i64Day = DayFromYear( y );
	return msPerDay * i64Day;
}

//YearFromTime(t) = the largest integer y (closest to positive infinity) such that TimeFromYear(y) <= t
int32_t CDateObject::YearFromTime( int64_t t )
{
	int32_t iYearEstimate = int32_t( t / (365 * int64_t(msPerDay)) ) + 1970 - 1;
	int32_t iTmp = 1;
	
	while( TimeFromYear( iYearEstimate + iTmp ) < t )
	{
		iTmp ++;
	}
	return iYearEstimate + iTmp - 1;
}

//InLeapYear(t) = 0 if DaysInYear(YearFromTime(t)) = 365
//= 1 if DaysInYear(YearFromTime(t)) = 366
int32_t CDateObject::InLeapYear( int64_t t )
{
	if( DaysInYear(YearFromTime(t)) == 365 )
		return 0;
	else
		return 1;
}

/*
MonthFromTime(t) =0 if 0 <= DayWithinYear(t) < 31
= 1 if 31 <= DayWithinYear (t) < 59+InLeapYear(t)
= 2 if 59+InLeapYear(t) <= DayWithinYear (t) < 90+InLeapYear(t)
= 3 if 90+InLeapYear(t) <= DayWithinYear (t) < 120+InLeapYear(t)
= 4 if 120+InLeapYear(t) <= DayWithinYear (t) < 151+InLeapYear(t)
= 5 if 151+InLeapYear(t) <= DayWithinYear (t) < 181+InLeapYear(t)
= 6 if 181+InLeapYear(t) <= DayWithinYear (t) < 212+InLeapYear(t)
= 7 if 212+InLeapYear(t) <= DayWithinYear (t) < 243+InLeapYear(t)
= 8 if 243+InLeapYear(t) <= DayWithinYear (t) < 273+InLeapYear(t)
= 9 if 273+InLeapYear(t) <= DayWithinYear (t) < 304+InLeapYear(t)
= 10 if 304+InLeapYear(t) <= DayWithinYear (t) < 334+InLeapYear(t)
= 11 if 334+InLeapYear(t) <= DayWithinYear (t) < 365+InLeapYear(t)
*/
int32_t CDateObject::MonthFromTime( int64_t t )
{
	int32_t i = DayWithinYear(t);
	int32_t iLeap = InLeapYear(t);
	if( i >= 0 && i < 31 ) return 0;
	else if( i >= 31 + iLeap  && i < 59 + iLeap ) return 1;
	else if( i >= 59 + iLeap  && i < 90 + iLeap ) return 2;
	else if( i >= 90 + iLeap  && i < 120 + iLeap ) return 3;
	else if( i >= 120 + iLeap  && i < 151 + iLeap ) return 4;
	else if( i >= 181 + iLeap  && i < 181 + iLeap ) return 5;
	else if( i >= 212 + iLeap  && i < 212 + iLeap ) return 6;
	else if( i >= 243 + iLeap  && i < 243 + iLeap ) return 7;
	else if( i >= 273 + iLeap  && i < 273 + iLeap ) return 8;
	else if( i >= 273 + iLeap  && i < 304 + iLeap ) return 9;
	else if( i >= 304 + iLeap  && i < 334 + iLeap ) return 10;
	else if( i >= 334 + iLeap  && i < 365 + iLeap ) return 11;

	Debug_Fatal( "Illegal parameter" );
	return -1;
}

int32_t CDateObject::DayWithinYear( int64_t t )
{
	return int32_t( Day(t) - DayFromYear(YearFromTime(t)) );
}

//DateFromTime(t) = DayWithinYear(t)+1 if MonthFromTime(t)=0
//= DayWithinYear(t)-30 if MonthFromTime(t)=1
//= DayWithinYear(t)-58-InLeapYear(t) if MonthFromTime(t)=2
//= DayWithinYear(t)-89-InLeapYear(t) if MonthFromTime(t)=3
//= DayWithinYear(t)-119-InLeapYear(t) if MonthFromTime(t)=4
//= DayWithinYear(t)-150-InLeapYear(t) if MonthFromTime(t)=5
//= DayWithinYear(t)-180-InLeapYear(t) if MonthFromTime(t)=6
//= DayWithinYear(t)-211-InLeapYear(t) if MonthFromTime(t)=7
//= DayWithinYear(t)-242-InLeapYear(t) if MonthFromTime(t)=8
//= DayWithinYear(t)-272-InLeapYear(t) if MonthFromTime(t)=9
//= DayWithinYear(t)-303-InLeapYear(t) if MonthFromTime(t)=10
//= DayWithinYear(t)-333-InLeapYear(t) if MonthFromTime(t)=11
int32_t CDateObject::DateFromTime( int64_t t )
{
	int32_t iDay = DayWithinYear(t);
	int32_t iLeap = InLeapYear(t);
	int32_t iMonth = MonthFromTime(t);
	int32_t iReturn = -1;

	const int32_t iDateTable[] = { 1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333 };
	switch( iMonth )
	{
	case 0:
		iReturn = iDay + 1;
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		iReturn = iDay - iDateTable[ iMonth ] - iLeap;
		break;
	default:
		Debug_Fatal( "Illegal parameter" );
		break;
	}
	return iReturn;
}

int32_t CDateObject::WeekDay( int64_t t )
{
	return int32_t((Day(t) + 4) % 7);
}

//LocalTime(t) = t + LocalTZA + DaylightSavingTA(t)
int32_t CDateObject::LocalTime( int64_t t )
{
	return int32_t(t + m_LocalTZA + DaylightSavingTA(t));
}

//UTC(t) = t - LocalTZA - DaylightSavingTA(t - LocalTZA)
int32_t CDateObject::ToUTC( int64_t t )
{
	return int32_t(t - m_LocalTZA - DaylightSavingTA(t - m_LocalTZA));
}

int32_t CDateObject::HourFromTime( int64_t t )
{
	return int32_t(((t / msPerHour) + HoursPerDay ) % HoursPerDay);
}

int32_t CDateObject::MinFromTime( int64_t t )
{
	return int32_t(((t / msPerMinute) + MinutesPerHour ) % MinutesPerHour);
}

int32_t CDateObject::SecFromTime( int64_t t )
{
	return int32_t(((t / msPerSecond) + SecondsPerMinute ) % SecondsPerMinute);
}

int32_t CDateObject::msFromTime( int64_t t )
{
	return int32_t(t % msPerSecond);
}

int32_t CDateObject::DaylightSavingTA( int64_t )
{
	return 0;
}

int64_t CDateObject::TimeFromYearMonth( int32_t iYear, int32_t iMonth )
{
	int64_t iTime = TimeFromYear( iYear );

	int64_t iDay = 0;
	int32_t iLeap = ( DaysInYear ( iYear ) != 365 );
	
	const int32_t iDateTable[] = { 1, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
	switch( iMonth )
	{
	case 0:
		iDay = 0;
		break;
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		iDay = iDateTable[ iMonth ] + iLeap;
		break;
	default:
		Debug_Fatal( "Illegal parameter" );
		break;
	}

	return iTime + iDay * msPerDay;
}

} //namespace CRI