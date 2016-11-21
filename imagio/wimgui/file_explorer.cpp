#include "file_explorer.h"
#include <stdio.h>

using namespace boost::filesystem;
namespace wimgui
{

file_explorer::file_explorer()
	: window("File explorer")
{
	init();
}

file_explorer::file_explorer(const char* _selected_path)
	: window("File explorer")
{
	init(_selected_path);
}

file_explorer::file_explorer(const char* _selected_path, const char* _title)
	: window(_title)
{
	init(_selected_path);
}

void file_explorer::draw_directory_entry(directory_info* entry, bool full_path)
{
	if (selected_directory && selected_directory->parent == entry)
		ImGui::SetNextTreeNodeOpen(true);
	bool node_open = ImGui::TreeNode(entry->path.generic_string().c_str(), " ");
	ImGui::SameLine();
	char label[512] = {};
	get_path_string(entry->path, label, 512, full_path);
	if (ImGui::Selectable(label, false))
	{
		select_directory(entry);
	}
	if (node_open)
	{
		load_children(entry);
		for (directory_info* child : entry->children)
			if (child->is_directory)
				draw_directory_entry(child);
		ImGui::TreePop();
	}
	//else
	//	kill_children(entry);
}

void file_explorer::draw_file_entry(directory_info* entry, bool full_path)
{
	char label[512] = {};
	get_path_string(entry->path, label, 512, full_path);
	if (ImGui::Selectable(label, false))
	{
		if (entry->is_directory || mouse_double_clicked())
			select_directory(entry);
		else
			selected_file = entry;
	}
	ImGui::NextColumn();
	if (entry->is_regular_file)
		ImGui::Text("%10d", file_size(entry->path));
	else if (entry->is_directory)
		ImGui::Text("<dir>");
	else
		ImGui::Text("  -  ");
	ImGui::NextColumn();
	ImGui::Text("  -  ");
	ImGui::NextColumn();
	ImGui::Text("  -  ");
	ImGui::NextColumn();
}

void file_explorer::show_directory_content()
{
	if (selected_directory)
		ImGui::Text("%s", selected_directory->path.generic_string().c_str());
	ImGui::Separator();
	ImGui::Columns(4, "File Info");
	ImGui::Text("File name"); ImGui::NextColumn();
	ImGui::Text("Size, bytes"); ImGui::NextColumn();
	ImGui::Text("Date created"); ImGui::NextColumn();
	ImGui::Text("Date modified"); ImGui::NextColumn();
	if (selected_directory)
	{
		load_children(selected_directory);
		for (directory_info* entry : selected_directory->children)
			draw_file_entry(entry);
	}
	ImGui::Columns(1);
}

void file_explorer::draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
	// FIXME: Padding should not be hardcoded
	ImGui::BeginChild("Files Zone", size - ImVec2(20, 80), true);
	ImGui::Columns(2, "File Explorer");
	ImGui::BeginChild("Directories", ImVec2(0, 0),
		false, ImGuiWindowFlags_HorizontalScrollbar);

	for (directory_info* top_path : top_paths)
		draw_directory_entry(top_path, true);
	ImGui::EndChild();

	ImGui::NextColumn();
	ImGui::BeginChild("Files", ImVec2(0, 0),
		false, ImGuiWindowFlags_HorizontalScrollbar);
	show_directory_content();
	ImGui::EndChild();
	ImGui::Columns(1);
	ImGui::EndChild();
	ImGui::BeginChild("Status Zone");
	ImGui::Text("This is the Buttons zone.");
	ImGui::EndChild();
	ImGui::PopStyleVar();
}

void file_explorer::select_directory(directory_info* entry)
{
	selected_directory_path = entry->path;
	selected_directory = entry;
}

#pragma region protected methods

void file_explorer::init(const char* _selected_path)
{
	init_top_paths();
	if (_selected_path)
		selected_directory_path = path(_selected_path);
	else
		selected_directory_path = current_path();
}

void file_explorer::init_top_paths()
{
	add_path(nullptr, new directory_info("/"));
}

void file_explorer::add_path(directory_info* parent, directory_info* child)
{
	if (parent)
	{
		parent->children.push_back(child);
		child->parent = parent;
	}
	else
		top_paths.push_back(child);
}

void file_explorer::kill_children(directory_info* parent)
{
	std::vector<directory_info*> children = parent->children;
	std::vector<directory_info*>::iterator iterator = children.begin();
	while (iterator != children.end())
	{
		directory_info* child = *iterator;
		kill_children(child);
		iterator = children.erase(iterator);
	}
}

void file_explorer::load_children(directory_info* entry, bool refresh)
{
	if (refresh)
		entry->children.erase(entry->children.begin(), entry->children.end());
	if (!entry->children.size())
	{
		directory_iterator end_directory;
		for (directory_iterator directory(entry->path);
			directory != end_directory;
			directory++)
			add_path(entry, new directory_info(directory->path()));
	}

}

int file_explorer::get_path_string(path _path, char* buffer, int count, bool full_path)
{
	int result;
	if (full_path)
		result = ImFormatString(buffer, count, "%s",
		_path.generic_string().c_str());
	else
		result = ImFormatString(buffer, count, "%s",
		_path.filename().generic_string().c_str());
	return result;
}

#pragma endregion

}
