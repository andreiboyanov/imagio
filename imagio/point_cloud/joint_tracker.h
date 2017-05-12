#pragma once

#ifndef IMAGIO_JOINT_TRACKER
#define IMAGIO_JOINT_TRACKER
#pragma once

#define GLM_FORCE_RADIANS
#pragma warning(push, 0)       
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

#include <vector>
#include <cmath>

#include "../3dpaint/3dpaint.h"

namespace imagio
{

class joint_tracker
{
private:
	const float pi = 3.1415926535897f;
	const float pnoise = 0.01f;
	const float pinvisible = 0.1f;
	const float sigma = 0.0001f;
	const int force_lambda = 100;
	const int em_itterations = 20;
	const float depth_occlude_tol = 0.03f;

	std::vector<glm::vec3> joints;
	std::vector<std::vector<float>> alfa_nk;
	std::vector<float> visibility_k;
	wimgui::vertex* points = nullptr;
	unsigned int points_count = 0;
public:
	joint_tracker(std::vector<glm::vec3> initial_joints) : joints(initial_joints) {};
	void new_frame(wimgui::vertex* vertices, unsigned int vertices_count);
	void calculate_visibility_k();
	void calculate_alfa_nk();
};

}

#endif
