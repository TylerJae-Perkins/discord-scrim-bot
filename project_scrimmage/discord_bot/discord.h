#pragma once

#include <dpp/dpp.h>
#include <string>
#include <stdint.h>
#include <unordered_map>
#include "defines.h"

struct channel_info_t {
	std::uint64_t challenge;//challenged notification chat
	std::uint64_t group_chat;//teams general chat
	std::uint64_t team_leader;//leaders ID
};

namespace discord {
	void initalize();
	void send_log(std::string log, std::string title, std::uint64_t channel_id);
	void send_checker(std::string log, std::string title, std::uint64_t channel_id, std::string challenger);

	inline std::unordered_map<std::string, channel_info_t> channels;
	inline dpp::cluster* bot;
};