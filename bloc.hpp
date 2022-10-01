#pragma once
#include <cinttypes>
#include <cstdint>
#include <cstring>
#include <cstdlib>

template<typename T=u_char>
struct bloc{
    T* ptr=NULL;
    size_t size=0;

    bloc(){}
    bloc(size_t size):ptr(new T[size]),size(size){}
    bloc(T* ptr, size_t size):ptr(ptr),size(size){}
    bloc(bloc& b):ptr(b.ptr),size(b.size){}

    T* operator &(){return ptr;}
    const T* operator &() const {return ptr;}
    operator T*(){return ptr;}
    operator const T*() const {return ptr;}
    T& operator [] (long n){return (ptr)[(n%size+size)%size];}
    const T& operator [] (long n) const {return (ptr)[(n%size+size)%size];}

    void destroy(){
        delete [] ptr;
    }

    void copy(const bloc& b){
        memcpy(ptr,b.ptr,size>b.size?b.size:size);
    }

    static bloc copy_of(const bloc& b){
        T* ptr=new T[b.size];
        memcpy(ptr,b,b.size);
        return bloc(ptr,b.size);
    }

    uint64_t hash() const{
        uint64_t h=0;
        u_char off=0;
        for(size_t n=0;n<size*sizeof(T);n++){
            h^=((uint64_t)((u_char*)ptr)[n])<<off;
            off=(off+8)%57;
        }
        return h;
    }
};
