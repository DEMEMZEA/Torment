#ifndef EXTRAS_CPP_HPP
#define EXTRAS_CPP_HPP
#include <bits/stdc++.h>
// MUST NOT INCLUDE ANYTHING RELATED TO DPP.
// If you want to add code/functions that includes things from dpp, add to extras.(h/c)pp or extras_dpp.(h/c)pp
std::string to_time(long long int time);
template<typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b);
template<typename T>
std::vector<T>& operator+=(std::vector<T>& a, const std::vector<T>& b);
#endif