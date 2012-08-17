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
 * Module   : StopWatchFunction
 * File     : StopWatch.cpp
 * Date     : 
 * Version  : 
 *
 ****************************************************************************/

/***************************************************************************
 *      Include file
 ***************************************************************************/
#include "stdafx.h"
#include "../inc/criScript.h"
namespace cri {
/***************************************************************************
 *      Variables
 ***************************************************************************/

/***************************************************************************
 *      Methods
 ***************************************************************************/

/***************************************************************************
 *Start the stopwatch
 ***************************************************************************/
void CStopWatch::Start()
{
	if( m_bStarted ) return;
	QueryPerformanceCounter( & m_liStart );
	m_bStarted = true;
}

/***************************************************************************
 *Stop the stopwatch
 ***************************************************************************/
void CStopWatch::Stop()
{
	if( !m_bStarted ) return;
	QueryPerformanceCounter( & m_liEnd );
	m_bStarted = false;
	m_liDuration.QuadPart += m_liEnd.QuadPart - m_liStart.QuadPart;
}


/***************************************************************************
 *Reset the stopwatch
 ***************************************************************************/
void CStopWatch::Reset()
{
	m_liDuration.QuadPart = 0;
	m_bStarted = false;
}

/***************************************************************************
 *Get stop watch result
 ***************************************************************************/
float CStopWatch::getResult()
{
	float f;
	LARGE_INTEGER liFreq;
	QueryPerformanceFrequency( &liFreq );
	f = float(m_liDuration.QuadPart) * 1000.f / float( liFreq.QuadPart );
	return f;
}

} //namespace CRI