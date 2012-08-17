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
 * Module   : RID management codes
 * File     : cilMetadata.cpp
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
 * ctor
 ***************************************************************************/
CMetaData::CMetaData() :	m_iNumCurrentStaticField( 0 ),
								m_iNumUserMethod( 0 ),
								m_iNumBuiltinMethod( 0 ),
								m_iNumUserParamList( 0 ),
								m_iNumBuiltinParamList( 0 ),
								m_iNumUserObject( 0 ),
								m_iNumBuiltinObject( 0 ),
								m_iNumBuitinString( 0 ),
								m_iNumUserString( 0 ),
								m_iNumProperty( 0 )
{
};

/***************************************************************************
 * operator =
 ***************************************************************************/
CMetaData& CMetaData::operator=( CMetaData& right )
{
	m_iNumCurrentStaticField = right.m_iNumCurrentStaticField;
	m_iNumUserMethod = right.m_iNumUserMethod;
	m_iNumBuiltinMethod = right.m_iNumBuiltinMethod;
	m_iNumUserParamList = right.m_iNumUserParamList;
	m_iNumBuiltinParamList = right.m_iNumBuiltinParamList;
	m_iNumUserObject = right.m_iNumUserObject;
	m_iNumBuiltinObject = right.m_iNumBuiltinObject;
	m_iNumBuitinString = right.m_iNumBuitinString;
	m_iNumUserString = right.m_iNumUserString;
	m_iNumProperty = right.m_iNumProperty;

	m_StaticFieldTypeTable = right.m_StaticFieldTypeTable;
	m_MethodTable			= right.m_MethodTable;
	m_BuiltinMethodTable	= right.m_BuiltinMethodTable;
	
	m_ObjectTable			= right.m_ObjectTable;
	m_BuiltinObjectTable	= right.m_BuiltinObjectTable;

	m_StringConstantPool	= right.m_StringConstantPool;
	m_BuiltinStringConstantPool = right.m_BuiltinStringConstantPool;
	m_BuiltinParamListTable = right.m_BuiltinParamListTable;
	m_ParamListTable		= right.m_ParamListTable;


	//m_mapProperty = right.m_mapProperty;
	hash_map< wstring, CVariable >::iterator itst = right.m_ObjectMap.begin();
	hash_map< wstring, CVariable >::iterator itend = right.m_ObjectMap.end();
	while( itst != itend )
	{
//		m_ObjectMap[ itst->first ] = itst->second;
		//Clear the flag temporary to copy the value
		OPERAND_FLAG flagTmp = (OPERAND_FLAG)itst->second.iOperandFlag;
		itst->second.iOperandFlag = OPERAND_FLAG_NONE;
		
		if( OperandType( itst->second.iOperandType ) == OPERAND_OBJECTREF )
			m_ObjectMap[ itst->first ] = new CVmObject( *itst->second.refObject );
		else
			m_ObjectMap[ itst->first ] = itst->second;

		//Restore the flag
		m_ObjectMap[ itst->first ].iOperandFlag = flagTmp;
		itst->second.iOperandFlag = flagTmp;
		itst++;
	}
	return *this;

}

/***************************************************************************
 * copy ctor
 ***************************************************************************/
CMetaData::CMetaData( CMetaData& right )
{
	m_iNumCurrentStaticField = right.m_iNumCurrentStaticField;
	m_iNumUserMethod = right.m_iNumUserMethod;
	m_iNumBuiltinMethod = right.m_iNumBuiltinMethod;
	m_iNumUserParamList = right.m_iNumUserParamList;
	m_iNumBuiltinParamList = right.m_iNumBuiltinParamList;
	m_iNumUserObject = right.m_iNumUserObject;
	m_iNumBuiltinObject = right.m_iNumBuiltinObject;
	m_iNumBuitinString = right.m_iNumBuitinString;
	m_iNumUserString = right.m_iNumUserString;
	m_iNumProperty = right.m_iNumProperty;

	m_StaticFieldTypeTable = right.m_StaticFieldTypeTable;
	m_MethodTable			= right.m_MethodTable;
	m_BuiltinMethodTable	= right.m_BuiltinMethodTable;
	
	m_ObjectTable			= right.m_ObjectTable;
	m_BuiltinObjectTable	= right.m_BuiltinObjectTable;

	m_StringConstantPool	= right.m_StringConstantPool;
	m_BuiltinStringConstantPool = right.m_BuiltinStringConstantPool;
	m_BuiltinParamListTable = right.m_BuiltinParamListTable;
	m_ParamListTable		= right.m_ParamListTable;


	//m_mapProperty = right.m_mapProperty;
	hash_map< wstring, CVariable >::const_iterator itst = right.m_ObjectMap.begin();
	hash_map< wstring, CVariable >::const_iterator itend = right.m_ObjectMap.end();
	while( itst != itend )
	{
		m_ObjectMap[ itst->first ] = itst->second;
		if( OperandType( itst->second.iOperandType ) == OPERAND_OBJECTREF )
			m_ObjectMap[ itst->first ] = new CVmObject( *itst->second.refObject );
		else
			m_ObjectMap[ itst->first ] = itst->second;
		itst++;
	}
}

