#include "../hpp/save.hpp"
#include "../hpp/jorkle.hpp" // for namespace jorkle_info
#include "../hpp/tracker.hpp" // for namespace tracker_info
#include <dpp/snowflake.h>
using json = nlohmann::json;



template<typename T1, typename T2>
inline void to_json(json& j, const std::pair<T1,T2>& p) {
j = { p.first, p.second };
}

template<typename T1, typename T2>
inline void from_json(const json& j, std::pair<T1,T2>& p) {
p.first = j.at(0).get<T1>();
p.second = j.at(1).get<T2>();
}

void save_jorkle() {

const std::string filename="data/jorkle_info.json";
json j;

// cooldowns_per_server
for (auto& [server_id, inner] : jorkle_info::cooldowns_per_server) {
json inner_j;
for (auto& [user_id, val] : inner)
inner_j[std::to_string(user_id)] = val;
j["cooldowns_per_server"][std::to_string(server_id)] = inner_j;
}

// server_points
for (auto& [server_id, inner] : jorkle_info::server_points) {
json inner_j;
for (auto& [user_id, val] : inner)
inner_j[std::to_string(user_id)] = val;
j["server_points"][std::to_string(server_id)] = inner_j;
}

// extra_server_dice
for (auto& [server_id, inner] : jorkle_info::extra_server_dice) {
json inner_j;
for (auto& [user_id, val] : inner)
inner_j[std::to_string(user_id)] = val; // pair<int,int> handled by to_json
j["extra_server_dice"][std::to_string(server_id)] = inner_j;
}

// extra_global_dice
for (auto& [id, val] : jorkle_info::extra_global_dice)
j["extra_global_dice"][std::to_string(id)] = val;

// disabled_jorkle_servers
for (auto& [id, val] : jorkle_info::disabled_jorkle_servers)
j["disabled_jorkle_servers"][std::to_string(id)] = val;

// disabled_jorkle_users
for (auto& [id, val] : jorkle_info::disabled_jorkle_users)
j["disabled_jorkle_users"][std::to_string(id)] = val;

//disabled_jorkle_per_server
for (auto& [server_id, inner] : jorkle_info::disabled_jorkle_per_server) {
json inner_j;
for (auto& [user_id, val] : inner)
inner_j[std::to_string(user_id)] = val;
j["disabled_jorkle_per_server"][std::to_string(server_id)] = inner_j;
}

// Write to file
std::ofstream out(filename);
out << j.dump(4);
}

