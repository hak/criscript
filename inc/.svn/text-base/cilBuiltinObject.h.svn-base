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
 * Module   : cil built in objectdefinitions
 * File     : cilBuiltinObject.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once

using namespace std;

#include "Lexer.h"

namespace cri {
	//---------------------------------------------------------------------------
	// Structure
	//---------------------------------------------------------------------------
	struct METHOD_INITIALIZATION_PARAM {
		const wchar_t* pStrSymbol;
		METHOD_TYPE methodType;
		PMETHOD pMethod;
		RID_BUILTIN_PARAM ridParam;
	};

	struct PROPERTY_INITIALIZATION_PARAM {
		const wchar_t* pStrSymbol;
		OPERAND_TYPE operandType;
		OPERAND_FLAG operandFlag;
		union {
			float	fValue;
			int32_t	iValue;
			uint32_t uiValue;
			PACCESSORMETHOD pMethod;
		};
	};

	//---------------------------------------------------------------------------
	// Constants
	//---------------------------------------------------------------------------
	const int32_t CONSTANT_ZERO = 0;
	const int32_t CONSTANT_ONE = 1;
	const RID RID_NOTDEFINED = -1;
	const RID RID_NULL = 0;
	const RID RID_LABELNAME = MDT_NAME | 0;
	const RID RID_NEED_CATCHCLAUSE = -1;		/*EN< Internal RID definition when a EH still needed */

	const int32_t SIZE_STRING = 256;
	const wchar_t STRING_INTERNAL_LABEL_PREFIX[]		= L"#%04x";
	const wchar_t STRING_INTERNAL_CATCHLABEL_PREFIX[]	= L"#C%04x";
	const wchar_t STRING_INTERNAL_FINALLYLABEL_PREFIX[]	= L"#F%04x";
	const wchar_t STRING_INTERNAL_TRYLABEL_PREFIX[]		= L"#T%04x";
	const wchar_t STRING_INTERNAL_SCOPEDELIMITER[]		= L".";
	const wchar_t STRING_INTERNAL_SCOPEDELIMITER_SINGLE	= L'.';
	const wchar_t STRING_INTERNAL_JOIN_DEFAULT_SEPARATOR[] = L",";
	const wchar_t STRING_OBJECTNAME_POSTFIX[]			= L"Object";
	const wchar_t STRING_TEMPVARIABLENAME_PREFIX[]		= L"Tmp";

	const wchar_t STRING_INTERNAL_CONSTRUCTORNAME[] = L"constructor";
	const wchar_t STRING_INTERNAL_CALLNAME[] = L"call";

	const wchar_t STRING_INTERNAL_UNDEFINED[]	= L"undefined";
	const wchar_t STRING_INTERNAL_NULL[]		= L"null";
	const wchar_t STRING_INTERNAL_THIS[]		= L"this";
	const wchar_t STRING_INTERNAL_TRUE[]		= L"true";
	const wchar_t STRING_INTERNAL_FALSE[]		= L"false";
	const wchar_t STRING_INTERNAL_SWITCHTMP[]	= L"switchTemporaryField";
	const wchar_t STRING_INTERNAL_DELETION[]	= L"deletion";
	const wchar_t STRING_INTERNAL_PROTOTYPE[]	= L"prototype";
	const wchar_t STRING_INTERNAL_LENGTH[]		= L"length";
	const wchar_t STRING_INTERNAL_TOSTRING[]	= L"toString";
	const wchar_t STRING_INTERNAL_NAN[]			= L"NaN";
	const wchar_t STRING_INTERNAL_INFINITY[]	= L"Infinity";
	const wchar_t STRING_INTERNAL_NEGATIVE_INFINITY[]	= L"-Infinity";
	const wchar_t STRING_INTERNAL_INFINITY_TOBEREPLACED[] = L"1.#INF";
	const wchar_t STRING_INTERNAL_ARGUMENTSTMP[]	= L"argumentsTemporaryField";

	/*EN Property name that is used internnaly for Array.length accessor*/
	const wchar_t STRING_PROPERTY_INTERNAL_LENGTH[] = L".length";	

	/*EN Property setter/getter name that will be invoked for any value that a property is not set in the object*/
	const wchar_t STRING_PROPERTY_INTERNAL_ANY[]	= L".any";
	
	const wchar_t STRING_INTERNAL_TYPEOF_OBJECT[]	= L"object";
	const wchar_t STRING_INTERNAL_TYPEOF_FUNCTION[]	= L"function";
	const wchar_t STRING_INTERNAL_TYPEOF_STRING[]	= L"string";
	const wchar_t STRING_INTERNAL_TYPEOF_NUMBER[]	= L"number";
	const wchar_t STRING_INTERNAL_TYPEOF_BOOLEAN[]	= L"boolean";