/***************************************************************************
 * dtor
 ***************************************************************************/
CMetaData::~CMetaData()
{
	terminate();
};

/***************************************************************************
 * Initialization
 ***************************************************************************/	
void CMetaData::initialize( void )
{
	//Clear object map
	m_ObjectMap.clear();

	m_StaticFieldTypeTable.clear();
	m_MethodTable.clear();
	m_BuiltinMethodTable.clear();

	m_ObjectTable.clear();
	m_BuiltinObjectTable.clear();

	m_StringConstantPool.clear();
	m_BuiltinStringConstantPool.clear();
}

/***************************************************************************
 * Termination
 ***************************************************************************/	
void CMetaData::terminate( void )
{
	m_StaticFieldTypeTable.clear();
	m_MethodTable.clear();
	m_BuiltinMethodTable.clear();

	m_ObjectTable.clear();
	m_BuiltinObjectTable.clear();

	m_StringConstantPool.clear();
	m_BuiltinStringConstantPool.clear();

	//Clear object map

	hash_map< wstring, CVariable >::iterator itSt = m_ObjectMap.begin();
	hash_map< wstring, CVariable >::iterator itEnd = m_ObjectMap.end();
	while( itSt != itEnd )
	{
		if( itSt->second.iOperandType == OPERAND_OBJECTREF )
			itSt->second.refObject->clearRefCount();
		itSt ++;
	}

	m_ObjectMap.clear();
}

/***************************************************************************
 * Static field RID
 ***************************************************************************/	
RID CMetaData::generateStaticFieldRID( const OPERAND_TYPE type,
									  const OPERAND_FLAG flag,
									  const RID ridConstraintType )
{
	assert( m_iNumCurrentStaticField <= MAX_RID_ENTRY );
	OPERAND_TYPE_FLAG value;

	value.type = type;
	value.flag = flag;
	value.ridConstraintType = ridConstraintType;
	m_StaticFieldTypeTable.push_back( value );
	return MDT_FIELDDEF | m_iNumCurrentStaticField++;
}

/***************************************************************************
 * Method RID
 ***************************************************************************/	
RID CMetaData::generateMethodRID( const METHOD_TYPE type,
											 const RID ridScope,
											 const RID ridArg,
											 const RID ridLocalVariable )
{
	assert( m_iNumUserMethod <= MAX_RID_ENTRY );

	METHOD_DEF method = { 0 };
	method.MethodType	= type;
	method.ridMethod	= MDT_METHODDEF | m_iNumUserMethod;
	method.ridScope		= ridScope;
	method.iILOffset	= 0;
	method.iNumArgments = (int32_t)getParamListSize( ridArg );
	method.iNumLocals	= (int32_t)getParamListSize( ridLocalVariable );
	method.iNumberOfReturnPass = 0;
	method.ridArgList	= ridArg;
	method.ridLocalVariableList = ridLocalVariable;
	m_MethodTable.push_back( method );
	return MDT_METHODDEF | m_iNumUserMethod++;
}

/***************************************************************************
 * EH clause RID
 ***************************************************************************/	
