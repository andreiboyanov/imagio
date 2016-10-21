#include "window.h"


namespace wimgui
{

#pragma region constructors

window::window(const char *_title)
{
	this->title = _title;
}

window::window()
{
	this->title = "Debug";
}

window::~window()
{

}

#pragma endregion

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

float window::get_preferred_width()
{
	return this->size.x;
}

void window::set_preferred_width(float width)
{
	this->size.x = width;
}

float window::get_preferred_height()
{
	return this->size.y;
}

void window::set_preferred_height(float height)
{
	this->size.y = height;
}

ImVec2 window::get_preferred_position()
{
	return this->position;
}

void window::set_preferred_position(float x, float y)
{
	this->position.x = x;
	this->position.y = y;
}


// FIXME: Replace the following two methods by current_size() -
// no need to call get_imgui_window() every time twice
float window::get_current_width()
{
	ImGuiWindow *window = this->get_imgui_window();
	return window ? window->Size.x : NULL;
}

float window::get_current_height()
{
	ImGuiWindow *window = this->get_imgui_window();
	return window ? window->Size.y : NULL;
}

void window::draw_imgui()
{
	if (this->visible)
	{
		ImGui::SetNextWindowSize(this->size, ImGuiSetCond_Always);
		ImGui::SetNextWindowPos(this->position, ImGuiSetCond_Always);
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

docker *window::docked_to()
{
	return this->dock;
}

void window::dock_to(docker *new_dock)
{
	this->dock = new_dock;

}

bool window::is_docked()
{
	return this->dock != nullptr;
}

ImGuiWindow* window::get_imgui_window()
{
	return ImGui::FindWindowByName(this->title);
}

bool window::is_collapsed()
{
	ImGuiWindow *window = this->get_imgui_window();
	return window ? this->get_imgui_window()->Collapsed : false;
}

}