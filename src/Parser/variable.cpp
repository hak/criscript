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
 * Module   : cil Virtual Machine/CodeGen
 * File     : Variable.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      CVariable::ctor
 ***************************************************************************/
CVariable::CVariable() : iOperandType(OPERAND_UNDEFINED), iOperandFlag( OPERAND_FLAG_NONE ), i64Value( 0 ),
							 	ridConstraintType( 0 )

{
};

CVariable::CVariable( const int32_t i )// : i64Value(0)
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_INT;
#else
	iOperandType = OPERAND_INT;
	iOperandFlag = OPERAND_FLAG_NONE;
	ridConstraintType = 0;
#endif
	iValue = i;
};

CVariable::CVariable( const int64_t i )
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_INT64;
#else
	iOperandType = OPERAND_INT64;
	iOperandFlag = OPERAND_FLAG_NONE;
	ridConstraintType = 0;
#endif
	i64Value = i;
};

CVariable::CVariable( const float f )// : i64Value(0)
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_FLOAT;
#else
	iOperandType = OPERAND_FLOAT;
	iOperandFlag = OPERAND_FLAG_NONE;
	ridConstraintType = 0;
#endif
	fValue = f;
};
	
CVariable::CVariable( const double d )
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_DOUBLE;
#else
	iOperandType = OPERAND_DOUBLE;
	iOperandFlag = OPERAND_FLAG_NONE;
	ridConstraintType = 0;
#endif
	dValue = d;
};

CVariable::CVariable( const VMOBJECTREF ref )// : i64Value(0)
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_OBJECTREF;
#else
	iOperandType = OPERAND_OBJECTREF;
	iOperandFlag = OPERAND_FLAG_NONE;
	ridConstraintType = 0;
#endif
	refObject = ref;
	if( refObject )
		refObject->addRef();
};

CVariable::CVariable( wstring* const p )// : i64Value(0)
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_STRING;
#else
	iOperandType = OPERAND_STRING;
	iOperandFlag = OPERAND_FLAG_NONE;
	ridConstraintType = 0;
#endif
	pString = new wstring( p->data(), p->length() );
};

CVariable::CVariable( const bool b )// : i64Value(0)
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_BOOLEAN;
#else
	iOperandType = OPERAND_BOOLEAN;
	iOperandFlag = OPERAND_FLAG_NONE;
	ridConstraintType = 0;
#endif
	iValue = int32_t(b);
};

CVariable::CVariable( const wchar_t * const p )// : i64Value(0)
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_STRING;
#else
	iOperandType = OPERAND_STRING;
	iOperandFlag = OPERAND_FLAG_NONE;
	ridConstraintType = 0;
#endif
	pString = new wstring( p );
};

CVariable::CVariable( const int32_t i, const OPERAND_FLAG flag )// : i64Value(0)
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_INT;
#else
	iOperandType = OPERAND_INT;
	ridConstraintType = 0;
#endif
	iOperandFlag = flag;
	iValue = i;
};

CVariable::CVariable( const float f, const OPERAND_FLAG flag )// : i64Value(0)
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_FLOAT;
#else
	iOperandType = OPERAND_FLOAT;
	ridConstraintType = 0;
#endif
	iOperandFlag = flag;
	fValue = f;
};

CVariable::CVariable( const double d, const OPERAND_FLAG flag )
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_DOUBLE;
#else
	iOperandType = OPERAND_DOUBLE ;
	ridConstraintType = 0;
#endif
	iOperandFlag = flag;
	dValue = d;
};

CVariable::CVariable( const VMOBJECTREF ref, const OPERAND_FLAG flag )// : i64Value(0)
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_OBJECTREF;
#else
	iOperandType = OPERAND_OBJECTREF;
	ridConstraintType = 0;
#endif
	iOperandFlag = flag;
	refObject = ref;
	if( refObject )
		refObject->addRef();
};

CVariable::CVariable( wstring* const p, const OPERAND_FLAG flag )// : i64Value(0)
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_STRING;
#else
	iOperandType = OPERAND_STRING;
	ridConstraintType = 0;
#endif
	iOperandFlag = flag;
	pString = new wstring( p->data(), p->length() );
};

CVariable::CVariable( const bool b, const OPERAND_FLAG flag )// : i64Value(0)
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_BOOLEAN;
#else
	iOperandType = OPERAND_BOOLEAN;
	ridConstraintType = 0;
#endif
	iOperandFlag = flag;
	bValue = b;
};

CVariable::CVariable( const wchar_t * const p, const OPERAND_FLAG flag )// : i64Value(0)
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_STRING;
#else
	iOperandType = OPERAND_STRING;
	ridConstraintType = 0;
#endif
	iOperandFlag = flag;
	pString = new wstring( p );
};