RID CMetaData::generateEhRID( const METHOD_TYPE type,
									const RID ridScope,
									const RID ridCatch,
									const RID ridFinally )
{
	assert( m_iNumUserMethod <= MAX_RID_ENTRY );
	assert( type == METHOD_EHCLAUSE );

	METHOD_DEF method = { 0 };
	method.MethodType	= type;
	method.ridMethod	= MDT_METHODDEF | m_iNumUserMethod;
	method.ridScope		= ridScope;
	method.iILOffset	= 0;
	method.iNumArgments = 0;
	method.iNumLocals	= 0;
	method.iNumberOfReturnPass = 0;
	method.ridCatch		= ridCatch;
	method.ridFinally	= ridFinally;
	m_MethodTable.push_back( method );
	return MDT_METHODDEF | m_iNumUserMethod++;
}

/***************************************************************************
 * Parameterlist RID
 ***************************************************************************/	
RID CMetaData::generateParamListRID( )
{
	assert( m_iNumUserParamList <= MAX_RID_ENTRY );
	vector< OPERAND_TYPE_FLAG > v;
	m_ParamListTable.push_back( v );
	return MDT_PARAMDEF | m_iNumUserParamList++;
}

/***************************************************************************
 * Parameterlist RID
 ***************************************************************************/	
int32_t CMetaData::addParamListEntry( const RID ridParamList,
									 const OPERAND_TYPE op,
									 const OPERAND_FLAG flag,
									 const RID ridConstraintType )
{
	assert( TypeFromToken(ridParamList) == MDT_PARAMDEF );
	int32_t iIndex;
	OPERAND_TYPE_FLAG value;
	value.type = op;
	value.flag = flag;
	value.ridConstraintType = ridConstraintType;

	if( isBuiltinRid( ridParamList ) )
	{
		iIndex = IndexFromBuiltinRid( ridParamList );
		assert( (int32_t)(m_BuiltinParamListTable[ iIndex ].size() - 1) < 0x8000 );
		m_BuiltinParamListTable[ iIndex ].push_back( value );
		return (int32_t)m_BuiltinParamListTable[ iIndex ].size() - 1;
	}
	else
	{
		iIndex = RidFromToken( ridParamList );
		assert( (int32_t)(m_ParamListTable[ iIndex ].size() - 1) < 0x8000 );
		m_ParamListTable[ iIndex ].push_back( value );
		return (int32_t)m_ParamListTable[ iIndex ].size() - 1;
	}
}

/***************************************************************************
 * Retrieve Parameterlist size
 ***************************************************************************/	
size_t CMetaData::getParamListSize( const RID ridParamList )
{
	if( ridParamList == 0 ) return 0;
	assert( TypeFromToken(ridParamList) == MDT_PARAMDEF );
	int32_t iIndex;
	if( isBuiltinRid( ridParamList ) )
	{
		iIndex = IndexFromBuiltinRid( ridParamList );
		return m_BuiltinParamListTable[ iIndex ].size();
	}
	else
	{
		iIndex = RidFromToken( ridParamList );
		return m_ParamListTable[ iIndex ].size();
	}
}

/***************************************************************************
 * Object information RID
 ***************************************************************************/	
RID CMetaData::generateUserObjectRID()
{
	assert( m_iNumUserObject <= MAX_RID_ENTRY );
	return MDT_TYPEDEF | m_iNumUserObject++;
}

/***************************************************************************
 * Property RID
 ***************************************************************************/	
RID CMetaData::generatePropertyRID()
{
	assert( m_iNumProperty <= MAX_RID_ENTRY );
	return MDT_PROPERTY | m_iNumProperty++;
}

/***************************************************************************
 * Generate built int Object information RID
 ***************************************************************************/	
RID CMetaData::generateBuiltinObjectRID()
{
	assert( -m_iNumBuiltinObject <= MAX_RID_ENTRY );
	return MDT_TYPEDEF | RidFromToken( --m_iNumBuiltinObject );
}

/***************************************************************************
 * Built in object RID generation
 ***************************************************************************/	
RID CMetaData::reserveBuiltinObjectRID( RID ridBuiltinObject )
{
	assert( TypeFromToken(ridBuiltinObject) == MDT_TYPEDEF );
	int32_t iIndex = IndexFromBuiltinRid( ridBuiltinObject );
	assert( iIndex >= 0 );

	if( m_BuiltinObjectTable.size() < (uint32_t)iIndex + 1 )
	{
		m_BuiltinObjectTable.resize( iIndex + 1 );
	}

	//Check if the object RID is registerred already.
	//if( m_BuiltinObjectTable[ iIndex ].getRID() != RID_NOTDEFINED )
	//	Debug_Fatal( "An object param list with reserved RID already registerred." );

	return ridBuiltinObject;
}