	const wchar_t NAME_BUILTIN_OBJECT_OBJECT[] = L"Object";
	const wchar_t NAME_BUILTIN_OBJECT_OBJECT_CALL[] = L"Object.call";
	const wchar_t NAME_BUILTIN_OBJECT_OBJECT_CTOR[] = L"Object.constructor";
	const wchar_t NAME_BUILTIN_OBJECT_OBJECT_TOSTRING[] = L"Object.toString";
	const wchar_t NAME_BUILTIN_OBJECT_OBJECT_TOLOCALESTRING[] = L"Object.toLocaleString";
	const wchar_t NAME_BUILTIN_OBJECT_OBJECT_ARGUMENTS[] = L"Object.arguments";
	const wchar_t NAME_BUILTIN_OBJECT_OBJECT_CALLER[] = L"Object.caller";
	const wchar_t NAME_BUILTIN_OBJECT_OBJECT_VALUEOF[] = L"Object.valueOf";
	const wchar_t NAME_BUILTIN_OBJECT_OBJECT_HASOWNPROPERTY[] = L"Object.hasOwnProperty";
	const wchar_t NAME_BUILTIN_OBJECT_OBJECT_ISPROTOTYPEOF[] = L"Object.isPrototypeOf";
	const wchar_t NAME_BUILTIN_OBJECT_OBJECT_PROPERTYISENUMERABLE[] = L"Object.propertyIsEnumerable";

	const wchar_t NAME_BUILTIN_ARRAY_OBJECT[] = L"Array";
	const wchar_t NAME_BUILTIN_ARRAY_OBJECT_CALL[] = L"Array.call";
	const wchar_t NAME_BUILTIN_ARRAY_OBJECT_CTOR[] = L"Array.constructor";
	const wchar_t NAME_BUILTIN_ARRAY_OBJECT_TOSTRING[] = L"Array.toString";
	const wchar_t NAME_BUILTIN_ARRAY_OBJECT_TOLOCALESTRING[] = L"Array.toLocaleString";
	const wchar_t NAME_BUILTIN_OBJECT_OBJECT_JOIN[] = L"Object.join";
	const wchar_t NAME_BUILTIN_ARRAY_OBJECT_JOIN[] = L"Array.join";
	const wchar_t NAME_BUILTIN_ARRAY_OBJECT_SORT[] = L"Array.sort";
	const wchar_t NAME_BUILTIN_ARRAY_OBJECT_REVERSE[] = L"Array.reverse";
	const wchar_t NAME_BUILTIN_ARRAY_OBJECT_VALUEOF[] = L"Array.valueOf";

	const wchar_t NAME_BUILTIN_STRING_OBJECT[] = L"String";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_CALL[] = L"String.call";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_CTOR[] = L"String.constructor";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_FROMCHARCODE[] = L"String.fromCharCode";	//global id
	const wchar_t NAME_BUILTIN_STRING_OBJECT_PROPERTY_FROMCHARCODE[] = L"fromCharCode";	//global id
	const wchar_t NAME_BUILTIN_STRING_OBJECT_CHARAT[] = L"Object.charAt";	//global id
	const wchar_t NAME_BUILTIN_STRING_OBJECT_CHARCODEAT[] = L"Object.charCodeAt";	//global id
	const wchar_t NAME_BUILTIN_STRING_OBJECT_CONCAT[] = L"Object.concat";	//global id
	const wchar_t NAME_BUILTIN_STRING_OBJECT_INDEXOF[] = L"Object.indexOf";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_LASTINDEXOF[] = L"Object.lastIndexOf";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_TOLOWERCASE[] = L"Object.toLowerCase";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_TOUPPERCASE[] = L"Object.toUpperCase";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_TOLOCALELOWERCASE[] = L"Object.toLocaleLowerCase";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_TOLOCALEUPPERCASE[] = L"Object.toLocaleUpperCase";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_SUBSTRING[] = L"Object.substr";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_SUBSTRING2[] = L"Object.substring";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_TOSTRING[] = L"String.toString";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_TOLOCALESTRING[] = L"String.toLocaleString";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_MATCH[] = L"Object.match";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_SPLIT[] = L"Object.split";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_REPLACE[] = L"Object.replace";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_SEARCH[] = L"Object.search";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_SLICE[] = L"Object.slice";
	const wchar_t NAME_BUILTIN_STRING_OBJECT_LOCALECOMPARE[] = L"Object.localeCompare";

