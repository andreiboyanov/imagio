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
		//tracker.new_frame(painter->get_vertices(), painter->get_vertex_index());
		//show_joints(true);
	}
}

void point_cloud_window::show_joints(bool draw_forces)
{
	for(unsigned int joint_index = 0; joint_index < joints.size(); ++joint_index)
	{
		auto const &joint = joints[joint_index];
		auto joint_name = std::get<2>(joint);
		auto joint_position = std::get<0>(joint);
		// auto joint_color = std::get<1>(joint);
		// painter->draw_point(joint_position, joint_color, 10.0f);
		if(draw_forces)
		{
			auto force = tracker.get_force_k()[joint_index];
			glm::vec3 vector_end = joint_position + (force / glm::vec3(1000, 1000, 1000));
			//painter->draw_line(joint_position, vector_end, joint_color, 2.0f);
			std::cout << "Force for joint " << joint_name << " (" << vector_end.x << ", " << vector_end.y << ", " << vector_end.z << ")" << std::endl;
		}
	}
}

void point_cloud_window::draw()
{
	view3d::draw();
	//std::vector<std::vector<float>>& distances = tracker.get_distance();
	std::vector<std::vector<float>>& alpha_kn = tracker.get_alpha_kn();
	if(alpha_kn.size() >= joints.size())
	{
		std::vector<unsigned int> joint_indices = { 0, 1, 2, 5, 6 };
		for(auto& joint_index : joint_indices)
		{
			auto& joint = joints[joint_index];
			//const float *joint_distance = &distances[joint_index][0];
			const float *joint_alpha = &alpha_kn[joint_index][0];
			int points_count = alpha_kn[joint_index].size();
			//int start_index = (int)(points_count / 2) - 200;
			//int end_index = (int)(points_count / 2) + 200;
			plot_graph((std::get<2>(joint) + " alpha").c_str(), joint_alpha, points_count);
			//ImGui::PlotLines((std::get<2>(joint) + " alpha").c_str(), joint_alpha, points_count);
		}
	}
}


// void point_cloud_window::highlight_point(wimgui::vertex& vertex)
// {
// 	vertex.size = 14.0f;
// }

// void point_cloud_window::unhighlight_point(wimgui::vertex& vertex)
// {
// 	vertex.size = 1.0f;
// }


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

	static int value_index = -1;
	if(ImGui::IsHovered(graph_inner_box, 0))
	{
		const float t = ImClamp((GImGui->IO.MousePos.x - graph_inner_box.Min.x) / (graph_inner_box.Max.x - graph_inner_box.Min.x), 0.0f, 0.9999f);
		value_index = start_index + (int)(t * items_count - 1) % items_count;

		// wimgui::vertex& vertex = painter->get_vertices()[value_index];
		// highlight_point(vertex);
	}
	else if(value_index > -1)
	{
		// wimgui::vertex& vertex = painter->get_vertices()[value_index];
		// unhighlight_point(vertex);
	}
	ImGui::PlotLines(label.c_str(), &data[start_index], items_count);
}

}
