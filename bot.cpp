#include <bits/stdc++.h>
#include "hpp/extras.hpp"
#include "hpp/jorkle.hpp"
#include "hpp/save.hpp"
#include "hpp/tracker.hpp"
#include <dpp/dpp.h>
using namespace std;

const string token = "MTQxNjk0OTc2MjIyMTAxNTEzMg.Gq2Hox.iUDaT5D5oIO19vlXN09k2Brx1NO-zFvXXDpmNQ";
const std::unordered_map<dpp::snowflake,int> bot_superusers{
{dpp::snowflake(480714970548404224),5},

};
const dpp::snowflake mainGuild_id{815006475004543037};

std::string to_lower(std::string s) {
std::transform(s.begin(), s.end(), s.begin(),
[](unsigned char c){ return std::tolower(c); });
return s;
}

void signalHandler(int signalNum) {
    save();
    cout << "\n\n\nPlease, avoid quitting the program with Ctrl+C, please try closing it with one of the proper shutdown commands\n\n\n" << endl;
    exit(signalNum);
}

int main(){

dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);
struct load_info{};
signal(SIGINT, signalHandler);



bot.on_log(dpp::utility::cout_logger());




bot.on_ready([&bot](const dpp::ready_t& event)-> dpp::task<void>{
(void)event;
if(dpp::run_once<struct load_info>()){
load();
}
if (dpp::run_once<struct register_bot_commands>()) {


dpp::slashcommand boa("boa","100% boa??",bot.me.id);
boa.add_option(
dpp::command_option(dpp::co_integer, "percentage","How much boa?", true)
.add_choice(dpp::command_option_choice("60%",60))
.add_choice(dpp::command_option_choice("100%",100))
.add_choice(dpp::command_option_choice("200%",200))
);
boa.add_option(dpp::command_option(dpp::co_boolean,"ephem","Is this message going to be ephemeral(just for the user) or not? True by default",false)
);

dpp::slashcommand jorkle_toggle("jorkle","Command to toggle Jorkle on/off",bot.me.id);
dpp::command_option toggle_server(dpp::co_sub_command,"server","Toggle it for the server (needs admin)");
toggle_server.add_option(
dpp::command_option(dpp::co_boolean,"state","will it be enabled?",true)
);
dpp::command_option toggle_user(dpp::co_sub_command,"self","Toggle it for yourself");
toggle_user.add_option(
dpp::command_option(dpp::co_string, "scope", "Server-specific or global toggle", true)
.add_choice(dpp::command_option_choice("server", "server"))
.add_choice(dpp::command_option_choice("general", "general"))
);
toggle_user.add_option(dpp::command_option(dpp::co_boolean,"state","Is it on?",true));

jorkle_toggle.add_option(toggle_user);
jorkle_toggle.add_option(toggle_server);

dpp::slashcommand permission_sync("perm-sync","Command to synchronize the permission of channels on a given category to that category's permissions",bot.me.id);
dpp::command_option sync_channel(dpp::co_sub_command,"channel","sync the permissions of a channel to its category");
sync_channel.add_option(
dpp::command_option(dpp::co_channel,"channel","The channel which you're going to sync",true)
);
dpp::command_option sync_category(dpp::co_sub_command,"category","sync the permissions of all channels on this category");
sync_category.add_option(
dpp::command_option(dpp::co_channel,"category","The category which you're going to sync",true).add_channel_type( dpp::CHANNEL_CATEGORY)
);
// permission_sync.add_option(
// dpp::command_option(dpp::co_channel,"category","The category which you're going to sync",true).add_channel_type( dpp::CHANNEL_CATEGORY)
// );
// permission_sync.add_option(
// dpp::command_option(dpp::co_channel,"channel","The channel which you're going to sync (if you don't say one, it syncs the whole category)",false)
// );
permission_sync.add_option(sync_channel);
permission_sync.add_option(sync_category);

dpp::slashcommand tracker_track("track","Command used to track suspicious users",bot.me.id);
tracker_track.add_option(
dpp::command_option(dpp::co_user,"user","User you want to start tracking",true)
);
dpp::slashcommand tracker_untrack("untrack","Command used to stop tracking users if you don't find them sus anymore",bot.me.id);
tracker_untrack.add_option(
dpp::command_option(dpp::co_user,"user","User you want to stop tracking",true)
);
dpp::slashcommand tracker_channel("tracker_channel","Set the channel that the messages of the tracked members will go to",bot.me.id);
tracker_channel.add_option(
dpp::command_option(dpp::co_channel,"channel","channel the messages will go to",true)
);
dpp::slashcommand global_tracker("global_tracker","",bot.me.id);
global_tracker.add_option(
dpp::command_option(dpp::co_string,"id","the id of the user you want to start tracking",true)
);
global_tracker.add_option(
dpp::command_option(dpp::co_boolean,"untrack","do you want to stop tracking this user?",false)
);

auto guildlist = co_await bot.co_current_user_get_guilds();
std::unordered_map<dpp::snowflake, dpp::guild> guilds = std::get<std::unordered_map<dpp::snowflake, dpp::guild>>(guildlist.value);

std::vector<dpp::slashcommand> global_commands{boa,jorkle_toggle};

std::vector<dpp::slashcommand> all_mainGuild_commands{};
std::unordered_map<uint64_t, std::vector<dpp::slashcommand>> mainGuild_commands={
{dpp::permissions::p_administrator,{global_tracker}}
};

std::vector<dpp::slashcommand> all_guild_commands{};
std::unordered_map<uint64_t, std::vector<dpp::slashcommand>> guild_commands={
{dpp::permissions::p_use_application_commands,{jorkle_toggle}},
{dpp::permissions::p_manage_channels,{permission_sync}},
{dpp::permissions::p_administrator,{tracker_track,tracker_untrack}}
};


//adding all commands

for (auto& [perm,commandlist]: guild_commands){
all_guild_commands+=commandlist;
for (auto command:commandlist){
command.set_default_permissions(perm);
}}

bot.global_bulk_command_create(global_commands);
for(auto[id,guild]:guilds){
bot.guild_bulk_command_create(all_guild_commands,id);
if(id!=mainGuild_id) continue;
bot.guild_bulk_command_create(all_mainGuild_commands,id);
}

}
});