	const wchar_t NAME_BUILTIN_REGEX_OBJECT[] = L"RegExp";
	const wchar_t NAME_BUILTIN_REGEX_OBJECT_CALL[] = L"RegExp.call";
	const wchar_t NAME_BUILTIN_REGEX_OBJECT_CTOR[] = L"RegExp.constructor";
	const wchar_t NAME_BUILTIN_REGEX_OBJECT_EXEC[] = L"RegExp.exec";
	const wchar_t NAME_BUILTIN_REGEX_OBJECT_TEST[] = L"RegExp.test";
	const wchar_t NAME_BUILTIN_REGEX_OBJECT_TOSTRING[] = L"RegExp.toString";
	const wchar_t NAME_BUILTIN_REGEX_OBJECT_TOLOCALESTRING[] = L"RegExp.toLocaleString";
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_SOURCE[] = L"source";	//Property name of the object
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_GLOBAL[] = L"global";	//Property name of the object
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_IGNORECASE[] = L"ignoreCase";	//Property name of the object
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_MULTILINE[] = L"multiline";	//Property name of the object
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_LASTINDEX[] = L"lastIndex";	//Property name of the object
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_INDEX[] = L"index";	//Property name of the object
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_INPUT[] = L"input";	//Property name of the object
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_LENGTH[] = L"length";	//Property name of the object
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_RESULT0[] = L"$0";
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_RESULT1[] = L"$1";
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_RESULT2[] = L"$2";
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_RESULT3[] = L"$3";
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_RESULT4[] = L"$4";
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_RESULT5[] = L"$5";
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_RESULT6[] = L"$6";
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_RESULT7[] = L"$7";
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_RESULT8[] = L"$8";
	const wchar_t NAME_BUILTIN_REGEX_PROPERTY_RESULT9[] = L"$9";

	const wchar_t NAME_BUILTIN_FUNCTION_PROPERTY_ARGUMENTS[] = L"arguments";
	const wchar_t NAME_BUILTIN_FUNCTION_PROPERTY_CALLEE[] = L"callee";
	const wchar_t NAME_BUILTIN_FUNCTION_PROPERTY_CALLER[] = L"caller";
	const wchar_t NAME_BUILTIN_FUNCTION_OBJECT[] = L"Function";
	const wchar_t NAME_BUILTIN_FUNCTION_OBJECT_CALL[] = L"Function.call";
	const wchar_t NAME_BUILTIN_FUNCTION_OBJECT_APPLY[] = L"Function.apply";
	const wchar_t NAME_BUILTIN_FUNCTION_OBJECT_CTOR[] = L"Function.constructor";
	const wchar_t NAME_BUILTIN_FUNCTION_OBJECT_CTORWITHRID[] = L"Function.constructorWithRid";

	const wchar_t NAME_BUILTIN_BOOLEAN_OBJECT[] = L"Boolean";
	const wchar_t NAME_BUILTIN_BOOLEAN_OBJECT_CALL[] = L"Boolean.call";
	const wchar_t NAME_BUILTIN_BOOLEAN_OBJECT_CTOR[] = L"Boolean.constructor";
	const wchar_t NAME_BUILTIN_BOOLEAN_OBJECT_TOSTRING[] = L"Boolean.toString";
	const wchar_t NAME_BUILTIN_BOOLEAN_OBJECT_VALUEOF[] = L"Boolean.valueOf";
	const wchar_t NAME_BUILTIN_BOOLEAN_OBJECT_TOLOCALESTRING[] = L"Boolean.toLocaleString";

	const wchar_t NAME_BUILTIN_ERROR_OBJECT[] = L"Error";
	const wchar_t NAME_BUILTIN_ERROR_OBJECT_CALL[] = L"Error.call";
	const wchar_t NAME_BUILTIN_ERROR_OBJECT_CTOR[] = L"Error.constructor";
	const wchar_t NAME_BUILTIN_ERROR_PROPERTY_MESSAGE[] = L"description";
	const wchar_t NAME_BUILTIN_ERROR_PROPERTY_NAME[] = L"name";
	const wchar_t NAME_BUILTIN_ERROR_PROPERTY_NUMBER[] = L"number";
	const int32_t ERROR_PROPERTY_NUMBER_DEFAULT = 0x0;

