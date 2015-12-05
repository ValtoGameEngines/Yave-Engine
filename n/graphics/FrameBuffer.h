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

#ifndef N_GRAPHICS_FRAMEBUFFER_H
#define N_GRAPHICS_FRAMEBUFFER_H

#include <n/utils.h>
#include "TextureArray.h"
#include "GL.h"

namespace n {
namespace graphics {

class FrameBuffer : NonCopyable
{
	void assertAttachements(uint index) {
		if(index >= getMaxAttachment()) {
			fatal("Too many framebuffer attachments.");
		}
	}

	template<typename T, typename U>
	void setupAttachments(uint index, T t, U u) {
		setupAttachment(index, t);
		setupAttachment(index + 1, u);
	}

	template<typename T, typename... Args>
	void setupAttachments(uint index, T t, Args... args) {
		setupAttachment(index, t);
		setupAttachments(index + 1, args...);
	}

	void setupAttachments(uint index) {
		assertAttachements(index);
	}

	void setupAttachment(uint index, bool enabled) {
		assertAttachements(index);
		drawBuffers[index] = enabled ? gl::Attachment(gl::ColorAtt0 + index) : gl::NoAtt;
	}

	void setupAttachment(uint index, ImageFormat::Format format) {
		assertAttachements(index);
		drawBuffers[index] = gl::Attachment(gl::ColorAtt0 + index);
		attachments[index] = Image(getSize(), format);
	}

	public:
		static constexpr uint Depth = -1;

		static uint getMaxAttachment() {
			return GLContext::getContext()->getHWInt(GLContext::MaxFboAttachements);
		}

		~FrameBuffer();

		bool isAttachmentEnabled(uint slot) const;
		bool isDepthEnabled() const;

		bool isActive() const;

		void bind() const;
		static void clear(bool color, bool depth);
		static void unbind();

		void blit(uint slot = 0, bool depth = false) const;

		math::Vec2ui getSize() const {
			return base;
		}

		Texture getAttachement(uint slot) const {
			return slot == Depth ? getDepthAttachement() : attachments[slot];
		}

		TextureArray asTextureArray() const {
			if(attArray) {
				return *attArray;
			}
			return TextureArray();
		}

		Texture getDepthAttachement() const {
			return depth ? *depth : Texture();
		}

		template<typename... Args>
		FrameBuffer(const math::Vec2ui &s, bool depthEnabled, Args... args): base(s), attachments(new Texture[getMaxAttachment()]), attArray(0), drawBuffers(new gl::Attachment[getMaxAttachment()]) {
			Image baseImage(base);
			for(uint i = 0; i != getMaxAttachment(); i++) {
				drawBuffers[i] = gl::NoAtt;
				attachments[i] = Texture(baseImage);
			}
			depth = depthEnabled ? new Texture(Image(base, ImageFormat::Depth32)) : 0;
			setupAttachments(0, args...);
			setup();
		}

	private:
		friend class GLContext;
		friend class FrameBufferPool;

		void setup();

		math::Vec2ui base;
		Texture *attachments;
		Texture *depth;
		TextureArray *attArray;
		gl::Attachment *drawBuffers;
		gl::Handle handle;
};

}
}

#endif // N_GRAPHICS_FRAMEBUFFER_H
