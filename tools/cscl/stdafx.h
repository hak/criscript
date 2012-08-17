// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//-------------------------------------------------------------
//Disable Warnings
//-------------------------------------------------------------
#pragma warning(disable:4065 4275) // disabling warnings that bison generates
//warning C4275: non dll-interface class 'stdext::exception' used as base for dll-interface class 'std::bad_cast'

//-------------------------------------------------------------
//Disable STL excepiton
//-------------------------------------------------------------
#ifndef _HAS_EXCEPTIONS
	#define _HAS_EXCEPTIONS 0	/*EN< Disable STL excepiton*/
#endif

#ifdef _XBOX
	//Xbox360
	#include "xbox360/stdafx_xbox360.h"
	using namespace std;
	using namespace stdext;

	//sprintf_s cast setting
	#define SPRINTF_CAST(a) (a)
#else
#ifdef __APPLE__	//OSX
	using namespace std;
	#include "osx/stdafx_osx.h"
	using namespace __gnu_cxx;

	//sprintf_s cast setting
	#define SPRINTF_CAST(a) ((char*)a)
#else
	//Win32
	#include "win32/stdafx_win32.h"
	using namespace std;
	using namespace stdext;

	//sprintf_s cast setting
	#define SPRINTF_CAST(a) (a)
#endif

#endif
