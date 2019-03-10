#include "point_cloud_window.h"


namespace imagio
{

void point_cloud_window::move_forward()
{
	if(current_frame < frames_count - 1)
	{
		current_frame++;
		show_current_frame();
	}
}

void point_cloud_window::move_backward()
{
	if(current_frame > 0)
	{
		current_frame--;
		show_current_frame();
	}
}

void point_cloud_window::show_current_frame()
{
	if(frames_count > 0)
	{
	}
}

void point_cloud_window::show_joints(bool draw_forces)
{
	for(unsigned int joint_index = 0; joint_index < joints.size(); ++joint_index)
	{
	}
}

void point_cloud_window::draw()
{
	view3d::draw();
}


void point_cloud_window::plot_graph(std::string label, const float* data, const unsigned int values_count, int start_index, int end_index)
{
	if(-1 == start_index) start_index = 0;
	if(-1 == end_index) end_index = values_count - 1;
	int items_count = end_index - start_index + 1;

	ImGuiWindow* imgui_window = get_imgui_window();
	const ImGuiStyle& style = GImGui->Style;
	const ImVec2 label_size = ImGui::CalcTextSize(label.c_str(), NULL, true);
	float graph_size_x = ImGui::CalcItemWidth();
	float graph_size_y = label_size.y + (style.FramePadding.y * 2);

	const ImRect graph_frame(imgui_window->DC.CursorPos, imgui_window->DC.CursorPos + ImVec2(graph_size_x, graph_size_y));
	const ImRect graph_inner_box(graph_frame.Min + style.FramePadding, graph_frame.Max - style.FramePadding);
	ImGui::RenderFrame(graph_inner_box.Min, graph_inner_box.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

	ImGui::PlotLines(label.c_str(), &data[start_index], items_count);
}

}
