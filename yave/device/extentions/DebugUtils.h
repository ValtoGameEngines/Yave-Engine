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
#ifndef YAVE_DEVICE_EXTENTIONS_DEBUGUTILS_H
#define YAVE_DEVICE_EXTENTIONS_DEBUGUTILS_H

#include <yave/graphics/vk/vk.h>

namespace yave {

class DebugUtils : NonCopyable {
	public:
		static const char* name();

		DebugUtils(VkInstance instance);
		~DebugUtils();

		void begin_region(VkCommandBuffer buffer, const char* name, const math::Vec4& color = math::Vec4()) const;
		void end_region(VkCommandBuffer buffer) const;

		void set_resource_name(DevicePtr dptr, u64 resource, const char* name) const;

	private:
		VkInstance _instance;
		VkDebugUtilsMessengerEXT _messenger = {};

		PFN_vkCmdBeginDebugUtilsLabelEXT _begin_label = nullptr;
		PFN_vkCmdEndDebugUtilsLabelEXT _end_label = nullptr;
		PFN_vkSetDebugUtilsObjectNameEXT _set_object_name = nullptr;

};

}

#endif // YAVE_DEVICE_EXTENTIONS_DEBUGUTILS_H
