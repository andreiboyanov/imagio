#include "dock.h"


namespace wimgui
{

docker::docker()
{
	this->style = dock_left;
}

docker::docker(dock_style style)
{
	this->style = style;
}

docker::~docker()
{

}

void docker::add_window(window *window)
{
	windows.push_back(window);
	window->docked_to(this);
	this->adjust();
}

void docker::remove_window(window *window)
{
	window;
}

ImVec2 docker::preferred_size(float width, float height)
{
	if (width > 0.0f)
		this->size.x = width;
	if (height > 0.0f)
		this->size.y = height;
	return this->size;
}

ImVec2 docker::preferred_position(float x, float y)
{
	if (x >= 0.0f)
		this->position.x = x;
	if (y >= 0.0f)
		this->position.y = y;
	return this->position;
}

void docker::draw(dock_draw_mode mode)
{
	mode;
}

void docker::adjust()
{
	if (this->sleep > 0) {
		this->sleep--;
		return;
	}
	if (!this->windows.size())
		return;
	for (auto window : this->windows)
	{
		float window_width = window->current_width();
		if (window_width != this->size.x) {
			this->size.x = window_width;
			break;
		}
	}
	float last_y = this->position.y;
	for (auto window : this->windows)
	{
		window->preferred_width(this->size.x);
		window->preferred_position(this->position.x, last_y);

		float window_height = window->current_height();
		if (!window->collapsed())
			window->preferred_height(window_height);

		last_y += window_height;
	}
}

}