#pragma once

#ifndef IMAGIO_POINT_CLOUD_WINDOW
#define IMAGIO_POINT_CLOUD_WINDOW

#include <stddef.h>
#include <iostream>
#include <map>

#pragma warning(push, 0)       
#pragma warning(pop)
#include "../viewers/3d/view3d.h"
#include "../viewers/3d/pointcloud_painter.h"
#include "../imgui/imgui.h"
#include "../point_cloud/joint_tracker.h"

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


class point_cloud_window : public wimgui::view3d
{
private:

	uint32_t frames_count;

	unsigned int current_frame = 0;
	std::map<int, float> k_values;
	ImColor point_cloud_color = ImColor(1.0f, 0.0f, 0.5f, 0.5f);
	joint_tracker tracker;
	wimgui::pointcloud_painter points;
	wimgui::vertex_array_type point_cloud;
	std::shared_ptr<wimgui::vertex_array_type> point_cloud_pointer;

public:
	point_cloud_window(const char* _title) : view3d(_title), tracker(joints)
	{
		add_painter(&points);
		for (float x = 0.0f; x <= 1.0f; x += 0.1f)
		{
			for (float y = 0.0f; y <= 1.0f; y += 0.1f)
			{
				for (float z = 0.0f; z <= 1.0f; z += 0.1f)
				{
					point_cloud.push_back({ x, y, z });
				}
			}
		}
		point_cloud_pointer = std::make_shared<wimgui::vertex_array_type>(point_cloud);
		points.set_data(point_cloud_pointer);
	}
    void move_forward();
    void move_backward();
    void show_current_frame();
	void show_joints(bool draw_forces = false);
	unsigned int get_current_frame() { return current_frame; }
	unsigned int get_frames_count() { return frames_count; }
	virtual void draw();

protected:
	void plot_graph(std::string label, const float* data, const unsigned int values_count, int start_index=-1, int end_index=-1);
};

}
#endif
