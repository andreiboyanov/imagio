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

int get_path_string(path _path, char* buffer, int count, bool full_path)
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

void file_explorer::draw_directory_entry(directory_info info, bool full_path)
{
	bool node_open = ImGui::TreeNode(info.path.generic_string().c_str(), " ");
	ImGui::SameLine();
	char label[512] = {};
	get_path_string(info.path, label, 512, full_path);
	if (ImGui::Selectable(label, false))
	{
		selected_directory_path = info.path;
	}
	if (node_open)
	{
		//directory_iterator end_entry;
		//for (directory_iterator entry(info.path); entry != end_entry; entry++)
		//{
		//	if (is_directory(entry->status()))
		//	{
		//		draw_directory_entry(entry->path());
		//	}
		//}
		for (directory_info child : info.children)
			if (child.is_directory)
				draw_directory_entry(child);
		ImGui::TreePop();
	}
}

void file_explorer::draw_file_entry(directory_info entry, bool full_path)
{
	char label[512] = {};
	get_path_string(entry.path, label, 512, full_path);
	if (ImGui::Selectable(label, false))
	{
		selected_file = &entry;
	}
	ImGui::NextColumn();
	struct stat file_stat;
	stat(entry.path.generic_string().c_str(), &file_stat);
	if (entry.is_regular_file)
		ImGui::Text("%10d", file_size(entry.path));
	else if (entry.is_directory)
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
		for (directory_info entry : selected_directory->children)
			draw_file_entry(entry);
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

	for (auto top_path : top_paths)
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
	top_paths.push_back(directory_info("/"));
}

#pragma endregion

}