	const wchar_t NAME_BUILTIN_NUMBER_OBJECT[] = L"Number";
	const wchar_t NAME_BUILTIN_NUMBER_OBJECT_CALL[] = L"Number.call";
	const wchar_t NAME_BUILTIN_NUMBER_OBJECT_CTOR[] = L"Number.constructor";
	const wchar_t NAME_BUILTIN_NUMBER_OBJECT_TOSTRING[] = L"Number.toString";
	const wchar_t NAME_BUILTIN_NUMBER_OBJECT_VALUEOF[] = L"Number.valueOf";
	const wchar_t NAME_BUILTIN_NUMBER_OBJECT_TOLOCALESTRING[] = L"Number.toLocaleString";

	const wchar_t NAME_BUILTIN_NUMBER_MAX_VALUE[]	= L"Number.MAX_VALUE";
	const wchar_t NAME_BUILTIN_NUMBER_MIN_VALUE[]	= L"Number.MIN_VALUE";
	const wchar_t NAME_BUILTIN_NUMBER_NAN[]			= L"Number.NaN";
	const wchar_t NAME_BUILTIN_NUMBER_NEGATIVE_INFINITY[] = L"Number.NEGATIVE_INFINITY";
	const wchar_t NAME_BUILTIN_NUMBER_POSITIVE_INFINITY[] = L"Number.POSITIVE_INFINITY";

	const wchar_t NAME_BUILTIN_MATH_OBJECT[] = L"Math";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_CTOR[] = L"Math.constructor";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_CEIL[] = L"Math.ceil";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_RANDOM[] = L"Math.random";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_FLOOR[] = L"Math.floor";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_ROUND[] = L"Math.round";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_COS[] = L"Math.cos";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_SIN[] = L"Math.sin";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_TAN[] = L"Math.tan";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_ACOS[] = L"Math.acos";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_ASIN[] = L"Math.asin";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_ATAN[] = L"Math.atan";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_ATAN2[] = L"Math.atan2";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_ABS[] = L"Math.abs";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_POW[] = L"Math.pow";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_LOG[] = L"Math.log";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_MIN[] = L"Math.min";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_MAX[] = L"Math.max";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_EXP[] = L"Math.exp";
	const wchar_t NAME_BUILTIN_MATH_OBJECT_SQRT[] = L"Math.sqrt";

	const wchar_t NAME_BUILTIN_MATH_PROPERTY_E[] = L"Math.E";
	const wchar_t NAME_BUILTIN_MATH_PROPERTY_LN10[] = L"Math.LN10";
	const wchar_t NAME_BUILTIN_MATH_PROPERTY_LN2[] = L"Math.LN2";
	const wchar_t NAME_BUILTIN_MATH_PROPERTY_LOG2E[] = L"Math.LOG2E";
	const wchar_t NAME_BUILTIN_MATH_PROPERTY_LOG10E[] = L"Math.LOG10E";
	const wchar_t NAME_BUILTIN_MATH_PROPERTY_PI[] = L"Math.PI";
	const wchar_t NAME_BUILTIN_MATH_PROPERTY_SQRT1_2[] = L"Math.SQRT1_2";
	const wchar_t NAME_BUILTIN_MATH_PROPERTY_SQRT2[] = L"Math.SQRT2";


	const wchar_t NAME_BUILTIN_DATE_OBJECT[] = L"Date";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_CALL[] = L"Date.call";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_CTOR[] = L"Date.constructor";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_SETTIME[] = L"Date.setTime";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_TOSTRING[] = L"Date.toString";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_TOLOCALESTRING[] = L"Date.toLocaleString";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_TOGMTSTRING[] = L"Date.toGMTString";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_UTC[] = L"Date.UTC";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_PARSE[] = L"Date.parse";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_GETTIMEZONEOFFSET[] = L"Date.getTimezoneOffset";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_GETTIME[] = L"Date.getTime";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_GETYEAR[] = L"Date.getYear";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_GETMONTH[] = L"Date.getMonth";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_GETDATE[] = L"Date.getDate";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_GETDAY[] = L"Date.getDay";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_GETHOURS[] = L"Date.getHours";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_GETMINUTES[] = L"Date.getMinutes";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_GETSECONDS[] = L"Date.getSeconds";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_GETMILLISECONDS[] = L"Date.getMilliseconds";

	const wchar_t NAME_BUILTIN_DATE_OBJECT_SETYEAR[] = L"Date.setYear";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_SETMONTH[] = L"Date.setMonth";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_SETDATE[] = L"Date.setDate";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_SETDAY[] = L"Date.setDay";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_SETHOURS[] = L"Date.setHours";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_SETMINUTES[] = L"Date.setMinutes";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_SETSECONDS[] = L"Date.setSeconds";
	const wchar_t NAME_BUILTIN_DATE_OBJECT_SETMILLISECONDS[] = L"Date.setMilliseconds";

