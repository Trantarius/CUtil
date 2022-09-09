#pragma once
#include <string>
#include <iostream>
#include "typedefs.hpp"

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

template <> inline string tostr(bool b){
    return b?"true":"false";
}


#define STD(type) \
template <> inline string tostr(type a){ \
    return std::to_string(a); \
}

STD(u_char)
STD(int)    STD(uint)
STD(short)  STD(ushort)
STD(long)   STD(ulong)
STD(float)  STD(double)

#undef STD

template<typename...Ts>
void print(Ts...args){
    (std::cout<<...<<tostr(args))<<std::endl;
}

template<typename...Ts>
void print_(Ts...args){
    (std::cout<<...<<(tostr(args)+" "))<<std::endl;
}

template<typename...Ts>
void printw(size_t width,Ts...args){
    static auto ensurewidth=[](size_t width,string s)->string{
        s.resize(width,' ');
        return s;
    };
    (std::cout<<...<<ensurewidth(width,tostr(args)))<<std::endl;
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



inline string hexstr(u8 n){
    char cstr[3];
    cstr[2]='\0';
    static const char hexdigits[]{'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    cstr[0]=hexdigits[n>>4];
    cstr[1]=hexdigits[n&0xf];
    return string(cstr);
}

inline string hexstr(u16 n){
    return hexstr((u8)(n>>8))+hexstr((u8)(n&0xff));
}

inline string hexstr(u32 n){
    return hexstr((u16)(n>>16))+hexstr((u16)(n&0xffff));
}

inline string hexstr(u64 n){
    return hexstr((u32)(n>>32))+hexstr((u32)(n&0xffffffff));
}



