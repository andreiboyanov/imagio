#include "point_cloud_window.h"


namespace imagio
{

int point_cloud_window::get_k_key(float value)
{
	int key = int(std::round(value * 1000.0f));
	return key;
}

void point_cloud_window::initialize_brown_radial()
{
	k_values.clear();
	for(int i = 0; i < brown_radial_lut_size; i++)
	{
		float r2 = i * rad_step2_depth;
		float pr2 = 1 + r2 * (distortion_model.k1 + r2 * (distortion_model.k2 + r2 * distortion_model.k3));
		int k_key = get_k_key(r2 * pr2);
		k_values[k_key] = 1 / pr2;
	}
}

void point_cloud_window::calculate_xy_from_depth(float z, float* x, float* y)
{
	float x1 = (*x - pinhole_model.cx) / distortion_model.fx;
	float y1 = (pinhole_model.cy - *y) / distortion_model.fy;
	float r2 = x1 * x1 + y1 * y1;
	int k_key = get_k_key(r2);
	std::map<int, float>::iterator k_value = k_values.lower_bound(k_key);
	if(k_value != k_values.begin())
		--k_value;
	float k = (k_value)->second;

	*x = z * x1 * k;
	*y = z * y1 * k;
}

void point_cloud_window::create_points_from_depth_image()
{
	painter->init_scene();

	skv_error_code error_code;
	size_t byte_count = 0;
	error_code = skv_get_frame_byte_count(skv_handle, depth_id, current_frame, &byte_count, nullptr);
	
	std::vector<uint16_t> data(byte_count / sizeof(uint16_t));
	error_code = skv_get_frame_data(skv_handle, depth_id, current_frame, data.data(), nullptr);

	int current_point = 0;
	for(int image_x = 0; image_x < image_width; image_x++)
	{
		for(int image_y = 0; image_y < image_height; image_y++)
		{
			int depth_index = image_width * image_y + image_x;
			float z = (float)data[depth_index] / 1000.0f;
			if(z < 32.0f && z > 0.001f)
			{
				//calculate_xy_from_depth(z, &x, &y);

				glm::vec3 point;
				camera->transform2dto3d((float)image_x, (float)image_y, z, &point);
				current_point++;
				painter->draw_point(point, point_cloud_color, 1.0f);
			}
		}
	}
}

void point_cloud_window::open_skv_depth(std::string filename)
{
	try
	{
		skv_error_code error_code = skv_open_file(&skv_handle, filename.c_str(), skv_read_only, nullptr);
		if(error_code)
		{
			std::cout << "Could not open the skv file." << std::endl;
			return;
		}

		error_code = skv_get_device_info(skv_handle, &device_info, nullptr);
		error_code = skv_get_stream_id(skv_handle, "depth_0", &depth_id, nullptr);

		skv_stream_type depth_stream_type;
		error_code = skv_get_stream_type(skv_handle, depth_id, &depth_stream_type, nullptr);
		assert(depth_stream_type == skv_stream_type_image);

		error_code = skv_get_image_stream_info(skv_handle, depth_id, &depth_info, nullptr);
		assert(depth_info.type == skv_image_type_int16);


		bool has_pinhole_model = false;
		error_code = skv_has_pinhole_model(skv_handle, depth_id, &has_pinhole_model, nullptr);

		bool has_distortion_model = false;
		error_code = skv_has_distortion_model(skv_handle, depth_id, &has_distortion_model, nullptr);

		if(!(has_pinhole_model && has_distortion_model))
		{
			std::cout << "Pinhole and/or distortion models missing in the skv file.";
			std::cout << " They are needed for depth 3d points calculation. " << std::endl;
			return;
		}

		error_code = skv_get_stream_frame_count(skv_handle, depth_id, &frames_count, nullptr);
		
		image_width = depth_info.width;
		image_height = depth_info.height;
		error_code = skv_get_pinhole_model(skv_handle, depth_id, &pinhole_model, nullptr);
		error_code = skv_get_distortion_model(skv_handle, depth_id, &distortion_model, nullptr);
		//initialize_brown_radial();

		camera = new fisheye_camera(pinhole_model, distortion_model, image_width, image_height);
		camera->build_lookup_table();

		show_current_frame();
	}
	// FIXME: Move exception catches to the appropriate place
	catch(softkinetic::skv::skv_exception& e)
	{
		std::cout << "File error: " << e.get_message() << std::endl;
	}
}

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
		create_points_from_depth_image();
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
		auto joint_color = std::get<1>(joint);
		painter->draw_point(joint_position, joint_color, 10.0f);
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
	paint_window::draw();
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


void point_cloud_window::highlight_point(wimgui::vertex& vertex)
{
	vertex.size = 14.0f;
}

void point_cloud_window::unhighlight_point(wimgui::vertex& vertex)
{
	vertex.size = 1.0f;
}


void point_cloud_window::plot_graph(std::string label, const float* data, const unsigned int values_count, int start_index, int end_index)
{
	if(-1 == start_index) start_index = 0;
	if(-1 == end_index) end_index = values_count - 1;
	int items_count = end_index - start_index + 1;

	ImGuiWindow* window = get_imgui_window();
	const ImGuiStyle& style = GImGui->Style;
	const ImVec2 label_size = ImGui::CalcTextSize(label.c_str(), NULL, true);
	float graph_size_x = ImGui::CalcItemWidth();
	float graph_size_y = label_size.y + (style.FramePadding.y * 2);

	const ImRect graph_frame(window->DC.CursorPos, window->DC.CursorPos + ImVec2(graph_size_x, graph_size_y));
	const ImRect graph_inner_box(graph_frame.Min + style.FramePadding, graph_frame.Max - style.FramePadding);
	ImGui::RenderFrame(graph_inner_box.Min, graph_inner_box.Max, ImGui::GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

	static int value_index = -1;
	if(ImGui::IsHovered(graph_inner_box, 0))
	{
		const float t = ImClamp((GImGui->IO.MousePos.x - graph_inner_box.Min.x) / (graph_inner_box.Max.x - graph_inner_box.Min.x), 0.0f, 0.9999f);
		value_index = start_index + (int)(t * items_count - 1) % items_count;

		wimgui::vertex& vertex = painter->get_vertices()[value_index];
		highlight_point(vertex);
	}
	else if(value_index > -1)
	{
		wimgui::vertex& vertex = painter->get_vertices()[value_index];
		unhighlight_point(vertex);
	}
	ImGui::PlotLines(label.c_str(), &data[start_index], items_count);
}

}
