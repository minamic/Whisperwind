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

#include "UtilSerialization_impl.h"
#include "RenderMappingDefines.h"
#include "Mesh.h"

namespace Engine
{
	template<class Archive>
	void serialize(Archive & ar, VertexElement & ve, const Util::u_int /*version*/)
	{
		ar & ve.StreamIndex;
		ar & ve.Offset;
		ar & ve.Type;
		ar & ve.Usage;
		ar & ve.UsageIndex;
	}

	template<class Archive>
	void serialize(Archive & ar, BufferData & bd, const Util::u_int /*version*/)
	{
		ar & bd.DataVec;
		ar & bd.DataSize;
		ar & bd.Stride;
	}

	template<class Archive>
	void serialize(Archive & ar, VertexMapping & vm, const Util::u_int /*version*/)
	{
		ar & vm.VertexCount;
		ar & vm.VertexDataVec;
		ar & vm.VertexElemVec;
		ar & vm.VertexUsage;
	}

	template<class Archive>
	void serialize(Archive & ar, IndexMapping & im, const Util::u_int /*version*/)
	{
		ar & im.HasIndex;
		ar & im.IndexData;
		ar & im.IndexFmt;
		ar & im.IndexUsage;
	}

	template<class Archive>
	void serialize(Archive & ar, RenderableMapping & rm, const Util::u_int /*version*/)
	{
		ar & rm.RenderableName;
		ar & rm.VertexBound;
		ar & rm.IndexBound;
		ar & rm.EffectName;
		ar & rm.TechniqueName;
		ar & rm.PrimType;
		ar & rm.PrimCount;
		ar & rm.RenderingType;
		ar & rm.ParamTextureVec;
		ar & rm.ParamValueVec;
	}

	template<class Archive>
	void serialize(Archive & ar, SubMesh & sm, const Util::u_int /*version*/)
	{
		ar & sm.mAABB;
		ar & sm.mRenderableMapping;
	}

	template<class Archive>
	void serialize(Archive & ar, Mesh & mesh, const Util::u_int /*version*/)
	{
		ar & mesh.mSubMeshVec;
		ar & mesh.mAABB;
	}

}