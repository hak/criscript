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
 * File     : cilVMInitialization.cpp
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
#include "float.h"
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

//Static members
RID CCilVm::m_ridObject;				//Object RID
RID CCilVm::m_ridFunctionObject;		//Function Object RID
RID CCilVm::m_ridSystemObject;			//System Object RID
RID CCilVm::m_ridStringObject;			//String object RID
RID CCilVm::m_ridArrayObject;			//Array object RID
RID CCilVm::m_ridErrorObject;			//Error object RID
RID CCilVm::m_ridRegexObject;			//Regex object RID
RID CCilVm::m_ridBooleanObject;			//Boolean object RID
RID CCilVm::m_ridNumberObject;			//Numer object RID
RID CCilVm::m_ridDateObject;			//Date object RID
RID CCilVm::m_ridMathObject;			//Math object RID

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 * ctor
 ***************************************************************************/	
CCilVm::CCilVm()
: m_pCurrentInstruction( NULL ),
  m_iTickCount( 0 ),
  m_iCallStackRoot( 1 ),
  m_status( VM_EXECUTE_DONE ),
  m_iNumGivenArguments( 0 ),
  m_ExceptionHandlerNestingLevel( 0 ),
  m_pDebugger( NULL ),
  m_iStaticFieldTalbeSize( 0 ),
  m_pStaticFieldTable( NULL ),
  m_iLocalVariableListIndex( 0 ),
  m_iArgListIndex( 0 ),
  m_iMaxLocalVariableListIndex( 0 ),
  m_iMaxArgListIndex( 0 )
{
	init();
}

/***************************************************************************
 * dtor
 ***************************************************************************/	
CCilVm::~CCilVm()
{
	flushPrintBuffer();
	clearEvalStack();
	delete[] m_pStaticFieldTable;
	clearLocalVariableList();
	clearArgList();
	m_SymbolInfoPool.clear();

	m_BuiltinPrototypeObjectTable.clear();

	//clearRefCount();
	cleanObjectPool();
	m_ObjectPool.clear();


	m_MetaData.terminate();

}

/***************************************************************************
 * Initializer
 ***************************************************************************/	
bool CCilVm::init ()
{
	//Version string not to be stripped out
    static const int8_t * version = criscript_version;

	m_status = VM_EXECUTE_INITIALIZING;
	
	//Initialize eval stack
	clearEvalStack();
	clearLocalVariableList();
	clearArgList();
	
	//Initialize print buffer
	m_strPrintBuffer = L"";

	delete [] m_pStaticFieldTable;
	
	m_SymbolInfoPool.clear();

	for( int32_t i = 0; i < ARRAYSIZE( m_Handler ); ++i )
		m_Handler[ i ] = NULL;

	//Initialize prototype object table
	m_BuiltinPrototypeObjectTable.clear();
	//Note that initialization of prototype object table need to be after metadata is set
	//because number of built in prototype object differs by metadata entries

	m_stopwatch.Reset();
	return true;
};

/***************************************************************************
 *      Initialization of prototype objects
 ***************************************************************************/
