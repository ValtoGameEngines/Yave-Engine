/*******************************
Copyright (c) 2016-2019 Gr�goire Angerand

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

#include "RayleighSkyPass.h"

#include <yave/framegraph/FrameGraph.h>

#include <yave/ecs/EntityWorld.h>

#include <yave/components/DirectionalLightComponent.h>
#include <y/core/Chrono.h>


namespace yave {

static math::Vec4 sun_data(const ecs::EntityWorld& world) {
	math::Vec4 dir;
	for(auto [l] : world.view<DirectionalLightComponent>().components()) {
		if(dir.w() < l.intensity()) {
			dir = math::Vec4(l.direction(), l.intensity());
		}
	}
	return dir.is_zero() ? math::Vec4(0.0f, 0.0f, 1.0f, 10.0f) : dir;

	/*float time = float(core::Chrono::program().to_secs());
	return math::Vec4(std::cos(time), 0.0f, std::sin(time), 20.0f);*/
}

RayleighSkyPass RayleighSkyPass::create(FrameGraph& framegraph, const SceneView& scene_view, FrameGraphImageId in_depth, FrameGraphImageId in_color) {

	math::Vec4 sun = sun_data(scene_view.world());
	struct SkyData {
		uniform::LightingCamera camera_data;
		math::Vec3 sun_direction;
		float sun_intensity;
		float origin_height;
		float planet_height;
		float atmo_height;
	} sky_data {
			scene_view.camera(),
			-sun.to<3>().normalized(),
			sun.w(),
			6360.0f * 1000.0f + 100.0f,
			6360.0f * 1000.0f,
			6420.0f * 1000.0f
		};


	FrameGraphPassBuilder builder = framegraph.add_pass("Rayleigh sky pass");

	auto depth = builder.declare_copy(in_depth);
	auto color = builder.declare_copy(in_color);
	auto buffer = builder.declare_typed_buffer<SkyData>(1);

	RayleighSkyPass pass;
	pass.depth = depth;
	pass.color = color;

	builder.add_uniform_input(buffer);
	builder.map_update(buffer);

	builder.add_depth_output(depth, Framebuffer::LoadOp::Load);
	builder.add_color_output(color, Framebuffer::LoadOp::Load);
	builder.set_render_func([=](CmdBufferRecorder& recorder, const FrameGraphPass* self) {
			TypedMapping<SkyData> mapping = self->resources()->mapped_buffer(buffer);
			mapping[0] = sky_data;

			auto render_pass = recorder.bind_framebuffer(self->framebuffer());
			const auto* material = recorder.device()->device_resources()[DeviceResources::RayleighSkyMaterialTemplate];
			render_pass.bind_material(material, {self->descriptor_sets()[0]});
			render_pass.draw(vk::DrawIndirectCommand(6, 1));
		});

	return pass;
}

}