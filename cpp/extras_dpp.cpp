#include "../hpp/extras_dpp.hpp"
#include <dpp/guild.h>
#include <dpp/permissions.h>

bool is_admin(dpp::guild_member member, dpp::guild guild){
return guild.base_permissions(member)&dpp::p_administrator;
}

bool is_admin(dpp::guild_member member, dpp::snowflake guild_id){
return is_admin(member,*dpp::find_guild(guild_id));
}

bool is_admin(dpp::user* userptr, dpp::guild guild){
return guild.base_permissions(userptr)&dpp::p_administrator;
}

bool is_admin(dpp::user user, dpp::guild guild){
return is_admin(&user,guild);
}

bool is_admin(dpp::user* userptr, dpp::snowflake guild_id){
return is_admin(userptr,*dpp::find_guild(guild_id));
}

bool is_admin(dpp::user user, dpp::snowflake guild_id){
return is_admin(&user,*dpp::find_guild(guild_id));
}