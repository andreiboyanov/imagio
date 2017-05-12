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
	for(auto const &joint : first_frame_joints)
	{
		auto joint_name = joint.first;
		auto joint_position = std::get<0>(joint.second);
		auto joint_color = std::get<1>(joint.second);
		painter->draw_point(joint_position.x, joint_position.z, joint_position.y,
							joint_color, 10.0f);
	}
}

}
