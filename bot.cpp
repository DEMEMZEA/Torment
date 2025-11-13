#include <bits/stdc++.h>
#include <cstdint>
#include <cstdlib>
#include <dpp/appcommand.h>
#include <dpp/dispatcher.h>
#include <dpp/dpp.h>
#include <dpp/message.h>
#include <dpp/snowflake.h>
#include <future>
#include <unordered_map>
using namespace std;

const string token = "MTQxNjk0OTc2MjIyMTAxNTEzMg.Gq2Hox.iUDaT5D5oIO19vlXN09k2Brx1NO-zFvXXDpmNQ";
constinit long long int a{3};

std::string to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return s;
}

unordered_map<dpp::snowflake, dpp::snowflake>Cooldown;



void jorkle(const dpp::message_create_t& event, dpp::cluster* bot){
if(event.msg.message_reference.message_id==0)return;
dpp::snowflake jorkler_id = event.msg.author.id;
dpp::snowflake jorkled_id;
bot->message_get(event.msg.id,event.msg.channel_id,[&bot,&jorkled_id]
    (const dpp::confirmation_callback_t& cb) {
    if (cb.is_error()) return;
    const dpp::message& original = std::get<dpp::message>(cb.value);
    jorkled_id=original.author.id;


});}

int main(){

dpp::cluster bot(token);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([&bot](const dpp::ready_t& event){
        if (dpp::run_once<struct register_bot_commands>()) {
        dpp::slashcommand boa("boa","100% boa??",bot.me.id);
        boa.add_option(
        dpp::command_option(dpp::co_integer, "percentage","How much boa?", true)
        .add_choice(dpp::command_option_choice("60%",60))
        .add_choice(dpp::command_option_choice("100%",100))
        .add_choice(dpp::command_option_choice("200%",200))
        );
        boa.add_option(dpp::command_option(dpp::co_boolean,"ephem","Is this message going to be ephemeral(just for the user) or not? True by default",false)
        .add_choice(dpp::command_option_choice("yes",true))
        .add_choice(dpp::command_option_choice("no",false))
        );
        

        bot.global_bulk_command_create({boa});
        }
    });

    bot.on_message_create([&bot](const dpp::message_create_t& event) {
    // Ignore bot messages (good practice)
    if (event.msg.author.is_bot())
        return;

    // Example: check message content
    if (to_lower(event.msg.content) == to_lower("Mods, Jorkle 'em!")) {
        jorkle(event,&bot);
    }
    });

    bot.on_slashcommand([](const dpp::slashcommand_t& event){
        if(event.command.get_command_name()=="boa"){
        int pct = get<int64_t>(event.get_parameter("percentage"));
        bool ephem{1};
        if(event.get_parameter("ephem").index()!=0) ephem = get<bool>(event.get_parameter("ephem"));
        string str{""};
        if(pct==60){
        str=("Boa boa boa boa b; I'm 60% boa?!");
        }
        if(pct==100){
        str=("Boa boa boa boa boa boa boa, I'm 100% boa!");
        }
        if(pct==200){
        str=("Boa boa boa boa boa boa boa, I'm 100% boa!\nBoa boa boa boa boa boa boa, I'm 200% boa!");
        }
        auto msg = dpp::message(str);
        if(ephem) msg.set_flags(dpp::m_ephemeral);
        event.reply(msg);
        }
    });

bot.start(dpp::st_wait);

return 0;
}