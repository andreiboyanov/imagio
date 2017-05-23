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

void point_cloud_window::calculate_xyz_from_depth(float& x, float& y, float& z)
{
	float x1 = (x - pinhole_model.cx) / distortion_model.fx;
	float y1 = (pinhole_model.cy - y) / distortion_model.fy;
	float r2 = x1 * x1 + y1 * y1;
	int k_key = get_k_key(r2);
	std::map<int, float>::iterator k_value = k_values.lower_bound(k_key);
	if(k_value != k_values.begin())
		--k_value;
	float k = (k_value)->second;

	x = z * x1 * k;
	y = z * y1 * k;
	z = z;
}

void point_cloud_window::create_points_from_depth_image()
{
	auto stream = skv_file.get_stream_by_name("depth_0");

	assert(stream.get_type() == skv_stream_type_image);
	assert(stream.get_image_type() == skv_image_type_int16);
	if(!(stream.has_pinhole_model() && stream.has_distortion_model()))
	{
		std::cout << "Pinhole and/or distortion models missing in the skv file.";
		std::cout << " They are needed for depth 3d points calculation. " << std::endl;
		return;
	}

	std::tuple<uint32_t, uint32_t> depth_resolution = stream.get_resolution();
	image_width = std::get<0>(depth_resolution);
	image_height = std::get<1>(depth_resolution);
	pinhole_model = stream.get_pinhole_model();
	distortion_model = stream.get_distortion_model();
	initialize_brown_radial();

	painter->init_scene();

	size_t byte_count = stream.get_frame_byte_count(current_frame);
	std::vector<uint16_t> data(byte_count / sizeof(uint16_t));
	stream.get_frame_data(current_frame, data);

	for(int image_x = 0; image_x < image_width; image_x++)
	{
		for(int image_y = 0; image_y < image_height; image_y++)
		{
			int depth_index = image_width * image_y + image_x;
			float x = (float)image_x, y = (float)image_y, z = (float)data[depth_index] / 1000.0f;
			if(z < 32.0f && z > 0.001f)
			{
				calculate_xyz_from_depth(x, y, z);
				painter->draw_point(x, y, z, point_cloud_color, 1.0f);
			}
		}
	}
	tracker.new_frame(painter->get_vertices(), painter->get_vertex_index());
}

void point_cloud_window::open_skv_depth(std::string filename)
{
	try
	{
		skv_file = softkinetic::skv::open_file(filename);

		std::string vendor_name, camera_model;
		std::tie(vendor_name, camera_model) = skv_file.get_device_info();

		auto stream = skv_file.get_stream_by_name("depth_0");
		frames_count = stream.get_frame_count();
		show_current_frame();
	}
	// FIXME: Move exception catches to the appropriate place
	catch(softkinetic::skv::file_error& e)
	{
		std::cout << "File error: " << e.get_message() << std::endl;
	}
	catch(softkinetic::skv::stream_error& e)
	{
		std::cout << "Stream error in stream " << e.get_stream_id() << ": " << e.get_message() << std::endl;
	}
	catch(softkinetic::skv::custom_buffer_error& e)
	{
		std::cout << "Custom buffer error in buffer " << e.get_custom_buffer_name() << ": " << e.get_message() << std::endl;
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
		show_joints();
	}
}

void point_cloud_window::show_joints()
{
	for(auto const &joint : joints)
	{
		auto joint_name = std::get<2>(joint);
		auto joint_position = std::get<0>(joint);
		auto joint_color = std::get<1>(joint);
		painter->draw_point(joint_position.x, joint_position.y, joint_position.z,
							joint_color, 10.0f);
	}
}

void point_cloud_window::draw()
{
	paint_window::draw();
	std::vector<std::vector<float>>& distances = tracker.get_distance();
	//std::vector<std::vector<float>>& alfa_nk = tracker.get_alfa_nk();
	if(distances.size() >= joints.size())
	{
		std::vector<unsigned int> joint_indices = { 0, 1, 2, 3, 4, 5, 6 };
		for(auto& joint_index : joint_indices)
		{
			auto& joint = joints[joint_index];
			const float *joint_distance = &distances[joint_index][0];
			//const float *joint_alfa = &alfa_nk[joint_index][0];
			int points_count = distances[joint_index].size();
			int start_index = (int)(points_count / 2) - 200;
			int end_index = (int)(points_count / 2) + 200;
			plot_graph((std::get<2>(joint) + " distance").c_str(), joint_distance, points_count, start_index, end_index);
			//ImGui::PlotLines((std::get<2>(joint) + " alfa").c_str(), joint_alfa, points_count);
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
	else if (value_index > -1)
	{
		wimgui::vertex& vertex = painter->get_vertices()[value_index];
		unhighlight_point(vertex);
	}
	ImGui::PlotLines(label.c_str(), &data[start_index], items_count);
	if(value_index > -1)
	{
		wimgui::vertex& vertex = painter->get_vertices()[value_index];
		ImGui::Text("vertext number = %d; x = %8.4g; y = %8.4g; z = %8.4g", value_index, vertex.position_x, vertex.position_y, vertex.position_z);
		glm::vec3 right_wrist = std::get<0>(joints[6]);
		glm::vec3 head = std::get<0>(joints[0]);
		float wrist_distance = std::sqrt(std::pow(vertex.position_x - right_wrist.x, 2) + std::pow(vertex.position_y - right_wrist.y, 2) + std::pow(vertex.position_z - right_wrist.z, 2));
		float head_distance = std::sqrt(std::pow(vertex.position_x - head.x, 2) + std::pow(vertex.position_y - head.y, 2) + std::pow(vertex.position_z - head.z, 2));
		ImGui::Text("right wrist distance = %8.4g; head distance = %8.4g", wrist_distance, head_distance);
	}
}

}
