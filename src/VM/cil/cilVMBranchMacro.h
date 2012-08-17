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
 * File     : cilVMBranchMacro.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once

/***************************************************************************
 *      Include file
 ***************************************************************************/
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

#define CheckBranch(OP,OP2)\
	{\
	wstring* pstrLhs;\
	wstring* pstrRhs;\
	lhs.iOperandType = (OPERAND_TYPE)OperandType( lhs.iOperandType );\
	rhs.iOperandType = (OPERAND_TYPE)OperandType( rhs.iOperandType );\
\
	if( lhs.iOperandType == rhs.iOperandType )\
	{\
		switch( lhs.iOperandType )\
		{\
		case OPERAND_UNDEFINED:\
			bTrue = true;\
			break;\
		case OPERAND_NULL:\
			bTrue = 0 OP 0;\
			break;\
		case OPERAND_NAN:\
			bTrue = false;\
			break;\
		case OPERAND_BOOLEAN:\
			bTrue = (lhs.bValue OP rhs.bValue );\
			break;\
		case OPERAND_INT:\
			bTrue = (lhs.iValue OP rhs.iValue );\
			break;\
		case OPERAND_INT64:\
			bTrue = (lhs.i64Value OP rhs.i64Value );\
			break;\
		case OPERAND_FLOAT:\
			bTrue = (lhs.fValue OP rhs.fValue );\
			break;\
		case OPERAND_DOUBLE:\
			bTrue = (lhs.dValue OP rhs.dValue );\
			break;\
		case OPERAND_OBJECTREF:\
			switch( rhs.getPrimitiveType() )\
			{\
			case OPERAND_BOOLEAN:\
				bTrue = (lhs.toInt() OP rhs.toInt() );\
				break;\
			case OPERAND_INT:\
				bTrue = (lhs.toInt() OP rhs.toInt() );\
				break;\
			case OPERAND_INT64:\
				bTrue = (lhs.toInt64() OP rhs.toInt64() );\
				break;\
			case OPERAND_FLOAT:\
				bTrue = (lhs.toFloat() OP rhs.toFloat() );\
				break;\
			case OPERAND_DOUBLE:\
				bTrue = (lhs.toDouble() OP rhs.toDouble() );\
				break;\
			case OPERAND_STRING:\
			default:\
				pstrLhs = lhs.toString( this );\
				pstrRhs = rhs.toString( this );\
				bTrue = *pstrLhs OP *pstrRhs;\
				delete( pstrLhs );\
				delete( pstrRhs );\
				break;\
			}\
			break;\
		case OPERAND_STRING:\
			bTrue = *lhs.pString OP *rhs.pString;\
			break;\
		default:\
			bTrue = false;\
			break;\
		}\
	}\
	else\
	{\
		/*Each case should have 7 entries
		// OPERAND_BOOLEAN
		// OPERAND_INT
		// OPERAND_INT64
		// OPERAND_FLOAT
		// OPERAND_DOUBLE
		// OPERAND_OBJECTREF
		// OPERAND_STRING*/\
		if( rhs.iOperandType == OPERAND_OBJECTREF )\
		{\
			switch( rhs.getPrimitiveType() )\
			{\
			case OPERAND_BOOLEAN:\
				bTrue = (lhs.toInt() OP rhs.toInt() );\
				break;\
			case OPERAND_INT:\
				bTrue = (lhs.toInt() OP rhs.toInt() );\
				break;\
			case OPERAND_INT64:\
				bTrue = (lhs.toInt64() OP rhs.toInt64() );\
				break;\
			case OPERAND_FLOAT:\
				bTrue = (lhs.toFloat() OP rhs.toFloat() );\
				break;\
			case OPERAND_DOUBLE:\
				bTrue = (lhs.toDouble() OP rhs.toDouble() );\
				break;\
			case OPERAND_STRING:\
			default:\
				pstrLhs = lhs.toString( this );\
				pstrRhs = rhs.toString( this );\
				bTrue = *pstrLhs OP *pstrRhs;\
				delete( pstrLhs );\
				delete( pstrRhs );\
				break;\
			}\
		}\
		else if( lhs.iOperandType == OPERAND_NAN\
			|| rhs.iOperandType == OPERAND_NAN )\
		{\
			bTrue = false;\
		}\
		else\
		{\
			switch( lhs.iOperandType )\
			{\
			case OPERAND_NULL:\
				switch( rhs.iOperandType )\
				{\
				case OPERAND_UNDEFINED:\
					bTrue = true;\
					break;\
				default:\
					bTrue = (0 OP rhs.toInt());\
					break;\
				}\
				break;\
			case OPERAND_UNDEFINED:\
				if( rhs.iOperandType == OPERAND_NULL ) bTrue = true;\
				break;\
			case OPERAND_STRING:\
				switch( rhs.iOperandType )\
				{\
				case OPERAND_BOOLEAN:\
					bTrue = ( lhs.toInt() OP rhs.iValue );\
					break;\
				case OPERAND_INT:\
					bTrue = (lhs.toInt() OP rhs.iValue );\
					break;\
				case OPERAND_INT64:\
					bTrue = ((int64_t)lhs.toInt64() OP rhs.i64Value );\
					break;\
				case OPERAND_FLOAT:\
					bTrue = (lhs.toFloat() OP rhs.fValue );\
					break;\
				case OPERAND_DOUBLE:\
					bTrue = (lhs.toDouble() OP rhs.dValue );\
					break;\
				case OPERAND_NULL:\
					bTrue = (lhs.toInt() OP 0 );\
					break;\
				default:\
					bTrue = false;\
					break;\
				}\
				break;\
			case OPERAND_BOOLEAN:\
				switch( rhs.iOperandType )\
				{\
				case OPERAND_INT:\
					bTrue = (int32_t(lhs.bValue) OP rhs.iValue );\
					break;\
				case OPERAND_INT64:\
					bTrue = (int64_t(lhs.bValue) OP rhs.i64Value );\
					break;\
				case OPERAND_STRING:\
					bTrue = (int32_t(lhs.bValue) OP rhs.toInt());\
					break;\
				case OPERAND_FLOAT:\
					bTrue = (float(lhs.bValue) OP rhs.fValue);\
					break;\
				case OPERAND_DOUBLE:\
					bTrue = (double(lhs.bValue) OP rhs.dValue);\
					break;\
				case OPERAND_NULL:\
					bTrue = (lhs.toInt() OP 0 );\
					break;\
				case OPERAND_BOOLEAN:\
					Debug_Fatal( "Illegal operand" );\
				default:\
					bTrue = false;\
					break;\
				}\
				break;\
			case OPERAND_INT:\
				switch( rhs.iOperandType )\
				{\
				case OPERAND_BOOLEAN:\
					bTrue = (lhs.iValue OP rhs.iValue );\
					break;\
				case OPERAND_INT64:\
					bTrue = (int64_t(lhs.iValue) OP rhs.i64Value );\
					break;\
				case OPERAND_STRING:\
					bTrue = (lhs.iValue OP rhs.toInt());\
					break;\
				case OPERAND_FLOAT:\
					bTrue = (float(lhs.iValue) OP rhs.fValue);\
					break;\
				case OPERAND_DOUBLE:\
					bTrue = (double(lhs.iValue) OP rhs.dValue);\
					break;\
				case OPERAND_NULL:\
					bTrue = (lhs.iValue OP 0 );\
					break;\
				case OPERAND_INT:\
					Debug_Fatal( "Illegal operand" );\
				default:\
					bTrue = false;\
					break;\
				}\
				break;\
			case OPERAND_INT64:\
				switch( rhs.iOperandType )\
				{\
				case OPERAND_BOOLEAN:\
					bTrue = (lhs.i64Value OP rhs.iValue );\
					break;\
				case OPERAND_INT:\
					bTrue = (int32_t(lhs.i64Value) OP rhs.iValue );\
					break;\
				case OPERAND_STRING:\
					bTrue = (lhs.i64Value OP rhs.toInt());\
					break;\
				case OPERAND_FLOAT:\
					bTrue = (float(lhs.i64Value) OP rhs.fValue);\
					break;\
				case OPERAND_DOUBLE:\
					bTrue = (double(lhs.i64Value) OP rhs.dValue);\
					break;\
				case OPERAND_NULL:\
					bTrue = (lhs.i64Value OP 0 );\
					break;\
				case OPERAND_INT64:\
					Debug_Fatal( "Illegal operand" );\
				default:\
					bTrue = false;\
					break;\
				}\
				break;\
			case OPERAND_FLOAT:\
				switch( rhs.iOperandType )\
				{\
				case OPERAND_BOOLEAN:\
					bTrue = (lhs.fValue OP rhs.iValue );\
					break;\
				case OPERAND_INT:\
					bTrue = (int32_t(lhs.fValue) OP rhs.iValue );\
					break;\
				case OPERAND_INT64:\
					bTrue = (int64_t(lhs.fValue) OP rhs.i64Value );\
					break;\
				case OPERAND_STRING:\
					bTrue = (lhs.fValue OP rhs.toFloat());\
					break;\
				case OPERAND_DOUBLE:\
					bTrue = (double(lhs.fValue) OP rhs.dValue);\
					break;\
				case OPERAND_NULL:\
					bTrue = (lhs.fValue OP 0 );\
					break;\
				case OPERAND_FLOAT:\
					Debug_Fatal( "Illegal operand" );\
				default:\
					bTrue = false;\
					break;\
				}\
				break;\
			case OPERAND_DOUBLE:\
				switch( rhs.iOperandType )\
				{\
				case OPERAND_BOOLEAN:\
					bTrue = (lhs.toInt() OP rhs.iValue );\
					break;\
				case OPERAND_INT:\
					bTrue = (int32_t(lhs.dValue) OP rhs.iValue );\
					break;\
				case OPERAND_INT64:\
					bTrue = (int64_t(lhs.dValue) OP rhs.i64Value );\
					break;\
				case OPERAND_STRING:\
					bTrue = (lhs.dValue OP rhs.toDouble());\
					break;\
				case OPERAND_FLOAT:\
					bTrue = (float(lhs.dValue) OP rhs.fValue);\
					break;\
				case OPERAND_NULL:\
					bTrue = (lhs.dValue OP 0 );\
					break;\
				case OPERAND_DOUBLE:\
					Debug_Fatal( "Illegal operand" );\
				default:\
					bTrue = false;\
					break;\
				}\
				break;\
			case OPERAND_OBJECTREF:\
				switch( lhs.getPrimitiveType() )\
				{\
				case OPERAND_INT:\
					switch( rhs.iOperandType )\
					{\
					case OPERAND_BOOLEAN:\
						bTrue = (lhs.toInt() OP rhs.iValue );\
						break;\
					case OPERAND_INT:\
						bTrue = (lhs.toInt() OP rhs.iValue );\
						break;\
					case OPERAND_INT64:\
						bTrue = (int64_t(lhs.toInt()) OP rhs.i64Value );\
						break;\
					case OPERAND_STRING:\
						pstrLhs = lhs.toString( this );\
						pstrRhs = rhs.toString( this );\
						bTrue = *pstrLhs OP *pstrRhs;\
						delete( pstrLhs );\
						delete( pstrRhs );\
						break;\
					case OPERAND_FLOAT:\
						bTrue = (lhs.toFloat() OP rhs.fValue);\
						break;\
					case OPERAND_DOUBLE:\
						bTrue = (lhs.toDouble() OP rhs.dValue);\
						break;\
					case OPERAND_OBJECTREF:\
						Debug_Fatal( "Illegal operand" );\
						break;\
					default:\
						bTrue = false;\
						break;\
					}\
					break;\
				case OPERAND_STRING:\
					pstrLhs = lhs.toString( this );\
					pstrRhs = rhs.toString( this );\
					bTrue = *pstrLhs OP *pstrRhs;\
					delete( pstrLhs );\
					delete( pstrRhs );\
					break;\
				case OPERAND_BOOLEAN:\
					bTrue = lhs.toInt() OP rhs.toInt();\
					break;\
				default:\
					bTrue = false;\
					break;\
				}\
				break;\
			default:\
				Debug_Fatal( "Illegal operand" );\
			}\
		}\
	}\
	}\

//End of macro


} //namespace CRI