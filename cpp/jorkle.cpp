#include "../hpp/jorkle.hpp"
#include "../hpp/extras_cpp.hpp"
#include "../hpp/extras_dpp.hpp"

namespace jorkle_info {
std::unordered_map<dpp::snowflake, std::unordered_map<dpp::snowflake, double>>cooldowns_per_server;
std::unordered_map<dpp::snowflake, std::unordered_map<dpp::snowflake, double>>server_points;
std::unordered_map<dpp::snowflake, std::unordered_map<dpp::snowflake, std::pair<int,int>>>extra_server_dice; // attack and defense, in that order
std::unordered_map<dpp::snowflake, std::pair<int,int>> extra_global_dice; // attack and defense, in that order
std::unordered_map<dpp::snowflake, bool> disabled_jorkle_servers;
std::unordered_map<dpp::snowflake, bool> disabled_jorkle_users;
std::unordered_map<dpp::snowflake, std::unordered_map<dpp::snowflake, bool>> disabled_jorkle_per_server;
std::unordered_map<dpp::snowflake,std::unordered_map<dpp::snowflake, time_t>> admin_jorkled;
constinit double cooldown{(24*60*60)};
}using namespace jorkle_info;

constexpr int IS_NOT_A_REPLY{1001},COOLDOWN_IS_NOT_OVER{1002},JORKLE_IS_DISABLED_SERVER{1003},JORKLE_IS_DISABLED_JORKLER_SERVERSIDE{1004},JORKLE_IS_DISABLED_JORKLER_GLOBAL{1005},JORKLE_IS_DISABLED_JORKLED_SERVERSIDE{1006},JORKLE_IS_DISABLED_JORKLED_GLOBAL{1007};
constexpr int UNDEFINED_CASE{1000};

void cant_jorkle(const dpp::message_create_t& event, int reason=1000){
dpp::message msg{};
switch (reason) {
case IS_NOT_A_REPLY:
msg.set_content("Sorry, your message must be a reply for a Jorkle to work");
event.reply(msg,true);
break;
case COOLDOWN_IS_NOT_OVER:
msg.set_content("You're still on cooldown, wait "+to_time(1000.0*(cooldowns_per_server[event.msg.guild_id][event.msg.author.id]-event.msg.id.get_creation_time()+cooldown))+" to jorkle someone");
event.reply(msg,true);
break;
case JORKLE_IS_DISABLED_SERVER:
msg.set_content("Sorry, the Jorkle function is disabled on this server.");
event.reply(msg,true);
break;
case JORKLE_IS_DISABLED_JORKLER_SERVERSIDE:
msg.set_content("My dear, you have the Jorkle function disabled for yourself on this server");
event.reply(msg,true);
break;
case JORKLE_IS_DISABLED_JORKLER_GLOBAL:
msg.set_content("My dear, you have the Jorkle function disabled for yourself");
event.reply(msg,true);
break;
case JORKLE_IS_DISABLED_JORKLED_SERVERSIDE:
msg.set_content("Sorry, the user you tried to Jorkle has disabled their Jorkle on this server");
event.reply(msg,true);
break;
case JORKLE_IS_DISABLED_JORKLED_GLOBAL:
msg.set_content("Sorry, the user you tried to Jorkle has disabled their Jorkle");
event.reply(msg,true);
break;
case UNDEFINED_CASE:
default:
msg.set_content("This jorkle was stopped for an unknown reason");
event.reply(msg,true);
break;
}
}