bot.on_message_create([&bot](const dpp::message_create_t& event)-> dpp::task<void> {
// Ignore bot messages (good practice)
if (event.msg.author.is_bot()){
co_return;
}

// Example: check message content
if (to_lower(event.msg.content) == to_lower("Mods, Jorkle 'em!")) {
if(event.msg.content=="Mods, Jorkle 'em!") co_await jorkle(event,bot,1);
else co_await jorkle(event,bot,0);
}
if(event.msg.content=="&shutdown"&&event.msg.is_dm()&&event.msg.author.id==480714970548404224){
bot.shutdown();
}


co_await tracker_check(event,bot);

co_return;
});



bot.on_slashcommand([&bot](const dpp::slashcommand_t& event)-> dpp::task<void> {
const std::string command{event.command.get_command_name()};
dpp::message msg{dpp::message("")};



if(command=="boa"){
int pct = std::get<std::int64_t>(event.get_parameter("percentage"));
bool ephem{1};
if(event.get_parameter("ephem").index()!=0) ephem = std::get<bool>(event.get_parameter("ephem"));
std::string str{""};
if(pct==60){
str=("Boa boa boa boa b; I'm 60% boa?!");
}
if(pct==100){
str=("Boa boa boa boa boa boa boa, I'm 100% boa!");
}
if(pct==200){
str=("Boa boa boa boa boa boa boa, I'm 100% boa!\nBoa boa boa boa boa boa boa, I'm 200% boa!");
}
msg.set_content(str);
if(ephem) msg.set_flags(dpp::m_ephemeral);

}



if(command=="jorkle"){
bool dm = event.command.get_channel().is_dm();
std::string subcommand = event.command.get_command_interaction().options[0].name;
bool state = !std::get<bool>(event.get_parameter("state"));

if(subcommand=="server"){
if(dm){
msg.set_content("Sorry, this subcommand must be executed on a server you're admin");
co_return;
}
dpp::permission perms = event.command.get_resolved_permission(event.command.member.user_id);
if(perms & dpp::p_administrator){
jorkle_info::disabled_jorkle_servers[event.command.member.guild_id]=state;
msg.set_content(std::string("Okay, Jorkle has been ") + (state?"disabled":"enabled") + " on this server");
}
else{
msg.set_content("Sorry, you must be have the \"Administrator\" Permission to execute the command on this server");
msg.set_flags(dpp::m_ephemeral);
}
}

if(subcommand=="self"){
msg.set_flags(dpp::m_ephemeral);
;
std::string scope = std::get<std::string>(event.get_parameter("scope"));
if(scope=="general"){
jorkle_info::disabled_jorkle_users[event.command.member.user_id]=state;
msg.set_content(std::string("Okay, Jorkle has been ") + (state?"disabled":"enabled") + " for you");
if(!dm) msg.set_flags(dpp::m_ephemeral);
}
if(scope=="server"){
if(dm){
msg.set_content("Sorry, there is no server for this scope to act on");
co_return;
}
jorkle_info::disabled_jorkle_per_server[event.command.member.guild_id][event.command.member.user_id]=state;
msg.set_content(std::string("Okay, Jorkle has been ") + (state?"disabled":"enabled") + " for you on this server!");
msg.set_flags(dpp::m_ephemeral);
}
}

}



if(command=="permsync"){
event.thinking(true);
std::string subcommand = event.command.get_command_interaction().options[0].name;

if(subcommand=="channel"){

dpp::snowflake channel_id = std::get<dpp::snowflake>(event.get_parameter("channel"));
dpp::channel* channelptr = dpp::find_channel(channel_id);
if(channelptr==nullptr){
co_return;
}
auto channel=*channelptr;
dpp::channel* categoryptr = dpp::find_channel(channel.parent_id);
if(categoryptr==nullptr){
co_return;
}
auto category=*categoryptr;
std::vector<dpp::permission_overwrite> perms = category.permission_overwrites;
for(auto& perm: channel.permission_overwrites){
co_await bot.co_channel_delete_permission(channel, perm.id);
}
for(auto& perm: perms){
co_await bot.co_channel_edit_permissions(channel_id,perm.id,perm.allow,perm.deny,perm.type);
}
msg.set_content("Done! Permissons syncronized for channel "+channel.name); 
msg.set_flags(dpp::m_ephemeral);
}

if(subcommand=="category"){

dpp::snowflake category_id = std::get<dpp::snowflake>(event.get_parameter("category"));
dpp::channel* categoryptr = dpp::find_channel(category_id);
if(categoryptr==nullptr){
co_return;
}
auto category=*categoryptr;
auto channellist = co_await bot.co_channels_get(event.command.guild_id);
dpp::channel_map channels = std::get<dpp::channel_map>(channellist.value);
std::vector<dpp::permission_overwrite> perms = category.permission_overwrites;
for(auto& [id,channel]: channels){
if(channel.parent_id!=category.id) continue;
for(auto& perm: channel.permission_overwrites){
co_await bot.co_channel_delete_permission(channel, perm.id);
}
for(auto& perm: perms){
co_await bot.co_channel_edit_permissions(channel.id,perm.id,perm.allow,perm.deny,perm.type);
}
}
msg.set_content("Done! Permissons syncronized for category "+category.name); 
msg.set_flags(dpp::m_ephemeral);

}

}



if(command=="track"){

dpp::snowflake guild_id = event.command.guild_id;
dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));
co_await server_tracker(guild_id,user_id);
msg.set_content("Done, the given user is now being tracked");
msg.set_flags(dpp::m_ephemeral);

}


