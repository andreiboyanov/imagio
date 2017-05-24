#include "joint_tracker.h"


namespace imagio
{

void joint_tracker::new_frame(wimgui::vertex* vertices, unsigned int vertices_count)
{
	points = vertices;
	points_count = vertices_count;
	calculate_visibility_k();
	calculate_alpha_kn();
	calculate_force_k();
	// to be continued... 
}

void joint_tracker::calculate_visibility_k()
{
	visibility_k.clear();
	visibility_k.resize(joints.size());
	for(auto& visibility : visibility_k)
	{
		visibility = 1.0f;
	}
}

void joint_tracker::calculate_alpha_kn()
{
	alpha_kn.clear();
	alpha_kn.resize(joints.size());
	distance.clear();
	distance.resize(joints.size());
	std::vector<float> k_sums = std::vector<float>(points_count, 0.0f);
	for(unsigned int k = 0; k < joints.size(); ++k)
	{
		glm::vec3 mk = std::get<0>(joints[k]);
		alpha_kn[k].resize(points_count);
		distance[k].resize(points_count);
		for(unsigned int n = 0; n < points_count; ++n)
		{
			wimgui::vertex cn = points[n];
			float delta_x = cn.position_x - mk.x;
			float delta_y = cn.position_y - mk.y;
			float delta_z = cn.position_z - mk.z;

			distance[k][n] = std::sqrt(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
			alpha_kn[k][n] = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;
			alpha_kn[k][n] /= (-2 * sigma);
			alpha_kn[k][n] = std::exp(alpha_kn[k][n]);
			alpha_kn[k][n] /= std::pow(2 * pi * sigma, 3.0f / 2.0f);

			k_sums[n] += alpha_kn[k][n];
		}
	}

	std::vector<float> alpha_max(points_count, std::numeric_limits<float>::min());
	std::vector<float> alpha_min(points_count, std::numeric_limits<float>::max());
	std::vector<unsigned int> k_alpha_max(points_count);
	std::vector<unsigned int> k_alpha_min(points_count);
	std::vector<unsigned int> k_distance_max(points_count);
	std::vector<unsigned int> k_distance_min(points_count);
	for(unsigned int n = 0; n < points_count; ++n)
	{
		k_alpha_max[n] = 0;
		k_alpha_min[n] = 0;
		k_distance_max[n] = 0;
		k_distance_min[n] = 0;
		float distance_max = std::numeric_limits<float>::min();
		float distance_min = std::numeric_limits<float>::max();
		for(unsigned int k = 0; k < joints.size(); ++k)
		{
			alpha_kn[k][n] /= (k_sums[n] + pnoise);

			if(alpha_kn[k][n] > alpha_max[n])
			{
				alpha_max[n] = alpha_kn[k][n];
				k_alpha_max[n] = k;
			}
			if(alpha_kn[k][n] < alpha_min[n])
			{
				alpha_min[n] = alpha_kn[k][n];
				k_alpha_min[n] = k;
			}
			if(distance[k][n] > distance_max)
			{
				distance_max = distance[k][n];
				k_distance_max[n] = k;
			}
			if(distance[k][n] < distance_min)
			{
				distance_min = distance[k][n];
				k_distance_min[n] = k;
			}
		}
	}

	for(unsigned int n = 0; n < points_count; ++n)
	{
		wimgui::vertex &cn = points[n];
		unsigned int k_max = k_alpha_max[n];
		float color_coefficient = alpha_kn[k_max][n] / alpha_max[n];
		ImColor mk_color = std::get<1>(joints[k_max]);
		cn.color_r = color_coefficient * mk_color.Value.x;
		cn.color_g = color_coefficient * mk_color.Value.y;
		cn.color_b = color_coefficient * mk_color.Value.z;
	}
}

void joint_tracker::calculate_force_k()
{
	force_k.clear();
	force_k.resize(joints.size());
	for(unsigned int k = 0; k < joints.size(); ++k)
	{
		glm::vec3 mk = std::get<0>(joints[k]);
		std::string mk_name = std::get<2>(joints[k]);
		glm::vec3 sum_distance(0.0f, 0.0f, 0.0f);
		for(unsigned int n = 0; n < points_count; ++n)
		{
			wimgui::vertex& cn = points[n];
			sum_distance.x += alpha_kn[k][n] * (cn.position_x - mk.x);
			sum_distance.y += alpha_kn[k][n] * (cn.position_y - mk.y);
			sum_distance.z += alpha_kn[k][n] * (cn.position_z - mk.z);
		}
		force_k[k].x = force_lambda * sum_distance.x;
		force_k[k].y = force_lambda * sum_distance.y;
		force_k[k].z = force_lambda * sum_distance.z;
		std::cout << "Force for joint " << mk_name << " (" << force_k[k].x << ", " << force_k[k].y << ", " << force_k[k].z << ")" << std::endl;
	}
}

}