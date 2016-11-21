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
	std::vector<directory_info*> children = std::vector<directory_info*>();
	struct stat file_stat;
public:	
	directory_info(boost::filesystem::path _path)
		: path(_path),
		is_directory(boost::filesystem::is_directory(path)),
		is_regular_file(boost::filesystem::is_regular_file(path))
	{
		stat(_path.generic_string().c_str() , &file_stat);
	}
	bool is_directory;
	bool is_regular_file;
};

class file_explorer : public window
{
	path selected_directory_path;
	directory_info* selected_directory;
	directory_info* selected_file;
	std::vector<directory_info*> top_paths;

public:
	file_explorer();
	file_explorer(const char* _selected_path);
	file_explorer(const char* _selected_path, const char* _title);
	~file_explorer() {}

	void draw();
	void draw_directory_entry(directory_info* info, bool full_path=false);
	void draw_file_entry(directory_info* info, bool full_path=false);
	void show_directory_content();
	void select_directory(directory_info* entry);

protected:
	void init(const char* slected_path = nullptr);
	void init_top_paths();
	void add_path(directory_info* parent, directory_info* child);
	void load_children(directory_info* parent, bool refresh=false);
	void kill_children(directory_info* parent);
	int get_path_string(path _path, char* buffer, int count, bool full_path);
};

}

#endif
