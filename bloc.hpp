#pragma once
#include <cinttypes>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

template<typename T=u_char,bool debug=false>
struct bloc{
    //utility to allow bloc<T,bloc::DEBUG> to be used instead of bloc<T,true>
    static constexpr bool DEBUG=true;

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
    T& operator [] (size_t n){
        if constexpr (debug){//this check only compiles if debug==true
            if(n>=size){
                throw std::out_of_range("index out of range: "+std::to_string(n)+" in size "
                                        +std::to_string(size));
            }
        }
        return (ptr)[n];
    }
    const T& operator [] (size_t n) const {
        if constexpr (debug){//this check only compiles if debug==true
            if(n>=size){
                throw std::out_of_range("index out of range: "+std::to_string(n)+" in size "
                                        +std::to_string(size));
            }
        }
        return (ptr)[(n%size+size)%size];
    }

    void destroy(){
        if constexpr (debug){
            if(ptr==NULL){
                throw std::logic_error("attempted to destroy a null bloc");
            }
        }
        delete [] ptr;
        ptr=NULL;
        size=0;
    }

    void copy(const bloc& b){
        if constexpr (debug){
            if(b.size!=size){
                throw std::logic_error("cannot copy a bloc of different size: "
                                       +std::to_string(size)+" vs "+std::to_string(b.size));
            }
        }
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
