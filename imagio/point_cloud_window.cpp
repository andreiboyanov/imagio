#include "point_cloud_window.h"


namespace imagio
{

void point_cloud_window::open_skv_depth(std::string filename)
{
	try
	{
		auto file = softkinetic::skv::open_file(filename);

		std::string vendor_name, camera_model;
		std::tie(vendor_name, camera_model) = file.get_device_info();
		auto& depth_stream = file.get_stream_by_name("depth_0");

		assert(depth_stream.get_type() == skv_stream_type_image);
		assert(depth_stream.get_image_type() == skv_image_type_int16);

		if (depth_stream.has_pinhole_model())
		{
			skv_pinhole_model pinhole = depth_stream.get_pinhole_model();
		}
		if (depth_stream.has_distortion_model())
		{
			skv_distortion_model distortion = depth_stream.get_distortion_model();
		}
		if (depth_stream.has_stereo_transform())
		{
			skv_stereo_transform stereo = depth_stream.get_stereo_transform();
		}

		size_t frame_count = depth_stream.get_frame_count();
		//for (int j = 0; j < frame_count; ++j)
		//{
		//	size_t byte_count = depth_stream.get_frame_byte_count(j);
		//	std::vector<uint16_t> data(byte_count / sizeof(uint16_t));
		//	depth_stream.get_frame_data(j, data);
		//}
		painter->init_scene();
		int image_width = 320;
		int image_height = 240;
		size_t byte_count = depth_stream.get_frame_byte_count(0);
		std::vector<uint16_t> data(byte_count / sizeof(uint16_t));
		depth_stream.get_frame_data(0, data);
		int count = 0;
		for (int i = 0; i < data.size() && count < 100; i+=50)
		{
			float y = float(i / image_width);
			float x = float(i % image_width);
			float depth = float(data[i]);
			if (depth > 0.0f && depth < 10000.0f)
			{
				painter->draw_point(Vector3f(x, y, -depth), ImColor(1.0f, 1.0f, 1.0f));
				count++;
			}
		}
		std::cout << "Added " << count << "points." << std::endl;
	}
	catch (softkinetic::skv::file_error& e)
	{
		std::cout << "File error: " << e.get_message() << std::endl;
	}
	catch (softkinetic::skv::stream_error& e)
	{
		std::cout << "Stream error in stream " << e.get_stream_id() << ": " << e.get_message() << std::endl;
	}
	catch (softkinetic::skv::custom_buffer_error& e)
	{
		std::cout << "Custom buffer error in buffer " << e.get_custom_buffer_name() << ": " << e.get_message() << std::endl;
	}
}

}