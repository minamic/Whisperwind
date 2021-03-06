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
#ifndef _SCENE_COMPONENT_H_
#define _SCENE_COMPONENT_H_

#include <boost/function.hpp>

#include "Util.h"

namespace Engine
{
	enum ComponentType
	{
		CT_PHYSICABLE,
		CT_MAX
	};

	class SceneComponent
	{
	protected:
		explicit SceneComponent(ComponentType type) :
		    mCompType(type),
			mCanUpdate(true)
		{}

		virtual ~SceneComponent()
		{}

	public:
		void preUpdate(Util::time elapsedTime);
		void postUpdate(Util::time elapsedTime);
		virtual const Util::Wstring & getName() const = 0;

		template <typename CallBack>
		void regPreUpdateCallback(CallBack cb) { mPreCallback = cb; }
		template <typename CallBack>
		void regPostUpdateCallback(CallBack cb) { mPostCallback = cb; }

	public:
		SET_GET_CONST_VALUE(ComponentType, CompType);

	protected:
		SET_GET_CONST_VALUE(bool, CanUpdate);

	private:
		virtual void preUpdate_impl(Util::time elapsedTime) = 0;
		virtual void postUpdate_impl(Util::time elapsedTime) = 0;

	private:
		typedef boost::function<void (ComponentType, Util::time)> Callback;
		Callback mPreCallback;
		Callback mPostCallback;

		ComponentType mCompType;

		bool mCanUpdate;

	private:
		DISALLOW_COPY_AND_ASSIGN(SceneComponent);
	};
}

#endif