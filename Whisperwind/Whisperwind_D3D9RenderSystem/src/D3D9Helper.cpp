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

#include "UtilWarningDisable.h"
/// This header should be at the front of boost/algorithm/string.hpp,I donnot know why.
#include "MakeCOMPtr.h"

#include <algorithm>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "DebugDefine.h"
#include "ExceptionDefine.h"
#include "CheckedCast.h"
#include "RenderMappingDefines.h"
#include "EngineManager.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "D3D9ForwardDeclare.h"
#include "D3D9Renderable.h"
#include "D3D9FormatMapping.h"
#include "D3D9RenderTexture.h"
#include "D3D9Helper.h"

namespace Engine
{
	//---------------------------------------------------------------------
	// ColorPredefines
	//---------------------------------------------------------------------
 	const D3DCOLOR ColorPredefines::WHITE = D3DCOLOR(0xFFFFFFFF);
 	const D3DCOLOR ColorPredefines::BLACK = D3DCOLOR(0x00000000);

	//---------------------------------------------------------------------
	// D3D9Helper
	//---------------------------------------------------------------------
	ID3DXEffectMap D3D9Helper::mEffectMap;
	//---------------------------------------------------------------------
	RenderablePtr D3D9Helper::createD3D9Renderable(const IDirect3DDevice9Ptr & device, const RenderableMappingPtr & rm)
	{
		/// check
#ifdef WHISPERWIND_DEBUG
		{
			BOOST_FOREACH(const BufferData & data, rm->VertexBound.VertexDataVec)
			{
				WHISPERWIND_ASSERT(data.DataVec.data() != NULL);
				WHISPERWIND_ASSERT(data.DataSize > 0);
				//WHISPERWIND_ASSERT(data.Stride > 0); /// Comment because sometimes we need zero stride.See the document "setStreamSource".
			}
			WHISPERWIND_ASSERT(rm->VertexBound.VertexCount > 0);
			if (rm->IndexBound.HasIndex)
			{
				WHISPERWIND_ASSERT(rm->IndexBound.IndexData.DataVec.data() != NULL);
				WHISPERWIND_ASSERT(rm->IndexBound.IndexData.DataSize > 0);
				WHISPERWIND_ASSERT(rm->IndexBound.IndexData.Stride > 0);
			}
		}
#endif

		D3D9RenderablePtr d3d9Renderable = boost::make_shared<D3D9Renderable>(device);

		/// Vertex bound
		{
			const DWORD usage = (rm->VertexBound.VertexUsage != BUF_STATIC) ? (D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY) : 0;
			const D3DPOOL pool = getCreationPool(rm->VertexBound.VertexUsage);
			const DWORD lockFlag = ((usage & D3DUSAGE_DYNAMIC) != 0) ? D3DLOCK_DISCARD : 0;

			IDirect3DVertexBuffer9 * vb = NULL;

			BOOST_FOREACH(const BufferData & bufData, rm->VertexBound.VertexDataVec)
			{
				DX_IF_FAILED_DEBUG_PRINT(device->CreateVertexBuffer(bufData.DataSize, usage, 0, pool, &vb, NULL));
				{
					void * buf = NULL;
					const Util::u_int8 * data = bufData.DataVec.data();
					DX_IF_FAILED_DEBUG_PRINT(vb->Lock(0, 0, &buf, lockFlag));
					std::copy(data, data + bufData.DataSize, static_cast<Util::u_int8 *>(buf));
					vb->Unlock();
				}
				WHISPERWIND_ASSERT(vb != NULL);

				d3d9Renderable->getVertexBound().VertexBufferVec.push_back(Util::makeCOMPtr(vb));
				d3d9Renderable->getVertexBound().VertexStrideVec.push_back(bufData.Stride);
			}
			
			d3d9Renderable->getVertexBound().VertexCount = rm->VertexBound.VertexCount;
			d3d9Renderable->getVertexBound().VertexDeclaration = 
				D3D9FormatMappingFactory::createD3D9VertexDeclaration(device, rm->VertexBound.VertexElemVec);
		}

		/// IB
		if (rm->IndexBound.HasIndex)
		{
			d3d9Renderable->setHasIndex(true);

			const DWORD usage = (rm->IndexBound.IndexUsage != BUF_STATIC) ? (D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY) : 0;
			const D3DPOOL pool = getCreationPool(rm->IndexBound.IndexUsage);

			IDirect3DIndexBuffer9 * ib = NULL;
			D3DFORMAT idxFmt = (INDEX_16 == rm->IndexBound.IndexFmt) ? D3DFMT_INDEX16 : D3DFMT_INDEX32;
			DX_IF_FAILED_DEBUG_PRINT(device->CreateIndexBuffer(rm->IndexBound.IndexData.DataSize, usage, idxFmt, pool, &ib, NULL));
			{
				const DWORD lockFlag = ((usage & D3DUSAGE_DYNAMIC) != 0) ? D3DLOCK_DISCARD : 0;

				void * buf = NULL;
				const Util::u_int8 * data = rm->IndexBound.IndexData.DataVec.data();
				DX_IF_FAILED_DEBUG_PRINT(ib->Lock(0, 0, &buf, lockFlag));
				std::copy(data, data + rm->IndexBound.IndexData.DataSize, static_cast<Util::u_int8 *>(buf));
				ib->Unlock();
			}
			WHISPERWIND_ASSERT(ib != NULL);

			IDirect3DIndexBuffer9Ptr ibPtr = Util::makeCOMPtr(ib);

			d3d9Renderable->setIndexBuffer(ibPtr);
		}

		/// Primitive
		{
			d3d9Renderable->setPrimType(D3D9FormatMappingFactory::getD3D9PrimType(rm->PrimType));
			d3d9Renderable->setPrimCount(rm->PrimCount);
		}

		/// Effect
		{
			if (rm->EffectName.empty())
			{
				WHISPERWIND_EXCEPTION("No effect!");
			}

			const ID3DXEffectPtr & effect = createD3D9Effect(device, rm->EffectName);
			d3d9Renderable->setEffect(effect);
		}

		/// Technique
		{
			if (rm->TechniqueName.empty())
			{
				WHISPERWIND_EXCEPTION("No technique!");
			}

			D3DXHANDLE tech = d3d9Renderable->getEffect()->GetTechniqueByName(rm->TechniqueName.c_str());
			DX_IF_NULL_DEBUG_PRINT(tech);

			d3d9Renderable->setTechnique(tech);
		}

		/// RenderType
		{
			d3d9Renderable->setRenderType(rm->RenderingType);
		}

		/// ParamValuePairVector
		{
			ParamValuePairVector & pairVec = d3d9Renderable->getParamValuePairVec();

			ParamValuePair pvPair;
			D3DXPARAMETER_DESC paramDesc;
			BOOST_FOREACH(const EffectParamValuePair & pair, rm->ParamValueVec)
			{
				pvPair.first = pair.first;

				const Util::String & value = pair.second;
				Util::StringVector strVec;
				boost::algorithm::split(strVec, value, boost::is_any_of(","));

				Util::size_t size = strVec.size();
				DX_IF_FAILED_DEBUG_PRINT(d3d9Renderable->getEffect()->GetParameterDesc(pair.first.c_str(), &paramDesc));
				switch (paramDesc.Type)
				{
				case D3DXPT_BOOL:
				case D3DXPT_INT:
					{
						VoidDataPtr value(WHISPERWIND_NEW(Util::s_int[size]));
						Util::s_int * valuePtr = boost::static_pointer_cast<Util::s_int>(value).get();
						for (Util::size_t it = 0; it < size; ++it)
						{
							*(valuePtr + it) = boost::lexical_cast<Util::s_int>(strVec[it]);
						}

						pvPair.second = value;

						break;
					}
				case D3DXPT_FLOAT:
					{
						VoidDataPtr value(WHISPERWIND_NEW(Util::real[size]));
						Util::real * valuePtr = boost::static_pointer_cast<Util::real>(value).get();
						for (Util::size_t it = 0; it < size; ++it)
						{
							*(valuePtr + it) = boost::lexical_cast<Util::real>(strVec[it]);
						}

						pvPair.second = value;

						break;
					}
				default:
					{
						WHISPERWIND_EXCEPTION("Donnot surpport this param format!");
						break;
					}
				}

				pairVec.push_back(pvPair);
			}
		}

		/// ParamTextureVec
		{
			ParamTexturePairVector & pairVec = d3d9Renderable->getParamTexturePairVec();

			ParamTexturePair ptPair;
			D3D9RenderTexturePtr texture;
			BOOST_FOREACH(const EffectParamValuePair & pair, rm->ParamTextureVec)
			{
				ptPair.first = pair.first;

				Util::Wstring texturePath = EngineManager::getSingleton().getResourceManager()->getResourcePath(Util::StringToWstring(pair.second));
				ptPair.second = Util::checkedPtrCast<D3D9RenderTexture>(EngineManager::getSingleton().getRenderSystem()->createRenderTextureFromFile(texturePath));

				pairVec.push_back(ptPair);
			}
		}

		return d3d9Renderable;
	}
	//---------------------------------------------------------------------
	const ID3DXEffectPtr & D3D9Helper::createD3D9Effect(const IDirect3DDevice9Ptr & device, const Util::String & effectName)
	{
		if (mEffectMap.find(effectName) == mEffectMap.end())
		{
#ifdef WHISPERWIND_DEBUG
			DWORD shaderFlags = D3DXSHADER_DEBUG | D3DXSHADER_SKIPOPTIMIZATION;
#else
			DWORD shaderFlags = D3DXSHADER_OPTIMIZATION_LEVEL3;
#endif
			ID3DXEffect * effect = NULL;
			Util::Wstring effectPath = EngineManager::getSingleton().getResourceManager()->getResourcePath(Util::StringToWstring(effectName));
			DX_IF_FAILED_DEBUG_PRINT(D3DXCreateEffectFromFile(device.get(), effectPath.c_str(), 
				NULL, NULL, shaderFlags, NULL, &effect, NULL));
			DX_IF_NULL_DEBUG_PRINT(effect);

			ID3DXEffectPtr effectPtr = Util::makeCOMPtr(effect);
			mEffectMap.insert(ID3DXEffectMap::value_type(effectName, effectPtr));
		}

		return mEffectMap[effectName];
	}
	//---------------------------------------------------------------------
	Util::u_int D3D9Helper::getPrimCount(D3DPRIMITIVETYPE type, Util::u_int vertexCount)
	{
		Util::u_int primCount = 0;

		switch (type)
		{
		case D3DPT_TRIANGLELIST :
			{
				primCount = vertexCount / 3;
				break;
			}
		case D3DPT_TRIANGLESTRIP :
			{
				primCount = vertexCount - 2;
				break;
			}
		case D3DPT_LINELIST :
			{
				primCount = vertexCount / 2;
			}
		default:
			WHISPERWIND_ASSERT(false);
		}
		WHISPERWIND_ASSERT(primCount > 0);

		return primCount;
	}
	//---------------------------------------------------------------------
	D3DPOOL D3D9Helper::getCreationPool(BufferUsageFlag flag)
	{
		return ((flag != BUF_STATIC) ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED);
	}

}