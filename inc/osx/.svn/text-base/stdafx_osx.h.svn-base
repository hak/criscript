// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <stdio.h>
#include <stdint.h>
//#include <tchar.h>
#include <assert.h>
#include <iostream>
#include <fstream>

#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <ext/hash_map>
#include <list>
#include <algorithm>
#include <cctype>
#include <math.h>
#include <float.h>
#include <cctype>

#include "osx/stopwatch_osx.h"

#define ARRAYSIZE(initParams) (sizeof((initParams))/sizeof((initParams)[0]))

const int8_t MAXBYTE = 0x7f;
const int8_t MAXCHAR = 0x7f;
const int8_t MINBYTE = 0x80;
const int8_t MINCHAR = 0x80;
const int16_t MAXSHORT = 0x7fff;
const int16_t MINSHORT = 0x8000;
const int32_t MAXINT = 0x7fffffff;
const int32_t MININT = 0x80000000;

#define _finite finite
#define _isnan isnan

//--------------------------------------------------------------
//Win32 specifc APIs
//--------------------------------------------------------------
#define sprintf_s sprintf
#define swprintf_s swprintf
#define sscanf_s sscanf
#define swscanf_s swscanf

//--------------------------------------------------------------
//swprintf definition
//--------------------------------------------------------------
#define INFO_STRING L"%S"

__inline int _wtoi(wstring str )
{
	int val;
	int len = str.length();
	char* astring = new char[len+1];
	memset(astring, 0, len+1);
	for (int i = 0; i < len; i++)
		astring[i] = (char)str[i];
	val = atoi(astring);
	delete[] astring;
	astring = NULL;
	return val;
}


__inline int64_t _wtoi64(wstring str )
{
	int64_t val;
	int len = str.length();
	char* astring = new char[len+1];
	memset(astring, 0, len+1);
	for (int i = 0; i < len; i++)
		astring[i] = (char)str[i];
	val = (int64_t)atoi(astring);
	delete[] astring;
	astring = NULL;
	return val;
}

__inline float _wtof(wstring str )
{
	float val;
	int len = str.length();
	char* astring = new char[len+1];
	memset(astring, 0, len+1);
	for (int i = 0; i < len; i++)
		astring[i] = (char)str[i];
	val = atoi(astring);
	delete[] astring;
	astring = NULL;
	return val;
}

//--------------------------------------------------------------
//To resolve hash compile error 
//wsring hash function
//--------------------------------------------------------------
namespace __gnu_cxx {
template<>
	struct hash<std::wstring>
	{
		size_t operator()(const std::wstring& __x) const
		{
			const wchar_t *__s = __x.data();
			
			unsigned long __h = 0;
			for ( ; *__s; ++__s)
				__h = 5 * __h + *__s;
			return size_t(__h);
		}
	};
	
	template<>
	struct hash<std::string>
	{
		size_t operator()(const std::string& __x) const
		{
			const char *__s = __x.data();
			
			unsigned long __h = 0;
			for ( ; *__s; ++__s)
				__h = 5 * __h + *__s;
			return size_t(__h);
		}
	};
}


// TODO: reference additional headers your program requires here