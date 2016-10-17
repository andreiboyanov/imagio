#include "../imgui/imgui.h"
#include "window.h"
#include"dock.h"


namespace wimgui
{

window::window(char *title)
{
	name(title);
}

window::window()
{
	name("Debug");
}

window::~window()
{

}

#pragma region flags

void window::show_title(bool show) {
	if (!show)
		flags |= ImGuiWindowFlags_NoTitleBar;
	else
		flags &= ~ImGuiWindowFlags_NoTitleBar;
}

void window::show_border(bool show) {
	if (show)
		flags |= ImGuiWindowFlags_ShowBorders;
	else
		flags &= ~ImGuiWindowFlags_ShowBorders;
}

void window::allow_resize(bool allow) {
	if (!allow)
		flags |= ImGuiWindowFlags_NoResize;
	else
		flags &= ~ImGuiWindowFlags_NoResize;
}

void window::allow_move(bool allow) {
	if (!allow)
		flags |= ImGuiWindowFlags_NoMove;
	else
		flags &= ~ImGuiWindowFlags_NoMove;
}

void window::show_scrollbar(bool show) {
	if (!show)
		flags |= ImGuiWindowFlags_NoScrollbar;
	else
		flags &= ~ImGuiWindowFlags_NoScrollbar;
}

void window::allow_collapse(bool allow) {
	if (!allow)
		flags |= ImGuiWindowFlags_NoCollapse;
	else
		flags &= ~ImGuiWindowFlags_NoCollapse;
}

void window::show_menu(bool show) {
	if (show)
		flags |= ImGuiWindowFlags_MenuBar;
	else
		flags &= ~ImGuiWindowFlags_MenuBar;
}


void window::allow_mouse_scroll(bool allow)
{
	if (!allow)
		flags |= ImGuiWindowFlags_NoScrollWithMouse;
	else
		flags &= ~ImGuiWindowFlags_NoScrollWithMouse;
}

void window::auto_resize(bool allow)
{
	if (allow)
		flags |= ImGuiWindowFlags_AlwaysAutoResize;
	else
		flags &= ~ImGuiWindowFlags_AlwaysAutoResize;
}

void window::save_settings(bool save)
{
	if (!save)
		flags |= ImGuiWindowFlags_NoSavedSettings;
	else
		flags &= ~ImGuiWindowFlags_NoSavedSettings;
}

void window::allow_inputs(bool allow)
{
	if (!allow)
		flags |= ImGuiWindowFlags_NoInputs;
	else
		flags &= ~ImGuiWindowFlags_NoInputs;
}

void window::horizontal_scrollbar(bool allow)
{
	if (allow)
		flags |= ImGuiWindowFlags_HorizontalScrollbar;
	else
		flags &= ~ImGuiWindowFlags_HorizontalScrollbar;
}

void window::always_horizontal_scrollbar(bool always)
{
	if (always)
		flags |= ImGuiWindowFlags_AlwaysHorizontalScrollbar;
	else
		flags &= ~ImGuiWindowFlags_AlwaysHorizontalScrollbar;
}

void window::always_vertical_scrollbar(bool always)
{
	if (always)
		flags |= ImGuiWindowFlags_AlwaysVerticalScrollbar;
	else
		flags &= ~ImGuiWindowFlags_AlwaysVerticalScrollbar;
}

void window::focus_on_appearing(bool allow)
{
	if (!allow)
		flags |= ImGuiWindowFlags_NoFocusOnAppearing;
	else
		flags &= ~ImGuiWindowFlags_NoFocusOnAppearing;
}

void window::to_front_on_focus(bool allow)
{
	if (!allow)
		flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	else
		flags &= ~ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void window::use_window_padding(bool use)
{
	if (use)
		flags |= ImGuiWindowFlags_AlwaysUseWindowPadding;
	else
		flags &= ~ImGuiWindowFlags_AlwaysUseWindowPadding;
}



void window::set_flags(ImGuiWindowFlags _flags) {
	flags = _flags;
}

#pragma endregion flags

float window::preferred_width(float width)
{
	if (width != NULL)
		this->preferred_size.x = width;
	return this->preferred_size.x;
}

float window::preferred_height(float height)
{
	if (height != NULL)
		this->preferred_size.y = height;
	return this->preferred_size.y;
}

// FIXME: Replace the following two methods by current_size() -
// no need to call imgui_window() every time twice
float window::current_width()
{
	ImGuiWindow *window = this->imgui_window();
	return window ? window->Size.x : NULL;
}

float window::current_height()
{
	ImGuiWindow *window = this->imgui_window();
	return window ? window->Size.y : NULL;
}

void window::draw_imgui()
{
	if (this->visible)
	{
		if (this->dock)
		{
			ImGui::SetNextWindowSize(this->preferred_size, ImGuiSetCond_Always);
		}
		ImGui::Begin(title, &visible, flags);
		draw();
		ImGui::End();
	}
}

void window::show(bool _visible)
{
	this->visible = _visible;
}

void window::draw()
{
	ImGui::Text("This is an empty window. "
		"Please implement your own draw() method");
}

char *window::name(char *new_name)
{
	if (new_name)
		this->title = new_name;
	return this->title;

}

docker *window::docked_to(docker *new_dock)
{
	if (new_dock)
		this->dock = new_dock;
	return this->dock;

}

ImGuiWindow *window::imgui_window()
{
	return ImGui::FindWindowByName(this->title);
}

bool window::collapsed()
{
	ImGuiWindow *window = this->imgui_window();
	return window ? this->imgui_window()->Collapsed : false;
}

}