#pragma once

#ifndef IMAGIO_POINT_CLOUD_WINDOW
#define IMAGIO_POINT_CLOUD_WINDOW

#include <stddef.h>
#include <iostream>

#pragma warning(push, 0)       
#include "softkinetic/skv.h"
#pragma warning(pop)
#include "3dpaint/paint_window.h"
#include "imgui/imgui.h"

namespace imagio
{

class point_cloud_window : public wimgui::paint_window
{
public:
	point_cloud_window(const char* _title) : paint_window(_title) {}
	void open_skv_depth(std::string filename);
};

}
#endif
