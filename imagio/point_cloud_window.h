#pragma once

#ifndef IMAGIO_POINT_CLOUD_WINDOW
#define IMAGIO_POINT_CLOUD_WINDOW

#include <stddef.h>
#include <iostream>

#pragma warning(push, 0)       
#include <json/json.h>
#include "softkinetic/skv.h"
#pragma warning(pop)
#include "3dpaint/paint_window.h"
#include "imgui/imgui.h"

namespace imagio
{

class point_cloud_window : public wimgui::paint_window
{
private:
    Json::Value depth_stream;
    int current_frame = 0;
    int image_width = 320;
public:
	point_cloud_window(const char* _title) : paint_window(_title) {}
	void open_json(std::string filename);
	void open_skv_depth(std::string filename);
    void move_forward();
    void move_backward();
    void show_current_frame();
};

}
#endif