/***************************************************************************
 * Built in object param list RID generation
 ***************************************************************************/	
RID CMetaData::reserveBuiltinParamListRID( const RID ridBuiltinObjectParamList )
{
	assert( TypeFromToken(ridBuiltinObjectParamList) == MDT_PARAMDEF );
	int32_t iIndex = IndexFromBuiltinRid( ridBuiltinObjectParamList );
	assert( iIndex >= 0 );

	if( m_BuiltinParamListTable.size() < (uint32_t)iIndex + 1 )
	{
		m_BuiltinParamListTable.resize( iIndex + 1 );
	}

	//Check if the object RID is registerred already.
	if( m_BuiltinParamListTable[ iIndex ].size() != 0 )
	{
		//Now functions can share parameter list
		//Debug_Fatal( "An object param list with reserved RID already registerred." );
		return ridBuiltinObjectParamList;
	}
	m_iNumBuiltinParamList ++;
	return ridBuiltinObjectParamList;
}

/***************************************************************************
 * Built in object method RID generation
 ***************************************************************************/	
RID CMetaData::reserveBuiltinMethodRID(	RID ridBuiltinMethod,
								METHOD_TYPE type,
								PMETHOD pFunction,
								RID ridScope,
								RID	ridArg,
								RID	ridLocalVariable )
{
	assert( TypeFromToken(ridBuiltinMethod) == MDT_METHODDEF );
	assert( !ridArg || TypeFromToken(ridArg) == MDT_PARAMDEF );
	assert( !ridLocalVariable || TypeFromToken(ridLocalVariable) == MDT_PARAMDEF );
	int32_t iIndex = IndexFromBuiltinRid( ridBuiltinMethod );
	assert( iIndex >= 0 );

	if( m_BuiltinMethodTable.size() < (uint32_t)iIndex + 1 )
	{
		m_BuiltinMethodTable.resize( iIndex + 1 );
	}

	//Check if the object RID is registerred already.
	if( m_BuiltinMethodTable[ iIndex ].ridMethod != 0 )
		Debug_Fatal( "An object param list with reserved RID already registerred." );

	METHOD_DEF method = { 0 };
	method.MethodType	= type;
	method.ridMethod	= ridBuiltinMethod;
	method.ridScope		= ridScope;
	method.lpFunction	= pFunction;
	method.iNumArgments = (int32_t)getParamListSize( ridArg );
	method.iNumLocals	= (int32_t)getParamListSize( ridLocalVariable );
	method.iNumberOfReturnPass = 0;
	method.ridArgList	= ridArg;
	method.ridLocalVariableList = ridLocalVariable;
	m_BuiltinMethodTable[ iIndex ] = method;

	return ridBuiltinMethod;
}

/***************************************************************************
 * StringConstant information RID
 ***************************************************************************/	
RID CMetaData::generateUserStringConstantRID( const wstring& s )
{
	assert( m_iNumUserString <= MAX_RID_ENTRY );
	m_StringConstantPool.push_back( s );
	return MDT_STRING | m_iNumUserString++;
}

/***************************************************************************
 * Generate built in string RID
 ***************************************************************************/	
RID CMetaData::generateBuiltinStringConstantRID()
{
	assert( -m_iNumBuitinString <= MAX_RID_ENTRY );
	return MDT_STRING | RidFromToken( --m_iNumBuitinString );
}

/***************************************************************************
 * Generate built in string RID
 ***************************************************************************/	
RID CMetaData::generateBuiltinMethodRID()
{
	assert( -m_iNumBuiltinMethod <= MAX_RID_ENTRY );
	return MDT_METHODDEF | RidFromToken( --m_iNumBuiltinMethod );
}

/***************************************************************************
 * Built in StringConstant RID generation
 ***************************************************************************/	
RID CMetaData::reserveBuiltinStringConstantRID( RID ridBuiltinStringConstant, const wstring& s )
{
	assert( TypeFromToken( ridBuiltinStringConstant ) == MDT_STRING );
	int32_t iIndex = IndexFromBuiltinRid( ridBuiltinStringConstant );
	assert( iIndex >= 0 );

	if( m_BuiltinStringConstantPool.size() < (uint32_t)iIndex + 1 )
	{
		m_BuiltinStringConstantPool.resize( iIndex + 1 );
	}

	//Check if the StringConstant RID is registerred already.
	if( m_BuiltinStringConstantPool[ iIndex ] != L"" )
		Debug_Fatal( "An StringConstant param list with reserved RID already registerred." );

	m_BuiltinStringConstantPool[ iIndex ] = s;

	return ridBuiltinStringConstant;
}

