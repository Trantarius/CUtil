#pragma once
#include <string>
#include <iostream>
#include "typedefs.hpp"

namespace util{

  using std::string;
  typedef const char* const_cstr_t;
  typedef char* cstr_t;

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
  STD(int)  STD(uint)
  STD(short)  STD(ushort)
  STD(long)   STD(ulong)
  STD(float)  STD(double)

  #undef STD

  template<typename...Ts>
  void print(Ts...args){
    (std::cout<<...<<tostr(args))<<std::endl;
  }

  template<typename...Ts>
  void printerr(Ts...args){
    (std::cerr<<...<<tostr(args))<<std::endl;
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

  inline void print_loadbar(double completion){
    string out="\r[";
    int total_length=64;
    int filled=total_length*completion;
    for(int n=0;n<filled;n++){
      out+='#';
    }
    for(int n=0;n<total_length-filled;n++){
      out+=' ';
    }
    out+="]\r";
    std::cout<<out;
    std::cout.flush();
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



  inline string hexstr(uint8_t n){
    char cstr[3];
    cstr[2]='\0';
    static const char hexdigits[]{'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    cstr[0]=hexdigits[n>>4];
    cstr[1]=hexdigits[n&0xf];
    return string(cstr);
  }

  inline string hexstr(uint16_t n){
    return hexstr((uint8_t)(n>>8))+hexstr((uint8_t)(n&0xff));
  }

  inline string hexstr(uint32_t n){
    return hexstr((uint16_t)(n>>16))+hexstr((uint16_t)(n&0xffff));
  }

  inline string hexstr(uint64_t n){
    return hexstr((uint32_t)(n>>32))+hexstr((uint32_t)(n&0xffffffff));
  }

  inline string hexstr(void* p){
    return hexstr((uint64_t)p);
  }



  namespace _str_rep_internal{
    template<typename T>
    struct _str_rep{
      const string text;
      _str_rep(const T& val):text(util::tostr(val)){}
      operator string(){return text;}
      string operator()(){return text;}
    };

    template<>
    struct _str_rep<string>{
      const string text;
      _str_rep(const string& val):text("\""+util::tostr(val)+"\""){}
      operator string(){return text;}
      string operator()(){return text;}
    };

    template<>
    struct _str_rep<char>{
      const string text;
      _str_rep(const char& val):text("\'"+string(&val,1)+"\'"){}
      operator string(){return text;}
      string operator()(){return text;}
    };



    template<>
    struct _str_rep<cstr_t>{
      const string text;
      _str_rep(const cstr_t& val):text("\""+util::tostr(val)+"\""){}
      operator string(){return text;}
      string operator()(){return text;}
    };

    template<>
    struct _str_rep<const_cstr_t>{
      const string text;
      _str_rep(const const_cstr_t& val):text("\""+util::tostr(val)+"\""){}
      operator string(){return text;}
      string operator()(){return text;}
    };

    template<size_t N>
    struct _str_rep<char[N]>{
      const string text;
      typedef char argtype[N];
      _str_rep(const argtype& val):text("\""+util::tostr(val)+"\""){}
      operator string(){return text;}
      string operator()(){return text;}
    };

  }

  template<typename T>
  inline string str_rep(const T& val){
    return _str_rep_internal::_str_rep<T>(val)();
  }


}
