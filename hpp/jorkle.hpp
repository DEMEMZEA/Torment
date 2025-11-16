#ifndef JORKLE_HPP
#define JORKLE_HPP
#include <dpp/dpp.h>
#include <bits/stdc++.h>
void cant_jorkle(const dpp::message_create_t& event, int reason);
dpp::task<void> jorkle(const dpp::message_create_t& event, dpp::cluster& bot, int extra_dice);
#endif