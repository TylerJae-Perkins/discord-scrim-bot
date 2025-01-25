#include <iostream>
#include <json.hpp>
#include <dpp/dpp.h>
#include "discord_bot/discord.h"

int main() {
	DWORD previous_mode;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &previous_mode);
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | (previous_mode & ~ENABLE_QUICK_EDIT_MODE));

	HANDLE std_output_handle = GetStdHandle(((DWORD)-11));
	SetConsoleTextAttribute(std_output_handle, 2);
	printf("[MESSAGE] : Server is now active! \n");

	std::thread discord_thread([]() {
		discord::initalize();
	});

	while (true) {

	}
}