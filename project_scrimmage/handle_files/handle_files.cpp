#include "handle_files.h"
#include <filesystem>

void handle_files::save_to_file() {
	CHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	std::string filepath = std::string(buffer).substr(0, pos) + "\\Teams\\";

	if (!std::filesystem::exists(filepath)) {
		if (!std::filesystem::create_directories(filepath)) {

		}
	}

	std::string strFileName = "teams";
	strFileName += ".txt";
	std::filesystem::path file(strFileName);

	std::ofstream ofsOutFile(filepath / file, std::ios::out | std::ios::trunc);

	if (!ofsOutFile.good()) {
		ofsOutFile.close();
	}

	std::ifstream loaded_file;
	loaded_file.open(filepath / file, std::ifstream::binary);

	nlohmann::json json;

	if (!loaded_file.good())
		return;

	try {
		if (loaded_file.peek() != std::ifstream::traits_type::eof())
			loaded_file >> json;

		loaded_file.close();
	}
	catch (std::ifstream::failure& ex) {
		discord::send_log(std::format("failed to load file: {}", ex.what()), "ERROR", 1331045016268832881);
		return;
	}

	for (auto& data : discord::channels) {
		json[data.first]["challenge"] = data.second.challenge;
		json[data.first]["group_chat"] = data.second.group_chat;
		json[data.first]["team_leader"] = data.second.team_leader;
	}

	std::ofstream store_file;
	store_file.open(filepath / file, std::ios::out | std::ios::trunc);

	store_file << json.dump(4);

	store_file.close();
}

void handle_files::load_from_file() {
	if (!discord::channels.empty())
		discord::channels.clear();

	CHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	std::string filepath = std::string(buffer).substr(0, pos) + "\\teams\\";

	std::string strFileName = "teams";
	strFileName += ".txt";
	std::filesystem::path file(strFileName);

	std::ifstream loaded_file;
	loaded_file.open(filepath / file, std::ifstream::binary);

	nlohmann::json json;

	if (!loaded_file.good()) {
		discord::send_log("failed to find file", "ERROR", 1331045016268832881);
		return;
	}

	try {
		if (loaded_file.peek() != std::ifstream::traits_type::eof())
			loaded_file >> json;

		loaded_file.close();

		for (auto& vars : json.items()) {
			channel_info_t info;
			info.challenge = vars.value()["challenge"].get<std::uint64_t>();
			info.group_chat = vars.value()["group_chat"].get<std::uint64_t>();
			info.team_leader = vars.value()["team_leader"].get<std::uint64_t>();

			discord::channels.emplace(vars.key(), info);
		}
	}
	catch (std::ifstream::failure& ex)
	{
		discord::send_log(std::format("failed to load file: {}", ex.what()), "ERROR", 1331045016268832881);
		return;
	}
}