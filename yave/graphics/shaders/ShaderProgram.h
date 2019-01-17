/*******************************
Copyright (c) 2016-2019 Grégoire Angerand

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
#ifndef YAVE_GRAPHICS_SHADERS_SHADERPROGRAM_H
#define YAVE_GRAPHICS_SHADERS_SHADERPROGRAM_H

#include "ShaderModule.h"

#include <y/core/AssocVector.h>

namespace yave {

class ShaderProgram : NonCopyable, public DeviceLinked {

	public:
		static constexpr u32 PerInstanceLocation = 8;

		ShaderProgram(const FragmentShader& frag, const VertexShader& vert, const GeometryShader& geom);

		core::ArrayView<vk::PipelineShaderStageCreateInfo> vk_pipeline_stage_info() const;
		core::ArrayView<vk::DescriptorSetLayout> descriptor_layouts() const;

		// should ALWAYS be sorted by location

		core::ArrayView<vk::VertexInputBindingDescription> attribute_bindings() const {
			return _vertex.bindings;
		}

		core::ArrayView<vk::VertexInputAttributeDescription> attributes_descriptions() const {
			return _vertex.attribs;
		}

		core::ArrayView<vk::PushConstantRange> push_constants() const {
			return _push_constants;
		}

	private:
		std::unordered_map<u32, core::Vector<vk::DescriptorSetLayoutBinding>> _bindings;
		core::Vector<vk::PushConstantRange> _push_constants;
		core::Vector<vk::DescriptorSetLayout> _layouts;
		core::Vector<vk::PipelineShaderStageCreateInfo> _stages;

		struct {
			core::Vector<vk::VertexInputAttributeDescription> attribs;
			core::Vector<vk::VertexInputBindingDescription> bindings;
		} _vertex;
};

}

#endif // YAVE_GRAPHICS_SHADERS_SHADERPROGRAM_H
