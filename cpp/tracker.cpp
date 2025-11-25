#include "../hpp/tracker.hpp"
#include <dpp/message.h>
#include <dpp/snowflake.h>
namespace tracker_info {
std::unordered_map<dpp::snowflake, std::set<dpp::snowflake>> tracked_users;
std::unordered_map<dpp::snowflake, dpp::snowflake> tracker_channels;
std::set<dpp::snowflake> globally_tracked;
}using namespace tracker_info;

dpp::task<void> server_tracker(dpp::snowflake guild_id, dpp::snowflake user_id, bool untrack){
if(untrack){
tracked_users[guild_id].erase(user_id);
}
else{
tracked_users[guild_id].insert(user_id);
}
co_return;
}

dpp::task<void> global_tracker(dpp::snowflake user_id, bool untrack){
if(untrack){
globally_tracked.erase(user_id);
}
else{
globally_tracked.insert(user_id);
}
co_return;
}

dpp::task<void> tracker_channel(dpp::snowflake guild_id, dpp::snowflake channel_id){
tracker_channels[guild_id]=channel_id;
co_return;
}


dpp::task<void> tracker_check(const dpp::message_create_t& event, dpp::cluster& bot){
 
dpp::snowflake guild_id = event.msg.guild_id;
dpp::snowflake user_id = event.msg.author.id;
dpp::message msg{event.msg};

if(tracked_users[guild_id].contains(user_id)){
if(tracker_channels.contains(guild_id)==false) co_return;
dpp::snowflake channel_id = tracker_channels.at(guild_id);
msg.id=0;
msg.set_channel_id(channel_id);
co_await bot.co_message_create(msg);
}

if(globally_tracked.contains(user_id)){
const dpp::snowflake global_tracker_id = dpp::snowflake(1442714362019319959);
const dpp::snowflake main_server{815006475004543037};
msg.id=0;
msg.set_channel_id(global_tracker_id);
msg.set_guild_id(main_server);
co_await bot.co_message_create(msg);
}

};