/***************************************************************************
 * High level helpers for builtin objects
 ***************************************************************************/	

/***************************************************************************
 * Retrieve string constant by RID
 ***************************************************************************/	
wstring& CMetaData::getStringConstant( RID rid )
{
	assert( TypeFromToken( rid ) == MDT_STRING );
	int32_t iIndex;
	if( isBuiltinRid( rid ) )
	{
		//Built in methods
		iIndex = IndexFromBuiltinRid( rid );
		return getBuiltinStringConstantPool()[ iIndex ];
	}
	else
	{
		//User defined methods
		iIndex = RidFromToken( rid );
		return getStringConstantPool()[ iIndex ];
	}
}

/***************************************************************************
 * Retrieve method def by RID
 ***************************************************************************/	
METHOD_DEF& CMetaData::getMethod( RID rid )
{
	assert( TypeFromToken( rid ) == MDT_METHODDEF );
	int32_t iIndex;
	if( isBuiltinRid( rid ) )
	{
		//Built in methods
		iIndex = IndexFromBuiltinRid( rid );
		return getBuiltinMethodTable()[ iIndex ];
	}
	else
	{
		iIndex = RidFromToken( rid );
		//User defined methods
		return getMethodTable()[ iIndex ];
	}
}

/***************************************************************************
 * Retrieve an object def by RID
 ***************************************************************************/	
CObjectTypeInfo& CMetaData::getObject( RID rid )
{
	assert( TypeFromToken( rid ) == MDT_TYPEDEF );
	int32_t iIndex;
	if( isBuiltinRid( rid ) )
	{
		//Built in methods
		iIndex = IndexFromBuiltinRid( rid );
		return getBuiltinObjectTable()[ iIndex ];
	}
	else
	{
		iIndex = RidFromToken( rid );
		//User defined methods
		return getObjectTable()[ iIndex ];
	}
}

/***************************************************************************
 * Retrieve an object name RID by ObjectRID
 ***************************************************************************/	
RID CMetaData::getObjectNameRID( RID ObjectRID )
{
	CObjectTypeInfo& info = getObject( ObjectRID );
	return info.getObjectName();
}

/***************************************************************************
 * Set builtin object;
 ***************************************************************************/	
bool CMetaData::setBuiltinObject(  const RID ridObject,
						const RID ridObjectName,
						const RID ridCtor,
						const RID ridCall )
{
	assert( TypeFromToken( ridObject ) == MDT_TYPEDEF );
	assert( TypeFromToken( ridObjectName ) == MDT_STRING );
	assert( TypeFromToken( ridCtor ) == MDT_METHODDEF || ridCtor == RID_NULL );
	assert( TypeFromToken( ridCall ) == MDT_METHODDEF || ridCall == RID_NULL  );

	bool bReturn = true;
	int32_t iIndex = IndexFromBuiltinRid( ridObject );
	
	RID rid = reserveBuiltinObjectRID( ridObject );
	if( rid == RID_NOTDEFINED )
		return false;

	m_BuiltinObjectTable[ iIndex ].setRID( ridObject );
	m_BuiltinObjectTable[ iIndex ].setObjectName( ridObjectName );
	m_BuiltinObjectTable[ iIndex ].setConstructorProperty( ridCtor );
	m_BuiltinObjectTable[ iIndex ].setCallProperty( ridCall );

	return bReturn;
}

/***************************************************************************
 * Set builtin string object;
 ***************************************************************************/	
bool CMetaData::setBuiltinString(  const RID ridObjectName,
						const wstring& strString )
{
	assert( TypeFromToken( ridObjectName ) == MDT_STRING );
	bool bReturn = true;
	RID rid = reserveBuiltinStringConstantRID( ridObjectName, strString );
	if( rid == RID_NOTDEFINED )
		return false;
	return bReturn;
}

/***************************************************************************
 * Set builtin method object;
 ***************************************************************************/	
