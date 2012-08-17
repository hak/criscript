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
 * Module   : CRICript header
 * File     : CRIScript.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once

//-------------------------------------------------------------
//Definitions, Pragmas
//-------------------------------------------------------------

#ifndef _HAS_EXCEPTIONS				//Tell STL not to use SEH
	#define _HAS_EXCEPTIONS 0
#endif

#pragma warning(disable:4065 4275) // disabling warnings that bison generates
//warning C4275: non dll-interface class 'stdext::exception' used as base for dll-interface class 'std::bad_cast'

//-------------------------------------------------------------
//enums
//-------------------------------------------------------------
//
// Token tags.
//
typedef enum COR_TOKEN_TYPE
	{
		MDT_MODULE               = 0x00000000,       //
		MDT_TYPEREF              = 0x01000000,       //
		MDT_TYPEDEF              = 0x02000000,       //
		MDT_FIELDDEF             = 0x04000000,       //
		MDT_METHODDEF            = 0x06000000,       //
		MDT_PARAMDEF             = 0x08000000,       //
		MDT_INTERFACEIMPL        = 0x09000000,       //
		MDT_MEMBERREF            = 0x0a000000,       //
		MDT_CUSTOMATTRIBUTE      = 0x0c000000,       //
		MDT_PERMISSION           = 0x0e000000,       //
		MDT_SIGNATURE            = 0x11000000,       //
		MDT_EVENT                = 0x14000000,       //
		MDT_PROPERTY             = 0x17000000,       //
		MDT_MODULEREF            = 0x1a000000,       //
		MDT_TYPESPEC             = 0x1b000000,       //
		MDT_ASSEMBLY             = 0x20000000,       //
		MDT_ASSEMBLYREF          = 0x23000000,       //
		MDT_FILE                 = 0x26000000,       //
		MDT_EXPORTEDTYPE         = 0x27000000,       //
		MDT_MANIFESTRESOURCE     = 0x28000000,       //
		MDT_GENERICPARAM         = 0x2a000000,       //
		MDT_METHAODSPEC           = 0x2b000000,       //
		MDT_GENERICPARAMCONSTRAINT = 0x2c000000,
		
		MDT_STRING               = 0x70000000,       //
		MDT_NAME                 = 0x71000000,       //
		MDT_BASETYPE             = 0x72000000,       // Leave this on the high end value. This does not correspond to metadata table
	} COR_TOKEN_TYPE;

//RID definitions
enum RID_BUILTIN_CONSTANT {
	RID_CONSTANT_UNDEFINED		= MDT_FIELDDEF | 0,
	RID_CONSTANT_NULL			= MDT_FIELDDEF | 1,
	RID_CONSTANT_THIS			= MDT_FIELDDEF | 2,
	RID_CONSTANT_BOOL_TRUE		= MDT_FIELDDEF | 3,
	RID_CONSTANT_BOOL_FALSE		= MDT_FIELDDEF | 4,
	RID_SWITCHTMP				= MDT_FIELDDEF | 5,
	RID_CONSTANT_DELETION		= MDT_FIELDDEF | 6,
	RID_CONSTANT_NAN			= MDT_FIELDDEF | 7,
	RID_CONSTANT_INFINITY		= MDT_FIELDDEF | 8,
	RID_ARGUMENTS_TMP			= MDT_FIELDDEF | 9,
};

/*EN An enum values that defines a number of parameters for a built-in object
 \ingroup MDL_LIB_BUILTINOBJECT
 */
enum RID_BUILTIN_PARAM {
	RID_BUILTINPARAM_NULL			= 0,						/*EN< No parameter */
	RID_BUILTINPARAM_DEFAULT_ZERO	= MDT_PARAMDEF| 0xffffff,	/*EN< 0 parameter */
	RID_BUILTINPARAM_DEFAULT_ONE	= MDT_PARAMDEF| 0xfffffe,	/*EN< 1 parameter */
	RID_BUILTINPARAM_DEFAULT_TWO	= MDT_PARAMDEF| 0xfffffd,	/*EN< 2 parameters */
	RID_BUILTINPARAM_DEFAULT_THREE	= MDT_PARAMDEF| 0xfffffc,	/*EN< 3 parameters */
	RID_BUILTINPARAM_DEFAULT_FOUR	= MDT_PARAMDEF| 0xfffffb,	/*EN< 4 parameters */
	RID_BUILTINPARAM_DEFAULT_FIVE	= MDT_PARAMDEF| 0xfffffa,	/*EN< 5 parameters */
	RID_BUILTINPARAM_DEFAULT_SIX	= MDT_PARAMDEF| 0xfffff9,	/*EN< 6 parameters */
	RID_BUILTINPARAM_DEFAULT_SEVEN	= MDT_PARAMDEF| 0xfffff8,	/*EN< 7 parameters */
	RID_BUILTINPARAM_DEFAULT_EIGHT	= MDT_PARAMDEF| 0xfffff7,	/*EN< 8 parameters */
	RID_BUILTINPARAM_DEFAULT_NINE	= MDT_PARAMDEF| 0xfffff6,	/*EN< 9 parameters */
};

