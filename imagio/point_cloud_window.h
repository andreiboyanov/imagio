#pragma once

#ifndef IMAGIO_POINT_CLOUD_WINDOW
#define IMAGIO_POINT_CLOUD_WINDOW

#include <stddef.h>
#include <iostream>
#include <map>

#pragma warning(push, 0)       
#include "softkinetic/skv.h"
#pragma warning(pop)
#include "3dpaint/paint_window.h"
#include "imgui/imgui.h"

namespace imagio
{

const ImColor red = ImColor(1.0f, 0.0f, 0.0f);
const ImColor blue = ImColor(0.0f, 0.0f, 1.0f);
const ImColor green = ImColor(0.0f, 1.0f, 0.0f);
const ImColor magenta = ImColor(1.0f, 0.0f, 1.0f);
const ImColor yellow = ImColor(1.0f, 1.0f, 0.0f);
const std::map<std::string, std::tuple<glm::vec3, const ImColor&>> first_frame_joints = {
	{"head",			{{ 0.049042556, 1.27974129,  0.391834855},		red}},
	{"left_shoulder",	{{ 0.223657891, 1.38038421,  0.186490983},		green}},
	{"right_shoulder",	{{-0.108562335, 1.39295137,  0.218086720},		green}},
	{ "left_forearm",	{{ 0.268882900, 1.27903306, -0.071093180},		blue}},
	{"right_forearm",	{{-0.174655050, 1.15550470,  0.084570530},		blue}},
	{"left_wrist",		{{ 0.226134900, 1.08374476, -0.203915060},		magenta}},
	{"right_wrist",		{{-0.121038109, 0.94242920,  0.183195367},		magenta}},
	{"hips",			{{ 0.049824394, 1.55311739, -0.190368563},		yellow}},
	{"left_hip",		{{ 0.162650300, 1.54967749, -0.191829562},		yellow}},
	{"right_hip",		{{-0.062769400, 1.55727637, -0.189216971},		yellow}},
	{"left_knee",		{{ 0.204919338, 1.28167045, -0.501541100},		yellow}},
	{"right_knee",		{{-0.113106430, 1.30452681, -0.509594142},		yellow}},
	{"left_foot",		{{ 0.227873027, 1.58939934, -0.756034400},		yellow}},
	{"right_foot",		{{-0.112991482, 1.60227287, -0.777440131},		yellow}},
};

class point_cloud_window : public wimgui::paint_window
{
private:
	softkinetic::skv::file skv_file;
    unsigned int current_frame = 0;
	unsigned int frames_count = 0;
    int image_width = 320;
    int image_height = 240;
	skv_pinhole_model pinhole_model;
	skv_distortion_model distortion_model;

	float brown_radial_lut_size = 400;
	float max_rad2_depth = 0.647376332409f;
	float rad_step2_depth = max_rad2_depth / brown_radial_lut_size;
	std::map<int, float> k_values;
	ImColor point_cloud_color = ImColor(1.0f, 1.0f, 0.0f);

public:
	point_cloud_window(const char* _title) : paint_window(_title) {}
	void open_skv_depth(std::string filename);
	void create_points_from_depth_image();
	void calculate_xyz_from_depth(float& x, float &y, float &z);
    void move_forward();
    void move_backward();
    void show_current_frame();
	void show_joints();
	unsigned int get_current_frame() { return current_frame; }
	unsigned int get_frames_count() { return frames_count; }

protected:
	void initialize_brown_radial();
	int get_k_key(float value);
};

}
#endif
