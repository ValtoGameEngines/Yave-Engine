/*******************************
Copyright (c) 2016-2017 Grégoire Angerand

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
#ifndef YAVE_BUFFER_SUBBUFFER_H
#define YAVE_BUFFER_SUBBUFFER_H

#include "SubBufferBase.h"
#include "Buffer.h"

namespace yave {


template<BufferUsage Usage>
class SubBuffer : public SubBufferBase {
	public:
		static constexpr BufferUsage usage = Usage;

		template<BufferUsage BufUsage, MemoryFlags Flags, BufferTransfer Transfer, typename = std::enable_if_t<(BufUsage & Usage) == Usage>>
		SubBuffer(const Buffer<BufUsage, Flags, Transfer>& buffer, usize offset, usize len) : SubBufferBase(buffer, offset, len) {
		}

		template<BufferUsage BufUsage, MemoryFlags Flags, BufferTransfer Transfer>
		explicit SubBuffer(const Buffer<BufUsage, Flags, Transfer>& buffer, usize offset = 0) : SubBuffer(buffer, offset, buffer.byte_size() - offset) {
		}
};


// find another name
template<BufferUsage Usage, MemoryFlags Flags = prefered_memory_flags(Usage), BufferTransfer Transfer = prefered_transfer(Flags)>
class SpecializedSubBuffer : public SubBuffer<Usage> {
	public:
		template<BufferUsage BufUsage, typename = std::enable_if_t<(BufUsage & Usage) == Usage>>
		SpecializedSubBuffer(const Buffer<BufUsage, Flags, Transfer>& buffer, usize offset, usize len) : SubBuffer<Usage>(buffer, offset, len) {
		}

		template<BufferUsage BufUsage>
		explicit SpecializedSubBuffer(const Buffer<BufUsage, Flags, Transfer>& buffer, usize offset = 0) : SubBuffer<Usage>(buffer, offset) {
		}
};


}

#endif // YAVE_BUFFER_SUBBUFFER_H
