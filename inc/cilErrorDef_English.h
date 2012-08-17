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
 * Module   : JScript Error definitions
 * File     : ErrorDef_English.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once

#include "stdafx.h"
#include "Error.h"

namespace cri {
	
const ERROR_DEF g_ErrorTable_English[] = {
//Tool error code
{ ERROR_FILENOTFOUND,	E_ERROR, true,  L"File " INFO_STRING L" not found" },
{ ERROR_INVALIDFILEENCODING,			E_ERROR, false,  L"Source File format is not in UTF16(BOM)" },
{ ERROR_INVALIDOPTIONSWITCH,			E_ERROR, true,  L"Unknown option switch '" INFO_STRING L"'" },

//Compiler error code
{ ERROR_CANNOTHAVEBREAK,		E_ERROR, false, L"Can't have 'break' outside of an iteration or a switch statement" },
{ ERROR_CANNOTHAVECONTINUE,		E_ERROR, false, L"Can't have 'continue' outside of an iteration statement" },
{ ERROR_CONDITIONALCOMPILATION, E_ERROR, false, L"Conditional compilation is turned off" },
{ ERROR_CANNOTHAVEDEFAULT,		E_ERROR, false, L"'default' can only appear once in a 'switch' statement" },
{ ERROR_EXPECTED_LP,			E_ERROR, false, L"Expected '('" },
{ ERROR_EXPECTED_RP,			E_ERROR, false, L"Expected ')'" },
{ ERROR_EXPECTED_SLASH,			E_ERROR, false, L"Expected '/" },
{ ERROR_EXPECTED_COLON,			E_ERROR, false, L"Expected ':'" },
{ ERROR_EXPECTED_SEMICOLON,		E_ERROR, false, L"Expected ';'" },
{ ERROR_EXPECTED_AT,			E_ERROR, false, L"Expected '@'" },
{ ERROR_EXPECTED_ATEND,			E_ERROR, false, L"Expected '@end'" },
{ ERROR_EXPECTED_RSB,			E_ERROR, false, L"Expected ']'" },
{ ERROR_EXPECTED_LCB,			E_ERROR, false, L"Expected '{'" },
{ ERROR_EXPECTED_RCB,			E_ERROR, false, L"Expected '}'" },
{ ERROR_EXPECTED_EQU,			E_ERROR, false, L"Expected '='" },
{ ERROR_EXPECTED_CATCH,			E_ERROR, false, L"Expected 'catch' or 'finally'" },
{ ERROR_EXPECTED_CONSTANT,		E_ERROR, false, L"Expected constant" },
{ ERROR_EXPECTED_HEX,			E_ERROR, false, L"Expected hexadecimal digit" },
{ ERROR_EXPECTED_ID,			E_ERROR, false, L"Expected identifier" },
{ ERROR_EXPECTED_ID_STRING_NUM, E_ERROR, false, L"Expected identifier, string or number" },
{ ERROR_EXPECTED_WHILE,			E_ERROR, false, L"Expected 'while'" },
{ ERROR_INVALIDCHARACTER,		E_ERROR, false, L"Invalid character" },
{ ERROR_LABELNOTFOUND,			E_ERROR, true,  L"Label \"" INFO_STRING L"\" not found" },
{ ERROR_LABELALREADYDEFINED,	E_ERROR, false, L"Label redefined" },
{ ERROR_INVALIDRETURN,			E_ERROR, false, L"'return' statement outside of function" },
{ ERROR_SYNTAXERROR,			E_ERROR, false, L"Syntax error" },
{ ERROR_INVALIDTHROW,			E_ERROR, false, L"Throw must be followed by an expression on the same source line" },
{ ERROR_INVALIDCOMMENT,			E_ERROR, false, L"Unterminated comment" },
{ ERROR_INVALIDSTRING,			E_ERROR, false, L"Unterminated string constant. \" expected." },
{ ERROR_INVALIDSTRINGCOLON,		E_ERROR, false, L"Unterminated string constant. ' expected." },
{ ERROR_RESERVEDWORD,			E_ERROR, true,  L"Reserved word \"" INFO_STRING L"\" can not be used." },
{ ERROR_IDALREADYDEFINED,		E_ERROR, true,  L"identifier \"" INFO_STRING L"\" already defined" },
{ ERROR_STATEMENTNOTALLOWED,	E_ERROR, false, L"Statement is not allowed" },
{ ERROR_FOR_INFINITE,			E_ERROR, false, L"Infinite 'for' loop is not allowed" },
{ ERROR_INTERNALIDALREADYDEFINED,E_ERROR, true,  L"Internal identifier \"" INFO_STRING L"\" already defined" },
{ ERROR_PARAMETERALREADYDEFINED,E_ERROR, true,  L"An argument \"" INFO_STRING L"\" already defined" },
{ ERROR_FUNCTIONNAMENOTFOUND,	E_ERROR, true,  L"Could not find a function name \"" INFO_STRING L"\"" },
{ ERROR_METHODNAMEASSIGNMENTNOTALLOWED,	E_ERROR, true,  L"Assigment to \"" INFO_STRING L"\" is not allowed by the compile setting" },
{ ERROR_ILLEGALCALLTYPE,	E_ERROR, true,  L"\"" INFO_STRING L"\" can not be a function nor a pointer" },
{ ERROR_CANNOTASSIGNCONSTANT,	E_ERROR, false,  L"Can not assign to a constant value" },
{ ERROR_CANNOTASSIGNREADONLY,	E_ERROR, true,  L"Can not assign to a readonly variable \"" INFO_STRING L"\"" },
{ ERROR_ONLYONEDEFAULTISALLOWED,	E_ERROR, false,  L"Just 1 default clause is allowed inside a switch clause" },
{ ERROR_CANNOTHAVECASE,			E_ERROR, false,  L"Can't have 'case/default' outside of a switch statement" },
{ ERROR_NOOBJECTCTOR,			E_ERROR, true,  L"The object \"" INFO_STRING L"\" does not have a constructor" },
{ ERROR_FUNCTIONCTORNOTSUPPORTED,			E_ERROR, false,  L"The Function constructor with arguments is not supported in this version" },
{ ERROR_WITHONLYACCEPTOBJREF,	E_ERROR, false,  L"'with' statement only accepts an object definition as the argument in this version for a performance reason" },
{ ERROR_OBJECTNAMENOTFOUND,		E_ERROR, true,  L"Can not find an object name \"" INFO_STRING L"\"" },
{ ERROR_BADREGEXLITERALFORMAT,	E_ERROR, false,  L"Bad RegExp literal format" },
{ ERROR_ILLEGALTYPESPECIFIER,	E_ERROR, true,  L"Illegal type name \"" INFO_STRING L"\"" },
{ ERROR_DYNAMICEVALNOTSUPPORTED,	E_ERROR, false,  L"Dynamic eval not supported in this version." },

{ INFO_CODE_NOEFFECT,			E_INFORMATION, false, L"A code has no effect" },
{ INFO_IF_NOEFFECT,				E_INFORMATION, false, L"IF statement has no effect" },
{ INFO_FOR_NOEFFECT,			E_INFORMATION, false, L"For statement has no effect" },
{ INFO_IDNOTREGISTERED,			E_INFORMATION, true, L"Variable \"" INFO_STRING L"\" is not registered explicitly" },
{ INFO_VARIABLENOTASSIGNED,		E_INFORMATION, true, L"Variable \"" INFO_STRING L"\" is not initialized here" },
{ INFO_DIFFERENTNUMARGS,		E_INFORMATION, false, L"A number of given arguments differs from the function signature" },
{ INFO_OBJECTINITIALIZORDUPLICATION, E_INFORMATION, true, L"Duplicating object property name \"" INFO_STRING L"\"." },
{ INFO_arrayLiteralTOOLARGE,	E_INFORMATION, false, L"Array size could be too large." },
{ INFO_INVOKINGVARIABLE,		E_INFORMATION, true,  L"Invoking variable \"" INFO_STRING L"\". Could involve runtime exception if the variable is not a function reference." },


//Runtime errors
{ ERROR_R_INVALIDARRAYLENGTH,	E_ERROR, false, L"Array length must be a finite positive integer" },
{ ERROR_R_INVALIDARRAYLENGTH2,	E_ERROR, false, L"Array length must be assigned a finite positive number" },
{ ERROR_R_EXPECTED_ARRAYOROBJECT, E_ERROR, false, L"Array or arguments object expected" },
{ ERROR_R_EXPECTED_BOOLEAN,		E_ERROR, false, L"Boolean expected" },
{ ERROR_R_CANNOTASSIGN_FUNCRESULT, E_ERROR, false, L"Could not assign to a function result" },
{ ERROR_R_CANNOTASSIGN_THIS,	E_ERROR, false, L"Could not assign to 'this'" },
{ ERROR_R_EXPECTED_DATE,		E_ERROR, false, L"Date object expected" },
{ ERROR_R_EXPECTED_ENUM,		E_ERROR, false, L"Enumerator object expected" },
{ ERROR_R_EXCEPTION_NOTCAUGHT,	E_ERROR, false, L"Exception thrown and not caught" },
{ ERROR_R_REG_EXPECTED_RP,		E_ERROR, false, L"Expected ')' in regular expression" },
{ ERROR_R_EXPECTED_RSB,			E_ERROR, false, L"Expected ']' in regular expression" },
{ ERROR_R_INVALID_FUNCPROTOTYPE,E_ERROR, false, L"Function does not have a valid prototype object" },
{ ERROR_R_EXPECTED_FUNCTION,	E_ERROR, false, L"Function expected" },
{ ERROR_R_ILLEGALASSIGNMENT,	E_ERROR, false, L"Illegal assignment" },
{ ERROR_R_INVALID_CHARACTER,	E_ERROR, false, L"Invalid range in character set" },
{ ERROR_R_EXPECTED_JSOBJECT,	E_ERROR, false, L"JScript object expected" },
{ ERROR_R_EXPECTED_NUMBER,		E_ERROR, false, L"Number expected" },
{ ERROR_R_EXPECTED_OBJECT,		E_ERROR, false, L"Object expected" },
{ ERROR_R_EXPECTED_OBJECTMEMBER,E_ERROR, false, L"Object member expected" },
{ ERROR_R_EXPECTED_REGEX,		E_ERROR, false, L"Regular Expression object expected" },
{ ERROR_R_EXPECTED_STRING,		E_ERROR, false, L"String expected" },
{ ERROR_R_REG_SYNTAXERROR,		E_ERROR, false, L"Syntax error in regular expression" },
{ ERROR_R_FRACTION_OUTOFRANGE,	E_ERROR, false, L"The number of fractional digits is out of range" },
{ ERROR_R_PRECISION_OUTOFRANGE,	E_ERROR, false, L"The precision is out of range" },
{ ERROR_R_INVALID_URIENCODE,	E_ERROR, false, L"The URI to be decoded is not a valid encoding" },
{ ERROR_R_INVALID_URICHARACTER, E_ERROR, false, L"The URI to be encoded contains an invalid character" },
{ ERROR_R_UNDEFINED_IDENTIFIER, E_ERROR, false, L"Undefined identifier" },
{ ERROR_R_UNEXPECTED_QUANTIFIER,E_ERROR, false, L"Unexpected quantifier" },
{ ERROR_R_EXPECTED_VBARRAY,		E_ERROR, false, L"VBArray expected" },

};

}	//namespace cri