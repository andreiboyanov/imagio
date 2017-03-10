#pragma once

#ifndef WIMGUI_PAINT_WINDOW_H
#define WIMGUI_PAINT_WINDOW_H

#include <stddef.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

#include "window.h"
#include "3dpaint.h"

namespace wimgui {

class paint_window : public window
{
private:
	wimgui::painter3d* painter;

public:
	paint_window(const char* _title);
	void draw();
};

}

#endif
