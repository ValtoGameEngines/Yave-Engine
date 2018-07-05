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
#ifndef EDITOR_VIEWS_FILEBROWSER_H
#define EDITOR_VIEWS_FILEBROWSER_H

#include <editor/ui/Frame.h>
#include <editor/ui/Widget.h>

#include <yave/serialize/filesystem.h>

#include <y/core/Functor.h>

namespace editor {

class FileBrowser : public Widget {
	public:
		FileBrowser();

		template<typename F>
		void set_callback(F&& func) {
			_callback = std::forward<F>(func);
		}

	private:
		void done();
		void cancel();

		void set_path(const fs::path& path);
		void input_path();

		void paint_ui(CmdBufferRecorder<>&, const FrameToken&) override;

		fs::path _current;

		static constexpr usize buffer_size = 512;
		std::array<char, buffer_size> _path_buffer;
		std::array<char, buffer_size> _name_buffer;

		int _selection = -1;

		core::Function<void(core::String)> _callback;

};

}

#endif // EDITOR_VIEWS_FILEBROWSER_H