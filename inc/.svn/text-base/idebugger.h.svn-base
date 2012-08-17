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
 * Module   : JScript Virtual Machine Debug interface
 * File     : cilDebugger.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once

#include "IDebugger.h"

using namespace std;
namespace cri {

class IDebugger {
public:
	IDebugger()
	{
		init();
	};
	~IDebugger() {
		terminate();
	};
	virtual bool init() { return true; }
	virtual void terminate() {}

};

}	//cri