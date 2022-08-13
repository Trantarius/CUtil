#pragma once
#include "timer.hpp"
#include "bloc.hpp"
#include "io.hpp"
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
string tostr(T a){
    return (string)a;
}

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

inline string size_format(size_t size){
    if(size>1000L*1000L*1000l*1000L*10L){
        return std::to_string(size/(1000L*1000L*1000l*1000L))+" TB";
    }
    if(size>1000L*1000L*1000l*10L){
        return std::to_string(size/(1000L*1000L*1000l))+" GB";
    }
    if(size>1000L*1000L*10L){
        return std::to_string(size/(1000L*1000L))+" MB";
    }
    if(size>1000L*10L){
        return std::to_string(size/(1000L))+" KB";
    }
    return std::to_string(size)+" B";
}

inline string hexstr(u64 n){
    char cstr[17];
    cstr[16]='\0';
    static const char hexdigits[]{'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    for(u8 off=0;off<64;off+=4){
        cstr[off/4]=hexdigits[(n>>off)&0xf];
    }
    return string(cstr);
}
inline string hexstr(u32 n){
    char cstr[9];
    cstr[8]='\0';
    static const char hexdigits[]{'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    for(u8 off=0;off<32;off+=4){
        cstr[off/4]=hexdigits[(n>>off)&0xf];
    }
    return string(cstr);
}
inline string hexstr(u16 n){
    char cstr[5];
    cstr[4]='\0';
    static const char hexdigits[]{'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    for(u8 off=0;off<16;off+=4){
        cstr[off/4]=hexdigits[(n>>off)&0xf];
    }
    return string(cstr);
}
inline string hexstr(u8 n){
    char cstr[3];
    cstr[2]='\0';
    static const char hexdigits[]{'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    for(u8 off=0;off<8;off+=4){
        cstr[off/4]=hexdigits[(n>>off)&0xf];
    }
    return string(cstr);
}
