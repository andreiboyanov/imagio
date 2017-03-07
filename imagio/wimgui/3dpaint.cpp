#include "3dpaint.h"

#include <iostream>

namespace wimgui
{


void painter3d::draw_line(vector3& from, vector3& to)
{
	ImGui::Text("Draw line");
	ImGui::Text("from (%f, %f, %f)", from.x(), from.y(), from.z());
	ImGui::Text("to (%f, %f, %f)", to.x(), to.y(), to.z());
	
}

}