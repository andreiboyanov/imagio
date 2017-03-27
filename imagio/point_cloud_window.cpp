#include "point_cloud_window.h"


namespace imagio
{

void point_cloud_window::open_json(std::string filename)
{
    std::ifstream input(filename);
    input >> depth_stream;
    current_frame = 0;
    show_current_frame();
}

void point_cloud_window::open_skv_depth(std::string filename)
{
	try
	{
		auto file = softkinetic::skv::open_file(filename);

		std::string vendor_name, camera_model;
		std::tie(vendor_name, camera_model) = file.get_device_info();
		auto& stream = file.get_stream_by_name("depth_0");

		assert(stream.get_type() == skv_stream_type_image);
		assert(stream.get_image_type() == skv_image_type_int16);

	 // if(stream.has_pinhole_model())
	 // {
	 // 	skv_pinhole_model pinhole = stream.get_pinhole_model();
	 // }
	 // if(stream.has_distortion_model())
	 // {
	 // 	skv_distortion_model distortion = stream.get_distortion_model();
	 // }
	 // if(stream.has_stereo_transform())
	 // {
	 // 	skv_stereo_transform stereo = stream.get_stereo_transform();
	 // }

		painter->init_scene();
		std::tuple<uint32_t, uint32_t> depth_resolution = stream.get_resolution();
		image_width = std::get<0>(depth_resolution);

		size_t byte_count = stream.get_frame_byte_count(current_frame);
		std::vector<uint16_t> data(byte_count / sizeof(uint16_t));
		stream.get_frame_data(current_frame, data);
	}
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
    current_frame++;
    if (current_frame >= (int)depth_stream.size()) current_frame = depth_stream.size();
    show_current_frame();
}

void point_cloud_window::move_backward()
{
    current_frame--;
    if(current_frame < 0) current_frame = 0;
    show_current_frame();
}

void point_cloud_window::show_current_frame()
{
    std::cout << "Showing frame " << current_frame << std::endl;
	unsigned int count = 0;
	ImColor point_color = ImColor(1.0f, 1.0f, 1.0f);
	for(int i = 0; i < (int)depth_stream[current_frame].size() &&
                      count < painter->get_max_vertices(); i+=3)
	{
		float x = depth_stream[current_frame][i].asFloat();
		float y = depth_stream[current_frame][i + 1].asFloat();
		float z = depth_stream[current_frame][i + 2].asFloat();
		if(z > 0.0f && z < 3000.0f)
		{
			painter->draw_point(x, y, -z, point_color);
			count++;
		}
	}
	std::cout << "Added " << count << " points of " << depth_stream[current_frame].size() << " total." << std::endl;
}

}
