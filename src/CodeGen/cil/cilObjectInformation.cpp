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
 * Module   : cilObjectInformation.cpp
 * File     : Manage object metadata
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

/***************************************************************************
 *      ctor
 ***************************************************************************/
CObjectTypeInfo::CObjectTypeInfo() : m_ridObject( RID_NOTDEFINED ),
									m_ridObjectName( 0 ),
									m_ridConstructor( 0 ),
									m_ridCall( 0 )
{
	m_mapProperty.clear();
}

/***************************************************************************
 *      dtor
 ***************************************************************************/
CObjectTypeInfo::~CObjectTypeInfo()
{
	m_mapProperty.clear();
}

/***************************************************************************
 *      Copy ctor
 ***************************************************************************/
CObjectTypeInfo::CObjectTypeInfo( const CObjectTypeInfo& right )
{
	m_ridObject = right.m_ridObject;
	m_ridObjectName = right.m_ridObjectName;
	m_ridConstructor = right.m_ridConstructor;
	m_ridCall = right.m_ridCall;
	//m_mapProperty = right.m_mapProperty;
	hash_map< wstring, CVariable >::const_iterator itst = right.m_mapProperty.begin();
	hash_map< wstring, CVariable >::const_iterator itend = right.m_mapProperty.end();
	while( itst != itend )
	{
		m_mapProperty[ itst->first ] = itst->second;
		if( OperandType( itst->second.iOperandType ) == OPERAND_OBJECTREF )
			itst->second.refObject->addRef();
		itst++;
	}

}

/***************************************************************************
 *      Helpers
 ***************************************************************************/
void CObjectTypeInfo::setRID( const RID rid )
{
	assert( TypeFromToken(rid) == MDT_PROPERTY
		 || TypeFromToken(rid) == MDT_METHODDEF
		 || TypeFromToken(rid) == MDT_TYPEDEF );
	m_ridObject = rid;
}

void CObjectTypeInfo::setObjectName( const RID rid )
{
	assert( TypeFromToken( rid ) == MDT_STRING );
	m_ridObjectName = rid;
}

RID CObjectTypeInfo::getObjectName( void )
{
	return m_ridObjectName;
}

/***************************************************************************
 *      Add a property
 ***************************************************************************/
void CObjectTypeInfo::addProperty( const wstring& name, const CVariable& value)
{
	m_mapProperty[ name ] = value;
	return;
}

} //namespace CRI