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

#ifndef N_GRAPHICS_VERTEXARRAYOBJECT
#define N_GRAPHICS_VERTEXARRAYOBJECT

#include "TriangleBuffer.h"
#include "StaticBuffer.h"
#include "VertexAttribs.h"
#include "Material.h"

namespace n {
namespace graphics {

template<typename T = float>
class VertexArrayObject : core::NonCopyable
{
	public:
		VertexArrayObject(const typename TriangleBuffer<T>::FreezedTriangleBuffer &tr) : radius(tr.radius),
			size(tr.indexes.size() % 3 ? uint(fatal("Invalid non-trianglulated mesh.")) : tr.indexes.size() / 3),
			data(tr.vertices), indexes(tr.indexes), handle(0) {
		}

		~VertexArrayObject() {
			if(handle) {
				gl::GLuint h = handle;
				GLContext::getContext()->addGLTask([=]() {
					gl::glDeleteVertexArrays(1, &h);
				});
			}
		}

		void draw(const Material &mat, const VertexAttribs &attributes = VertexAttribs(), uint renderFlags = RenderFlag::None, uint instances = 1) const {
			mat.bind(renderFlags);
			bind();
			bindAttribs(attributes);
			gl::glDrawElementsInstanced(GL_TRIANGLES, 3 * size, GLType<uint>::value, 0, instances);
		}

		T getRadius() const {
			return radius;
		}

	private:
		static uint maxAttribs() {
			return GLContext::getContext()->getHWInt(GLContext::MaxVertexAttribs) - 4;
		}

		void bindAttribs(const VertexAttribs &att) const {
			if(att.attribs.isEmpty()) {
				return;
			}
			fatal("Attribs not supported");
		}

		void bind() const {
			if(!handle) {
				data.bind();
				gl::glGenVertexArrays(1, &handle);
				gl::glBindVertexArray(handle);
				gl::glVertexAttribPointer(0, 3, GLType<T>::value, GL_FALSE, sizeof(Vertex<T>), 0);
				gl::glVertexAttribPointer(1, 3, GLType<T>::value, GL_FALSE, sizeof(Vertex<T>), (void *)(2 * sizeof(T) + 3 * sizeof(T)));
				gl::glVertexAttribPointer(2, 3, GLType<T>::value, GL_FALSE, sizeof(Vertex<T>), (void *)(2 * sizeof(T) + 2 * 3 * sizeof(T)));
				gl::glVertexAttribPointer(3, 2, GLType<T>::value, GL_FALSE, sizeof(Vertex<T>), (void *)(3 * sizeof(T)));
				gl::glEnableVertexAttribArray(0);
				gl::glEnableVertexAttribArray(1);
				gl::glEnableVertexAttribArray(2);
				gl::glEnableVertexAttribArray(3);
			} else {
				gl::glBindVertexArray(handle);
			}
			indexes.bind();
		}


		T radius;
		uint size;
		StaticBuffer<Vertex<T>, Array> data;
		StaticBuffer<uint, Index> indexes;

		mutable gl::GLuint handle;
};


}
}

#endif // N_GRAPHICS_VERTEXARRAYOBJECT

