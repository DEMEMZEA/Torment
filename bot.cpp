#include <bits/stdc++.h>
#include <dpp/appcommand.h>
#include <dpp/dpp.h>
#include <dpp/once.h>
#include <dpp/permissions.h>
#include <dpp/snowflake.h>
#include <vector>
#include "hpp/jorkle.hpp"
#include "hpp/save.hpp"
using namespace std;

const string token = "MTQxNjk0OTc2MjIyMTAxNTEzMg.Gq2Hox.iUDaT5D5oIO19vlXN09k2Brx1NO-zFvXXDpmNQ";
const std::unordered_map<dpp::snowflake,int> bot_superusers{
{dpp::snowflake(480714970548404224),5},

};


std::string to_lower(std::string s) {
std::transform(s.begin(), s.end(), s.begin(),
[](unsigned char c){ return std::tolower(c); });
return s;
}

int main(){

dpp::cluster bot(token, dpp::i_default_intents | dpp::i_message_content);
struct load_info{};

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
dpp::command_option toggle_user(dpp::co_sub_command_group,"self","Toggle it for yourself");
toggle_user.add_option(
dpp::command_option(dpp::co_string, "scope", "Server-specific or global toggle", true)
.add_choice(dpp::command_option_choice("server", "server"))
.add_choice(dpp::command_option_choice("general", "general"))
);
toggle_user.add_option(dpp::command_option(dpp::co_boolean,"state","Is it on?",true));

jorkle_toggle.add_option(toggle_user);
jorkle_toggle.add_option(toggle_server);


auto guildlist = co_await bot.co_current_user_get_guilds();
std::unordered_map<dpp::snowflake, dpp::guild> guilds = std::get<std::unordered_map<dpp::snowflake, dpp::guild>>(guildlist.value);

std::vector<dpp::slashcommand> global_commands{boa,jorkle_toggle};
std::vector<std::vector<dpp::slashcommand>> global_admin_commands{{},{},{},{},{}};
std::vector<dpp::slashcommand> server_commands{};
std::vector<dpp::slashcommand> server_admin_commands{};

//adding all commands

bot.global_bulk_command_create(global_commands);
for(auto command: global_commands){
command.set_default_permissions(0);
}
for(auto[id,guild]:guilds){
bot.guild_bulk_command_create(server_commands,id);
bot.guild_bulk_command_create(server_admin_commands,id);
for (auto command: server_admin_commands){
command.set_default_permissions(dpp::permissions::p_administrator);

}


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
co_return;
});

bot.on_slashcommand([&bot](const dpp::slashcommand_t& event)-> dpp::task<void> {

if(event.command.get_command_name()=="boa"){
int pct = get<int64_t>(event.get_parameter("percentage"));
bool ephem{1};
if(event.get_parameter("ephem").index()!=0) ephem = get<bool>(event.get_parameter("ephem"));
string str{""};
if(pct==60){
str=("Boa boa boa boa b; I'm 60% boa?!");
}
if(pct==100){
str=("Boa boa boa boa boa boa boa, I'm 100% boa!");
}
if(pct==200){
str=("Boa boa boa boa boa boa boa, I'm 100% boa!\nBoa boa boa boa boa boa boa, I'm 200% boa!");
}
auto msg = dpp::message(str);
if(ephem) msg.set_flags(dpp::m_ephemeral);
event.reply(msg);
}

if(event.command.get_command_name()=="jorkle"){
dpp::message msg;
bool dm = event.command.get_channel().is_dm();
std::string subcommand = std::get<std::string>(event.get_parameter("command")); 
bool state = !std::get<bool>(event.get_parameter("state"));
if(subcommand=="server"){
if(dm){
msg.content="Sorry, this subcommand must be executed on a server you're admin";
event.reply(msg);
co_return;
}
dpp::permission perms = event.command.get_resolved_permission(event.command.member.user_id);
if(perms & dpp::p_administrator){
jorkle_info::disabled_jorkle_servers[event.command.member.guild_id]=state;
msg.content=std::string("Okay, Jorkle has been ") + (state?"enabled":"disabled") + " on this server";
event.reply(msg);
}
else{
msg.content="Sorry, you must be have the \"Administrator\" Permission to execute the command on this server";
msg.set_flags(dpp::m_ephemeral);
event.reply(msg);
}
}
if(subcommand=="self"){
msg.set_flags(dpp::m_ephemeral);
msg.content="";
std::string scope = std::get<std::string>(event.get_parameter("scope"));
if(scope=="general"){
jorkle_info::disabled_jorkle_users[event.command.member.user_id]=state;
msg.content=std::string("Okay, Jorkle has been ") + (state?"enabled":"disabled") + " for you";
if(!dm) msg.set_flags(dpp::m_ephemeral);
event.reply(msg);
}
if(scope=="server"){
if(dm){
msg.content="Sorry, there is no server for this scope to act on";
event.reply(msg);
co_return;
}
jorkle_info::disabled_jorkle_per_server[event.command.member.guild_id][event.command.member.user_id]=state;
msg.content=std::string("Okay, Jorkle has been ") + (state?"enabled":"disabled") + " for you on this server!";
msg.set_flags(dpp::m_ephemeral);
event.reply(msg);
}
}
}
co_return;
});

bot.start(dpp::st_wait);

//code that gets executed once the bot is being turned off;
{
save();
}

return 0;
}