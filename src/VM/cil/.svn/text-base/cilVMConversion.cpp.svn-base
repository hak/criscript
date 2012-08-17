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
 * File     : cilVMConversion.cpp.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "criScript.h"
#include "cilVM.h"
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *      Boolean Conversion
 ***************************************************************************/
void CCilVm::convertToBoolean()
{
	CVariable& var = getEvalStackFirstEntry();
	switch( OperandType( var.iOperandType ) )
	{
	case OPERAND_UNDEFINED:
	case OPERAND_NULL:
	case OPERAND_NAN:
		var.iOperandType = OPERAND_BOOLEAN;
		var.bValue = false;
		break;
	case OPERAND_OBJECTREF:
		{
		bool b = var.refObject->getValue().toBoolean();
		if( var.refObject )
			var.refObject->release();
		var.iOperandType = OPERAND_BOOLEAN;
		var.bValue = b;
		var.bValue = true;
		}
		break;
	case OPERAND_INT:
	case OPERAND_UNSIGNEDINT:
	case OPERAND_INT64:
	case OPERAND_UNSIGNEDINT64:
		var.iOperandType = OPERAND_BOOLEAN;
		break;
	case OPERAND_FLOAT:
		var.iOperandType = OPERAND_BOOLEAN;
		if( var.fValue == 0 )
			var.bValue = false;
		else
			var.bValue = true;
		break;
	case OPERAND_DOUBLE:
		var.iOperandType = OPERAND_BOOLEAN;
		if( var.dValue == 0 )
			var.bValue = false;
		else
			var.bValue = true;
		break;
	case OPERAND_STRING:
		var.iOperandType = OPERAND_BOOLEAN;
		if( !var.pString->size() )
		{
			delete( var.pString );
			var.bValue = false;
		}
		else
		{
			delete( var.pString );
			var.bValue = true;
		}
		break;
	case OPERAND_BOOLEAN:
		//Do nothing
		break;
	default:
		Debug_Fatal( "Unsupported Type conversion" );
		break;
	}

}


} //namespace CRI