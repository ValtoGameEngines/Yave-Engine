/*******************************
Copyright (c) 2016-2017 Gr�goire Angerand

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
#ifndef YAVE_PIPELINE_PIPELINE_H
#define YAVE_PIPELINE_PIPELINE_H

#include <yave/commands/RecordedCmdBuffer.h>
#include <yave/swapchain/FrameToken.h>

namespace yave {

namespace detail {
struct NodeData;
}

class DependencyGraphNode;


class NodeBase : NonCopyable {
	public:
		virtual ~NodeBase() {
		}

	protected:
		friend class DependencyGraphNode;

		virtual void compute_dependencies(DependencyGraphNode&) = 0;
};

class Node : public NodeBase {
	protected:
		friend class detail::NodeData;
		virtual void process(const FrameToken&) = 0;
};

class SecondaryRenderer : public NodeBase {
	protected:
		friend class detail::NodeData;
		virtual void process(const FrameToken&, CmdBufferRecorder<CmdBufferUsage::Secondary>&&) = 0;
};

class Renderer : public NodeBase {
	protected:
		friend class detail::NodeData;

#warning Renderer does not need to store its own result while other nodes do
		virtual void process(const FrameToken&, CmdBufferRecorder<>&) = 0;
};

}

#endif // YAVE_PIPELINE_PIPELINE_H