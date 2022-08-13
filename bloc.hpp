#pragma once
#include <cstring>
#include <cinttypes>

#define uchar unsigned char
typedef uint64_t hash_t;

struct bloc{
    uchar* ptr=NULL;
    size_t size=0;

    bloc(){}
    bloc(size_t size):ptr(new uchar[size]),size(size){}
    bloc(uchar* ptr, size_t size):ptr(ptr),size(size){}
    bloc(const bloc& b):ptr(b.ptr),size(b.size){}

    uchar* operator &(){return ptr;}
    const uchar* operator &() const {return ptr;}
    operator uchar*(){return ptr;}
    operator const uchar*() const {return ptr;}
    uchar& operator [] (long n){return (ptr)[(n%size+size)%size];}
    const uchar& operator [] (long n) const {return (ptr)[(n%size+size)%size];}

    void destroy(){
        delete [] ptr;
    }

    static bloc copy(bloc& b){
        bloc ret(b.size);
        memcpy(ret,b,b.size);
        return ret;
    }
    static const bloc copy(const bloc& b){
        uchar* ptr=new uchar[b.size];
        memcpy(ptr,b,b.size);
        return bloc(ptr,b.size);
    }

    hash_t hash() const{
        hash_t h=0;
        uchar off=0;
        for(size_t n=0;n<size;n++){
            h^=((hash_t)ptr[n])<<off;
            off=(off+8)%57;
        }
        return h;
    }
};

#undef uchar
