#pragma once

#ifndef WIMGUI_FILE_EXPLORER_H
#define WIMGUI_FILE_EXPLORER_H

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#include <sys/stat.h>
#include <vector>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"

#include "window.h"

using namespace boost::filesystem;

namespace wimgui
{

struct directory_info
{
	path path;
	directory_info* parent;
	directory_info* selected_child;
	std::vector<directory_info> children = std::vector<directory_info>();
public:	
	directory_info(const char* _path)
		: path(_path)
	{}
	bool is_directory;
	bool is_regular_file;
};

class file_explorer : public window
{
	path selected_directory_path;
	directory_info* selected_directory;
	directory_info* selected_file;
	std::vector<directory_info> top_paths;

public:
	file_explorer();
	file_explorer(const char* _selected_path);
	file_explorer(const char* _selected_path, const char* _title);
	~file_explorer() {}

	void draw();
	void draw_directory_entry(directory_info info, bool full_path=false);
	void draw_file_entry(directory_info info, bool full_path=false);
	void show_directory_content();

protected:
	void init(const char* slected_path = nullptr);
	void init_top_paths();
};

}

#endif
