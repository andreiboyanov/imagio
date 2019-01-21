#include "painter3d.h"

#include <iostream>

namespace wimgui
{

void painter3d::move(float x, float y)
{
	if(x > 0) {}
	if(y > 0) {}
}

void painter3d::move(float x, float y, float z)
{
	if(x > 0) {}
	if(y > 0) {}
	if(z > 0) {}
}

void painter3d::stop_moving()
{
}

void painter3d::rotate(float x, float y)
{
	if(x > 0) {}
	if(y > 0) {}

	glm::mat4 x_rotation, y_rotation;
	glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
	glm::vec3 y_axis(0.0f, 1.0f, 0.0f);
	x_rotation = glm::rotate(x_rotation, radians(x), y_axis);
	y_rotation = glm::rotate(y_rotation, radians(y), x_axis);
	model_matrix = temp_model_matrix * x_rotation * y_rotation;
}

void painter3d::rotate(float x, float y, float z)
{
	if(x > 0) {}
	if(y > 0) {}
	if(z > 0) {}
}

void painter3d::stop_rotating()
{
	temp_model_matrix = model_matrix;
}

void painter3d::scale(float wheel)
{
	model_matrix = glm::scale(model_matrix, glm::vec3((10 + wheel) / 10));
	temp_model_matrix = model_matrix;
}

void painter3d::set_rotation(float x, float y, float z)
{
	glm::mat4 x_rotation, y_rotation, z_rotation;
	glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
	glm::vec3 y_axis(0.0f, 1.0f, 0.0f);
	glm::vec3 z_axis(0.0f, 0.0f, 1.0f);
	x_rotation = glm::rotate(x_rotation, x, x_axis);
	y_rotation = glm::rotate(y_rotation, y, y_axis);
	z_rotation = glm::rotate(z_rotation, z, z_axis);
	model_matrix = x_rotation * y_rotation * z_rotation;
	stop_rotating();
}

void painter3d::set_translation(float x, float y, float z)
{
	if(x > 0) {}
	if(y > 0) {}
	if(z > 0) {}
}

}
