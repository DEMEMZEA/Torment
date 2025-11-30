#ifndef JORKLE_HPP
#define JORKLE_HPP
#include <dpp/dpp.h>
#include <bits/stdc++.h>
void cant_jorkle(const dpp::message_create_t& event, int reason);
dpp::task<void> jorkle(const dpp::message_create_t& event, dpp::cluster& bot, int extra_dice);
namespace jorkle_info{
extern std::unordered_map<dpp::snowflake, std::unordered_map<dpp::snowflake, double>>cooldowns_per_server;
extern std::unordered_map<dpp::snowflake, std::unordered_map<dpp::snowflake, double>>server_points;
extern std::unordered_map<dpp::snowflake, std::unordered_map<dpp::snowflake, std::pair<int,int>>>extra_server_dice; // attack and defense, in that order
extern std::unordered_map<dpp::snowflake, std::pair<int,int>> extra_global_dice; // attack and defense, in that order
extern std::unordered_map<dpp::snowflake, bool> disabled_jorkle_servers;
extern std::unordered_map<dpp::snowflake, bool> disabled_jorkle_users;
extern std::unordered_map<dpp::snowflake, std::unordered_map<dpp::snowflake, bool>> disabled_jorkle_per_server;
extern std::unordered_map<dpp::snowflake,std::unordered_map<dpp::snowflake, time_t>> admin_jorkled;
extern double cooldown;
};
#endif