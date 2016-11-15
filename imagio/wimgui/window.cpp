#include "window.h"


namespace wimgui
{

#pragma region constructors

window::window(const char *_title)
{
	title = _title;
}

window::window()
{
	title = "Debug";
}

window::~window()
{

}

#pragma endregion

#pragma region flags

void window::show_title(bool _show) {
	if (!_show)
		flags |= ImGuiWindowFlags_NoTitleBar;
	else
		flags &= ~ImGuiWindowFlags_NoTitleBar;
}

void window::show_border(bool _show) {
	if (_show)
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

void window::show_scrollbar(bool _show) {
	if (!_show)
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

void window::show_menu(bool _show) {
	if (_show)
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

float window::get_width()
{
	return size.x;
}

void window::set_width(float width)
{
	size.x = width;
}

float window::get_height()
{
	return size.y;
}

void window::set_height(float height)
{
	size.y = height;
}

ImVec2 window::get_size()
{
	return size;
}

void window::set_size(float x, float y)
{
	size.x = x;
	size.y = y;
}


ImVec2 window::get_position()
{
	return position;
}

void window::set_position(float x, float y)
{
	position.x = x;
	position.y = y;
}


// FIXME: Replace the following two methods by current_size() -
// no need to call get_imgui_window() every time twice
float window::get_current_width()
{
	ImGuiWindow *imgui_window = get_imgui_window();
	return imgui_window ? imgui_window->Size.x : -1.0f;
}

float window::get_current_height()
{
	ImGuiWindow *imgui_window = get_imgui_window();
	return imgui_window ? imgui_window->Size.y : -1.0f;
}

void window::draw_imgui()
{
	if (visible)
	{
		init_draw();

		if (is_moving())
		{
			ImGuiWindow* imgui_window = get_imgui_window();
			if (imgui_window)
			{
				ImVec2 imgui_position = imgui_window->Pos;
				set_position(imgui_position.x, imgui_position.y);
			}
		}
		else
		{
			ImGui::SetNextWindowPos(position, ImGuiSetCond_Always);
		}

		ImGui::SetNextWindowSize(size, ImGuiSetCond_Always);

		ImGui::Begin(title, &visible, flags);
		draw();
		finish_draw();
		ImGui::End();
	}
}

void window::show(bool _visible)
{
	visible = _visible;
}

void window::init_draw()
{

}

void window::draw()
{
	ImGui::Text("This is an empty window. "
		"Please implement your own draw() method");
}

void window::finish_draw()
{

}

docker *window::docked_to()
{
	return dock;
}

void window::dock_to(docker *new_dock)
{
	dock = new_dock;

}

bool window::is_docked()
{
	return dock != nullptr;
}

ImGuiWindow* window::get_imgui_window()
{
	return ImGui::FindWindowByName(title);
}

bool window::is_collapsed()
{
	ImGuiWindow *imgui_window = get_imgui_window();
	return imgui_window ? imgui_window->Collapsed : false;
}

void window::set_collapsed(bool collapsed)
{
	ImGuiWindow *imgui_window = get_imgui_window();
	imgui_window->Collapsed = collapsed;
}

bool window::is_moving()
{
	ImGuiWindow* imgui_window = get_imgui_window();
	ImGuiContext& context = *GImGui;
	return (context.MovedWindow == imgui_window);
}

// FIXME: Isn't working properly...
bool window::is_resizing()
{
    ImGuiWindow* imgui_window = get_imgui_window();
    if (!imgui_window)
        return false;
	ImGuiContext& context = *GImGui;
    const float window_rounding = context.Style.WindowRounding;
    const float resize_corner_size = ImMax(context.FontSize * 1.35f,
                                           window_rounding +
                                                1.0f +
                                                context.FontSize * 0.2f);
    ImVec2 corner = imgui_window->Rect().GetBR();
    const ImRect resize_rectangle(corner.x - resize_corner_size * 0.75f,
                                  corner.y - resize_corner_size * 0.75f,
                                  corner.x, corner.y);
    const ImGuiID resize_id = imgui_window->GetID("#CHECK");
    bool hovered, held;
    ImGui::ButtonBehavior(resize_rectangle, resize_id, &hovered, &held,
                          ImGuiButtonFlags_FlattenChilds);
    return held;
}

void window::set_cursor_position(ImVec2 _position)
{
    ImGuiWindow* imgui_window = get_imgui_window();
    imgui_window->DC.CursorPos.x = _position.x;
    imgui_window->DC.CursorPos.y = _position.y;
}

ImVec2 window::get_cursor_position()
{
    ImGuiWindow* imgui_window = get_imgui_window();
    return imgui_window->DC.CursorPos;
}

// Copy/Paste from
// https://github.com/guillaumechereau/goxel/blob/master/src/imgui_user.inl
void window::draw_vertical_text(const char *text, ImVec2 _position)
{
	ImGuiWindow* imgui_window = get_imgui_window();
	if (!imgui_window)
		return;

	ImGuiContext& context = *GImGui;
	const ImGuiStyle& style = context.Style;
	ImFont *font = context.Font;

	const ImFont::Glyph *glyph;
	char c;

	const  ImU32 text_color = 
		ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);
	while ((c = *text++)) {
		glyph = font->FindGlyph(c);
		if (!glyph) continue;

		imgui_window->DrawList->PrimReserve(6, 4);
		imgui_window->DrawList->PrimQuadUV(
			_position + ImVec2(glyph->Y0, -glyph->X0),
			_position + ImVec2(glyph->Y0, -glyph->X1),
			_position + ImVec2(glyph->Y1, -glyph->X1),
			_position + ImVec2(glyph->Y1, -glyph->X0),

			ImVec2(glyph->U0, glyph->V0),
			ImVec2(glyph->U1, glyph->V0),
			ImVec2(glyph->U1, glyph->V1),
			ImVec2(glyph->U0, glyph->V1),
			text_color);
		_position.y -= glyph->XAdvance;
	}
}


background_window::background_window(const char* _title) : window(_title)
{
	allow_inputs(true);
	allow_mouse_scroll(false);
	allow_resize(false);
	allow_move(false);
	always_horizontal_scrollbar(false);
	always_vertical_scrollbar(false);
	save_settings(true);
	auto_resize(false);
	focus_on_appearing(false);
	horizontal_scrollbar(false);
	show(true);
	show_border(false);
	show_menu(false);
	show_scrollbar(false);
	show_title(false);
	to_front_on_focus(false);
	use_window_padding(false);
}

background_window::~background_window()
{

}

void background_window::init_draw()
{
	ImGuiStyle &style = ImGui::GetStyle();
	ImVec4 color(0.0f, 0.0f, 0.0f, 0.0f);
	
	save_style(style);
	style.Colors[ImGuiCol_WindowBg] = color;
	style.WindowPadding.x = 0.0f;
	style.WindowPadding.y = 0.0f;
}

void background_window::finish_draw()
{
	ImGuiStyle &style = ImGui::GetStyle();
	restore_style(style);
}

#pragma region private methods

void background_window::save_style(ImGuiStyle& style)
{
	normal_window_background = style.Colors[ImGuiCol_WindowBg];
	normal_window_padding.x = style.WindowPadding.x;
	normal_window_padding.y = style.WindowPadding.y;
}

void background_window::restore_style(ImGuiStyle& style)
{
	style.Colors[ImGuiCol_WindowBg] = normal_window_background;
	style.WindowPadding.x = normal_window_padding.x;
	style.WindowPadding.y = normal_window_padding.y;
}

#pragma endregion

}
