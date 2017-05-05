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


//def build_2d_to_3d_matrix(self) :
//	distortion_model = self.camera_configuration.distortion_model
//	pinhole_model = self.camera_configuration.pinhole_model
//	for x in range(0, self.stream_configuration.width) :
//		for y in range(0, self.stream_configuration.height) :
//			x1 = (x - pinhole_model.cx) / distortion_model.fx
//			y1 = (pinhole_model.cy - y) / distortion_model.fy
//
//			r2 = x1 * x1 + y1 * y1
//
//			k = self.get_nearest_less_k(r2)
//
//			elementValues = []
//			elementValues.append(x1 * k / 1000)
//			elementValues.append(1 / 1000)
//			elementValues.append(y1 * k / 1000)
//
//			self.brownRadial2Dto3DMatrix[y, x] = np.array(elementValues)
//
//def build_brown_radial_table(self) :
//			self.kValues = self.buildKValues()
//			self.brownRadial2Dto3DMatrix = np.empty((self.stream_configuration.height,
//													 self.stream_configuration.width, 3))
//			self.build_2d_to_3d_matrix()
//
//def get_nearest_less_k(self, key) :
//
//			key = self.create_dict_key(key)
//
//			index = self.kValues.bisect(key)
//			if index == self.brown_radial_lut_size :
//				index = index - 1
//				key = self.kValues.iloc[index]
//				return self.kValues[key]
//
//def create_dict_key(self, value) :
//				return int(round(value, 3) * 1000)
//
//def transform2dTo3dBrownRadial(self, x, y, depth) :
//				pixelValues = self.brown_radial_lut[x, y]
//				x1 = pixelValues[0]
//				y1 = pixelValues[1]
//				k = pixelValues[2]

//
//				x1 = x1 * k
//				y1 = y1 * k
//
//				return np.array([x1 * depth, y1 * depth, depth])

int point_cloud_window::get_k_key(float value)
{
	int key = int(value * 1000);
	return key;
}

void point_cloud_window::initialize_brown_radial()
{
	k_values.clear();
	for(int i = 0; i < brown_radial_lut_size; i++)
	{
		float r2 = i * rad_step2_depth;
		float pr2 = 1 + r2 * (distortion_model.k1 + r2 * (distortion_model.k2 + r2 * distortion_model.k3));
		int key = get_k_key(r2 * pr2);
		k_values[key] = 1 / pr2;
	}
}

void point_cloud_window::calculate_xyz_from_depth(float& x, float& y, float& z)
{
	float x1 = (x - pinhole_model.cx) / distortion_model.fx;
	float y1 = (pinhole_model.cy - y) / distortion_model.fy;
	float r2 = x1 * x1 * y1 * y1;
	std::map<int, float>::iterator k_value = k_values.lower_bound(get_k_key(r2));
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
		std::cout << " They are needed for depth 3d points calkculation. " << std::endl;
		return;
	}

	std::tuple<uint32_t, uint32_t> depth_resolution = stream.get_resolution();
	image_width = std::get<0>(depth_resolution);
	image_height = std::get<1>(depth_resolution);
	pinhole_model = stream.get_pinhole_model();
	distortion_model = stream.get_distortion_model();
	initialize_brown_radial();

	painter->init_scene();

	std::vector<std::vector<float>> repers = {
		{0.0f, 0.0f, 0.5f},
		{320.0f, 0.0f, 0.5f},
		{320.0f, 240.0f, 0.5f},
		{0.0f, 240.0f, 0.5f},
		{0.0f, 0.0f, 1.0f},
		{320.0f, 0.0f, 1.0f},
		{320.0f, 240.0f, 1.0f},
		{0.0f, 240.0f, 1.0f},
		{0.0f, 0.0f, 1.5f},
		{320.0f, 0.0f, 1.5f},
		{320.0f, 240.0f, 1.5f},
		{0.0f, 240.0f, 1.5f},
	};
	ImColor reper_color(1.0f, 0.0f, 0.0f);
	ImColor depth_reper_color(0.0f, 1.0f, 0.0f);
	for(auto& reper_point : repers)
	{
		float x = reper_point[0], y = reper_point[1], z = reper_point[2];
		painter->draw_point(x / 320.0f, y / 240.0f, z, reper_color, 10.0f);
		calculate_xyz_from_depth(x, y, z);
		painter->draw_point(y, -x, z, depth_reper_color, 5.0f);
	}

	size_t byte_count = stream.get_frame_byte_count(current_frame);
	std::vector<uint16_t> data(byte_count / sizeof(uint16_t));
	stream.get_frame_data(current_frame, data);

	for(int image_x = 0; image_x < image_width; image_x++)
	{
		for(int image_y = 0; image_y < image_height; image_y++)
		{
			int depth_index = image_height * image_x + image_y;
			float x = (float)image_x, y = (float)image_y, z = (float)data[depth_index] / 1000.0f;
			if(z < 32.0f && z > 0.001f)
			{
				calculate_xyz_from_depth(x, y, z);
				painter->draw_point(y, -x, z, point_cloud_color, 1.0f);
			}
		}
	}

}

void point_cloud_window::open_skv_depth(std::string filename)
{
	try
	{
		skv_file = softkinetic::skv::open_file(filename);

		std::string vendor_name, camera_model;
		std::tie(vendor_name, camera_model) = skv_file.get_device_info();
		create_points_from_depth_image();
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
	if(current_frame >= (int)depth_stream.size()) current_frame = depth_stream.size() - 1;
	std::cout << "frames = " << depth_stream.size() << "; current_frame = " << current_frame << std::endl;
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
	float scale = 1100.0f;
	painter->clear();
	std::cout << "Draw balls for frame " << current_frame << std::endl;
	ImColor ball_color = ImColor(255, 100, 100);
	for(int i = 0; i < 9; i++)
	{
		painter->draw_point(100.0f * i / scale, 100.0f * current_frame / scale, 0.0f, ball_color, 20.0f);
	}
	std::cout << "Showing frame " << current_frame << std::endl;
	unsigned int count = 0;
	ImColor point_color = ImColor(100, 100, 100);
	for(int i = 0; i < (int)depth_stream[current_frame].size() &&
		count < painter->get_max_vertices();
		i += 3)
	{
		float x = depth_stream[current_frame][i].asFloat() / scale;
		float y = depth_stream[current_frame][i + 1].asFloat() / scale;
		float z = depth_stream[current_frame][i + 2].asFloat() / scale;
		if(x || y || z)
		{
			painter->draw_point(x, z, -y, point_color, 2.0f);
			count++;
		}
	}
	std::cout << "Added " << count << " points of " << depth_stream[current_frame].size() / 3 << " total." << std::endl;
}

}
