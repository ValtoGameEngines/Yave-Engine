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
#ifndef EDITOR_WIDGETS_WIDGET_H
#define EDITOR_WIDGETS_WIDGET_H

#include <editor.h>

#include <yave/swapchain/FrameToken.h>

namespace editor {

class Widget : NonCopyable {
	public:
		enum Flags {
			NoFlags,
			NoWindow
		};

		Widget(const char* title, Flags flags = NoFlags);
		virtual ~Widget();

		void paint();
		void show();

		bool is_visible() const;

		const char* title() const;

	protected:
		virtual void paint_ui() = 0;


	private:
		const char* _title;
		Flags _flags;
		bool _visible = true;

};

}

#endif // EDITOR_WIDGETS_WIDGET_H