CVariable::CVariable( const OPERAND_FLAG flag )// : i64Value(0)
{
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_UNDEFINED;
#else
	iOperandType = OPERAND_UNDEFINED;
	ridConstraintType = 0;
#endif
	iOperandFlag = flag ;
};
	
/***************************************************************************
 *      CVariable::setFlag
 ***************************************************************************/
void CVariable::setFlag( const OPERAND_FLAG flag )
{
	iOperandFlag = flag;
}

/***************************************************************************
 *      CVariable::Copy ctor
 ***************************************************************************/
CVariable::CVariable( const CVariable& right )
{
	//iOperandType = right.iOperandType;
	//iOperandFlag = right.iOperandFlag;
	//ridConstraintType = right.ridConstraintType;
	iPrameters = right.iPrameters;

	//If already keeping a object ref...
	switch( OperandType( iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		//i64Value = 0;
		refObject = right.refObject;
		if( refObject )
		{
			assert( refObject->getRefCount() );
			refObject->addRef();
		}break;
	case OPERAND_STRING:
		//i64Value = 0;
		pString = new wstring( *right.pString );
		break;
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
	case OPERAND_DOUBLE:
	default:
		i64Value = right.i64Value;
		break;
	}
};

/***************************************************************************
 *      CVariable::Operator=
 ***************************************************************************/
CVariable& CVariable::operator=( const CVariable& right )
{
	//Check if the variable is READONLY
	if( iOperandFlag & OPERAND_FLAG_READONLY )
		return *this;

	if( OperandType( right.iOperandType ) == OPERAND_SYSTEM_DELETION )
	{
		if( iOperandFlag & OPERAND_FLAG_DONTDELETE )
			return *this;
		else
		{
			//If already keeping a object ref...
			switch( OperandType( iOperandType ) )
			{
			case OPERAND_OBJECTREF:
				if( refObject )
					refObject->release();
				break;
			case OPERAND_STRING:
				delete pString;	//Delete old string
				break;
			default:
				break;
			}
			iOperandType = OPERAND_UNDEFINED;
			iOperandFlag = OPERAND_FLAG_DONTENUM;
			return *this;
		}
	}

	//If this already keeps an object ref...
	switch( OperandType( iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		if( refObject )
			refObject->release();
		break;
	case OPERAND_STRING:
		delete pString;	//Delete old string
		break;
	default:
		break;
	}

	//iOperandType = (OPERAND_TYPE)right.iOperandType;
	//iOperandFlag = right.iOperandFlag;
	iPrameters = right.iPrameters;

	switch( OperandType( right.iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		refObject = right.refObject;
		//ridConstraintType = right.ridConstraintType;
		if( refObject )
		{
			refObject->addRef();
		}
		break;
	case OPERAND_STRING:
		pString = new wstring( *right.pString );
		break;
	case OPERAND_DOUBLE:
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
		i64Value = right.i64Value;
		break;
	default:
		iValue = right.iValue;
		break;
	}

	return *this;
}

/***************************************************************************
 *      CVariable::moveTo
 *		Copy a pointer/ref without tweaking ref count, 
 ***************************************************************************/
void CVariable::moveTo( CVariable& dest )
{
	//Check if the variable is READONLY
	if( dest.iOperandFlag & OPERAND_FLAG_READONLY )
		return;

	uint32_t iOperand = OperandType( iOperandType );
	switch( iOperand )
	{
	case OPERAND_SYSTEM_DELETION:
		if( dest.iOperandFlag & OPERAND_FLAG_DONTDELETE )
			return;
		else
		{
			dest.iOperandType = OPERAND_UNDEFINED;
			dest.iOperandFlag = OPERAND_FLAG_DONTENUM;
		}
		break;
	case OPERAND_STRING:
		dest.iOperandType = (OPERAND_TYPE)iOperand;
		dest.pString = pString;
		pString = NULL;	//Clear original pointer not to be deleted
		break;
	case OPERAND_OBJECTREF:
		dest.iOperandType = (OPERAND_TYPE)iOperand;
		
		//Should not override restrict type
		//dest.ridConstraintType = ridConstraintType;
		
		//Does not want to invoke Copy ctor	
		dest.iValue = iValue;
		assert( dest.refObject->getRefCount() );
		refObject = NULL;	//Clear original pointer not to be deleted
		break;
	default:
		dest.iOperandType = (OPERAND_TYPE)iOperand;
		dest.i64Value = i64Value;
		break;
	}
}

/***************************************************************************
 *      CVariable::moveToWithFlags
 *		Copy a pointer/ref without tweaking ref count, 
 ***************************************************************************/
void CVariable::moveToWithFlags( CVariable& dest )
{
	//Check if the variable is READONLY
	if( dest.iOperandFlag & OPERAND_FLAG_READONLY )
		return;

	uint32_t iOperand = OperandType( iOperandType );
	switch( iOperand )
	{
	case OPERAND_SYSTEM_DELETION:
		if( dest.iOperandFlag & OPERAND_FLAG_DONTDELETE )
			return;
		else
		{
			dest.iOperandType = OPERAND_UNDEFINED;
			dest.iOperandFlag = OPERAND_FLAG_DONTENUM;
		}
		break;
	case OPERAND_STRING:
		dest.iPrameters = iPrameters;
		dest.pString = pString;
		pString = NULL;	//Clear original pointer not to be deleted
		break;
	case OPERAND_OBJECTREF:
		dest.iOperandType = (OPERAND_TYPE)iOperand;
		dest.iOperandFlag = iOperandFlag;
		
		//Should not override restrict type
		//dest.ridConstraintType = ridConstraintType;
		
		//Does not want to invoke Copy ctor	
		dest.iValue = iValue;
		assert( dest.refObject->getRefCount() );
		refObject = NULL;	//Clear original pointer not to be deleted
		break;
	case OPERAND_DOUBLE:
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
		dest.iPrameters = iPrameters;
		dest.i64Value = i64Value;
		break;
	default:
		dest.iPrameters = iPrameters;
		dest.iValue = iValue;
		break;
	}
}

/***************************************************************************
 *      CVariable::SetObject reference and increment ref count
 ***************************************************************************/
void CVariable::setObjectRef( const VMOBJECTREF obj )
{
	//Check if the variable is READONLY
	if( iOperandFlag & OPERAND_FLAG_READONLY )
		return;

	//If already keeping a object ref...
	switch( OperandType( iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		if( refObject )
			refObject->release();
		break;
	case OPERAND_STRING:
		delete pString;	//Delete old string
		break;
	default:
		break;
	}

#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_OBJECTREF;
#else
	iOperandType = OPERAND_OBJECTREF;
	iOperandFlag = OPERAND_FLAG_NONE;
	ridConstraintType = 0;
#endif
	refObject = obj;
	refObject->addRef();
}

/***************************************************************************
 *      CVariable::setObjectRefWithFlags reference and increment ref count
 ***************************************************************************/
void CVariable::setObjectRefWithFlags( const VMOBJECTREF obj, const OPERAND_FLAG flag, const RID ridConstraint )
{
	//Check if the variable is READONLY
	if( iOperandFlag & OPERAND_FLAG_READONLY )
		return;

	//If already keeping a object ref...
	switch( OperandType( iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		if( refObject )
			refObject->release();
		break;
	case OPERAND_STRING:
		delete pString;	//Delete old string
		break;
	default:
		break;
	}

	iOperandType = OPERAND_OBJECTREF;
	iOperandFlag = flag;
	ridConstraintType = ridConstraint;
	refObject = obj;
	refObject->addRef();
}

/***************************************************************************
 *      CVariable::setString set string value to the variable
 ***************************************************************************/
void CVariable::setString( const wstring& str )
{
	//Check if the variable is READONLY
	if( iOperandFlag & OPERAND_FLAG_READONLY )
		return;

	switch( OperandType( iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		//If already keeping a object ref...
		if( refObject )
			refObject->release();
		break;
	case OPERAND_STRING:
		delete pString;
		break;
	default:
		break;
	}
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_STRING;
#else
	iOperandType = OPERAND_STRING;
	iOperandFlag = OPERAND_FLAG_NONE;
#endif
	pString = new wstring( str );

}

/***************************************************************************
 *      CVariable::setInt set integer value to the variable
 ***************************************************************************/
void CVariable::setInt( const int32_t i )
{
	//Check if the variable is READONLY
	if( iOperandFlag & OPERAND_FLAG_READONLY )
		return;

	switch( OperandType( iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		//If already keeping a object ref...
		if( refObject )
			refObject->release();
		break;
	case OPERAND_STRING:
		delete pString;
		break;
	default:
		break;
	}
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_INT;
#else
	iOperandType = OPERAND_INT;
	iOperandFlag = OPERAND_FLAG_NONE;
#endif
	iValue = i;
}

/***************************************************************************
 *      CVariable::setInt64 set Integer value to the variable
 ***************************************************************************/
void CVariable::setInt64( const int64_t i64 )
{
	//Check if the variable is READONLY
	if( iOperandFlag & OPERAND_FLAG_READONLY )
		return;

	switch( OperandType( iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		//If already keeping a object ref...
		if( refObject )
			refObject->release();
		break;
	case OPERAND_STRING:
		delete pString;
		break;
	default:
		break;
	}
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_INT64;
#else
	iOperandType = OPERAND_INT64;
	iOperandFlag = OPERAND_FLAG_NONE;
#endif
	i64Value = i64;
}

/***************************************************************************
 *      CVariable::setFloat set Integer value to the variable
 ***************************************************************************/
void CVariable::setFloat( const float f )
{
	//Check if the variable is READONLY
	if( iOperandFlag & OPERAND_FLAG_READONLY )
		return;

	switch( OperandType( iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		//If already keeping a object ref...
		if( refObject )
			refObject->release();
		break;
	case OPERAND_STRING:
		delete pString;
		break;
	default:
		break;
	}
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_FLOAT;
#else
	iOperandType = OPERAND_FLOAT;
	iOperandFlag = OPERAND_FLAG_NONE;
#endif
	fValue = f;
}

/***************************************************************************
 *      CVariable::setDouble set Double value to the variable
 ***************************************************************************/
void CVariable::setDouble( const double d )
{
	//Check if the variable is READONLY
	if( iOperandFlag & OPERAND_FLAG_READONLY )
		return;

	switch( OperandType( iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		//If already keeping a object ref...
		if( refObject )
			refObject->release();
		break;
	case OPERAND_STRING:
		delete pString;
		break;
	default:
		break;
	}
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_DOUBLE;
#else
	iOperandType = OPERAND_DOUBLE;
	iOperandFlag = OPERAND_FLAG_NONE;
#endif
	dValue = d;
}

/***************************************************************************
 *      CVariable::setStringPointer set string value to the variable
 ***************************************************************************/
void CVariable::setStringPointer( wstring* pstr )
{
	//Check if the variable is READONLY
	if( iOperandFlag & OPERAND_FLAG_READONLY )
		return;

	switch( OperandType( iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		//If already keeping a object ref...
		if( refObject )
			refObject->release();
		break;
	case OPERAND_STRING:
		delete pString;
		break;
	default:
		break;
	}
#ifdef WIN32
	//Assuming bitfield layout is contiguous in memory
	iPrameters = OPERAND_STRING;
#else
	iOperandType = OPERAND_STRING;
	iOperandFlag = OPERAND_FLAG_NONE;
#endif
	pString = pstr;
}

/***************************************************************************
 *      CVariable::dtor
 ***************************************************************************/
CVariable::~CVariable()
{
	switch( OperandType( iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		//Release an object if referenced
		if( refObject )
		{
			refObject->release();
			//if( !refObject->release() )
			//	delete refObject;
			refObject = NULL;
		}
		break;
	case OPERAND_STRING:
		delete pString;	//Delete old string
		pString = NULL;
		break;
	default:
		break;
	}
};

/***************************************************************************
 *      CVariable::toString
 ***************************************************************************/
wstring* CVariable::toString( CCilVm* pVm )
{
	CVariable var = *this;
	var.convertToString( pVm, 10 );
	wstring* pString = var.pString;
	var.pString = NULL;
	return pString;	
}

/***************************************************************************
 *      CVariable::toString
 ***************************************************************************/
wstring* CVariable::toString( CCilVm* pVm, int32_t iRadix )
{
	CVariable var = *this;
	var.convertToString( pVm, iRadix );
	wstring* pString = var.pString;
	var.pString = NULL;
	return pString;
}

/***************************************************************************
 *      CVariable::convertToString
 ***************************************************************************/
void CVariable::convertToString( CCilVm* pVm )
{
	convertToString( pVm, 10 );
}

/***************************************************************************
 *      CVariable::convertToString
 ***************************************************************************/
void CVariable::convertToString( CCilVm* pVm, const int iRadix )
{
	assert( pVm );
	assert( iRadix );
	assert( iRadix <=16 );

	const wchar_t tableChar[] = {
		L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7',
		L'8', L'9', L'a', L'b', L'c', L'd', L'e', L'f',
	};

	wstring* pTmpString;
	int64_t i64Tmp;
	int32_t iIndex;
	float fIndex;
	double dIndex;
	int32_t iPrecision = 0;

	switch( OperandType( iOperandType ) )
	{
		case OPERAND_INT:
		case OPERAND_UNSIGNEDINT:
			pTmpString = new wstring();
			if( iValue < 0 )
			{
				pTmpString->append( L"-" );
				iValue = -iValue;
			}
			iIndex = (int32_t)pTmpString->size();
			while( iValue )
			{
				pTmpString->insert( iIndex, &tableChar[ iValue % iRadix ], 1 );
				iValue = iValue / iRadix;
			}
			
			if( !pTmpString->length() ) pTmpString->append( L"0" );
			pString = pTmpString;
			break;
		case OPERAND_INT64:
		case OPERAND_UNSIGNEDINT64:
			pTmpString = new wstring();
			if( i64Value < 0 )
			{
				pTmpString->append( L"-" );
				i64Value = -i64Value;
			}
			iIndex = (int32_t)pTmpString->size();
			while( i64Value )
			{
				pTmpString->insert( iIndex, &tableChar[ i64Value % iRadix ], 1 );
				i64Value = i64Value / iRadix;
			}
			if( !pTmpString->length() ) pTmpString->append( L"0" );
			pString = pTmpString;
			break;
		case OPERAND_FLOAT:
			if( !_finite( fValue ) )
			{
				if( fValue < 0 )
					pString = new wstring( STRING_INTERNAL_NEGATIVE_INFINITY );
				else
					pString = new wstring( STRING_INTERNAL_INFINITY );
				break;
			}
			else if( fValue > LONG_LONG_MAX || fValue < LONG_LONG_MIN )
			{
				//exponent
				int32_t iExponent = static_cast<int32_t>(log10( fabs( fValue ) ));
				double dMantissa = fValue / pow( 10.0, iExponent );
				CVariable var( dMantissa );
				pString = var.toString( pVm, iRadix );
				pString->append( L"e" );
				CVariable varExp( iExponent );
				pTmpString = varExp.toString( pVm, iRadix );
				pString->append( *pTmpString );
				delete( pTmpString );
				break;
			}
			else	
				pTmpString = new wstring();
			
			if( fValue < 0 )
			{
				pTmpString->append( L"-" );
				fValue = -fValue;
			}
			iIndex = static_cast<int32_t>(pTmpString->size());
			i64Tmp = static_cast<int64_t>(floor( fValue ));
			if( !i64Tmp ) pTmpString->append( L"0" );
			
			while( i64Tmp )
			{
				pTmpString->insert( iIndex, &tableChar[ i64Tmp % iRadix ], 1 );
				i64Tmp = i64Tmp / iRadix;
			}
			fValue = static_cast<float>(modf( fValue, &dIndex ));
			if( fValue ) pTmpString->append( L"." );
			
			while( fValue )
			{
				fValue *= iRadix;
				fValue = modff( fValue, &fIndex );
				if( ++iPrecision >= TOSTRING_PRECISION_SINGLE )
				{
					pTmpString->append( &tableChar[ (int32_t)fIndex ], 1 );
					if( fValue >= 5.f / iRadix )
					{
						wstring::reverse_iterator itBegin = pTmpString->rbegin();
						wstring::reverse_iterator itEnd = pTmpString->rend();
						while( itBegin != itEnd )
						{
							if( *itBegin == L'.'  || *itBegin == L'-' )
							{
								itBegin++;
								continue;
							}
							if( *itBegin - L'0' + 1 < iRadix )
							{
								*itBegin = (*itBegin)+1;
								itBegin = itEnd;
							}
							else
							{
								pTmpString = &pTmpString->erase( pTmpString->length() - 1, 1 );
								itBegin = pTmpString->rbegin();
								itEnd = pTmpString->rend();
							}
						}
					}
					fValue = 0;
				}
				else
					pTmpString->append( &tableChar[ (int32_t)fIndex ], 1 );
			}
			cleanupFloatString( *pTmpString );
			pString = pTmpString;
			break;
		case OPERAND_DOUBLE:
			if( !_finite( dValue ) )
			{
				if( fValue < 0 )
					pString = new wstring( STRING_INTERNAL_NEGATIVE_INFINITY );
				else
					pString = new wstring( STRING_INTERNAL_INFINITY );
				break;
			}
			else if( dValue > LONG_LONG_MAX || dValue < LONG_LONG_MIN )
			{
				//exponent
				int32_t iExponent = static_cast<int32_t>(log10( fabs( dValue ) ));
				double dMantissa = dValue / pow( 10.0, iExponent );
				CVariable var( dMantissa );
				pString = var.toString( pVm, iRadix );
				pString->append( L"e" );
				CVariable varExp( iExponent );
				pTmpString = varExp.toString( pVm, iRadix );
				pString->append( *pTmpString );
				delete( pTmpString );
				break;
			}
			else
				pTmpString = new wstring();

			if( dValue < 0 )
			{
				pTmpString->append( L"-" );
				dValue = -dValue;
			}
			
			iIndex = static_cast<int32_t>(pTmpString->size());

			i64Tmp = static_cast<int64_t>(floor( dValue ));
			if( !i64Tmp ) pTmpString->append( L"0" );

			while( i64Tmp )
			{
				pTmpString->insert( iIndex, &tableChar[ i64Tmp % iRadix ], 1 );
				i64Tmp = i64Tmp / iRadix;
			}
			dValue = modf( dValue, &dIndex );
			if( dValue ) pTmpString->append( L"." );
			
			while( dValue )
			{
				dValue *= iRadix;
				dValue = modf( dValue, &dIndex );
				if( ++iPrecision >= TOSTRING_PRECISION_DOUBLE )
				{
					pTmpString->append( &tableChar[ (int32_t)dIndex ], 1 );
					if( dValue >= 5.f / iRadix )
					{
						wstring::reverse_iterator itBegin = pTmpString->rbegin();
						wstring::reverse_iterator itEnd = pTmpString->rend();
						while( itBegin != itEnd )
						{
							if( *itBegin == L'.'  || *itBegin == L'-' )
							{
								itBegin++;
								continue;
							}
							if( *itBegin - L'0' + 1 < iRadix )
							{
								*itBegin = (*itBegin)+1;
								itBegin = itEnd;
							}
							else
							{
								pTmpString = &pTmpString->erase( pTmpString->length() - 1, 1 );
								itBegin = pTmpString->rbegin();
								itEnd = pTmpString->rend();
							}
						}
					}
					dValue = 0;
				}
				else
					pTmpString->append( &tableChar[ (int32_t)dIndex ], 1 );
			}
			cleanupFloatString( *pTmpString );
			pString = pTmpString;
			break;
		case OPERAND_UNDEFINED:
			pString = new wstring( STRING_INTERNAL_UNDEFINED );
			break;
		case OPERAND_NULL:
			pString = new wstring( STRING_INTERNAL_NULL );
			break;
		case OPERAND_NAN:
			pString = new wstring( STRING_INTERNAL_NAN );
			break;
		case OPERAND_STRING:
			break;
		case OPERAND_BOOLEAN:
			switch( iValue )
		{
			case 0:
				pString = new wstring( STRING_INTERNAL_FALSE );
				break;
			default:
				pString = new wstring( STRING_INTERNAL_TRUE );
				break;
		}
			break;
		case OPERAND_OBJECTREF:
		{
			CVariable* pvar = refObject->lookupProperty( STRING_INTERNAL_TOSTRING );
			if( pvar != NULL && OperandType( pvar->iOperandType ) == OPERAND_OBJECTREF )
			{
				//call toString?
				//May push something to EvalStack, thus this pointer could be invalidated...
				CVariable* pResult = pVm->invokeMethod( pvar->refObject->getCall(),
													   1,
													   this );
				wstring* pConvertedString = new wstring();
				this->setStringPointer( pConvertedString );
				
				if( pResult->iOperandType != OPERAND_STRING )
				{
					
				}
				else
					pConvertedString->append( *pResult->pString );
				delete( pResult );
				pString = pConvertedString;
				break;
			}
			else
			{
				//No toString definition.
				//Just put the result of getValue
				pString = refObject->getValue().toString( pVm, iRadix );
			}
		}
			break;
		default:
			Debug_Fatal( "Not implemented yet" );
			break;
	}
	iOperandType = OPERAND_STRING;
	return;
}
	
/***************************************************************************
 *      CVariable::toInt
 ***************************************************************************/
int32_t CVariable::toInt( void )
{
	int32_t iRetValue;
	switch( OperandType( iOperandType ) )
	{
	case OPERAND_BOOLEAN:
		iRetValue = (bValue != false);
		break;
	case OPERAND_INT:
	case OPERAND_UNSIGNEDINT:
		iRetValue = iValue;
		break;
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
		iRetValue = (int32_t)i64Value;
		break;
	case OPERAND_FLOAT:
		iRetValue = (int32_t)fValue;
		break;
	case OPERAND_DOUBLE:
		iRetValue = (int32_t)dValue;
		break;
	case OPERAND_OBJECTREF:
		iRetValue = (int32_t)refObject->getValue().toInt();
		break;
	case OPERAND_UNDEFINED:
	case OPERAND_NULL:
	case OPERAND_NAN:
		iRetValue = (int32_t)0;
		break;
	case OPERAND_STRING:
		if( pString )
			iRetValue = _wtoi( pString->data() );
		else
			iRetValue = 0;
		break;
	default:
		Debug_Fatal( "Not implemented yet" );
		break;
	}
	return iRetValue;
}

/***************************************************************************
 *      CVariable::toBoolean
 *		ECMA262 9.2
 ***************************************************************************/
bool CVariable::toBoolean( void )
{
	bool bRetValue;
	switch( OperandType( iOperandType ) )
	{
	case OPERAND_BOOLEAN:
		bRetValue = bValue;
		break;
	case OPERAND_INT:
	case OPERAND_UNSIGNEDINT:
		bRetValue = (iValue != 0 );
		break;
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
		bRetValue = (i64Value != 0 );
		break;
	case OPERAND_FLOAT:
		if( !_finite( fValue ) )
			bRetValue = false;
		else
			bRetValue = (fValue != 0 );
		break;
	case OPERAND_DOUBLE:
		if( !_finite( fValue ) )
			bRetValue = false;
		else
			bRetValue = (dValue != 0 );
		break;
	case OPERAND_OBJECTREF:
		bRetValue = false;
		break;
	case OPERAND_UNDEFINED:
	case OPERAND_NULL:
		bRetValue = false;
		break;
	case OPERAND_STRING:
		if( *pString == STRING_INTERNAL_TRUE )
			bRetValue = true;
		else if( *pString == STRING_INTERNAL_FALSE )
			bRetValue = false;
		else
			bRetValue = ( _wtoi( pString->data() ) != 0 );
		break;
	case OPERAND_NAN:
		bRetValue = false;
		break;
	default:
		Debug_Fatal( "Not implemented yet" );
		break;
	}
	return bRetValue;
}

/***************************************************************************
 *      CVariable::toDouble
 ***************************************************************************/
double CVariable::toDouble( void )
{
	double retValue;
	switch( OperandType( iOperandType ) )
	{
	case OPERAND_BOOLEAN:
		retValue = (double)bValue;
		break;
	case OPERAND_INT:
	case OPERAND_UNSIGNEDINT:
		retValue = (double)iValue;
		break;
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
		retValue = (double)i64Value;
		break;
	case OPERAND_FLOAT:
		retValue = (double)fValue;
		break;
	case OPERAND_DOUBLE:
		retValue = (double)dValue;
		break;
	case OPERAND_OBJECTREF:
		retValue = refObject->getValue().toDouble();
		break;
	case OPERAND_UNDEFINED:
	case OPERAND_NULL:
	case OPERAND_NAN:
		retValue = (double)0;
		break;
	case OPERAND_STRING:
		retValue = _wtof( pString->data() );
		break;
	default:
		Debug_Fatal( "Not implemented yet" );
		break;
	}
	return retValue;
}

/***************************************************************************
 *      CVariable::toFloat
 ***************************************************************************/
float CVariable::toFloat( void )
{
	float retValue;
	switch( OperandType( iOperandType ) )
	{
	case OPERAND_BOOLEAN:
		retValue = (float)bValue;
		break;
	case OPERAND_INT:
	case OPERAND_UNSIGNEDINT:
		retValue = (float)iValue;
		break;
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
		retValue = (float)i64Value;
		break;
	case OPERAND_FLOAT:
		retValue = (float)fValue;
		break;
	case OPERAND_DOUBLE:
		retValue = (float)dValue;
		break;
	case OPERAND_OBJECTREF:
		retValue = refObject->getValue().toFloat();
		break;
	case OPERAND_NAN:
		//Extension
	case OPERAND_UNDEFINED:
	case OPERAND_NULL:
		retValue = (float)0;
		break;
	case OPERAND_STRING:
		{
		CVariable var;
		CNumberObject::parseNumber( var, pString );
		retValue = var.toFloat();
		}
		break;
	default:
		Debug_Fatal( "Not implemented yet" );
		break;
	}
	return retValue;
}

/***************************************************************************
 *      CVariable::toInt64
 ***************************************************************************/
int64_t CVariable::toInt64( void )
{
	int64_t retValue;
	switch( OperandType( iOperandType ) )
	{
	case OPERAND_BOOLEAN:
		retValue = (int64_t)bValue;
		break;
	case OPERAND_INT:
	case OPERAND_UNSIGNEDINT:
		retValue = (int64_t)iValue;
		break;
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
		retValue = (int64_t)i64Value;
		break;
	case OPERAND_FLOAT:
		retValue = (int64_t)fValue;
		break;
	case OPERAND_DOUBLE:
		retValue = (int64_t)dValue;
		break;
	case OPERAND_OBJECTREF:
		retValue = refObject->getValue().toInt64();
		break;
	case OPERAND_UNDEFINED:
	case OPERAND_NULL:
	case OPERAND_NAN:
		retValue = (int64_t)0;
		break;
	case OPERAND_STRING:
		retValue = _wtoi64( pString->data() );
		break;
	default:
		Debug_Fatal( "Not implemented yet" );
		break;
	}
	return retValue;
}

/***************************************************************************
 *      CVariable::getPrimitiveType
 ***************************************************************************/
OPERAND_TYPE CVariable::getPrimitiveType( PRIMITIVETYPE_PREFERENCE prefferred )
{
	RID rid;
	switch( prefferred )
	{
	default:
	case PRIMITIVETYPE_PREFERRED:
		switch( OperandType( iOperandType ) )
		{
		case OPERAND_OBJECTREF:
		{
			//if( ridConstraintType )
			//	rid = getConstraintRid();
			//else
			//	rid = refObject->getRID(); 
			rid = refObject->getRID(); 

			if( rid == CCilVm::getBooleanObjectRID() )
			{
				return OPERAND_BOOLEAN;
			}
			else if( rid == CCilVm::getNumberObjectRID() )
			{
				return (OPERAND_TYPE)refObject->getValue().iOperandType;
			}
			else if( rid == CCilVm::getFunctionObjectRID() )
			{
				return OPERAND_STRING;
			}
			return OPERAND_STRING;
			break;
		}
		default:
			return (OPERAND_TYPE)iOperandType;
		}
		break;
	case PRIMITIVETYPE_NUMBER:	//Can convert to number?
		switch( OperandType( iOperandType ) )
		{
		case OPERAND_OBJECTREF:
		{
			//if( ridConstraintType )
			//	rid = getConstraintRid();
			//else
			//	rid = refObject->getRID(); 
			rid = refObject->getRID();
			if(  TypeFromToken( rid ) == MDT_METHODDEF )
			{
				return OPERAND_OBJECTREF;
			} else if( rid == CCilVm::getBooleanObjectRID() )
			{
				return OPERAND_BOOLEAN;
			}
			else if( rid == CCilVm::getNumberObjectRID() )
			{
				return (OPERAND_TYPE)refObject->getValue().iOperandType;
			}
			else if( rid == CCilVm::getDateObjectRID() ||
				rid == CCilVm::getObjectRID() ||
				rid == CCilVm::getArrayObjectRID() ||
				rid == CCilVm::getFunctionObjectRID() ||
				rid == CCilVm::getErrorObjectRID() ||
				rid == CCilVm::getRegexObjectRID()
				)
			{
				return OPERAND_OBJECTREF;
			}
			return OPERAND_STRING;
			break;
		}
		default:
			return (OPERAND_TYPE)iOperandType;
		}
		break;
	}
}

/***************************************************************************
 *      CVariable::convertToPrimitiveType
 ***************************************************************************/
void CVariable::convertToPrimitiveType( CCilVm* pVm )
{
	switch( OperandType( iOperandType ) )
	{
	case OPERAND_OBJECTREF:
	{
		RID rid = refObject->getRID(); 
		if( rid == CCilVm::getBooleanObjectRID() )
		{
			bool b = refObject->getValue().toBoolean();
			refObject->release();
			bValue = b;
			iOperandType = OPERAND_BOOLEAN;
		}
		else if( rid == CCilVm::getNumberObjectRID() )
		{
			//Dup it before it possiblly released..
			CVariable var = refObject->getValue();

			refObject->release();
			iOperandType = var.iOperandType;
			i64Value = var.i64Value;
		}
		else if( rid == CCilVm::getStringObjectRID() )
		{
			convertToString( pVm );
		}
		break;
	}
	default:
		break;
	}
}

/***************************************************************************
 *      CVariable::cleanupFloatString, toString helper
 ***************************************************************************/
void CVariable::cleanupFloatString( wstring& String )
{
	if( String.find( L'.' ) != string::npos )
	{
		while( *String.rbegin() == L'0' )
		{
			String.erase( String.end() -1, String.end() );
		}
		while( *String.rbegin() == L'.' )
		{
			String.erase( String.end() -1, String.end() );
		}
		//Replace infinity string
		//For VisualStudio
		//const wchar_t STRING_INTERNAL_INFINITY_TOBEREPLACED[] = L"1.#INF";
		size_t iInfIndex = String.find( STRING_INTERNAL_INFINITY_TOBEREPLACED );
		if( iInfIndex != string::npos )
		{
			String.replace( iInfIndex, sizeof( STRING_INTERNAL_INFINITY_TOBEREPLACED ) / sizeof( wchar_t ), STRING_INTERNAL_INFINITY );
		}
	}
}

/***************************************************************************
 *      CVariable::addObjectRef
 ***************************************************************************/
void CVariable::addObjectRef()
{
	assert( iOperandType == OPERAND_OBJECTREF );
	if( refObject )
	{
		refObject->addRef();
	}
}

/***************************************************************************
 *      CVariable::erase
 ***************************************************************************/
void CVariable::erase()
{
	switch( OperandType( iOperandType ) )
	{
	case OPERAND_OBJECTREF:
		//Release an object if referenced
		if( refObject )
		{
			refObject->release();
			//if( !refObject->release() )
			//	delete refObject;
			//refObject = NULL;
		}
		break;
	case OPERAND_STRING:
		delete pString;	//Delete old string
		//pString = NULL;
		break;
	default:
		break;
	}
	iValue = 0;
}

} //namespace CRI	