void load_jorkle() {
const std::string filename="data/jorkle_info.json";
std::ifstream in(filename);
if (!in.is_open()) return;

json j;
in >> j;

// cooldowns_per_server
jorkle_info::cooldowns_per_server.clear();
for (auto& [server_str, inner_j] : j["cooldowns_per_server"].items()) {
dpp::snowflake server_id = std::stoull(server_str);
std::unordered_map<dpp::snowflake, double> inner_map;
for (auto& [user_str, val] : inner_j.items())
inner_map[std::stoull(user_str)] = val.get<double>();
jorkle_info::cooldowns_per_server[server_id] = inner_map;
}

// server_points
jorkle_info::server_points.clear();
for (auto& [server_str, inner_j] : j["server_points"].items()) {
dpp::snowflake server_id = std::stoull(server_str);
std::unordered_map<dpp::snowflake, double> inner_map;
for (auto& [user_str, val] : inner_j.items())
inner_map[std::stoull(user_str)] = val.get<double>();
jorkle_info::server_points[server_id] = inner_map;
}

// extra_server_dice
jorkle_info::extra_server_dice.clear();
for (auto& [server_str, inner_j] : j["extra_server_dice"].items()) {
dpp::snowflake server_id = std::stoull(server_str);
std::unordered_map<dpp::snowflake, std::pair<int,int>> inner_map;
for (auto& [user_str, pair_j] : inner_j.items())
inner_map[std::stoull(user_str)] = pair_j.get<std::pair<int,int>>();
jorkle_info::extra_server_dice[server_id] = inner_map;
}

// extra_global_dice
jorkle_info::extra_global_dice.clear();
for (auto& [id_str, pair_j] : j["extra_global_dice"].items())
jorkle_info::extra_global_dice[std::stoull(id_str)] = pair_j.get<std::pair<int,int>>();

// disabled_jorkle_servers
jorkle_info::disabled_jorkle_servers.clear();
for (auto& [id_str, val] : j["disabled_jorkle_servers"].items())
jorkle_info::disabled_jorkle_servers[std::stoull(id_str)] = val.get<bool>();

// disabled_jorkle_users
jorkle_info::disabled_jorkle_users.clear();
for (auto& [id_str, val] : j["disabled_jorkle_users"].items())
jorkle_info::disabled_jorkle_users[std::stoull(id_str)] = val.get<bool>();

//disabled_jorkle_per_server
jorkle_info::disabled_jorkle_per_server.clear();
for (auto& [server_str, inner_j] : j["disabled_jorkle_per_server"].items()) {
dpp::snowflake server_id = std::stoull(server_str);
std::unordered_map<dpp::snowflake, bool> inner_map;
for (auto& [user_str, val] : inner_j.items())
inner_map[std::stoull(user_str)] = val.get<bool>();
jorkle_info::disabled_jorkle_per_server[server_id] = inner_map;
}
}

void save_tracker(){

const std::string filename="data/tracker_info.json";
json j;

//tracked_users
for (const auto& [guild_id, users]: tracker_info::tracked_users) {
    json inner_j;
    for (const auto& user_id : users)
        inner_j.push_back(std::to_string(user_id));
    j["tracked_users"][std::to_string(guild_id)] = inner_j;
}

//tracker_channels
for (const auto& [guild_id, channel_id]: tracker_info::tracker_channels){
    j["tracker_channels"][std::to_string(guild_id)] = std::to_string(channel_id);
}

//globally_tracked
for(const auto& user: tracker_info::globally_tracked){
    j["globally_tracked"].push_back(std::to_string(user));
}

//message_templates
for (const auto& [guild_id, msg]: tracker_info::message_templates){
    j["tracker_channels"][std::to_string(guild_id)] = msg;
}

std::ofstream out(filename);
out << j.dump(4);

}

void load_tracker(){

const std::string filename="data/tracker_info.json";
std::ifstream in(filename);
if (!in.is_open()) return;

json j;
in >> j;

//tracked_users
tracker_info::tracked_users.clear();
for(auto& [server_str,userlist]: j["tracked_users"].items()){
dpp::snowflake server_id = std::stoull(server_str);
std::set<dpp::snowflake> inner_set;
for(auto& [user_str,val]: userlist.items()){
dpp::snowflake user_id = std::stoull(val.get<std::string>());
inner_set.insert(user_id);
}
tracker_info::tracked_users[server_id]=inner_set;
}

//tracker_channels
tracker_info::tracker_channels.clear();
for(auto& [server_str,val]: j["tracker_channels"].items()){
dpp::snowflake server_id = std::stoull(server_str);
dpp::snowflake channel_id = std::stoull(val.get<std::string>());
tracker_info::tracker_channels[server_id]=channel_id;
}

//globally_tracked
tracker_info::globally_tracked.clear();
for(auto& val:j["globally_tracked"]){
tracker_info::globally_tracked.insert(std::stoull(val.get<std::string>()));
}

//message_templates
tracker_info::message_templates.clear();
for(auto& [server_str,val]: j["message_templates"].items()){
dpp::snowflake server_id = std::stoull(server_str);
std::string msg = val.get<std::string>();
tracker_info::message_templates[server_id]=msg;
}

}

void save(){

save_jorkle();
save_tracker();

}

void load(){

load_jorkle();
load_tracker();

}
