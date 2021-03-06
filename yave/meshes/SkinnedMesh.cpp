/*******************************
Copyright (c) 2016-2020 Grégoire Angerand

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**********************************/

#include "SkinnedMesh.h"

#include <yave/graphics/buffers/TypedWrapper.h>
#include <yave/graphics/commands/CmdBufferRecorder.h>
#include <yave/graphics/commands/RecordedCmdBuffer.h>
#include <yave/device/Device.h>

namespace yave {

SkinnedMesh::SkinnedMesh(DevicePtr dptr, const MeshData& mesh_data) :
		_triangle_buffer(dptr, mesh_data.triangles().size()),
		_vertex_buffer(dptr, mesh_data.skinned_vertices().size()),
		_skeleton(mesh_data.bones()),
		_radius(mesh_data.radius()) {

	_indirect_data.indexCount = mesh_data.triangles().size() * 3;
	_indirect_data.instanceCount = 1;

	CmdBufferRecorder recorder(dptr->create_disposable_cmd_buffer());
	Mapping::stage(_triangle_buffer, recorder, mesh_data.triangles().data());
	Mapping::stage(_vertex_buffer, recorder, mesh_data.skinned_vertices().data());
	dptr->graphic_queue().submit<SyncSubmit>(RecordedCmdBuffer(std::move(recorder)));
}

const TriangleBuffer<>& SkinnedMesh::triangle_buffer() const {
	return _triangle_buffer;
}

const SkinnedVertexBuffer<>& SkinnedMesh::vertex_buffer() const {
	return _vertex_buffer;
}

const VkDrawIndexedIndirectCommand& SkinnedMesh::indirect_data() const {
	return _indirect_data;
}

const Skeleton& SkinnedMesh::skeleton() const {
	return _skeleton;
}

float SkinnedMesh::radius() const {
	return _radius;
}

}
