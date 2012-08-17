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
 * Module   : cil Virtual Machine
 * File     : CCilVmStaticField.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criscript.h"
#include "cilVm.h"
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

//Reserve vector size
//Fill in field type

/***************************************************************************
 *      Store from top of the stack to a static field
 ***************************************************************************/
void CCilVm::storeStaticField( const RID rid )
{
	assert( TypeFromToken( rid ) == MDT_FIELDDEF );

	uint32_t iIndex = RidFromToken( rid );

	if( m_pStaticFieldTable[ iIndex ].getFlag() & OPERAND_FLAG_READONLY )
	{
		//Read only variable
		popEvalStack();
		return;
	}

	if( m_pStaticFieldTable[ iIndex ].getFlag() & OPERAND_FLAG_STRICT )
	{
		//Test type constraint
		storeAsRestrictedType( m_pStaticFieldTable[ iIndex ] );
		popEvalStack();
		return;
	}

	getEvalStackFirstEntry().moveTo( m_pStaticFieldTable[ iIndex ] );
	popEvalStackFast();

}

/***************************************************************************
 *      Load from a static field to Top of the stack
 ***************************************************************************/
void CCilVm::loadStaticField( const RID rid )
{
	assert( TypeFromToken( rid ) == MDT_FIELDDEF );
	uint32_t iIndex = RidFromToken( rid );
	pushEvalStack( m_pStaticFieldTable[ iIndex ] );
}

/***************************************************************************
 *      Initialize static field
 ***************************************************************************/
bool CCilVm::setStaticFieldTypeTable( vector< OPERAND_TYPE_FLAG >& vecSFType )
{
	CVariable slot;
	int32_t i = 0;
	m_iStaticFieldTalbeSize = (uint32_t)vecSFType.size();
	m_pStaticFieldTable = new CVariable[ m_iStaticFieldTalbeSize ];

	vector< OPERAND_TYPE_FLAG >::iterator it;
	for( it = vecSFType.begin(); it != vecSFType.end(); ++it )
	{
		slot.iOperandType = it->type;
		slot.iOperandFlag = it->flag;
		slot.ridConstraintType = it->ridConstraintType;

		m_pStaticFieldTable[ i ] = slot;
		i++;
	}
	return true;
}

/***************************************************************************
 *      get Field Value (Debugger helper)
 ***************************************************************************/
CVariable& CCilVm::getStaticField( const RID rid )
{
	assert( TypeFromToken( rid ) == MDT_FIELDDEF );
	uint32_t iIndex = RidFromToken( rid );
	assert( iIndex < m_iStaticFieldTalbeSize );
	return m_pStaticFieldTable[ iIndex ];
}

/***************************************************************************
 *      Set Field Value (Debugger helper)
 ***************************************************************************/
bool CCilVm::setStaticField( const RID rid, const CVariable& value )
{
	assert( TypeFromToken( rid ) == MDT_FIELDDEF );
	uint32_t iIndex = RidFromToken( rid );
	m_pStaticFieldTable[ iIndex ] = value;
	return true;
}

bool CCilVm::setStaticField( const RID rid, const CVariable& value, bool bForceWriting )
{
	assert( TypeFromToken( rid ) == MDT_FIELDDEF );
	uint32_t iIndex = RidFromToken( rid );
	if( bForceWriting )
	{
		OPERAND_FLAG op = m_pStaticFieldTable[ iIndex ].getFlag();
		m_pStaticFieldTable[ iIndex ].setFlag( OPERAND_FLAG( op & ~OPERAND_FLAG_READONLY ) );
		m_pStaticFieldTable[ iIndex ] = value;
	}
	else
		m_pStaticFieldTable[ iIndex ] = value;

	return true;
}


/***************************************************************************
 *      Initialize meta data
 ***************************************************************************/
