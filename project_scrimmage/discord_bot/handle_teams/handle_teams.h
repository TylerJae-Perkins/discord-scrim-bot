#pragma once
#include "../discord.h"

#include <string>

namespace handle_teams {
	channel_info_t get_channel_info(std::string team_name);
	void add_new_team(std::string team_name, std::string challenge, std::string group_chat, std::string team_leader);
	void remove_team(std::string team_name);
}