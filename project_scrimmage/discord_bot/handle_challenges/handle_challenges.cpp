#include "handle_challenges.h"
#include "../discord.h"

void handle_challenges::challenger(std::string team_id, std::string challenger) {
	std::unordered_map<std::string, channel_info_t>::const_iterator found = discord::channels.find(team_id);
	if (found == discord::channels.end()) {
		discord::send_log("Unable to find team!", "", 1331032209217552598);
		return;
	}

	auto item = discord::channels.at(team_id);

	discord::send_checker(std::format("You have been challenged by {}!", challenger), "You've been challenged to a scrim!", item.challenge, challenger);
}