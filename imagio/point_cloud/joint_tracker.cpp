#include "joint_tracker.h"


namespace imagio
{

void joint_tracker::new_frame(wimgui::vertex* vertices, unsigned int vertices_count)
{
	points = vertices;
	points_count = vertices_count;
	calculate_visibility_k();
	calculate_alfa_nk();
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

void joint_tracker::calculate_alfa_nk()
{
	alfa_nk.clear();
	alfa_nk.resize(joints.size());
	distance.clear();
	distance.resize(joints.size());
	std::vector<float> k_sums = std::vector<float>(points_count, 0.0f);
	for(unsigned int k = 0; k < joints.size(); ++k)
	{
		glm::vec3 mk = std::get<0>(joints[k]);
		alfa_nk[k].resize(points_count);
		distance[k].resize(points_count);
		for(unsigned int n = 0; n < points_count; ++n)
		{
			wimgui::vertex cn = points[n];
			float delta_x = cn.position_x - mk.x;
			float delta_y = cn.position_y - mk.y;
			float delta_z = cn.position_z - mk.z;

			distance[k][n] = std::sqrt(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
			alfa_nk[k][n] = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;
			alfa_nk[k][n] /= (-2 * sigma);
			alfa_nk[k][n] = std::exp(alfa_nk[k][n]);
			alfa_nk[k][n] /= std::pow(2 * pi * sigma, 3.0f / 2.0f);

			k_sums[n] += alfa_nk[k][n];
		}
	}

	std::vector<unsigned int> k_alfa_max(points_count);
	std::vector<unsigned int> k_alfa_min(points_count);
	std::vector<unsigned int> k_distance_max(points_count);
	std::vector<unsigned int> k_distance_min(points_count);
	for(unsigned int n = 0; n < points_count; ++n)
	{
		k_alfa_max[n] = 0;
		k_alfa_min[n] = 0;
		k_distance_max[n] = 0;
		k_distance_min[n] = 0;
		float alfa_max = std::numeric_limits<float>::min();
		float alfa_min = std::numeric_limits<float>::max();
		float distance_max = std::numeric_limits<float>::min();
		float distance_min = std::numeric_limits<float>::max();
		for(unsigned int k = 0; k < joints.size(); ++k)
		{
			alfa_nk[k][n] /= (k_sums[n] + pnoise);

			if(alfa_nk[k][n] > alfa_max)
			{
				alfa_max = alfa_nk[k][n];
				k_alfa_max[n] = k;
			}
			if(alfa_nk[k][n] < alfa_min)
			{
				alfa_min = alfa_nk[k][n];
				k_alfa_min[n] = k;
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
		unsigned int k_min = k_distance_min[n];
		ImColor mk_color = std::get<1>(joints[k_min]);
		cn.color_r = mk_color.Value.x;
		cn.color_g = mk_color.Value.y;
		cn.color_b = mk_color.Value.z;
	}

}

}