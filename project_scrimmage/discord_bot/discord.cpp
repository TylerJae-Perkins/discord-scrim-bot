#include "discord.h"

#include <dpp/unicode_emoji.h>

#include <string>

#include "../hasher/hasher.h"
#include "../handle_files/handle_files.h"

#include "handle_challenges/handle_challenges.h"
#include "handle_teams/handle_teams.h"

void discord::initalize() {
    dpp::cluster sbot(BOT_KEY);
    handle_files::load_from_file();

    sbot.on_slashcommand([&sbot](const dpp::slashcommand_t& event) {
        auto command = FNV1A(event.command.get_command_name());

        switch (command) {
        case FNV1A("challenge_team"): {
            dpp::snowflake team_name = std::get<dpp::snowflake>(event.get_parameter("team_name"));
            std::string challenger = event.command.get_issuing_user().username;

            handle_challenges::challenger(team_name.str(), challenger);
            event.reply(dpp::message("Successfully Challenged!").set_flags(dpp::m_ephemeral));
        }
        break;
        case FNV1A("add_team"): {
            dpp::snowflake team_name = std::get<dpp::snowflake>(event.get_parameter("team_name"));
            std::string challenge = std::get<std::string>(event.get_parameter("challenge"));
            std::string group_chat = std::get<std::string>(event.get_parameter("group_chat"));
            std::string team_leader = std::get<std::string>(event.get_parameter("team_leader"));

            handle_teams::add_new_team(team_name.str(), challenge, group_chat, team_leader);

            event.reply(dpp::message("Successfully Add New Team!").set_flags(dpp::m_ephemeral));
        }
        break;
        case FNV1A("remove_team"): {
            dpp::snowflake team_name = std::get<dpp::snowflake>(event.get_parameter("team_name"));

            handle_teams::remove_team(team_name.str());

            event.reply(dpp::message("Successfully Removed Team!").set_flags(dpp::m_ephemeral));
        }
        break;
        }
    });

    sbot.on_ready([&sbot](const dpp::ready_t& event) {
        dpp::slashcommand challenge_team("challenge_team", "challenge a team to a scrim", sbot.me.id);
        challenge_team.add_option(
            dpp::command_option(dpp::co_role, "team_name", "team", true)
        );

        dpp::slashcommand add_team("add_team", "add a team to the bot", sbot.me.id);
        add_team.add_option(dpp::command_option(dpp::co_role, "team_name", "team", true));
        add_team.add_option(dpp::command_option(dpp::co_string, "challenge", "challenge", true));
        add_team.add_option(dpp::command_option(dpp::co_string, "group_chat", "general", true));
        add_team.add_option(dpp::command_option(dpp::co_string, "team_leader", "leader", true));

        dpp::slashcommand remove_team("remove_team", "removed a team", sbot.me.id);
        remove_team.add_option(dpp::command_option(dpp::co_role, "team_name", "team", true));

        sbot.guild_bulk_command_create({ add_team, remove_team, challenge_team }, 1330886912927469618);
    });

    sbot.on_select_click([&sbot](const dpp::select_click_t& event) {
        std::string command_name = event.command.get_command_name();
        auto command = FNV1A(command_name);

        switch (FNV1A(event.values[0])) {
        case FNV1A("accept"):
            event.reply("You accepted the scrim!");
            sbot.direct_message_create(event.custom_id, dpp::message("Your scrim request was accepted!"));
            break;
        case FNV1A("decline"):
            event.reply("You declined the scrim!");
            sbot.direct_message_create(event.custom_id, dpp::message("Your scrim request was declined!"));
            break;
        }
    });

    bot = &sbot;
    sbot.start(dpp::st_wait);
}

void discord::send_log(std::string log, std::string title, std::uint64_t channel_id)
{
    dpp::embed embed = dpp::embed()
        .set_color(0xFF0099)
        .set_title(title)
        .add_field("", log, true)
        .set_timestamp(time(0));

    auto msg = dpp::message(channel_id, embed);
    msg.set_guild_id(GUILD_ID);
    bot->message_create(msg);
}

void discord::send_checker(std::string log, std::string title, std::uint64_t channel_id, std::string challenger)
{
    dpp::embed embed = dpp::embed()
        .set_color(0xFF0099)
        .set_title(title)
        .add_field("", log, true)
        .set_timestamp(time(0));

    auto msg = dpp::message(channel_id, embed);

    msg.add_component(
        dpp::component().add_component(
            dpp::component()
            .set_type(dpp::cot_selectmenu)
            .set_placeholder("Do you accept the scrim?")
            .add_select_option(dpp::select_option("accept", "accept", "Accept Scrim"))
            .add_select_option(dpp::select_option("decline", "decline", "Decline Scrim"))
            .set_label(challenger)
        )
    );
    msg.set_guild_id(GUILD_ID);
    bot->message_create(msg);
}