#pragma once

#ifndef IMAGIO_POINT_CLOUD_WINDOW
#define IMAGIO_POINT_CLOUD_WINDOW

#include <stddef.h>
#include <iostream>

#include "softkinetic/skv.h"
#include "wimgui/paint_window.h"

namespace imagio
{

class point_cloud_window : public wimgui::paint_window
{
public:
	point_cloud_window(const char* title) : paint_window(title) {}
	void open_skv_depth(std::string filename);

};

}
#endif