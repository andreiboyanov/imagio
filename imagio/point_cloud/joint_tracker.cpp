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
	alfa_nk.resize(points_count);
	for(unsigned int n = 0; n < points_count; ++n)
	{
		alfa_nk[n].resize(joints.size());
		for(unsigned int k = 0; k < joints.size(); ++k)
		{
			wimgui::vertex cn = points[n];
			glm::vec3 mk = joints[k];
			float delta_x = cn.position_x - mk.x;
			float delta_y = cn.position_y - mk.y;
			float delta_z = cn.position_z - mk.z;
			alfa_nk[n][k] = delta_x * delta_x + delta_y * delta_y + delta_z * delta_z;
			alfa_nk[n][k] /= (2 * sigma);
			alfa_nk[n][k] /= std::pow(2 * pi * sigma, 3/2);
		}
	}
}

}