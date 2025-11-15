#include "../hpp/jorkle.hpp"

namespace jorkle_info {
std::unordered_map<dpp::snowflake, std::unordered_map<dpp::snowflake, dpp::snowflake>>cooldowns_per_server;
std::unordered_map<dpp::snowflake, std::unordered_map<dpp::snowflake, double>>server_points;
std::unordered_map<dpp::snowflake, std::unordered_map<dpp::snowflake, std::pair<int,int>>>extra_server_dice; // attack and defense, in that order
std::unordered_map<dpp::snowflake, std::pair<int,int>> extra_global_dice; // attack and defense, in that order
constinit long long int cooldown{24*60*60*1000*1<<12};
}
using namespace jorkle_info;

void cant_jorkle(const dpp::message_create_t& event, dpp::cluster* bot, int reason){

}

void jorkle(const dpp::message_create_t& event, dpp::cluster& bot){
if(event.msg.message_reference.message_id==0)return; // make sure it replies
dpp::snowflake time = event.msg.id;
dpp::snowflake server_id = event.msg.guild_id;
dpp::snowflake jorkler_id = event.msg.author.id;
if(time-cooldowns_per_server[server_id][jorkler_id]<cooldown)return; // make sure the cooldown is over


bot.message_get(event.msg.id,event.msg.channel_id,[&bot,&server_id,&jorkler_id,&time]
    (const dpp::confirmation_callback_t& cb) {
    if (cb.is_error()) return;
    const dpp::message& original = std::get<dpp::message>(cb.value);
    dpp::snowflake jorkled_id=original.author.id;

int dice{3};

dice += extra_server_dice[server_id][jorkler_id].first;
dice += extra_global_dice[jorkler_id].first;
dice -= extra_server_dice[server_id][jorkled_id].second;
dice -= extra_global_dice[jorkled_id].second;
double chance;
double jorkler_pts = server_points[server_id][jorkler_id];
double jorkled_pts = server_points[server_id][jorkled_id];

});}