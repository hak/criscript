// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <stdio.h>
#include <tchar.h>
#include <assert.h>
#include <iostream>
#include <fstream>

#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <hash_map>
#include <list>
#include <algorithm>
#include <cctype>
#include <math.h>
#include <float.h>

#ifdef _WIN32_WINNT
#include <windows.h>
#endif

#include "../inc/stdint.h"
#include "win32/stopwatch.h"

//--------------------------------------------------------------
//swprintf definition
//--------------------------------------------------------------
#define INFO_STRING L"%s"

#define LONG_LONG_MAX LLONG_MAX
#define LONG_LONG_MIN LLONG_MIN

