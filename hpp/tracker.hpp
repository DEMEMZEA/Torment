#ifndef TRACKER_HPP
#define TRACKER_HPP
#include <dpp/dpp.h>
#include <bits/stdc++.h>
dpp::task<void> server_tracker(dpp::snowflake guild_id, dpp::snowflake user_id, bool untrack=false);
dpp::task<void> global_tracker(dpp::snowflake user_id, bool untrack=false);
dpp::task<void> tracker_channel(dpp::snowflake guild_id, dpp::snowflake channel_id);
dpp::task<void> tracker_check(const dpp::message_create_t& event, const dpp::cluster& bot);
namespace tracker_info {
extern std::unordered_map<dpp::snowflake, std::set<dpp::snowflake>> tracked_users;
extern std::unordered_map<dpp::snowflake, dpp::snowflake> tracker_channels;
extern std::set<dpp::snowflake> globally_tracked;
}
#endif