	const wchar_t NAME_BUILTIN_DATE_PROPERTY_DATA1[] = L"Date._data1";

	const wchar_t NAME_BUILTIN_SYSTEM_OBJECT[] = L"System";
	const wchar_t NAME_BUILTIN_SYSTEM_OBJECT_CTOR[] = L"System.Constructor";
	const wchar_t NAME_BUILTIN_SYSTEM_LOADPROPERTYBYSTRINGRID[] = L"System.LoadPropertyByStringRid";
	const wchar_t NAME_BUILTIN_SYSTEM_STOREPROPERTYBYSTRINGRID[] = L"System.StorePropertyByStringRid";
	const wchar_t NAME_BUILTIN_SYSTEM_LOADPROPERTYBYEXPRESSION[] = L"System.LoadPropertyByExpression";
	const wchar_t NAME_BUILTIN_SYSTEM_STOREPROPERTYBYEXPRESSION[] = L"System.StorePropertyByExpression";
	const wchar_t NAME_BUILTIN_SYSTEM_LOADobjectLiteral[] = L"System.LoadobjectLiteral";
	const wchar_t NAME_BUILTIN_SYSTEM_LOADarrayLiteral[] = L"System.LoadarrayLiteral";
	const wchar_t NAME_BUILTIN_SYSTEM_STRICTEQUAL[] = L"System.StrictEqual";
	const wchar_t NAME_BUILTIN_SYSTEM_STRICTNOTEQUAL[] = L"System.StrictNotEqual";
	const wchar_t NAME_BUILTIN_SYSTEM_LOADPROTOTYPE[] = L"System.LoadPrototype";
	const wchar_t NAME_BUILTIN_SYSTEM_STOREPROTOTYPE[] = L"System.StorePrototype";
	const wchar_t NAME_BUILTIN_SYSTEM_INSTANCEOF[] = L"System.instanceof";
	const wchar_t NAME_BUILTIN_SYSTEM_IN[]		= L"System.in";
	const wchar_t NAME_BUILTIN_SYSTEM_TYPEOF[]	= L"System.typeof";
	const wchar_t NAME_BUILTIN_SYSTEM_FORIN[]	= L"System.forIn";
	const wchar_t NAME_BUILTIN_SYSTEM_PRINT[]	= L"print";
	const wchar_t NAME_BUILTIN_SYSTEM_EVAL[]	= L"eval";
	const wchar_t NAME_BUILTIN_SYSTEM_ESCAPE[]	= L"escape";
	const wchar_t NAME_BUILTIN_SYSTEM_UNESCAPE[] = L"unescape";
	const wchar_t NAME_BUILTIN_SYSTEM_INT64[]		= L"int64";
	const wchar_t NAME_BUILTIN_SYSTEM_INT[]		= L"int";
	const wchar_t NAME_BUILTIN_SYSTEM_UINT[]		= L"uint";
	const wchar_t NAME_BUILTIN_SYSTEM_DOUBLE[]	= L"double";
	const wchar_t NAME_BUILTIN_SYSTEM_FLOAT[]	= L"float";
	const wchar_t NAME_BUILTIN_SYSTEM_LONG[]	= L"long";
	const wchar_t NAME_BUILTIN_SYSTEM_ULONG[]	= L"ulong";
	const wchar_t NAME_BUILTIN_SYSTEM_SHORT[]	= L"short";
	const wchar_t NAME_BUILTIN_SYSTEM_USHORT[]	= L"ushort";
	const wchar_t NAME_BUILTIN_SYSTEM_CHAR[]	= L"char";
	const wchar_t NAME_BUILTIN_SYSTEM_BYTE[]	= L"byte";
	const wchar_t NAME_BUILTIN_SYSTEM_SBYTE[]	= L"sbyte";
	const wchar_t NAME_BUILTIN_SYSTEM_ISNAN[]	= L"isNaN";
	const wchar_t NAME_BUILTIN_SYSTEM_ISFINITE[]	= L"isFinite";
	const wchar_t NAME_BUILTIN_SYSTEM_PARSEINT[]	= L"parseInt";
	const wchar_t NAME_BUILTIN_SYSTEM_PARSEFLOAT[]	= L"parseFloat";

	const wchar_t NAME_BUILTIN_SYSTEM_PROPERTY_ENVIRONMENT[] = L"Environment";
	const wchar_t NAME_BUILTIN_SYSTEM_PROPERTY_EXITCODE[] = L"ExitCode";

