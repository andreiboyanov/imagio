#include "file_explorer.h"
#include <stdio.h>
#include <sys/stat.h>

using namespace boost::filesystem;
namespace wimgui
{

file_explorer::file_explorer() : window("File explorer")
{
	top_path = current_path();
	selected_path = top_path;
}

file_explorer::file_explorer(const char* _title) : window(_title)
{
	top_path = current_path();
	selected_path = top_path;
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

void file_explorer::draw_directory_entry(path _path, bool full_path)
{
	bool node_open = ImGui::TreeNode(_path.generic_string().c_str(), " ");
	ImGui::SameLine();
	char label[512] = {};
	get_path_string(_path, label, 512, full_path);
	if (ImGui::Selectable(label, false))
	{
		selected_path = _path;
	}
	if (node_open)
	{
		directory_iterator end_entry;
		for (directory_iterator entry(_path); entry != end_entry; entry++)
		{
			if (is_directory(entry->status()))
			{
				draw_directory_entry(entry->path());
			}
		}
		ImGui::TreePop();
	}
}

void file_explorer::draw_file_entry(path _path, bool full_path)
{
	char label[512] = {};
	get_path_string(_path, label, 512, full_path);
	if (ImGui::Selectable(label, false))
	{
		selected_file = _path;
	}
	ImGui::SameLine(300.0f);
	struct stat file_stat;
	stat(_path.generic_string().c_str(), &file_stat);
	if (is_regular_file(_path))
		ImGui::Text("%20d bytes", file_size(_path));
	else if (is_directory(_path))
		ImGui::Text("<dir>");
	else
		ImGui::Text("  -  ");
}

void file_explorer::show_directory_content()
{
	ImGui::Text("%s", selected_path.generic_string().c_str());
	ImGui::Separator();
	ImGui::Columns(4, "File Info");
	ImGui::Text("");
	directory_iterator end_entry;
	for (directory_iterator entry(selected_path); entry != end_entry; entry++)
	{
		draw_file_entry(entry->path());
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

}
