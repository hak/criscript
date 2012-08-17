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
 * Module   : stdint definitions
 * File     : stdint.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once

#include <limits.h>

#ifdef __APPLE__
typedef __int8_t			int8_t;
typedef __int16_t			int16_t;
typedef __int32_t			int32_t;
typedef __int64_t			int64_t;
typedef __uint8_t			uint8_t;
typedef __uint16_t			uint16_t;
typedef __uint32_t			uint32_t;
typedef __uint64_t			uint64_t;

/* 7.18.1.2 Minimum-width integer types */
typedef int8_t           int_least8_t;
typedef int16_t         int_least16_t;
typedef int32_t         int_least32_t;
typedef int64_t         int_least64_t;
typedef uint8_t         uint_least8_t;
typedef uint16_t       uint_least16_t;
typedef uint32_t       uint_least32_t;
typedef uint64_t       uint_least64_t;

/* 7.18.1.3 Fastest-width integer types */
typedef int8_t            int_fast8_t;
typedef int16_t          int_fast16_t;
typedef int32_t          int_fast32_t;
typedef int64_t          int_fast64_t;
typedef uint8_t          uint_fast8_t;
typedef uint16_t        uint_fast16_t;
typedef uint32_t        uint_fast32_t;
typedef uint64_t        uint_fast64_t;

/* 7.18.1.4 Integer types capable of holding object pointers */

#ifndef _INTPTR_T
#define _INTPTR_T
typedef long   intptr_t;
#endif /* _INTPTR_T */

#ifndef _UINTPTR_T
#define _UINTPTR_T
typedef unsigned long   uintptr_t;
#endif /* _UINTPTR_T */


/* 7.18.1.5 Greatest-width integer types */
#ifndef _INTMAX_T
#define _INTMAX_T
#ifdef __INTMAX_TYPE__
typedef __INTMAX_TYPE__             intmax_t;
#else /* __INTMAX_TYPE__ */
typedef long long                intmax_t;
#endif /* __INTMAX_TYPE__ */
#endif /* _INTMAX_T */

#ifndef _UINTMAX_T
#define _UINTMAX_T
#ifdef __UINTMAX_TYPE__
typedef __UINTMAX_TYPE__             uintmax_t;
#else /* __UINTMAX_TYPE__ */
typedef unsigned long long      uintmax_t;
#endif /* __UINTMAX_TYPE__ */
#endif /* _UINTMAX_T */

#else
//
//Win32, Xbox360
//
typedef __int8            int8_t;
typedef __int16           int16_t;
typedef __int32           int32_t;
typedef __int64           int64_t;
typedef unsigned __int8   uint8_t;
typedef unsigned __int16  uint16_t;
typedef unsigned __int32  uint32_t;
typedef unsigned __int64  uint64_t;
#endif