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
	softkinetic::skv::file skv_file;
    int current_frame = 0;
    int image_width = 320;
    int image_height = 240;
	skv_pinhole_model pinhole_model;
	skv_distortion_model distortion_model;

	float brown_radial_lut_size = 400;
	float max_rad2_depth = 0.647376332409;
	float rad_step2_depth = max_rad2_depth / brown_radial_lut_size;
	std::map<int, float> k_values;
public:
	point_cloud_window(const char* _title) : paint_window(_title) {}
	void open_json(std::string filename);
	void open_skv_depth(std::string filename);
	void create_points_from_depth_image();
	void calculate_xyz_from_depth(float& x, float &y, float &z);
    void move_forward();
    void move_backward();
    void show_current_frame();

protected:
	void initialize_brown_radial();
};

}
#endif
