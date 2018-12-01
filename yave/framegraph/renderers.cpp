/*******************************
Copyright (c) 2016-2018 Gr�goire Angerand

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

#include "renderers.h"

namespace yave {

SceneRender SceneRender::create(FrameGraphBuilder& builder, const SceneView& view) {
	SceneRender render;

	builder.create(render._camera_buffer, builder.device(), usize(1));
	builder.create(render._transform_buffer, builder.device(), max_batch_count);

	render._scene_view = view;
	render._camera_set = DescriptorSet(builder.device(), {Binding(builder.resources().get(render._camera_buffer))});

	return render;
}

void SceneRender::render(RenderPassRecorder& recorder, const FrameGraphResources& resources) const {
	auto& camera_buffer = resources.get(_camera_buffer);
	auto& transform_buffer = resources.get(_transform_buffer);

	// fill render data
	{
		{
			auto camera_mapping = TypedMapping(camera_buffer);
			camera_mapping[0] = _scene_view.camera().viewproj_matrix();
		}

		if(transform_buffer.size() < _scene_view.scene().renderables().size() +_scene_view.scene().static_meshes().size()) {
			y_fatal("SceneRender::_attrib_buffer overflow");
		}

		auto transform_mapping = TypedMapping(transform_buffer);
		u32 attrib_index = 0;
		{
			// renderables
			for(const auto& r : _scene_view.scene().renderables()) {
				transform_mapping[attrib_index++] = r->transform();
			}

			// static meshes
			for(const auto& r : _scene_view.scene().static_meshes()) {
				transform_mapping[attrib_index++] = r->transform();
			}
		}
	}

	// render stuff
	{
		u32 attrib_index = 0;

#warning clean unnecessary buffer binding
		recorder.bind_attrib_buffers({transform_buffer, transform_buffer});

		// renderables
		{
			for(const auto& r : _scene_view.scene().renderables()) {
				r->render(recorder, Renderable::SceneData{_camera_set, attrib_index++});
			}
		}

		// static meshes
		{
			for(const auto& r : _scene_view.scene().static_meshes()) {
				r->render(recorder, Renderable::SceneData{_camera_set, attrib_index++});
			}
		}
	}
}

GBufferPass& render_gbuffer(FrameGraph& framegraph, const SceneView& view, const math::Vec2ui& size) {
	static constexpr vk::Format depth_format = vk::Format::eD32Sfloat;
	static constexpr vk::Format color_format = vk::Format::eR8G8B8A8Unorm;
	static constexpr vk::Format normal_format = vk::Format::eR16G16B16A16Unorm;

	return framegraph.add_callback_pass<GBufferPass>("G-buffer pass",
		[&](FrameGraphBuilder& builder, GBufferPass& pass) {
			builder.create(pass.depth, builder.device(), depth_format, size);
			builder.create(pass.color, builder.device(), color_format, size);
			builder.create(pass.normal, builder.device(), normal_format, size);

			builder.render_to(pass.depth);
			builder.render_to(pass.color);
			builder.render_to(pass.normal);

			auto&& res = builder.resources();
			pass.gbuffer = Framebuffer(builder.device(), res.get(pass.depth), {res.get(pass.color), res.get(pass.normal)});
			pass.scene = SceneRender::create(builder, view);
		},

		[](CmdBufferRecorder& recorder, const GBufferPass& pass, const FrameGraphResources& resources) {
			auto render_pass = recorder.bind_framebuffer(pass.gbuffer);
			pass.scene.render(render_pass, resources);
		}
	);
}

}