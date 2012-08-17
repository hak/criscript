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
 * File     : CCilVmObject.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "cilVm.h"
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      VMObject ctor
 ***************************************************************************/
CVmObject::CVmObject(): m_iRefCount(0),
						m_varPrototype( NULL ),
						m_varParent( NULL )
{
	//setProperty Accessor
	setPropertyAccessor( STRING_INTERNAL_PROTOTYPE, prototypeAccessor );
};

/***************************************************************************
 *      VMObject dtor
 ***************************************************************************/
CVmObject::~CVmObject()
{
//Should check refcount...
//	assert( m_iRefCount == 0 );
}

/***************************************************************************
 *      Copy ctor
 ***************************************************************************/
CVmObject::CVmObject( const CVmObject& right ) : CObjectTypeInfo( right )
{
	m_iRefCount = right.m_iRefCount;
	m_varPrototype = right.m_varPrototype;
	m_varParent = NULL;
	addRef();

	m_mapProperty = right.m_mapProperty;
	m_varValue = right.m_varValue;
}
/***************************************************************************
 *      VMObject Addref
 ***************************************************************************/
int32_t CVmObject::addRef()
{
	assert( m_iRefCount <= INT_MAX );
	return ++m_iRefCount;
}

/***************************************************************************
 *      VMObject Release
 ***************************************************************************/
int32_t CVmObject::release()
{
//	assert( m_iRefCount > 0 );
	if( m_iRefCount > 0 )
	{
		m_iRefCount--;

//		delete this;
		return 0;

		//TODO:
		//Invoke Garbage collector if necessary
	}
	return m_iRefCount;
}

/***************************************************************************
 *      Clear Ref Count Release
 ***************************************************************************/
void CVmObject::clearRefCount( void )
{
	m_iRefCount = 0;
	hash_map< wstring, CVariable >& map = getPropertyMap();
	if( map.size() )
	{
		hash_map< wstring, CVariable >::iterator itSt = map.begin();
		hash_map< wstring, CVariable >::iterator itEd = map.end();
		while( itSt != itEd )
		{
			if( OperandType( itSt->second.iOperandType ) == OPERAND_OBJECTREF )
			{
				itSt->second.refObject->clearRefCount();
			}
			itSt ++;
		}
	}

	if( OperandType( m_varPrototype.iOperandType ) == OPERAND_OBJECTREF )
		m_varPrototype.refObject->clearRefCount();

	if( OperandType( m_varValue.iOperandType ) == OPERAND_OBJECTREF )
		m_varValue.refObject->clearRefCount();
}

/***************************************************************************
 *      Retrieve a property info from a property name
 ***************************************************************************/
CVariable* CVmObject::lookupProperty( const wstring& name )
{
	//Note that prototype object is also stored in the map
	hash_map< wstring, CVariable >::iterator it;
	it = m_mapProperty.find( name );
	if( it != m_mapProperty.end() )
	{
		if( OperandType( it->second.iOperandType ) == OPERAND_SYSTEM_ACCESSOR )
		{
			//Invoke accessor method
			return PACCESSORMETHOD( it->second.pPointerInternal )( NULL, this, &name, NULL, true ); 
		}
		return &it->second;
	}
	else
	{
		if( m_varPrototype.refObject != NULL && m_varPrototype.refObject != this )
		{
			return m_varPrototype.refObject->lookupProperty( name );
		}
		else
		{
			//return null
			return NULL;
		}
	}
}

hash_map< wstring, CVariable >::iterator CVmObject::lookupPropertyIterator( const wstring& name )
{
	hash_map< wstring, CVariable >::iterator it;
	it = m_mapProperty.find( name );
	if( it != m_mapProperty.end() )
	{
		assert( it->second.iOperandType != OPERAND_SYSTEM_ACCESSOR );
		return it;
	}
	else
	{
		if( m_varPrototype.refObject != NULL && m_varPrototype.refObject != this )
		{
			return m_varPrototype.refObject->lookupPropertyIterator( name );
		}
		else
		{
			return m_mapProperty.end();
		}
	}
}

/***************************************************************************
 *      Retrieve a property info from a property name with setter/getter involved
 ***************************************************************************/
CVariable* CVmObject::lookupProperty( CCilVm* const pVm, const wstring& name, CVariable* const pThis )
{
	//Note that prototype object is also stored in the map
	hash_map< wstring, CVariable >::iterator it;
	it = m_mapProperty.find( name );
	if( it != m_mapProperty.end() )
	{
		if( OperandType( it->second.iOperandType ) == OPERAND_SYSTEM_ACCESSOR )
		{
			//Invoke accessor method
			return PACCESSORMETHOD( it->second.pPointerInternal )( pVm, this, &name, pThis, true ); 
		}
		return &it->second;
	}
	else
	{
		if( m_varPrototype.refObject != NULL && m_varPrototype.refObject != this )
		{
			return m_varPrototype.refObject->lookupProperty( pVm, name, pThis );
		}
		else
		{
			//return null
			return NULL;
		}
	}
}

