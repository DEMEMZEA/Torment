#include "../hpp/extras.hpp"

std::string to_time(long long int time){
if(time<=0) throw std::invalid_argument("time amount must be positive");;
std::string resp;
int ms = static_cast<int>(time%1000);
time/=1000;
int sec = static_cast<int>(time%60);
time/=60;
int min = static_cast<int>(time%60);
time/=60;
int hr = static_cast<int>(time%24);
time/=24;
int d = static_cast<int>(time);
resp = "";

if(d) resp += std::to_string(d)+":";

if(d) resp += std::format("{:02}:", hr);
else if(hr) resp += std::to_string(hr) + ":";

if(d || hr) resp += std::format("{:02}:", min);
else if(min) resp += std::to_string(min) + ":";

if(d || hr || min) resp += std::format("{:02}", sec);
else if(sec) resp += std::to_string(sec);
else resp += "0";

if(ms) resp += std::format(".{:03}", ms);

return resp;
}