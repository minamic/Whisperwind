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
#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include "Util.h"
#include "MathDefine.h"
#include "EngineForwardDeclare.h"
#include "RenderMappingDefines.h"

namespace Engine
{
	class WHISPERWIND_API Renderable
	{
	protected:
		Renderable() : 
		    mRenderType(RT_OPAQUE)
		{}

		virtual ~Renderable()
		{}

	public:
		void setWorldViewProj(CXMMATRIX wvp) { XMStoreFloat4x4(&mWorldViewProjMatrix, wvp); }
		void setWorldMatrix(CXMMATRIX world) { XMStoreFloat4x4(&mWorldMatrix, world); }
		void setWorldViewMatrix(CXMMATRIX worldView) { XMStoreFloat4x4(&mWorldViewMatrix, worldView); }

		inline void setEffect(const Util::String & effectName);
		inline void setTechnique(const Util::String & techName);
		inline void setEffectSemanticValue(const Util::String & semanticName, const void * data);
		inline void setEffectParamValue(const Util::String & paramName, const void * data);
		inline void setTexture(const Util::String & paramName, const RenderTexturePtr & texture);

		void preRender(Util::time elapsedTime);
		void postRender(Util::time elapsedTime);

	public:
		SET_GET_CONST_VALUE(RenderType, RenderType);
		SET_GET_CONST_VALUE(Util::AABBPtr, AABB);

	private:
		virtual void setEffect_impl(const Util::String & effectName) = 0;
		virtual void setTechnique_impl(const Util::String & techName) = 0;
		virtual void setEffectSemanticValue_impl(const Util::String & semanticName, const void * data) = 0;
		virtual void setEffectParamValue_impl(const Util::String & paramName, const void * data) = 0;
		virtual void setTexture_impl(const Util::String & paramName, const RenderTexturePtr & texture) = 0;

	private:
		RenderType mRenderType;
		XMFLOAT4X4 mWorldViewProjMatrix;
		XMFLOAT4X4 mWorldViewMatrix;
		XMFLOAT4X4 mWorldMatrix;
		Util::AABBPtr mAABB;

	private:
		DISALLOW_COPY_AND_ASSIGN(Renderable);
	};

}

#endif