//-------------------------------------------------------------
//Includes
//-------------------------------------------------------------
#include "stdint.h"
#include "versions.h"
#include "lexer.h"
#include "parser.h"
#include "error.h"
#include "cilErrorDef_English.h"
#include "cilCodeGen.h"
#include "cilVm.h"
#include "cilDebugger.h"
#include "variable.h"

//-------------------------------------------------------------
//Enable this definition for JScript extension
//-------------------------------------------------------------
/*EN
 \brief Compiler defines to enable JScript compatible extension
*/
#define ENABLE_EXTENSION_JSCRIPT ( 1 )
/*EN
 \brief Compiler defines to enable ES4 compatible features
 */
#define ENABLE_EXTENSION_ES4 ( 1 )
/*EN
 \brief Compiler defines to enable Dynamic language feature
 */
#define ENABLE_DYNAMIC_LANGUAGE ( 1 )

/***************************************************************************
 *      VM definitions
 ***************************************************************************/
/*EN
 \brief Indicates using SWITCH for VM dispatch
*/
#define VM_DISPATCH_SWITCH (1)
/*EN
 \brief Indicates using direct threaded model for VM dispatch
*/
//#define VM_DISPATCH_DIRECT_THREADED (1)

/*EN
 \brief Enable VM to track an usage of stack entries
 */
//#define VM_TRACK_MAXSTACKUSAGE ( 1 )

/*EN
 \brief Set VM to erase variable contents when popping from the stack
 */
#define VM_TRACK_ERASEWHENPOP ( 1 )

#ifdef _XBOX
	//BigEndian
	#define CRISCRIPT_BIG_ENDIAN
#endif

/***************************************************************************
 * Enable this definition to debug the parser
 ***************************************************************************/

//#define DEBUG_PARSING


#ifdef _XBOX
/***************************************************************************
 * Definitions for Xbox360
 ***************************************************************************/
#ifdef  NDEBUG

#define Debug_Fatal(_Message)     ((void)0)
#define Debug_Warning(_Message)     ((void)0)

#else

#ifdef  __cplusplus
extern "C" {
#endif
	
	_CRTIMP void __cdecl _wassert(__in_z const wchar_t * _Message,
								  __in_z const wchar_t *_File,
								  __in unsigned _Line);
	
#ifdef  __cplusplus
}
#endif

#define Debug_Fatal(_Message) (void)(_wassert(_CRT_WIDE(#_Message),\
_CRT_WIDE(__FILE__), __LINE__), 0)
#define Debug_Warning(_Message) (void)(OutputDebugStringW(_CRT_WIDE(_Message)))
#endif

#else

#ifdef __APPLE__
/***************************************************************************
 * Definitions for OSX
 ***************************************************************************/
#ifdef  NDEBUG

#define Debug_Fatal(_Message)     ((void)0)
#define Debug_Warning(_Message)     ((void)0)

#else

#ifdef  __cplusplus
extern "C" {
#endif
	
//	_CRTIMP void __cdecl _wassert(__in_z const wchar_t * _Message,
//								  __in_z const wchar_t *_File,
//								  __in unsigned _Line);
	
#ifdef  __cplusplus
}
#endif

#define Debug_Fatal(_Message) (void)(assert(_Message))
#define Debug_Warning(_Message) (void)(printf(_Message))
#endif

#else
/***************************************************************************
 * Definitions for PC
 ***************************************************************************/
#ifdef  NDEBUG
#ifndef Debug_Fatal
	#define Debug_Fatal(_Message)     ((void)0)
#endif

#ifndef Debug_Warning
	#define Debug_Warning(_Message)     ((void)0)
#endif

#else

#ifdef  __cplusplus
extern "C" {
#endif
	
	_CRTIMP void __cdecl _wassert(__in_z const wchar_t * _Message,
								  __in_z const wchar_t *_File,
								  __in unsigned _Line);
	
#ifdef  __cplusplus
}
#endif
#endif

#ifndef Debug_Fatal
#define Debug_Fatal(_Message) (void)(_wassert(_CRT_WIDE(#_Message),\
_CRT_WIDE(__FILE__), __LINE__), 0)
#endif

#ifndef Debug_Warning
#define Debug_Warning(_Message) (void)(OutputDebugString(_CRT_WIDE(_Message)))
#endif

#endif

#endif

using namespace std;
using namespace cri;

namespace cri {
//YACC variables
extern int32_t yydebug;

extern int32_t yyparse(void); 

}	//namespace cri



