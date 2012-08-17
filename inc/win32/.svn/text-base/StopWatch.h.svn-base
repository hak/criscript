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
 * Module   : JScript Error handlers
 * File     : Error.h
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/
#pragma once
//#include "stdafx.h"

using namespace std;

namespace cri {
/***************************************************************************
 *      Error info handler class
 ***************************************************************************/
class CStopWatch {
	LARGE_INTEGER m_liStart;
	LARGE_INTEGER m_liEnd;
	bool m_bStarted;
	LARGE_INTEGER m_liDuration;
public:
	CStopWatch() {Reset();}
	~CStopWatch() {Reset();}
	void Start();
	void Stop();
	void Reset();
	float getResult();
};

}	//namespace cri

