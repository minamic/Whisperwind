/*-------------------------------------------------------------------------
This source file is a part of Whisperwind.(GameEngine + GamePlay + GameTools)
For the latest info, see http://lisuyong.com

Copyright (c) 2012 Suyong Li (79481268@qq.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
-------------------------------------------------------------------------*/

/** for windows.h's warning level */
#pragma warning(push, 3)
#include <windows.h>
#pragma warning(pop)

#include "ExceptionDefine.h"
#include "Timer.h"

namespace Util
{
	//---------------------------------------------------------------------
	// Timer
	//---------------------------------------------------------------------
	Timer::Timer() :
		mLastTimeStamp(0),
		mZoom(1.0)
	{}
	//---------------------------------------------------------------------
	time Timer::getElapsedTime()
	{
		return getElapsedTime_impl();
	}
	//---------------------------------------------------------------------
	void Timer::sleep(Util::u_int milliTime)
	{
		return sleep_impl(milliTime);
	}

	//---------------------------------------------------------------------
	// WindowsTimer
	//---------------------------------------------------------------------
	time WindowsTimer::getElapsedTime_impl()
	{
		/// Use static for performance.
		static u_int64 freq = 0;
		if (0 == freq)
		{
			LARGE_INTEGER freqInt;
			IF_NULL_EXCEPTION(QueryPerformanceFrequency(&freqInt), "QueryPerformanceFrequency failed!");

			freq = static_cast<u_int64>(freqInt.QuadPart);
		}

		static LARGE_INTEGER curTime;
		BOOST_VERIFY(TRUE == QueryPerformanceCounter(&curTime));
		
		static u_int64 curTimeStamp;
		curTimeStamp = curTime.QuadPart * 1000 / freq;

		if (0 == mLastTimeStamp)
			mLastTimeStamp = curTimeStamp;

		time result = static_cast<time>(curTimeStamp - mLastTimeStamp) / 1000;

		mLastTimeStamp = curTimeStamp;

		return result;
	}
	//---------------------------------------------------------------------
	void WindowsTimer::sleep_impl(Util::u_int milliTime)
	{
		::Sleep(milliTime);
	}
}