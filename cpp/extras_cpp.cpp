#include "../hpp/extras_cpp.hpp"

std::string to_time(long long int time){
if(time==0) throw std::invalid_argument("time amount must be nonnull");
if(time<0) return (to_time(-time)+" ago");
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

std::string to_time(double time){
if(time==0) throw std::invalid_argument("time amount must be nonnull");
if(time<0) return (to_time(-time)+" ago");
time = floor(time);
std::string resp;
int ms = static_cast<int>(std::fmod(time,1000.0));
time/=1000;
time = floor(time);
int sec = static_cast<int>(std::fmod(time,60.0));
time/=60;
time = floor(time);
int min = static_cast<int>(std::fmod(time,60.0));
time/=60;
time = floor(time);
int hr = static_cast<int>(std::fmod(time,24.0));
time/=24;
time = floor(time);
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

template<typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b) {
std::vector<T> result;
result.reserve(a.size() + b.size());

result.insert(result.end(), a.begin(), a.end());
result.insert(result.end(), b.begin(), b.end());
return result;
}

template<typename T>
std::vector<T>& operator+=(std::vector<T>& a, const std::vector<T>& b) {
if(a.empty()){
a = b;               // optional fast-path
} 
else{
a.reserve(a.size() + b.size());
a.insert(a.end(), b.begin(), b.end());
}
return a;
}

