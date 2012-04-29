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
#ifndef _D3D9_HELPER_H_
#define _D3D9_HELPER_H_

/** for windows.h's warning level */
#pragma warning(push, 3)
#include <windows.h>
#pragma warning(pop)

#include <d3d9.h>
#include <DxErr.h>
#include "Util.h"

namespace Engine
{
	struct ColorPredefines
	{
		static const D3DCOLOR WHITE;
		static const D3DCOLOR BLACK;
	};
}

#define DX_IF_FAILED_RETURN_FALSE(x) \
	{ \
        HRESULT hr; hr = (x); \
	    if (FAILED(hr)) \
        { \
		     DEBUG_PRINT(DXGetErrorString(hr)); \
			 return false; \
        } \
    }

#ifdef WHISPERWIND_DEBUG
#define DX_IF_FAILED_DEBUG_PRINT(x) \
	{ \
	    HRESULT hr = S_OK; hr = (x); \
	    if (FAILED(hr)) \
		{ \
		    DEBUG_PRINT(DXGetErrorString(hr)); \
		} \
	}
#else
#define DX_IF_FAILED_DEBUG_PRINT(x) \
	(x);
#endif

#endif