#ifndef EXTRAS_DPP_HPP
#define EXTRAS_DPP_HPP
#include <dpp/dpp.h>
bool is_admin(dpp::guild_member member, dpp::guild guild);
bool is_admin(dpp::guild_member member, dpp::snowflake guild_id);
bool is_admin(dpp::user* userptr, dpp::guild guild);
bool is_admin(dpp::user user, dpp::guild guild);
bool is_admin(dpp::user* userptr, dpp::snowflake guild_id);
bool is_admin(dpp::user user, dpp::snowflake guild_id);
#endif