	const wchar_t STRING_UNESCAPEDSTRING[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@*_+-./";

	//---------------------------------------------------------------------------
	// Class definitions
	//---------------------------------------------------------------------------

	class CMetaData;
	class CVmObject;

	/*EN An interface class for a built-in object
	A bult-in object should be derived from the class
	\ingroup MDL_LIB_BUILTINOBJECT
	*/
	class CBuiltinObject {
	public:
		/*EN Invoked when the code generator is being initialized
		\param CodeGen A reference to the code gen instance being initialized
		\return true if succeeded initializing the object otherwise false
		*/
		virtual bool InitializeCodeGen( ICodeGen& CodeGen ) = 0;

		/*EN Invoked when the VM is being initialized
		\param vm A reference to the VM instance being initialized
		\return true if succeeded initializing the object otherwise false
		*/
		virtual bool InitializeVm( CCilVm& vm ) = 0;
		
		/*EN Ctor*/
		CBuiltinObject() {};
		
		/*EN Dtor*/
		virtual ~CBuiltinObject() {};
	};

	/*EN Built-in Object object implementations
	\ingroup MDL_LIB_BUILTINOBJECT
	*/
	class CObjectObject : public CBuiltinObject {
		static void ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void toString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void valueOf( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void hasOwnProperty( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void isPrototypeOf( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void propertyIsEnumerable( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static METHOD_INITIALIZATION_PARAM initParams[];
		static PROPERTY_INITIALIZATION_PARAM initPropertyParams[];

		/*EN An accessor for the argument varialbe*/
		static CVariable* argumentAccessor( CCilVm* const pVm,
											CVmObject* const pObject,
											const wstring* const pName,
											CVariable* const var,
											const bool bSet );

		/*EN An accessor for the caller varialbe*/
		static CVariable* callerAccessor( CCilVm* const pVm,
											CVmObject* const pObject,
											const wstring* const pName,
											CVariable* const var,
											const bool bSet );
	public:
		bool InitializeCodeGen( ICodeGen& CodeGen );
		bool InitializeVm( CCilVm& vm );
	};

	//---------------------------------------------------------------------------
	// Array
	//---------------------------------------------------------------------------
	const int32_t MAX_ARRAYLENGTH = 65536;	/*EN< Maximum size of an array object length
											\ingroup MDL_LIB_BUILTINOBJECT	
											*/

	/*EN Built-in Array object implementations
	\ingroup MDL_LIB_BUILTINOBJECT
	*/
	class CArrayObject : public CBuiltinObject {
		static METHOD_INITIALIZATION_PARAM initParams[];

		/*EN An accessor for the lengtgh varialbe*/
		static CVariable* lengthAccessor( CCilVm* const pVm,
										CVmObject* const pObject,
										const wstring* const pName, 
										CVariable* const var,
										const bool bSet );
		/*EN An accessor for any variable that is not in the array*/
		static CVariable* anyAccessor( CCilVm* const pVm,
										CVmObject* const pObject,
										const wstring* const pName, 
										CVariable* const var,
										const bool bSet );
	public:
		bool InitializeCodeGen( ICodeGen& CodeGen );
		bool InitializeVm( CCilVm& vm );
		static void ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void toString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void join( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void sort( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void reverse( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void valueOf( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static bool CheckArrayIndex( const wstring& str );	/*EN< Check if the string is array index*/ 
		static bool Predicater( const CVariable& elem1,
					const CVariable& elem2 );						/*EN< Array.sort predicater*/
		static bool CilPredicater( const CVariable& elem1,
					const CVariable& elem2 );						/*EN< Array.sort predicater with script function given
															Note that it's using static member for a function pointer*/
		static CCilVm* g_pVm;								/*EN< Static member for Array.sort*/
		static RID g_ridPredicater;							/*EN< Static member for Array.sort*/
	};

	struct REPLACE_PARAMS;

	//---------------------------------------------------------------------------
	// String
	//---------------------------------------------------------------------------
	/*EN Built-in String object implementations
	\ingroup MDL_LIB_BUILTINOBJECT
	*/
	class CStringObject : public CBuiltinObject {
		static void call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void fromCharCode( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void charAt( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void charCodeAt( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void concat( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void indexOf( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void lastIndexOf( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void toLowerCase( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void toUpperCase( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void substring( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void toString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void match( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void split( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void replace( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void search( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void slice( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void localeCompare( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static METHOD_INITIALIZATION_PARAM initParams[];

		static void ReplacementHelper( wstring& str, REPLACE_PARAMS* pParam );

		/*EN An accessor for the length varialbe*/
		static CVariable* lengthAccessor(CCilVm* const pVm,
										CVmObject* const pObject,
										const wstring* const pName,
										CVariable* const var,
										const bool bSet );
		static CVariable m_varLength;
	public:
		static void ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		bool InitializeCodeGen( ICodeGen& CodeGen );
		bool InitializeVm( CCilVm& vm );
	};

	//---------------------------------------------------------------------------
	// Math
	//---------------------------------------------------------------------------
	/*EN Built-in the Math object implementations
	\ingroup MDL_LIB_BUILTINOBJECT
	*/
	class CMathObject : public CBuiltinObject {
		static void ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void ceil( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void floor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void random( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void round( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void cos( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void sin( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void tan( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void acos( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void asin( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void atan( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void atan2( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void abs( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void pow( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void log( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void minImpl( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void maxImpl( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void exp( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void sqrt( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static METHOD_INITIALIZATION_PARAM initParams[];
		static PROPERTY_INITIALIZATION_PARAM initPropertyParams[];

	public:
		bool InitializeCodeGen( ICodeGen& CodeGen );
		bool InitializeVm( CCilVm& vm );
	};

	//---------------------------------------------------------------------------
	// Number
	//---------------------------------------------------------------------------
	/*EN Built-in Number object implementations
	\ingroup MDL_LIB_BUILTINOBJECT
	*/
	class CNumberparser : public CStringLexer {
	public:
		//Override it not to initialize keyword map
		CNumberparser() {};
		virtual ~CNumberparser() {};
		virtual bool init();
	protected:
		//Override tokenFound
		//void tokenFound( const int32_t iToken, const int32_t iLine, const int32_t iColumm);
	};

	class CNumberObject : public CBuiltinObject {
		static void call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void toString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void toLocaleString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void valueOf( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static METHOD_INITIALIZATION_PARAM initParams[];
		static PROPERTY_INITIALIZATION_PARAM initPropertyParams[];

		static CNumberparser* m_pNumberparser;

	public:
		static void ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		bool InitializeCodeGen( ICodeGen& CodeGen );
		bool InitializeVm( CCilVm& vm );
		static bool parseNumber( CVariable& var, const wstring* const pstr );
		
		CNumberObject()
		{
			if( m_pNumberparser == NULL )
			{
				m_pNumberparser = new CNumberparser();
			}

		}
		virtual ~CNumberObject()
		{
		}
	};

	//---------------------------------------------------------------------------
	// Boolean
	//---------------------------------------------------------------------------
	/*EN Built-in Boolean object implementations
	\ingroup MDL_LIB_BUILTINOBJECT
	*/
	class CBooleanObject : public CBuiltinObject {
		static void call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static METHOD_INITIALIZATION_PARAM initParams[];

	public:
		static void ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void toString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void valueOf( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		bool InitializeCodeGen( ICodeGen& CodeGen );
		bool InitializeVm( CCilVm& vm );
	};

	//---------------------------------------------------------------------------
	// Error
	//---------------------------------------------------------------------------
	/*EN Built-in Error object implementations
	\ingroup MDL_LIB_BUILTINOBJECT
	*/
	class CErrorObject : public CBuiltinObject {
		static void call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static METHOD_INITIALIZATION_PARAM initParams[];

	public:
		static void ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		bool InitializeCodeGen( ICodeGen& CodeGen );
		bool InitializeVm( CCilVm& vm );
	};

	//---------------------------------------------------------------------------
	// Date
	//---------------------------------------------------------------------------
	struct TIME_ZONE {
		wchar_t* strTimeZone;
		int8_t cHour;
		int8_t cMinute;
	};

	const int64_t HoursPerDay = 24;
	const int64_t MinutesPerHour = 60;
	const int64_t SecondsPerMinute = 60;
	const int64_t msPerSecond = 1000;
	const int64_t msPerMinute = 60000;
	const int64_t msPerHour = 3600000;
	const int64_t msPerDay = 86400000;
	const wchar_t* const strWeekday[] = { L"Sun", L"Mon", L"Tue", L"Wed", L"Thu", L"Fri", L"Sat"};
	const wchar_t* const strMonth[] = { L"Jan", L"Feb", L"Mar", L"Apr", L"May", L"Jun", L"Jul", L"Aug", L"Sep", L"Oct", L"Nov", L"Dec" };

	/*EN Built-in Date object implementations
	\ingroup MDL_LIB_BUILTINOBJECT
	*/
	class CDateObject : public CBuiltinObject {
		static const TIME_ZONE TimeZoneSettings[];
		static void ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void setTime( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void getTimezoneOffset( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void toString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void toGMTString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void toLocaleString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void UTC( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void parse( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void getTime( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void getDate( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void getDay( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void getHours( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void getMinutes( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void getMonth( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void getSeconds( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void getYear( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void setDate( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void setHours( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void setMinutes( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void setMonth( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void setSeconds( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void setYear( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static METHOD_INITIALIZATION_PARAM initParams[];

		static int64_t TimeWithinDay( int64_t );
		static int64_t TimeFromYear( int32_t t );
		static int32_t DayFromYear( int32_t );
		static int64_t TimeFromYearMonth( int32_t, int32_t );
		static int64_t Day( int64_t t );
		static int32_t DaysInYear( int32_t );
		static int32_t YearFromTime( int64_t t );
		static int32_t InLeapYear( int64_t );
		static int32_t MonthFromTime( int64_t );
		static int32_t DayWithinYear( int64_t );
		static int32_t DateFromTime( int64_t );
		static int32_t WeekDay( int64_t );
		static int32_t HourFromTime( int64_t );
		static int32_t MinFromTime( int64_t );
		static int32_t SecFromTime( int64_t );
		static int32_t msFromTime( int64_t );
		static int32_t DaylightSavingTA( int64_t );

		int32_t LocalTime( int64_t );
		int32_t ToUTC( int64_t );
		
		//Vars
		int32_t m_LocalTZA;
		int64_t m_iTime;
		int16_t m_currentTimeZoneIndex;
		void setTimeZone( wchar_t* pstrTimeZone );
	public:
		bool InitializeCodeGen( ICodeGen& CodeGen );
		bool InitializeVm( CCilVm& vm );
	};

	//---------------------------------------------------------------------------
	// Function
	//---------------------------------------------------------------------------
	/*EN Built-in Function object implementations
	\ingroup MDL_LIB_BUILTINOBJECT
	*/
	class CFunctionObject : public CBuiltinObject {
		static void ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void CreateFunctionObject( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void apply( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static METHOD_INITIALIZATION_PARAM initParams[];

		static RID m_ridObjectName;

	public:
		bool InitializeCodeGen( ICodeGen& CodeGen );
		bool InitializeVm( CCilVm& vm );
	};

	//---------------------------------------------------------------------------
	// Regex
	//---------------------------------------------------------------------------
	/*EN Built-in Regex object implementations
	\ingroup MDL_LIB_BUILTINOBJECT
	*/
	class CRegexObject : public CBuiltinObject {
		static void ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void exec( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void test( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void toString( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );

		static METHOD_INITIALIZATION_PARAM initParams[];

	public:
		bool InitializeCodeGen( ICodeGen& CodeGen );
		bool InitializeVm( CCilVm& vm );

		//Called from String, too
		static void ExecMain( CCilVm* const pVm,
							CVariable& varRegex,
							CVariable& varString,
							uint32_t flags = 0 );
	};

	//---------------------------------------------------------------------------
	// System
	//---------------------------------------------------------------------------
	/*EN Built-in System object implementations
	\ingroup MDL_LIB_BUILTINOBJECT
	*/
	class CSystemObject : public CBuiltinObject {
		//Default method type
		static void ctor( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void call( CCilVm* const pVm,
									const int32_t iNumArguments, 
									CVariable* pArguments );
		static void LoadPropertyByStringRid( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void StorePropertyByStringRid( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void LoadPropertyByExpression( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void StorePropertyByExpression( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void LoadobjectLiteral( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void StrictEqual( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void StrictNotEqual( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static OPERAND_TYPE getType( CVariable& var );
		static void LoadPrototype( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void StorePrototype( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void instanceOf( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void In( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void Typeof( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void ForIn( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void print( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void eval( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void escape( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void unescape( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void int64Cast( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void intCast( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void doubleCast( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void floatCast( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		//static void longCast( CCilVm* const pVm,
		//									const int32_t iNumArguments, 
		//									CVariable* pArguments );
		//static void shortCast( CCilVm* const pVm,
		//									const int32_t iNumArguments, 
		//									CVariable* pArguments );
		//static void charCast( CCilVm* const pVm,
		//									const int32_t iNumArguments, 
		//									CVariable* pArguments );
		//static void byteCast( CCilVm* const pVm,
		//									const int32_t iNumArguments, 
		//									CVariable* pArguments );
		static void isNaN( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void isFinite( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void parseInt( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static void parseFloat( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		static METHOD_INITIALIZATION_PARAM initParams[];

	public:
		static void LoadarrayLiteral( CCilVm* const pVm,
											const int32_t iNumArguments, 
											CVariable* pArguments );
		bool InitializeCodeGen( ICodeGen& CodeGen );
		bool InitializeVm( CCilVm& vm );
	};

}//namespace cri
