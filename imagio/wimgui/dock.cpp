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

ImVec2 docker::preferred_size()
{
	return this->size;
}

void docker::draw(dock_draw_mode mode)
{
	mode;
}

void docker::adjust_size()
{
	if (!this->windows.size())
		return;
	float max_width = -9999.0f;
	for (auto window : this->windows)
	{
		float window_width = std::fmaxf(window->current_width(), window->preferred_width());
		if (window_width > max_width)
			max_width = window_width;
		}
	for (auto window : this->windows)
	{
		window->preferred_width(max_width);

		float window_height = window->current_height();
		if (window_height > 50.0f)
			window->preferred_height(window_height);
	}
}

}