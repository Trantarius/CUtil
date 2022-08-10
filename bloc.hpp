#include <cstring>

struct bloc{
    char* ptr=NULL;
    size_t size=0;

    bloc(){}
    bloc(size_t size):ptr(new char[size]),size(size){}
    bloc(char* ptr, size_t size):ptr(ptr),size(size){}
    bloc(const bloc& b):ptr(b.ptr),size(b.size){}

    char* operator &(){return ptr;}
    const char* operator &() const {return ptr;}
    operator char*(){return ptr;}
    operator const char*() const {return ptr;}
    char& operator [] (long n){return (ptr)[(n%size+size)%size];}
    const char& operator [] (long n) const {return (ptr)[(n%size+size)%size];}

    static bloc copy(bloc& b){
        bloc ret(b.size);
        memcpy(ret,b,b.size);
        return ret;
    }
    static const bloc copy(const bloc& b){
        char* ptr=new char[b.size];
        memcpy(ptr,b,b.size);
        return bloc(ptr,b.size);
    }
};
