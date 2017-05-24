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
#include <tuple>
#include <algorithm>
#include <iostream>
#include <iomanip>

#include "../3dpaint/3dpaint.h"

namespace imagio
{

class joint_tracker
{
private:
	const float pi = 3.1415926535897f;
	const float pnoise = 0.01f;
	const float pinvisible = 0.1f;
	//const float sigma = 0.0001f;
	const float sigma = 0.0001f;
	const int force_lambda = 100;
	const int em_itterations = 20;
	const float depth_occlude_tol = 0.03f;

	std::vector<std::tuple<glm::vec3, const ImColor&, std::string>> joints;
	wimgui::vertex* points = nullptr;
	unsigned int points_count = 0;

	std::vector<std::vector<float>> alpha_kn;
	std::vector<glm::vec3> force_k;
	std::vector<std::vector<float>> distance;
	std::vector<float> visibility_k;
public:
	joint_tracker(std::vector<std::tuple<glm::vec3, const ImColor&, std::string>> initial_joints) : joints(initial_joints) {};
	void new_frame(wimgui::vertex* vertices, unsigned int vertices_count);
	void calculate_visibility_k();
	void calculate_alpha_kn();
	void calculate_force_k();
	std::vector<std::vector<float>>& get_alpha_kn() { return alpha_kn; }
	std::vector<std::vector<float>>& get_distance() { return distance; }
	std::vector<glm::vec3>& get_force_k() { return force_k; }
};

}

#endif
