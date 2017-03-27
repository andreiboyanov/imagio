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

	 // if(depth_stream.has_pinhole_model())
	 // {
	 // 	skv_pinhole_model pinhole = depth_stream.get_pinhole_model();
	 // }
	 // if(depth_stream.has_distortion_model())
	 // {
	 // 	skv_distortion_model distortion = depth_stream.get_distortion_model();
	 // }
	 // if(depth_stream.has_stereo_transform())
	 // {
	 // 	skv_stereo_transform stereo = depth_stream.get_stereo_transform();
	 // }

		painter->init_scene();
		std::tuple<uint32_t, uint32_t> depth_resolution = depth_stream.get_resolution();
		int image_width = std::get<0>(depth_resolution);

		static int frame = 0;
		size_t byte_count = depth_stream.get_frame_byte_count(frame);
		std::vector<uint16_t> data(byte_count / sizeof(uint16_t));
		depth_stream.get_frame_data(frame, data);
		frame++;
		unsigned int count = 0;
		ImColor point_color = ImColor(1.0f, 1.0f, 1.0f);
		for(size_t i = 0; i < data.size() && count < painter->get_max_vertices(); i++)
		{
			float y = float(i / image_width);
			float x = float(i % image_width);
			float depth = float(data[i]);
			if(depth < 3000.0f)
			{
				painter->draw_point(x, -y, -depth, point_color);
				count++;
			}
		}
		std::cout << "Added " << count << " points of " << data.size() << " total." << std::endl;
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

}