bool CCilVm::setMetaData( CMetaData& MetaData )
{
	if( m_status != VM_EXECUTE_INITIALIZING ) return false;

	if( !m_SymbolInfoPool.size() )
	{
		//Symbol Info not set yet
		return false;
	}
	m_MetaData = MetaData;

	//Initialize Static field table
	setStaticFieldTypeTable( m_MetaData.getStaticFieldTypeTable() );

	//Initialize prototype table
	initializePrototypeObjects();

	//Initialize objects in a static field table
	for( uint32_t i = 0; i < m_iStaticFieldTalbeSize; ++i )
	{
		if( m_pStaticFieldTable[ i ].ridConstraintType )
		{
			//Yep, i'm one of the ristricted objects
			CVmObject* pObj = createObject( m_pStaticFieldTable[ i ].GetConstraintRid() );
			m_pStaticFieldTable[ i ].setObjectRefWithFlags( pObj,
				m_pStaticFieldTable[ i ].getFlag(),
				m_pStaticFieldTable[ i ].GetConstraintRid() );
		}
	}

	m_status = VM_EXECUTE_INITIALIZED;

	return true;
}

/***************************************************************************
 * Set symbol information
 ***************************************************************************/
bool CCilVm::setSymbolInformation( hash_map<wstring, CG_SYMBOL_INFORMATION>& symbolinfo)
{
	if( m_status != VM_EXECUTE_INITIALIZING ) return false;

	hash_map<wstring, CG_SYMBOL_INFORMATION>::iterator it = symbolinfo.begin();

	pair<wstring, CG_SYMBOL_INFORMATION> pair;
	for( ; it != symbolinfo.end(); ++it)
	{
		pair = *it;
		m_SymbolInfoPool[ pair.first ] = pair.second;
	}
	return true;
}

/***************************************************************************
 * Query symbol information
 ***************************************************************************/
CG_SYMBOL_INFORMATION& CCilVm::querySymbolInformation( const wstring& symbolName )
{
	return m_SymbolInfoPool[ symbolName ];
}

/***************************************************************************
 * Convert var and store to type constraint var
 * Src is on the top of eval stack
 ***************************************************************************/
void CCilVm::storeAsRestrictedType( CVariable& varDest )
{
	assert( varDest.getFlag() & OPERAND_FLAG_STRICT );

	switch( varDest.iOperandType )
	{
	case OPERAND_UNDEFINED:
	case OPERAND_NULL:
	case OPERAND_NAN:
		//Throw TypeError
		ThrowExceptionFromVm( &wstring( ERRORSTRING_TYPEERROR_STORESTATICFIELD ),
			&wstring( NAME_BUILTIN_ERROR_OBJECT ),
			ERROR_TYPEERROR_STORESTATICFIELD );
		break;
	case OPERAND_INT:
		varDest.iValue = getEvalStackFirstEntry().toInt();
		break;
	case OPERAND_INT64:
		varDest.i64Value = getEvalStackFirstEntry().toInt64();
		break;
	case OPERAND_FLOAT:
		varDest.fValue = getEvalStackFirstEntry().toFloat();
		break;
	case OPERAND_DOUBLE:
		varDest.dValue = getEvalStackFirstEntry().toDouble();
		break;
	case OPERAND_BOOLEAN:
		varDest.bValue = getEvalStackFirstEntry().toBoolean();
		break;
	case OPERAND_STRING:
		varDest.pString = getEvalStackFirstEntry().toString( this );
		break;
	case OPERAND_OBJECTREF:
		{
			//Try object conversion
			CObjectTypeInfo& info = getMetaData().getObject( varDest.GetConstraintRid() );
			RID ridCtor = info.getConstructor();
			CVariable vars[ 2 ];
			vars[ 0 ] = varDest;
			vars[ 1 ] = getEvalStackFirstEntry();
			CVariable* pVar = invokeMethod( ridCtor, 2, vars );
			delete( pVar );
		}
		break;
	case OPERAND_UNSIGNEDINT:
	case OPERAND_UNSIGNEDINT64:
	case OPERAND_SYSTEM_DELETION:
	default:
		Debug_Fatal( "Unhandled type constraint");
		break;
	}
}

} //namespace CRI