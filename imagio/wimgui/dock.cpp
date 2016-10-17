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
	this->adjust_size();
}

void docker::remove_window(window *window)
{
	window;
}

ImVec2 docker::preferred_size(float width, float height)
{
	if (width)
		this->size.x = width;
	if (height)
		this->size.y = height;
	return this->size;
}

void docker::draw(dock_draw_mode mode)
{
	mode;
}

void docker::adjust_size()
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
	for (auto window : this->windows)
	{
		window->preferred_width(this->size.x);

		if (!window->collapsed())
		{
			float window_height = window->current_height();
			window->preferred_height(window_height);
		}
	}
}

}