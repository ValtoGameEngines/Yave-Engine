/*******************************
Copyright (C) 2013-2015 gregoire ANGERAND

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
**********************************/

#ifndef N_GRAPHICS_SHADER
#define N_GRAPHICS_SHADER

#include <n/core/String.h>
#include "GL.h"

namespace n {
namespace graphics {

class ShaderCombinaison;

namespace internal {

class ShaderBase : core::NonCopyable
{
	public:
		const core::String &getLogs() const {
			return logs;
		}

		uint getVersion() const {
			return version;
		}

		bool isValid() {
			return handle && version;
		}

	protected:
		friend class graphics::ShaderCombinaison;

		ShaderBase() : handle(0) {
		}

		~ShaderBase() {
			if(handle) {
				GLContext::getContext()->addGLTask([=]() { gl::glDeleteShader(handle); });
			}
		}

		gl::GLuint handle;
		uint version;
		core::String logs;
};

}

enum ShaderType
{
	FragmentShader = 0,
	VertexShader = 1,
	GeometryShader = 2
};

template<ShaderType Type>
class Shader : public internal::ShaderBase
{
	public:
		Shader(const core::String &src, uint vers = 420) : internal::ShaderBase() {
			version = load(src, vers);
		}

		~Shader() {
		}

	private:
		uint load(core::String src, uint vers) {
			core::String libs[] = {
					"vec4 n_gbuffer0(vec4 color, vec3 normal, float roughness, float metal) {"
						"return color;"
					"}"
					"vec4 n_gbuffer1(vec4 color, vec3 normal, float roughness, float metal) {"
						"return vec4(normalize(normal) * 0.5 + 0.5, roughness);"
					"}",
					"",
					""
			};
			uint vit = src.find("#version");
			if(vit != -1u) {
				uint l = src.find("\n", vit);
				if(l != -1u) {
					bool ok = true;
					uint v = src.subString(vit + 9).get<uint>([&]() { ok = false; });
					if(ok && v) {
						vers = v;
					} else {
						logs += "Unable to parse #version.\n";
					}
				}
				src.replace("#version", "//#version");
			}
			gl::GLenum glType[] = {GL_FRAGMENT_SHADER, GL_VERTEX_SHADER, GL_GEOMETRY_SHADER};
			handle = gl::glCreateShader(glType[Type]);
			core::String ver = core::String("#version ") + vers + "\n";
			const char *strs[] = {ver.toChar(), libs[Type].toChar(), src.toChar()};
			gl::glShaderSource(handle, 3, strs, 0);
			gl::glCompileShader(handle);
			int res = 0;
			gl::glGetShaderiv(handle, GL_COMPILE_STATUS, &res);
			if(!res) {
				int size = 0;
				gl::glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &size);
				char *msg = new char[size + 1];
				gl::glGetShaderInfoLog(handle, size, &size, msg);
				gl::glDeleteShader(handle);
				handle = 0;
				msg[size] = '\0';
				logs += msg;
				delete[] msg;
				return 0;
			}
			return vers;
		}

};


}
}

#endif // N_GRAPHICS_SHADER