/***************************************************************************
 *      Update property info
 ***************************************************************************/
void CVmObject::setProperty( const wstring& name, CVariable& var )
{
	hash_map< wstring, CVariable >::iterator it;
	it = m_mapProperty.find( name );

	if( it != m_mapProperty.end() )
	{
		if( OperandType( it->second.iOperandType ) == OPERAND_SYSTEM_ACCESSOR )
		{
			//Invoke accessor method
			PACCESSORMETHOD( it->second.pPointerInternal )( NULL, this, &name, &var, false ); 
			return;
		}
		it->second = var;
	}
	else
	{
		m_mapProperty[ name ] = var; 
	}
	return;
}

/***************************************************************************
 *      Update property info with getter
 ***************************************************************************/
void CVmObject::setProperty( CCilVm* const pVm, const wstring& name, CVariable& var )
{
	hash_map< wstring, CVariable >::iterator it;
	it = m_mapProperty.find( name );

	if( it != m_mapProperty.end() )
	{
		if( OperandType( it->second.iOperandType ) == OPERAND_SYSTEM_ACCESSOR )
		{
			//Invoke accessor method
			PACCESSORMETHOD( it->second.pPointerInternal )( pVm, this, &name, &var, false ); 
			return;
		}

		it->second = var;
	}
	else
	{
		it = m_mapProperty.find( STRING_PROPERTY_INTERNAL_ANY );
		if( it != m_mapProperty.end() )
		{
			assert( OperandType( it->second.iOperandType ) == OPERAND_SYSTEM_ACCESSOR );
			//Invoke accessor method
			PACCESSORMETHOD( it->second.pPointerInternal )( pVm, this, &name, &var, false ); 
			return;
		}

		m_mapProperty[ name ] = var; 
	}
	return;
}

/***************************************************************************
 *      Update property info
 ***************************************************************************/
void CVmObject::setProperty( CCilVm* const pVm,
				 const wstring&	name,
				 const RID ridMethod,
				 const int32_t iLength )
{
	assert( pVm != NULL );
	assert( TypeFromToken( ridMethod ) == MDT_METHODDEF );

	//Create function object and store it to static field
	CVmObject newobject;
	RID ridObj = pVm->getFunctionObjectRID();

	newobject.setRID( ridObj );
	newobject.setObjectName( pVm->getMetaData().getObjectNameRID( ridObj ) );
	newobject.setPrototypeObject( &pVm->getPrototypeObject( ridObj ) );
	newobject.setConstructorProperty( ridMethod );
	newobject.setCallProperty( ridMethod );

	pVm->getObjectPool().push_back( newobject );

	CVariable var;
	var.setObjectRef( &pVm->getObjectPool().back() );

	assert( iLength >= 0 );
	CVariable varInt( iLength, OPERAND_FLAG_DONTENUM );
	var.refObject->setProperty( STRING_INTERNAL_LENGTH, varInt );

	//Store the ObjectRef to a static field
	m_mapProperty[ name ] = var; 
}

/***************************************************************************
 *      Remove property info
 ***************************************************************************/
void CVmObject::removeProperty( const wstring& name )
{
	hash_map< wstring, CVariable >::iterator it;
	it = m_mapProperty.find( name );
	if( it != m_mapProperty.end() )
	{
		//Remove it
		if( !(it->second.iOperandFlag & OPERAND_FLAG_DONTDELETE ) )
		{
			m_mapProperty.erase( it );
		}
	}

	return;
}

/***************************************************************************
 *      Set Property  accessor
 ***************************************************************************/
void CVmObject::setPropertyAccessor( const wstring& name, PACCESSORMETHOD pAccessor )
{
	m_mapProperty[ name ].iOperandType = OPERAND_SYSTEM_ACCESSOR;
	m_mapProperty[ name ].pPointerInternal = (void*)pAccessor;
	m_mapProperty[ name ].iOperandFlag = OPERAND_FLAG( OPERAND_FLAG_DONTDELETE | OPERAND_FLAG_DONTENUM );
}

/***************************************************************************
 *      Prototype accessor
 ***************************************************************************/
CVariable* CVmObject::prototypeAccessor(CCilVm* const pVm,
										CVmObject* const pObject,
										const wstring* const pName,
										CVariable* const pVar,
										bool bSet )
{
	assert( pObject != NULL );
	if( bSet )
	{
		//Getter implementation
		assert( pVar == NULL );
		return &pObject->m_varPrototype;
	}
	else
	{
		//Setter implementation
		assert( pVar->iOperandType == OPERAND_OBJECTREF );
		pObject->m_varPrototype.setObjectRef( pVar->refObject );

		return NULL;
	}
}

} //namespace CRI