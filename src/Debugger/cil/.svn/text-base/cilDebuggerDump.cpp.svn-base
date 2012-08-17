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
 * Module   : cil Virtual Machine Debugger
 * File     : cilDebuggerdump.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
#include "cilVm.h"
#include "cilDebugger.h"
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      dump VM Stats
 ***************************************************************************/
void CCilDebugger::debugDumpStats( void )
{
	assert( m_pVM != NULL );
	m_pVM->debugDumpStats();
}

/***************************************************************************
 *      dump VM Static fields
 ***************************************************************************/
void CCilDebugger::debugDumpStaticFields( uint32_t iMaxIteration )
{
	assert( m_pVM != NULL );
	uint32_t iNumDump = 0;
	CG_SYMBOL_INFORMATION info;
	hash_map<wstring, CG_SYMBOL_INFORMATION>::iterator it;
	
	cout << "Static field values:\n";
	cout << m_SymbolInfoPool.size() << " fields defined\n"; 

	for( it = m_SymbolInfoPool.begin();	it != m_SymbolInfoPool.end(); ++it )
	{
		if( iMaxIteration && iNumDump ++ >= iMaxIteration )
			break;
		info = (*it).second;
		switch( TypeFromToken( info.rid ) )
		{
		case MDT_FIELDDEF:
			{
			CVariable& slot = m_pVM->getStaticField( info.rid );

			if( !(slot.iOperandFlag & OPERAND_FLAG_DONTENUM) )
			{		
				wcout << (*it).first << "\t";
				if( slot.iOperandFlag & OPERAND_FLAG_STRICT )
				{
					cout << "s";
				}
				if( !(slot.iOperandFlag & OPERAND_FLAG_READONLY) )
				{
					cout << "w";
				}
				if( !(slot.iOperandFlag & OPERAND_FLAG_DONTDELETE) )
				{
					cout << "d";
				}

				cout << "\t:";
				int32_t iOperand = OperandType( slot.iOperandType );
				switch( iOperand )
				{
				case 	OPERAND_NULL:
					cout << "NULL";
					break;
				case 	OPERAND_OBJECTREF:
					cout << "ObjRef: " << slot.refObject;
					break;
				case 	OPERAND_UNDEFINED:
					cout << "undefined";
					break;
				case 	OPERAND_NAN:
					cout << "NaN";
					break;
				case 	OPERAND_BOOLEAN:
					cout << "Bool: " << slot.bValue;
					break;
				case 	OPERAND_INT:
					cout << "int32_t: " << slot.iValue;
					break;
				case 	OPERAND_INT64:
					cout << "Int64: " << slot.i64Value;
					break;
				case 	OPERAND_FLOAT:
					cout << "Float: " << slot.fValue;
					break;
				case 	OPERAND_DOUBLE:
					cout << "Double: " << slot.dValue;
					break;
				case 	OPERAND_STRING:
					wcout << L"String: \"" << (slot.pString->data()) << L"\"";
					break;
				case 	OPERAND_SYSTEM_DELETION:
					cout << "deleted";
					break;
				}
				cout << "\n";
			}
			break;
			}
		case MDT_TYPEDEF:
		case MDT_METHODDEF:
		case MDT_PARAMDEF:
		case MDT_NAME:
		case MDT_PROPERTY:
			break;
		default:
			Debug_Fatal( "Unknown field\n" );
			break;
		}
	}


}

/***************************************************************************
 *      dump VM Objects
 ***************************************************************************/
void CCilDebugger::debugDumpObjects( uint32_t )
{
	assert( m_pVM != NULL );
}


} //namespace CRI