bool CMetaData::setBuiltinMethod(  const RID ridMethod,
						const METHOD_TYPE methodType,
						const PMETHOD pMethod,
						const RID ridParam )
{
	assert( TypeFromToken( ridMethod ) == MDT_METHODDEF );
	assert( TypeFromToken( ridParam ) == MDT_PARAMDEF || ridParam == RID_BUILTINPARAM_NULL );

	bool bReturn = true;
	if( RID_NOTDEFINED == reserveBuiltinParamListRID( ridParam ) )
		return false;
	if( RID_NOTDEFINED == reserveBuiltinMethodRID( ridMethod,
									methodType,
									pMethod,
									0,
									ridParam,
									0 ) )
		return false;

	return bReturn;
}

/***************************************************************************
 * Object map manupilation
 ***************************************************************************/	
CVariable* CMetaData::registerVariable( hash_map< wstring, CVariable >& currentmap, const wstring& strName )
{
	CVariable var;
	size_t iIndex;
	if( wstring::npos == ( iIndex = strName.find( STRING_INTERNAL_SCOPEDELIMITER ) ) )
	{
		hash_map< wstring, CVariable >::iterator it;
		it = currentmap.find( strName );
		if( it == currentmap.end() )
		{
			//Register new varialbe & return it
			currentmap[ strName ] = var;
			it = currentmap.find( strName );

			return &it->second;
		}
		else
			return &it->second;			
	}
	else
	{
		//narrow down object scope and look up recursively
		//Retrieve Root
		wstring strRoot = strName.substr( 0, iIndex );
		wstring strLeaf = strName.substr( iIndex + 1, strName.length() );
		CVariable* pvar = registerVariable( currentmap, strRoot );
		if( pvar->iOperandType == OPERAND_UNDEFINED )
		{
			//Newly registered variable
			//Register Object info
			pvar->iOperandType = OPERAND_OBJECTREF;
			pvar->refObject = new CVmObject();
		}
		return registerVariable( pvar->refObject->getPropertyMap(), strLeaf );
	}
}

/***************************************************************************
 * Query specific name variable in the map
 ***************************************************************************/	
CVariable* CMetaData::queryVariable( hash_map< wstring, CVariable >& currentmap, const wstring& strName )
{
	CVariable var;
	if( wstring::npos == strName.find( STRING_INTERNAL_SCOPEDELIMITER ) )
	{
		hash_map< wstring, CVariable >::iterator it;
		it = currentmap.find( strName );
		if( it == currentmap.end() )
		{
			return NULL;
		}
		else
			return &it->second;			
	}
	else
	{
		//narrow down object scope and look up recursively
		//Retrieve Root
		wstring strRoot = strName.substr( 0, strName.find( STRING_INTERNAL_SCOPEDELIMITER ) );
		wstring strLeaf = strName.substr( strName.find( STRING_INTERNAL_SCOPEDELIMITER ) + 1, strName.length() );
		CVariable* pvar = queryVariable( currentmap, strRoot );
		if( pvar == NULL || OperandType( pvar->iOperandType ) != OPERAND_OBJECTREF )
		{
			return NULL;
		}

		return queryVariable( pvar->refObject->getPropertyMap(), strLeaf );
	}
}

/***************************************************************************
 * registerBuiltinObject
 ***************************************************************************/	
RID CMetaData::registerBuiltinObject( const wstring& strQualifiedName )
{
	CVariable* pvar = registerVariable( m_ObjectMap, strQualifiedName );

	if( pvar == NULL || OperandType( pvar->iOperandType ) != OPERAND_UNDEFINED )
	{
		//Variable is already defined
		return RID_NOTDEFINED;
	}
	//Generate built in object RID
	//Those differs by compilation environment
	//However complation and runtime environment should be kept same
	RID ridObject = generateBuiltinObjectRID();

	//Initialize built in stringa table
	RID ridString = generateBuiltinStringConstantRID();
	assert( ridString != RID_NOTDEFINED );
	setBuiltinString( ridString, strQualifiedName );
									
	//Initialize object information
	CVmObject* pObj =	new CVmObject();
	pObj->setRID( ridObject );
	pObj->setObjectName( ridString );

	//Initialize variable
	pvar->iOperandType = OPERAND_OBJECTREF;
	pvar->refObject = pObj;

	bool bReturn = setBuiltinObject( ridObject,
								ridString,
								RID_NULL,
								RID_NULL );
	if( !bReturn )
	{
		return RID_NOTDEFINED;
	}

	return ridObject;
}

