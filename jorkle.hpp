#ifndef JORKLE_HPP
#define JORKLE_HPP
#include <dpp/dpp.h>
#include <bits/stdc++.h>
void cant_jorkle(const dpp::message_create_t& event, dpp::cluster* bot, int reason);
void jorkle(const dpp::message_create_t& event, dpp::cluster& bot);
#endif