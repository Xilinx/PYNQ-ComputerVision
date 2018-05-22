/******************************************************************************
 *  Copyright (c) 2018, Xilinx, Inc.
 *  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1.  Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *
 *  2.  Redistributions in binary form must reproduce the above copyright 
 *      notice, this list of conditions and the following disclaimer in the 
 *      documentation and/or other materials provided with the distribution.
 *
 *  3.  Neither the name of the copyright holder nor the names of its 
 *      contributors may be used to endorse or promote products derived from 
 *      this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION). HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/
 
/*****************************************************************************
*
*     Author: Kristof Denolf <kristof@xilinx.com>
*     Date:   2016/06/02
*
*****************************************************************************/

#include "HRTimer.h"


HRTimer::HRTimer() {
#if defined(WIN32)
	QueryPerformanceFrequency(&frequency);
	clks_per_us = (float)frequency.QuadPart/float(1000000.0);
#else
	start = 0;
	stop = 0;
#endif
	us = 0;
}

HRTimer::~HRTimer() {
}

void HRTimer::StartTimer() {
#if defined(WIN32)
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&start);
	SetThreadAffinityMask(::GetCurrentThread(),oldmask);
#else
	start = getCurrent();
#endif
}

void HRTimer::StopTimer() {
#if defined(WIN32)
	DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
	QueryPerformanceCounter(&stop);
	SetThreadAffinityMask(::GetCurrentThread(), oldmask);
	us = (US_TYPE)((stop.QuadPart - start.QuadPart)/clks_per_us);
#else
	stop = getCurrent();
	us = stop - start;
#endif
}

void HRTimer::RestartTimer() {
	StopTimer();
	StartTimer();

}

US_TYPE HRTimer::GetElapsedUs() {
	return us;
}

US_TYPE HRTimer::GetCurrentTime() {
#if defined(WIN32)
    LARGE_INTEGER now;
    DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);
	QueryPerformanceCounter(&now);
	SetThreadAffinityMask(::GetCurrentThread(), oldmask);
    return (US_TYPE)((now.QuadPart - start.QuadPart)/clks_per_us);
#else
    US_TYPE now = getCurrent();
    US_TYPE time = now - start;
    return time;
#endif    
}