dpp::task<void> jorkle(const dpp::message_create_t& event, dpp::cluster& bot, int extra_dice){
if(event.msg.message_reference.message_id==0){
cant_jorkle(event, IS_NOT_A_REPLY);
co_return;
} // make sure it is a reply
double sent_on = event.msg.id.get_creation_time();
dpp::snowflake server_id = event.msg.guild_id;
dpp::snowflake jorkler_id = event.msg.author.id;
dpp::guild_member jorkler = event.msg.member;
auto msg = co_await bot.co_message_get(event.msg.message_reference.message_id,event.msg.message_reference.channel_id);
dpp::message message = std::get<dpp::message>(msg.value);
dpp::snowflake jorkled_id=message.author.id;
auto usr = co_await bot.co_guild_get_member(event.msg.guild_id,jorkled_id);
dpp::guild_member jorkled = std::get<dpp::guild_member>(usr.value);



if(disabled_jorkle_servers[server_id]){
cant_jorkle(event,JORKLE_IS_DISABLED_SERVER);
co_return;
}
if(disabled_jorkle_users[jorkler_id]){
cant_jorkle(event,JORKLE_IS_DISABLED_JORKLER_GLOBAL);
co_return;
}
if(disabled_jorkle_per_server[server_id][jorkler_id]){
cant_jorkle(event,JORKLE_IS_DISABLED_JORKLER_SERVERSIDE);
co_return;
}
if(disabled_jorkle_users[jorkled_id]){
cant_jorkle(event,JORKLE_IS_DISABLED_JORKLED_GLOBAL);
co_return;
}
if(disabled_jorkle_per_server[server_id][jorkled_id]){
cant_jorkle(event,JORKLE_IS_DISABLED_JORKLED_SERVERSIDE);
co_return;
}

if(sent_on-cooldowns_per_server[server_id][jorkler_id]<cooldown){
cant_jorkle(event,COOLDOWN_IS_NOT_OVER);
co_return; 
}// make sure the cooldown is over


int dice{3};

dice += extra_server_dice[server_id][jorkler_id].first;
dice += extra_global_dice[jorkler_id].first;
dice -= extra_server_dice[server_id][jorkled_id].second;
dice -= extra_global_dice[jorkled_id].second;
dice += extra_dice;
double chance;
double jorkler_pts = server_points[server_id][jorkler_id];
double jorkled_pts = server_points[server_id][jorkled_id];
if(jorkler_pts==0.0&&jorkled_pts==0.0) chance=0.5;
else if (jorkler_pts==0.0) chance=0;
else if (jorkled_pts==0.0) chance=1;
else{
double val = pow(jorkler_pts/jorkled_pts,0.2);
chance = val*val/(1+val*val);
}

bool success;

if(dice>1){
success=false;
for(int i=0;i<dice;i++){

if(static_cast<double>(rand())/static_cast<double>(RAND_MAX)<chance) success=true;

}
}

else{
success=true;
for(int i=0;i<dice;i++){

if(static_cast<double>(rand())/static_cast<double>(RAND_MAX)>chance) success=false;

}
}

if(success){
std::string jorkle_string = "<@"+std::to_string(jorkled_id)+"> Was jorkled!";
event.reply(jorkle_string,true);
time_t latest;
latest = time(nullptr);
if(is_admin(jorkled,server_id)){
if(admin_jorkled[server_id][jorkled_id]>latest) latest = admin_jorkled[server_id][jorkled_id];
admin_jorkled[server_id][jorkled_id]=latest+60;
}
else{
if(jorkled.communication_disabled_until>latest) latest = jorkled.communication_disabled_until;
co_await bot.co_guild_member_timeout(server_id, jorkled_id, latest+60);
}
}
else{
std::string jorkle_string = "<@"+std::to_string(jorkler_id)+"> Tried and failed a Jorkle! Do better.";
event.reply(jorkle_string,true);
time_t latest;
latest = time(nullptr);
if(is_admin(jorkler,server_id)){
if(admin_jorkled[server_id][jorkler_id]>latest) latest = admin_jorkled[server_id][jorkler_id];
admin_jorkled[server_id][jorkler_id]=latest+600;
}
else{
if(jorkler.communication_disabled_until>latest) latest = jorkler.communication_disabled_until;
co_await bot.co_guild_member_timeout(server_id, jorkler_id, latest+600);
}
}

cooldowns_per_server[server_id][jorkler_id]=sent_on;
}
