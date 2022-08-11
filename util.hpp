#pragma once
#include "timer.hpp"
#include "bloc.hpp"
#include <iostream>
#include <string>
#include <cinttypes>

typedef int8_t i8;      typedef uint8_t u8;
typedef int16_t i16;    typedef uint16_t u16;
typedef int32_t i32;    typedef uint32_t u32;
typedef int64_t i64;    typedef uint64_t u64;

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
