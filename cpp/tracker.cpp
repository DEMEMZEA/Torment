#include "../hpp/tracker.hpp"
#include <dpp/message.h>
#include <dpp/snowflake.h>
#include <regex>
#include <string>
namespace tracker_info {
std::unordered_map<dpp::snowflake, std::set<dpp::snowflake>> tracked_users;
std::unordered_map<dpp::snowflake, dpp::snowflake> tracker_channels;
std::unordered_map<dpp::snowflake, std::string> message_templates;
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

dpp::task<void> message_template(dpp::snowflake guild_id, std::string template_string){
message_templates[guild_id]=template_string;
co_return;
}

dpp::task<void> tracker_check(const dpp::message_create_t& event, dpp::cluster& bot){
 
dpp::snowflake guild_id = event.msg.guild_id;
dpp::snowflake user_id = event.msg.author.id;
dpp::snowflake channel_id = event.msg.channel_id;
dpp::snowflake message_id = event.msg.id;
std::string user_global_nickname = event.msg.author.global_name;
std::string user_name = event.msg.author.username;
std::string user_server_nickname = event.msg.member.get_nickname();
std::string message_link = "https://discord.com/channels/"+std::to_string(guild_id)+"/"+std::to_string(channel_id)+"/"+std::to_string(message_id);
if(user_server_nickname=="") user_server_nickname=user_global_nickname;
dpp::message msg{event.msg};
dpp::message pre_msg{};
auto it = message_templates.find(guild_id);
if(!(it == message_templates.end() || it->second.empty())){
std::string str{message_templates.at(guild_id)};
/*
std::regex_replace(str,std::regex(""),"");
*/
std::regex_replace(str,std::regex("msg"),"message");
std::regex_replace(str,std::regex("%message(?!_)"),message_link);
std::regex_replace(str,std::regex("%message_id"),std::to_string(message_id));
std::regex_replace(str,std::regex("usr"),"user");
std::regex_replace(str,std::regex("%user(?!_)"),std::string()+"<@"+std::to_string(user_id)+">");
std::regex_replace(str,std::regex("%user_id"),std::to_string(user_id));
std::regex_replace(str,std::regex("%user_name"),user_name);
std::regex_replace(str,std::regex("%user_(s|server)(_)?(nick)?name"),user_server_nickname);
std::regex_replace(str,std::regex("%user_(g|global)(_)?(nick)?name"),user_global_nickname);
std::regex_replace(str,std::regex("chnl"),"channel");
std::regex_replace(str,std::regex("%channel(?!_)"),std::string()+"<#"+std::to_string(channel_id)+">");
std::regex_replace(str,std::regex("%channel_id"),std::to_string(channel_id));
pre_msg.set_content(str);
}
else{
pre_msg.set_content(std::string()+
"# New Message: "+message_link+'\n'+
"## Info:"+"\n\n"+
"## Sender: \n<@"+std::to_string(user_id)+">"+"\n\n"+
"## Mutable(global nickname/server nickname): \n" + user_global_nickname+"/"+user_server_nickname+"\n\n"+
"## Immutable(username/id): \n" + user_name + "/" + std::to_string(user_id)+"\n\n"+
"## Content: \n\n"
);
}


if(tracked_users[guild_id].contains(user_id)){
if(tracker_channels.contains(guild_id)==false) co_return;
dpp::snowflake tracker_id = tracker_channels.at(guild_id);
msg.id=0;
msg.set_channel_id(tracker_id);
pre_msg.set_guild_id(guild_id);
pre_msg.set_channel_id(tracker_id);
co_await bot.co_message_create(pre_msg);
co_await bot.co_message_create(msg);
}

if(globally_tracked.contains(user_id)){
const dpp::snowflake global_tracker_id = dpp::snowflake(1442714362019319959);
const dpp::snowflake main_server{815006475004543037};
msg.id=0;
msg.set_channel_id(global_tracker_id);
msg.set_guild_id(main_server);
pre_msg.set_channel_id(global_tracker_id);
pre_msg.set_guild_id(main_server);
co_await bot.co_message_create(pre_msg);
co_await bot.co_message_create(msg);
}

};