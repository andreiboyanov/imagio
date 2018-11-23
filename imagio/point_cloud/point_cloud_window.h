#pragma once

#ifndef IMAGIO_POINT_CLOUD_WINDOW
#define IMAGIO_POINT_CLOUD_WINDOW

#include <stddef.h>
#include <iostream>
#include <map>

#pragma warning(push, 0)       
#include "softkinetic/skv.h"
#pragma warning(pop)
#include "../3dpaint/paint_window.h"
#include "../imgui/imgui.h"
#include "../point_cloud/joint_tracker.h"

#include "camera_model.h"

namespace imagio
{

const ImColor color_000 = ImColor(0.0f, 0.0f, 0.0f);
const ImColor color_001 = ImColor(0.0f, 0.0f, 1.0f);
const ImColor color_010 = ImColor(0.0f, 1.0f, 0.0f);
const ImColor color_011 = ImColor(0.0f, 1.0f, 1.0f);
const ImColor color_100 = ImColor(1.0f, 0.0f, 0.0f);
const ImColor color_101 = ImColor(1.0f, 0.0f, 1.0f);
const ImColor color_110 = ImColor(1.0f, 1.0f, 0.0f);
const ImColor color_111 = ImColor(1.0f, 1.0f, 1.0f);

const std::vector<std::tuple<glm::vec3, const ImColor&, std::string>> joints = {
	{{ 0.049042556,  0.391834855, 1.27974129},		color_001,		"head"},
	{{ 0.223657891,  0.186490983, 1.38038421},		color_010,		"left_shoulder"},
	{{-0.108562335,  0.218086720, 1.39295137},		color_100,		"right_shoulder"},
	{{ 0.268882900, -0.071093180, 1.27903306},		color_011,		"left_forearm"},
	{{-0.174655050,  0.084570530, 1.15550470},		color_101,		"right_forearm"},
	{{ 0.226134900, -0.203915060, 1.08374476},		color_110,		"left_wrist"},
	{{-0.121038109,  0.183195367, 0.94242920},		color_100,		"right_wrist"},
	{{ 0.049824394, -0.190368563, 1.55311739},		color_000,		"hips"},
	{{ 0.162650300, -0.191829562, 1.54967749},		color_000,		"left_hip"},
	{{-0.062769400, -0.189216971, 1.55727637},		color_000,		"right_hip"},
	{{ 0.204919338, -0.501541100, 1.28167045},		color_100,		"left_knee"},
	{{-0.113106430, -0.509594142, 1.30452681},		color_010,		"right_knee"},
	{{ 0.227873027, -0.756034400, 1.58939934},		color_000,		"left_foot"},
	{{-0.112991482, -0.777440131, 1.60227287},		color_000,		"right_foot"},
};

class point_cloud_window : public wimgui::paint_window
{
private:
	skv_handle* skv_handle;
	skv_device_info device_info;
	uint32_t depth_id;
	skv_image_stream_info depth_info;
	uint32_t frames_count;

	unsigned int current_frame = 0;
    int image_width = 320;
    int image_height = 240;
	skv_pinhole_model pinhole_model;
	skv_distortion_model distortion_model;

	fisheye_camera* camera;

	float brown_radial_lut_size = 400;
	float max_rad2_depth = 0.647376332409f;
	float rad_step2_depth = max_rad2_depth / brown_radial_lut_size;
	std::map<int, float> k_values;
	ImColor point_cloud_color = ImColor(1.0f, 0.0f, 0.5f, 0.5f);

	joint_tracker tracker;

public:
	point_cloud_window(const char* _title) : paint_window(_title), tracker(joints) {}
	void open_skv_depth(std::string filename);
	void create_points_from_depth_image();
	void calculate_xy_from_depth(float z, float* x, float* y);
    void move_forward();
    void move_backward();
    void show_current_frame();
	void show_joints(bool draw_forces = false);
	unsigned int get_current_frame() { return current_frame; }
	unsigned int get_frames_count() { return frames_count; }
	virtual void point_cloud_window::draw();

protected:
	void initialize_brown_radial();
	int get_k_key(float value);
	void plot_graph(std::string label, const float* data, const unsigned int values_count, int start_index=-1, int end_index=-1);
	void highlight_point(wimgui::vertex& vertex);
	void unhighlight_point(wimgui::vertex& vertex);
};

}
#endif
