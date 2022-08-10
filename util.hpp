#pragma once
#include "timer.hpp"
#include "bloc.hpp"
#include <iostream>
#include <string>

typedef u_char uchar;
using std::string;

template<typename T>
string tostr(T a);

template <> inline string tostr(string& a){
   return a;
}
template <> inline string tostr(const char* a){
    return string(a);
}

//casts to int first to force the char to print as a number
template <> inline string tostr(char a){
    return std::to_string((int)a);
}


#define STD(type) \
template <> inline string tostr(type a){ \
    return std::to_string(a); \
}

STD(uchar)
STD(int)    STD(uint)
STD(short)  STD(ushort)
STD(long)   STD(ulong)
STD(float)  STD(double)

#undef STD

template<typename...Ts>
void print(Ts...args){
    (std::cout<<...<<tostr(args))<<std::endl;
}
