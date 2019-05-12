#include "view3d.h"

namespace wimgui
{


void render_3dpaint(const ImDrawList* parent_list, const ImDrawCmd* draw_command)
{
	if(parent_list) {}
	view3d* view = (view3d *)draw_command->UserCallbackData;
	view->gl_paint();
}


view3d::view3d(const char* _title) : window(_title)
{
	show_title(true);
	show_border(true);
	allow_resize(true);
	allow_move(true);
	show_scrollbar(true);
	allow_collapse(true);
	show_menu(false);
}

void view3d::gl_paint()
{
	for (auto& painter: painters)
	{
		painter->gl_paint(*this);
	}
}

void view3d::draw()
{
	init_scene();

	ImGui::Text("3d view");

	if (ImGui::IsMouseDragging(1, 1.0f))
	{
		ImGui::Text("Draging");
		ImVec2 delta = ImGui::GetMouseDragDelta(1, 1.0f);
		ImGui::Text("(%.2f, %.2f)", delta.x, delta.y);
		move(delta.x, -delta.y);
	}
	if (ImGui::IsMouseReleased(1))
	{
		stop_moving();
	}
	if (ImGui::IsMouseDragging(2, 1.0f))
	{
		ImGui::Text("Rotating");
		ImVec2 delta = ImGui::GetMouseDragDelta(2, 1.0f);
		ImGui::Text("(%.2f, %.2f)", delta.x, delta.y);
		rotate(delta.x, delta.y);
	}
	if (ImGui::IsMouseReleased(2))
	{
		stop_rotating();
	}

	ImGui::GetWindowDrawList()->AddCallback(render_3dpaint, this);

	float mouse_wheel = ImGui::GetIO().MouseWheel;
	if(mouse_wheel)
	{
		ImGui::Text("%f", mouse_wheel);
		scale(mouse_wheel);
	}
}

void view3d::center()
{
	ImRect canvas = get_content_rectangle();
	set_translation(canvas.GetWidth() / 2.0f, canvas.GetHeight() / 2.0f, 0.0f);
}

void view3d::view_top()
{
	set_rotation(radians(-90.0f), 0.0f, 0.0f);
}

void view3d::view_bottom()
{
	set_rotation(radians(90.0f), 0.0f, 0.0f);
}

void view3d::view_left()
{
	set_rotation(0.0f, radians(90.0f), 0.0f);
}

void view3d::view_right()
{
	set_rotation(0.0f, radians(-90.0f), 0.0f);
}

void view3d::view_front()
{
	set_rotation(0.0f, 0.0f, 0.0f);
}

void view3d::view_back()
{
	set_rotation(0.0f, radians(180.0f), 0.0f);
}

void view3d::init_scene()
{
	// projection_matrix = glm::ortho(-4.0f / 3.0f, 4.0f / 3.0f, -1.0f, 1.0f, -2.0f, 2.0f);
	ImRect canvas = get_content_rectangle();
	float window_ratio = canvas.GetWidth() / canvas.GetHeight();
	projection_matrix = glm::ortho(
		-window_ratio, window_ratio, -1.0f, 1.0f, -2.0f, 2.0f
	);
	// projection_matrix = glm::perspective(45.0f, window_ratio, 1.0f, -2.0f);
}

void view3d::move(float x, float y)
{
	ImRect canvas = get_content_rectangle();
	glm::vec4 view_port(canvas.Min.x, canvas.Min.y, canvas.Max.x, canvas.Max.y);
	glm::vec3 projected_translation = (
		glm::unProject(glm::vec3(x, y, 0.0f), camera_matrix, projection_matrix, view_port) -
		glm::unProject(glm::vec3(0.0f), camera_matrix, projection_matrix, view_port)
	);
	camera_matrix = glm::translate(temp_camera_matrix, projected_translation);
}

void view3d::move(float x, float y, float z)
{
	if(x > 0) {}
	if(y > 0) {}
	if(z > 0) {}
}

void view3d::stop_moving()
{
	temp_camera_matrix = camera_matrix;
}

void view3d::rotate(float x, float y)
{
	glm::mat4 x_rotation, y_rotation;
	const glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
	const glm::vec3 y_axis(0.0f, 1.0f, 0.0f);
	x_rotation = glm::rotate(x_rotation, radians(x), y_axis);
	y_rotation = glm::rotate(y_rotation, radians(y), x_axis);
	camera_matrix = x_rotation * y_rotation * temp_camera_matrix;
}

void view3d::rotate(float x, float y, float z)
{
	if(x > 0) {}
	if(y > 0) {}
	if(z > 0) {}
}

void view3d::stop_rotating()
{
	temp_camera_matrix = camera_matrix;
}

void view3d::scale(float wheel)
{
	camera_matrix = glm::scale(camera_matrix, glm::vec3((10 + wheel) / 10));
	temp_camera_matrix = camera_matrix;
}

void view3d::set_rotation(float x, float y, float z)
{
	glm::mat4 x_rotation, y_rotation, z_rotation;
	glm::vec3 x_axis(1.0f, 0.0f, 0.0f);
	glm::vec3 y_axis(0.0f, 1.0f, 0.0f);
	glm::vec3 z_axis(0.0f, 0.0f, 1.0f);
	x_rotation = glm::rotate(x_rotation, x, x_axis);
	y_rotation = glm::rotate(y_rotation, y, y_axis);
	z_rotation = glm::rotate(z_rotation, z, z_axis);
	camera_matrix = x_rotation * y_rotation * z_rotation;
	stop_rotating();
}

void view3d::set_translation(float x, float y, float z)
{
	if(x > 0) {}
	if(y > 0) {}
	if(z > 0) {}
}

void view3d::add_painter(painter3d *painter)
{
	painters.push_back(painter);
}

void view3d::remove_painter(painter3d *painter)
{
	if(painter) {}
}

}
