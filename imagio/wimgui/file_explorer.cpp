#include "file_explorer.h"

using namespace boost::filesystem;
namespace wimgui
{

void file_explorer::draw_directory_entry(path _path, bool full_path)
{
	bool node_open = ImGui::TreeNode(_path.generic_string().c_str(), " ");
	ImGui::SameLine();
	if (full_path)
		ImGui::Text("%s", _path.generic_string().c_str());
	else
		ImGui::Text("%s", _path.filename().generic_string().c_str());
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

void file_explorer::draw()
{
	ImGui::Columns(2, "File Explorer");
	ImGui::Separator();
	ImGui::BeginChild(ImGui::GetID("Directories"));

	path current = boost::filesystem::current_path();
	draw_directory_entry(current, true);
	ImGui::EndChild();
	ImGui::NextColumn();
	ImGui::Text("File list");
	ImGui::Columns(1);
	ImGui::Separator();
}

}
