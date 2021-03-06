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
#ifndef _DEBUG_RESOURCE_H_
#define _DEBUG_RESOURCE_H_

#include "Util.h"
#include "Resource.h"

namespace Engine
{
	enum DebugType
	{
		DT_NODE,
		DT_OBJECT,
		DT_RENDERABLE
	};

	class DebugResource : public Resource
	{
	public:
		DebugResource()
		{}

		~DebugResource()
		{}

	public:
		WHISPERWIND_API static const Util::Wstring & getDebugCreateType();
		WHISPERWIND_API static const Util::Wstring & getDebugNodeSuffix();
		WHISPERWIND_API static const Util::Wstring & getDebugObjectSuffix();

	public:
		GET_CONST_VALUE(RenderableMappingPtr, RenderableMapping);

	private:
		virtual void load(const Util::Wstring & name);

		void constructRenderableMapping(const Util::AABBPtr & aabb, DebugType type);

	private:
		RenderableMappingPtr mRenderableMapping;

		/// NOTE:Since be used as dll exported,we cannot just export these members as public.
		/// We must use non-inline access functions to get them and exported the functions.
		/// So why? Just look here as I donnot know clearly: http://stackoverflow.com/questions/2479784/exporting-static-data-in-a-dll
		static const Util::Wstring mDebugCreateType;
		static const Util::Wstring mDebugNodeSuffix;
		static const Util::Wstring mDebugObjectSuffix;
	};

}

#endif