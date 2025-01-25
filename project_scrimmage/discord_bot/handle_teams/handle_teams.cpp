#include "handle_teams.h"
#include "../../handle_files/handle_files.h"

channel_info_t handle_teams::get_channel_info(std::string team_name) {
	return channel_info_t();
}

void handle_teams::add_new_team(std::string team_name, std::string challenge, std::string group_chat, std::string team_leader) {
	channel_info_t info;
	std::istringstream st(challenge);
	st >> info.challenge;

	std::istringstream st2(group_chat);
	st2 >> info.group_chat;

	std::istringstream st3(team_leader);
	st3 >> info.team_leader;

	discord::channels.emplace(team_name, info);

	discord::send_log(std::format("Added New Team: {}", team_name), "", 1331032199218335784);

	handle_files::save_to_file();
}

void handle_teams::remove_team(std::string team_name) {
	if (discord::channels.empty()) {
		discord::send_log("NO TEAMS STORED! TELL TEAAJAY IMMEDIATELY!", "", 1331032209217552598);
		return;
	}

	std::unordered_map<std::string, channel_info_t>::const_iterator found = discord::channels.find(team_name);
	if (found == discord::channels.end()) {
		discord::send_log("Unable to find team!", "", 1331032209217552598);
		return;
	}

	discord::channels.erase(discord::channels.find(team_name));

	discord::send_log(std::format("Removed Team: {}", team_name), "", 1331032209217552598);

	handle_files::save_to_file();
}