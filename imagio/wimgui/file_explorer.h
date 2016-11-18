#pragma once

#ifndef WIMGUI_FILE_EXPLORER_H
#define WIMGUI_FILE_EXPLORER_H

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

#include "window.h"

using namespace boost::filesystem;

namespace wimgui
{

class file_explorer : public window
{

	path selected_path;
	path selected_file;
	path top_path;

public:
	file_explorer();
	file_explorer(const char* _title);
	~file_explorer() {}

	void draw();
	void draw_directory_entry(path _path, bool full_path=false);
	void draw_file_entry(path _path, bool full_path=false);
	void show_directory_content();
};

}

#endif
