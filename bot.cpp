#include <bits/stdc++.h>
#include <dpp/appcommand.h>
#include <dpp/dispatcher.h>
#include <dpp/dpp.h>
using namespace std;

const string token = "MTQxNjk0OTc2MjIyMTAxNTEzMg.Gq2Hox.iUDaT5D5oIO19vlXN09k2Brx1NO-zFvXXDpmNQ";

int main(){


dpp::cluster bot(token);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([&bot](const dpp::ready_t& event){
        bot.global_command_create(dpp::slashcommand("boa","100% boa??",bot.me.id));
    });

    bot.on_slashcommand([](const dpp::slashcommand_t& event){
        if(event.command.get_command_name()=="boa"){
        event.reply("Boa boa boa boa boa boa boa, I'm 100% boa");
        }
    });

bot.start(dpp::st_wait);

return 0;
}