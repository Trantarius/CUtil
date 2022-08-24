#pragma once
#include <cstring>
#include <stdexcept>

#define VEC_DEBUG

template<typename T>
class vec{
    T* data;
    size_t _size;
    vec(){}
public:
    vec(size_t n):data(new T[n]),_size(n){}
    template<typename A,typename B,typename...Cs>
    vec(A a,B b,Cs...cs):data(new T[sizeof...(cs)+2]{a,b,cs...}),_size(sizeof...(cs)+2){}
    vec(const vec& b):data(new T[b._size]),_size(b._size){
        memcpy(data,b.data,sizeof(T)*_size);
    }

    ~vec(){
        delete [] data;
    }

#ifdef VEC_DEBUG
#define BOUNDS_CHECK \
if(n<0||n>=_size){\
    throw std::out_of_range("vec idx out of bounds; idx:"+std::to_string(n)+" size:"+std::to_string(_size));\
}
#else
#define BOUNDS_CHECK
#endif
    T& operator [] (size_t n){
        BOUNDS_CHECK
        return data[n];
    }
    const T& operator [] (size_t n) const {
        BOUNDS_CHECK
        return data[n];
    }
#undef BOUNDS_CHECK

    void operator = (const vec& b){
#ifdef VEC_DEBUG
        if(_size!=b._size){
            throw std::logic_error("vec size mismatch: "+std::to_string(_size)+" vs "+std::to_string(b._size));
        }
#endif
        memcpy(data,b.data,sizeof(T)*_size);
    }

    T* operator &(){return data;}
    const T* operator &() const{return data;}

    inline size_t size() const {return _size;}
};

#define UOP(OP)                            \
template<typename T>                       \
vec<T> operator OP(const vec<T>& b){       \
    vec<T> ret(b.size());                  \
    for(size_t n=0;n<b.size();n++){        \
        ret[n] = OP b[n];                  \
    }                                      \
    return ret;                            \
}

UOP(-)
UOP(~)
UOP(!)
#undef UOP

#ifdef VEC_DEBUG
#define SIZE_CHECK \
if(a.size()!=b.size()){\
    throw std::logic_error("vec size mismatch: "+std::to_string(a.size())+" vs "+std::to_string(b.size())); \
}
#else
#define SIZE_CHECK
#endif

#define BIOP(OP)                                                    \
template<typename T>                                                \
vec<T> operator OP (const vec<T>& a,const vec<T>& b){               \
    SIZE_CHECK                                                      \
    vec<T> ret(a.size());                                           \
    for(size_t n=0;n<a.size();n++){                                 \
        ret[n] = a[n] OP b[n];                                      \
    }                                                               \
    return ret;                                                     \
}

BIOP(+)
BIOP(-)
BIOP(*)
BIOP(/)
BIOP(&)
BIOP(|)
BIOP(^)
BIOP(<<)
BIOP(>>)
BIOP(&&)
BIOP(||)
#undef BIOP

template<typename T>
bool operator == (const vec<T>& a,const vec<T>& b){
    SIZE_CHECK
    for(size_t n=0;n<a.size();n++){
        if(a[n]!=b[n]){
            return false;
        }
    }
    return true;
}

template<typename T>
bool operator != (const vec<T>& a,const vec<T>& b){
    return !(a==b);
}


template<typename T>
T dot(const vec<T>& a,const vec<T>& b){
    SIZE_CHECK
    T sum=0;
    for(size_t n=0;n<a.size();n++){
        sum+=a[n]*b[n];
    }
    return sum;
}

template<typename T>
T lensqr(const vec<T>& a){
    return dot(a,a);
}

template<typename T>
T len(const vec<T>& a){
    return sqrt(lensqr(a));
}

#undef SIZE_CHECK
