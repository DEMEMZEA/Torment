#ifndef SAVE_HPP
#define SAVE_HPP
#include <nlohmann/json.hpp>
#include <bits/stdc++.h>
inline void from_json(const nlohmann::json& j, std::pair<int,int>& p);
inline void to_json(nlohmann::json& , const std::pair<int,int>& p);
void load();
void save();
#endif