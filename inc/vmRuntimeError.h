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
 * Module   : VM runtime error definitions
 * File     : vmRuntimeError.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once

using namespace std;

namespace cri {

/***************************************************************************
 *      VM runtime error code that is thrown from VM
 ***************************************************************************/
const int32_t ERROR_TYPEERROR_ARRAY_TOSTRING	= 0x800A13A7;
const int32_t ERROR_TYPEERROR_DATE_TOSTRING		= 0x800A138E;
const int32_t ERROR_TYPEERROR_NUMBER_TOSTRING	= 0x800A1389;
const int32_t ERROR_TYPEERROR_STORESTATICFIELD	= 0x800A000D;
const int32_t ERROR_TYPEERROR_UNDEFINEDPROPERTY	= 0x800A01A8;
const int32_t ERROR_ILLEGALCALL					= 0x800A138A;
const int32_t ERROR_OUT_OF_MEMORY				= 0x800A01A8;
const int32_t ERROR_ARRAY_LENGTH				= 0x800A13A5;

const wchar_t ERRORSTRING_TYPEERROR_ARRAY_TOSTRING[] =	L"Array::toString TypeError";
const wchar_t ERRORSTRING_TYPEERROR_DATE_TOSTRING[]	=	L"Date::toString TypeError";
const wchar_t ERRORSTRING_TYPEERROR_DATE_CTOR[]	=		L"Date::ctor TypeError";
const wchar_t ERRORSTRING_TYPEERROR_NUMBER_TOSTRING[] = L"Number::toString TypeError";
const wchar_t ERRORSTRING_TYPEERROR_STORESTATICFIELD[] =L"Global::Store field TypeError";
const wchar_t ERRORSTRING_ILLEGALCALL[] =		L"Function expected";
const wchar_t ERRORSTRING_TYPEERROR_UNDEFINEDPROPERTY[]=L"Object required";

const wchar_t ERRORSTRING_OUTOFMEMORY[] =	L"Out of memory";
const wchar_t ERRORSTRING_ARRAY_LENGTH[]=	L"Invalid Array length";


}//namespace cri
