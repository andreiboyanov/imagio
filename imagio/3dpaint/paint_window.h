#pragma once

#ifndef WIMGUI_PAINT_WINDOW_H
#define WIMGUI_PAINT_WINDOW_H

#include <stddef.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

#include "../wimgui/window.h"
#include "3dpaint.h"

namespace wimgui {

class paint_window : public window
{
protected:
	painter3d* painter;

public:
	paint_window(const char* _title);
	painter3d* get_painter() { return painter; }
	void draw();
	void center();
	void view_top();
	void view_bottom();
	void view_left();
	void view_right();
	void view_front();
	void view_back();
	void init_scene();
};

}

#endif
