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
 * File     : versions.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once

//-------------------------------------------------------------
//Version strings
//-------------------------------------------------------------
namespace cri {

#define CRISCRIPT_VER_MAJOR		0	/* #MAJOR_NUMBER# */
#define CRISCRIPT_VER_MINOR		91	/* #MINOR_NUMBER# */
#define CRISCRIPT_VER_REVISION	0	/* #REVISION_NUMBER# */
#define CRISCRIPT_VER_BUILD		137	/* #BUILD_NUMBER# */

/* Macros for version definition */
#define	CRISCRIPT_GLUE_VERSION_STRING(a,b,c,d)	#a"."#b"."#c"."#d
#define	CRISCRIPT_MAKE_VERSION_STRING(a,b,c,d)	CRISCRIPT_GLUE_VERSION_STRING(a,b,c,d)

#define	CRISCRIPT_MAKE_VERSION_UINT64(a,b,c,d)	(((a)<<48)|((b)<<32)|((c)<<16)|(d))
#define CRISCRIPT_VERSION_STRING	CRISCRIPT_MAKE_VERSION_STRING(CRISCRIPT_VER_MAJOR,\
															CRISCRIPT_VER_MINOR,\
															CRISCRIPT_VER_REVISION,\
															CRISCRIPT_VER_BUILD)
#define CRISCRIPT_VERSION_UINT64	CRISCRIPT_MAKE_VERSION_UINT64(CRISCRIPT_VER_MAJOR,\
																CRISCRIPT_VER_MINOR,\
																CRISCRIPT_VER_REVISION,\
																CRISCRIPT_VER_BUILD)
//CRIScript Version Number
#ifdef  NDEBUG
const int8_t * volatile const criscript_version =
      (const int8_t*)"\nCRIScript Ver."CRISCRIPT_VERSION_STRING" Build:" __DATE__ " " __TIME__ "\n";
#else
const int8_t * volatile const criscript_version =
      (const int8_t*)"\nCRIScript Ver."CRISCRIPT_VERSION_STRING" Debug Build:" __DATE__ " " __TIME__ "\n";
#endif

/*EN Structure that represents versions */
struct CRISCRIPT_VERSIONS {
	/*EN Major version number */
	static const uint16_t major = CRISCRIPT_VER_MAJOR;
    /*EN Minor version number */
	static const uint16_t minor = CRISCRIPT_VER_MINOR;
    /*EN Revision number*/
	static const uint16_t revision = CRISCRIPT_VER_REVISION;
    /*EN Build number. A build number is incremented as a part of a nightly buuld process.*/
	static const uint16_t build = CRISCRIPT_VER_BUILD;
};

const CRISCRIPT_VERSIONS criscript_version_struct = {};

}	//namespace cri