/***************************************************************************
 * registerBuiltinMethod
 ***************************************************************************/	
RID CMetaData::registerBuiltinMethod( const wstring& strQualifiedName,
									const METHOD_TYPE methodType,
									const RID_BUILTIN_PARAM ridParam )
{
	CVariable* pvar = registerVariable( m_ObjectMap, strQualifiedName );

	if( pvar == NULL || OperandType( pvar->iOperandType ) != OPERAND_UNDEFINED )
	{
		//Variable is already defined
		return RID_NOTDEFINED;
	}
	RID ridMethod = generateBuiltinMethodRID();
	pvar->iOperandType = OPERAND_INT;
	pvar->iValue = ridMethod;
	
	bool bReturn = setBuiltinMethod( ridMethod, methodType, NULL, ridParam );
	if( !bReturn )
	{
		return RID_NOTDEFINED;
	}

	size_t iIndex;
	if( wstring::npos != ( iIndex = strQualifiedName.rfind( STRING_INTERNAL_SCOPEDELIMITER ) ) )
	{
		wstring strRoot = strQualifiedName.substr( 0, iIndex );
		wstring strLeaf = strQualifiedName.substr( strQualifiedName.rfind( STRING_INTERNAL_SCOPEDELIMITER ) + 1,
							strQualifiedName.length() );
		CVariable* pvar = queryVariable( m_ObjectMap, strRoot );
		assert( pvar != NULL );
		assert( OperandType( pvar->iOperandType ) == OPERAND_OBJECTREF );

		if( !strLeaf.compare( STRING_INTERNAL_CONSTRUCTORNAME ) )
		{
			CObjectTypeInfo* pObj = (CObjectTypeInfo*)pvar->refObject;
			pObj->setConstructorProperty( ridMethod );
			//Update metadata
			setBuiltinObject( pObj->getRID(),
								pObj->getObjectName(),
								pObj->getConstructor(),
								pObj->getCall() );

		}
		else if( !strLeaf.compare( STRING_INTERNAL_CALLNAME ) )
		{
			CObjectTypeInfo* pObj = (CObjectTypeInfo*)pvar->refObject;
			pObj->setCallProperty( ridMethod );
			//Update metadata itsefl
			setBuiltinObject( pObj->getRID(),
								pObj->getObjectName(),
								pObj->getConstructor(),
								pObj->getCall() );
		}
	}

	return ridMethod;
}

/***************************************************************************
 * queryBuiltinObjectRid
 ***************************************************************************/	
RID CMetaData::queryBuiltinObjectRid( const wstring& strQualifiedName )
{
	CVariable* pvar = queryVariable( m_ObjectMap, strQualifiedName );
	if( pvar == NULL || OperandType( pvar->iOperandType ) != OPERAND_OBJECTREF )
	{
		//Object RID is not defined
		return RID_NOTDEFINED;
	}
	RID ridObject = ((CObjectTypeInfo*)pvar->refObject)->getRID();
	
	return ridObject;
}

/***************************************************************************
 * queryBuiltinObjectRid
 ***************************************************************************/	
RID CMetaData::queryBuiltinMethodRid( const wstring& strQualifiedName )
{
	CVariable* pvar = queryVariable( m_ObjectMap, strQualifiedName );
	if( pvar == NULL || OperandType( pvar->iOperandType ) != OPERAND_INT )
	{
		//Object RID is not defined
		return RID_NOTDEFINED;
	}
	RID ridObject = (RID)pvar->iValue;
	assert( TypeFromToken( ridObject) == MDT_METHODDEF );
	
	return ridObject;
}

/***************************************************************************
 * queryBuiltinStringRid
 ***************************************************************************/	
RID CMetaData::queryBuiltinStringRid( const wstring& strName )
{
	RID ridString = RID_NOTDEFINED;
	vector< wstring >::iterator it;

	int32_t iIndex = 0xffffff;
	//
	//Really slow
	//
	for( it = m_BuiltinStringConstantPool.begin(); it != m_BuiltinStringConstantPool.end(); ++ it )
	{
		if( !it->compare( strName ) )
		{
			ridString = MDT_STRING | iIndex;
			break;
		}
		iIndex --;
	}
	return ridString;
}

} //namespace CRI