void CCilVm::initializePrototypeObjects()
{
	bool bReturn;

	//Initialize RIDs
	m_ridObject = getMetaData().queryBuiltinObjectRid( wstring( NAME_BUILTIN_OBJECT_OBJECT ) );
	assert( m_ridObject != RID_NOTDEFINED );
	m_ridFunctionObject = getMetaData().queryBuiltinObjectRid( wstring( NAME_BUILTIN_FUNCTION_OBJECT ) );
	assert( m_ridFunctionObject != RID_NOTDEFINED );
	m_ridSystemObject = getMetaData().queryBuiltinObjectRid( wstring( NAME_BUILTIN_SYSTEM_OBJECT ) );
	assert( m_ridSystemObject != RID_NOTDEFINED );
	m_ridStringObject = getMetaData().queryBuiltinObjectRid( wstring( NAME_BUILTIN_STRING_OBJECT ) );
	assert( m_ridStringObject != RID_NOTDEFINED );
	m_ridArrayObject = getMetaData().queryBuiltinObjectRid( wstring( NAME_BUILTIN_ARRAY_OBJECT ) );
	assert( m_ridArrayObject != RID_NOTDEFINED );
	m_ridErrorObject = getMetaData().queryBuiltinObjectRid( wstring( NAME_BUILTIN_ERROR_OBJECT ) );
	assert( m_ridErrorObject != RID_NOTDEFINED );
	m_ridRegexObject = getMetaData().queryBuiltinObjectRid( wstring( NAME_BUILTIN_REGEX_OBJECT ) );
	assert( m_ridRegexObject != RID_NOTDEFINED );
	m_ridBooleanObject = getMetaData().queryBuiltinObjectRid( wstring( NAME_BUILTIN_BOOLEAN_OBJECT ) );
	assert( m_ridBooleanObject != RID_NOTDEFINED );
	m_ridNumberObject = getMetaData().queryBuiltinObjectRid( wstring( NAME_BUILTIN_NUMBER_OBJECT ) );
	assert( m_ridNumberObject != RID_NOTDEFINED );
	m_ridDateObject = getMetaData().queryBuiltinObjectRid( wstring( NAME_BUILTIN_DATE_OBJECT ) );
	assert( m_ridDateObject != RID_NOTDEFINED );
	m_ridMathObject = getMetaData().queryBuiltinObjectRid( wstring( NAME_BUILTIN_MATH_OBJECT ) );
	assert( m_ridMathObject != RID_NOTDEFINED );

	//Reserve the size
	m_BuiltinPrototypeObjectTable.reserve( m_MetaData.getBuiltinObjectTable().size() + 1 );
	m_BuiltinPrototypeObjectTable.resize( m_MetaData.getBuiltinObjectTable().size() + 1 );
	
	//Initialize this object
	//Create function object and store it to static field
	CVmObject newobject;
	newobject.setRID( m_ridObject );
	newobject.setObjectName( getMetaData().getObjectNameRID( m_ridObject ) );
	newobject.setPrototypeObject( &getPrototypeObject( getObjectRID() ) );
	getObjectPool().push_back( newobject );

	CVariable var;
	var.setObjectRef( &getObjectPool().back() );
	storeArgumentList( var );

	//Initialize global scope callstack
	VM_CALLSTACK callstack;
	callstack.type = VM_CALLSTACK_CALLSTACK;
	callstack.status = VM_EHCLAUSE_NORMAL;
	callstack.pInstruction = m_pCurrentInstruction;
	callstack.ridMethod = 0;
	callstack.iLocalVariableStartIndex = getCurrentLocalVariableIndex();
	callstack.iArgumentListStartIndex = 0;
	callstack.iNumArgument = getCurrentArgumentListIndex();		//include This object
	callstack.bConstructor = false;
	m_CallStack.push_back( callstack );

	m_iCurrentThisPointerIndex = 0;

	//
	//NOTE: Function object should be initialized first.
	//
	// Initialize Function prototype Object
	CFunctionObject function;
	bReturn = function.InitializeVm( *this );

	// Initialize System prototype Object
	CSystemObject system;
	bReturn = system.InitializeVm( *this );

	// Initialize object prototype  Object
	CObjectObject obj;
	bReturn = obj.InitializeVm( *this );

	// Initialize array prototype  Object
	CArrayObject arrayObject;
	bReturn = arrayObject.InitializeVm( *this );

	// Initialize String prototype  Object
	CStringObject stringobject;
	bReturn = stringobject.InitializeVm( *this );

	// Initialize Math prototype  Object
	CMathObject mathobject;
	bReturn = mathobject.InitializeVm( *this );

	// Initialize Number prototype  Object
	CNumberObject numberobject;
	bReturn = numberobject.InitializeVm( *this );

	// Initialize boolean prototype  Object
	CBooleanObject booleanobject;
	bReturn = booleanobject.InitializeVm( *this );

	// Initialize error prototype  Object
	CErrorObject errorobject;
	bReturn = errorobject.InitializeVm( *this );

	// Initialize date prototype  Object
	CDateObject dateobject;
	bReturn = dateobject.InitializeVm( *this );

	// Initialize Regex prototype Object
	CRegexObject regex;
	bReturn = regex.InitializeVm( *this );

	//Also initialize built in values
	assert( OperandType(m_pStaticFieldTable[ RidFromToken( RID_CONSTANT_BOOL_TRUE ) ].iOperandType) == OPERAND_BOOLEAN );
	m_pStaticFieldTable[ RidFromToken( RID_CONSTANT_BOOL_TRUE ) ].iValue = true;

	assert( OperandType(m_pStaticFieldTable[ RidFromToken( RID_CONSTANT_BOOL_FALSE ) ].iOperandType) == OPERAND_BOOLEAN );
	m_pStaticFieldTable[ RidFromToken( RID_CONSTANT_BOOL_FALSE ) ].iValue = false;

	assert( OperandType(m_pStaticFieldTable[ RidFromToken( RID_CONSTANT_NAN ) ].iOperandType) == OPERAND_NAN );
	m_pStaticFieldTable[ RidFromToken( RID_CONSTANT_NAN ) ].iValue = 0;

	assert( OperandType(m_pStaticFieldTable[ RidFromToken( RID_CONSTANT_INFINITY ) ].iOperandType) == OPERAND_FLOAT );
	m_pStaticFieldTable[ RidFromToken( RID_CONSTANT_INFINITY ) ].iValue = 0x7f800000;
}

/***************************************************************************
 * Reset the VM execution status
 ***************************************************************************/	
bool CCilVm::reset( const VM_RESET_TYPE iResetType )
{
	assert( iResetType == VM_RESET_DEFAULT );

	bool bReturn = true;

	//Flush existing status
	flushPrintBuffer();

	if( m_status == VM_EXECUTE_INITIALIZING )
		return false;	//Could not set to Initialized status

	if( m_status == VM_EXECUTE_INITIALIZED )
		return true;	//just return

	m_status = VM_EXECUTE_INITIALIZED;

	//Rest stacks
	clearCallStack();
	clearEvalStack();
	clearStaticFields();
	clearLocalVariableList();
	clearArgList();

	//Reset obj pool
	cleanObjectPool();
	m_ObjectPool.clear();

	m_BuiltinPrototypeObjectTable.clear();

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

	//Reset IP
	m_pCurrentInstruction = &m_CurrentILPool.ILPool[ 0 ];

	//Reset timer
	m_stopwatch.Reset();

	return bReturn;
};

/***************************************************************************
 * Verifier
 ***************************************************************************/	
bool CCilVm::verify()
{
	return true;
};

} //namespace CRI