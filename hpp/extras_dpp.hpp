#ifndef EXTRAS_DPP_HPP
#define EXTRAS_DPP_HPP
#include <dpp/dpp.h>
// MUST NOT INCLUDE ANYTHING ALREADY NOT INCLUDED IN DPP.
// If you want to add code/functions that includes things from somewhere else, add to extras.(h/c)pp or extras_cpp.(h/c)pp
bool is_admin(dpp::guild_member member, dpp::guild guild);
bool is_admin(dpp::guild_member member, dpp::snowflake guild_id);
bool is_admin(dpp::user* userptr, dpp::guild guild);
bool is_admin(dpp::user user, dpp::guild guild);
bool is_admin(dpp::user* userptr, dpp::snowflake guild_id);
bool is_admin(dpp::user user, dpp::snowflake guild_id);
#endif