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
		for (auto& entry:
                boost::make_iterator_range(directory_iterator(_path), {}))
		{
			if (is_directory(entry))
			{
				draw_directory_entry(entry);
			}
		}
		ImGui::TreePop();
	}
}

void file_explorer::draw_directory_entry(directory_entry entry, bool full_path)
{
	draw_directory_entry(path(entry), full_path);
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
