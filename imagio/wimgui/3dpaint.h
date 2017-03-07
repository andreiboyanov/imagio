#pragma once

#ifndef WIMGUI_3DPAINT_H
#define WIMGUI_3DPAINT_H

#include <Eigen/Dense>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

namespace wimgui
{

class vector3 : public Eigen::Vector3f
{
public:
	vector3(int _x, int _y, int _z) : Eigen::Vector3f(_x, _y, _z) {}
	vector3(vector3& _other) : Eigen::Vector3f(_other) {}
};

class painter3d
{
private:

public:
	void draw_line(vector3& from, vector3& to);
};

}

#endif