if(command=="untrack"){

dpp::snowflake guild_id = event.command.guild_id;
dpp::snowflake user_id = std::get<dpp::snowflake>(event.get_parameter("user"));
co_await server_tracker(guild_id,user_id,true);
msg.set_content("Done, the given user is now not being tracked anymore");
msg.set_flags(dpp::m_ephemeral);

}



if(command=="tracker_channel"){

dpp::snowflake guild_id = event.command.guild_id;
dpp::snowflake channel_id = std::get<dpp::snowflake>(event.get_parameter("channel"));
co_await tracker_channel(guild_id,channel_id);
msg.set_content("Okay, selected channel <#"+std::to_string(channel_id)+"> ");
msg.set_flags(dpp::m_ephemeral);

}



if(command=="global_tracker"){

dpp::snowflake user_id = stoull(std::get<std::string>(event.get_parameter("id")));
bool untrack{0};
if(event.get_parameter("untrack").index()!=0) untrack = std::get<bool>(event.get_parameter("untrack"));
co_await global_tracker(user_id,untrack);

}



event.reply(msg);
co_return;
});



bot.start(dpp::st_wait);



std::jthread timed_thread([](std::stop_token st) {
int i{0};
while (!st.stop_requested()) {
std::this_thread::sleep_for(40ms);
i++;
if(i%90000)save();
}
});

//code that gets executed once the bot is being turned off, if it gets turned off correctly;

{
cout << "\nending..." << endl;
save();
cout << "\nSaving ended!" << endl;
timed_thread.request_stop();
cout << "\nAutosaver ended" << endl;
cout << "\nEverything finished. Ready to shut down."<<endl;
}


return 0;
}