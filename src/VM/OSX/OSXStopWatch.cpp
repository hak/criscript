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

#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

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
	m_liStart = mach_absolute_time();
	m_bStarted = true;
}

/***************************************************************************
 *Stop the stopwatch
 ***************************************************************************/
void CStopWatch::Stop()
{
	if( !m_bStarted ) return;
	m_liEnd = mach_absolute_time();
	
	m_bStarted = false;
	
	m_liDuration += m_liEnd - m_liStart;
}


/***************************************************************************
 *Reset the stopwatch
 ***************************************************************************/
void CStopWatch::Reset()
{
	m_liDuration = 0;
	m_bStarted = false;
}

/***************************************************************************
 *Get stop watch result
 ***************************************************************************/
/* this code uses calls from the CoreServices framework, so to get this to work you need to
add the "-framework CoreServices" parameter g++ in the linking stage. This code was adapted from:
http://developer.apple.com/qa/qa2004/qa1398.html
*/
float CStopWatch::getResult()
{
	float f;
	Nanoseconds elapsedTime = AbsoluteToNanoseconds( *(AbsoluteTime *) &m_liDuration );
	f = float( double(*(int64_t*)&elapsedTime) / 1000000000.0 );
	return f;
}

} //namespace CRI