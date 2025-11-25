#ifndef EXTRAS_HPP
#define EXTRAS_HPP
#include <bits/stdc++.h>
std::string to_time(long long int time);
template<typename T>
std::vector<T> operator+(const std::vector<T>& a, const std::vector<T>& b);
template<typename T>
std::vector<T>& operator+=(std::vector<T>& a, const std::vector<T>& b);
#endif