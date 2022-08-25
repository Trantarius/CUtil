#pragma once
#include "vec.hpp"

#define MAT_DEBUG

template<typename T>
class mat{
    T* data;
    size_t _rows;
    size_t _cols;

public:
    mat(size_t r,size_t c):data(new T[r*c]),_rows(r),_cols(c){}
    mat(size_t s):mat(s,s){}
    template<typename...Ts>
    mat(size_t r,size_t c,T a,Ts...bs):data(new T[r*c]{a,bs...}),_rows(r),_cols(c){}
    mat(const mat<T>& b):data(new T[b._cols*b._rows]),_rows(b._rows),_cols(b._cols){
        memcpy(data,b.data,sizeof(T)*_rows*_cols);
    }

    ~mat(){
        delete [] data;
    }

    T* operator[](size_t r){
        return data+r*_cols;
    }
    const T* operator[](size_t r) const {
        return data+r*_cols;
    }

    static mat<T> identity(size_t s){
        mat<T> ret(s);
        for(size_t n=0;n<s;n++){
            for(size_t m=0;m<s;m++){
                ret[n][m]=(char)(n==m);
            }
        }
        return ret;
    }

    mat<T>& operator = (const mat<T>& b){
#ifdef MAT_DEBUG
        if(_rows!=b._rows||_cols!=b._cols){
            throw std::logic_error("mat size mismatch: "+std::to_string(_rows)+"x"+std::to_string(_cols)+" vs "+std::to_string(b._rows)+"x"+std::to_string(b._cols));
        }
#endif
        memcpy(data,b.data,sizeof(T)*_rows*_cols);
        return *this;
    }

    template<typename B>
    mat<B> as(){
        mat<B> ret(_rows,_cols);
        for(size_t n=0;n<_rows*_cols;n++){
            ret.data[n] = (B)(data[n]);
        }
        return ret;
    }

    T* operator &(){return data;}
    const T* operator &() const{return data;}

    inline size_t rows() const {return _rows;}
    inline size_t cols() const {return _cols;}

    static mat<T>* new_array(size_t r,size_t c,size_t count){
        char* cptr=new char[(r*c*sizeof(T)+sizeof(mat<T>))*count];
        vec<T>* mptr=(mat<T>*)cptr;
        T* eptr=(T*)(cptr+sizeof(mat<T>)*count);
        for(size_t n=0;n<count;n++){
            mptr[n].data=eptr;
            mptr[n]._rows=r;
            mptr[n]._cols=c;
            eptr+=r*c;
        }
        return mptr;
    }

    static mat<T>* new_array(size_t s,size_t count){
        return new_array(s,s,count);
    }

    static void delete_array(mat<T>* ptr){
        delete [] (char*)(ptr);
    }

    operator std::string(){
        std::string ret;
        for(size_t n=0;n<rows();n++){
            ret+="[ ";
            for(size_t m=0;m<cols();m++){
                ret+=tostr(data[n*_rows+m])+" ";
            }
            ret+="]\n";
        }
        return ret;
    }
};


template<typename T>
mat<T> operator -(const mat<T>& b){
    mat<T> ret(b.rows(),b.cols());
    for(size_t n=0;n<b.size();n++){
        ret[n] = 0 -b[n];
    }
    return ret;
}

#ifdef MAT_DEBUG
#define SIZE_CHECK \
if(a.rows()!=b.rows()||a.cols()!=b.cols){\
    throw std::logic_error("mat size mismatch: "+std::to_string(a.rows())+"x"+std::to_string(a.cols())+" vs "+std::to_string(b.rows())+"x"+std::to_string(b.cols())); \
}
#else
#define SIZE_CHECK
#endif

template<typename T>
mat<T> operator + (const mat<T>& a,const mat<T>& b){
    SIZE_CHECK
    mat<T> ret(a.rows(),a.cols());
    for(size_t n=0;n<a.rows();n++){
        for(size_t m=0;m<a.cols();m++){
            ret[n][m] = a[n][m] + b[n][m];
        }
    }
    return ret;
}

template<typename T>
mat<T> operator - (const mat<T>& a,const mat<T>& b){
    SIZE_CHECK
    mat<T> ret(a.rows(),a.cols());
    for(size_t n=0;n<a.rows();n++){
        for(size_t m=0;m<a.cols();m++){
            ret[n][m] = a[n][m] - b[n][m];
        }
    }
    return ret;
}



template<typename T>
mat<T> operator *(const mat<T>& a,const mat<T>& b){
#ifdef MAT_DEBUG
    if(a.cols()!=b.rows()){
        throw std::logic_error("mat*mat size mismatch: "+std::to_string(a.rows())+"x"+std::to_string(a.cols())+" * "+std::to_string(b.rows())+"x"+std::to_string(b.cols()));
    }
#endif
    mat<T> ret(a.rows(),b.cols());
    for(size_t n=0;n<a.rows();n++){
        for(size_t m=0;m<b.cols();m++){
            ret[n][m]=0;
            for(size_t i=0;i<a.cols();i++){
                ret[n][m]+=a[n][i]*b[i][m];
            }
        }
    }
    return ret;
}

template<typename T>
vec<T> operator *(const mat<T>& a,const vec<T>& b){
#ifdef MAT_DEBUG
    if(a.cols()!=b.size()){
        throw std::logic_error("mat*vec size mismatch: "+std::to_string(a.rows())+"x"+std::to_string(a.cols())+" * "+std::to_string(b.size()));
    }
#endif
    vec<T> ret(a.rows());
    for(size_t n=0;n<a.rows();n++){
        ret[n]=0;
        for(size_t i=0;i<a.cols();i++){
            ret[n]+=a[n][i]*b[i];
        }
    }
    return ret;
}

template<typename T>
vec<T> operator *(const vec<T>& a,const mat<T>& b){
#ifdef MAT_DEBUG
    if(a.size()!=b.rows()){
        throw std::logic_error("vec*mat size mismatch: "+std::to_string(b.size())+" * "+std::to_string(a.rows())+"x"+std::to_string(a.cols()));
    }
#endif
    vec<T> ret(a.cols());
    for(size_t m=0;m<a.cols();m++){
        ret[m]=0;
        for(size_t i=0;i<a.cols();i++){
            ret[m]+=a[i]*b[i][m];
        }
    }
    return ret;
}

template<typename T>
mat<T> operator *(const mat<T>& a,T b){
    mat<T> ret(a.rows(),a.cols());
    for(size_t n=0;n<a.rows();n++){
        for(size_t m=0;m<a.cols();m++){
            ret[n][m]*=b;
        }
    }
    return ret;
}

template<typename T>
mat<T> operator *(T b,const mat<T>& a){
    return a*b;
}

template<typename T>
bool operator == (const mat<T>& a,const mat<T>& b){
    SIZE_CHECK
    for(size_t n=0;n<a.rows();n++){
        for(size_t m=0;m<a.cols();m++){
            if(a[n][m]!=b[n][m]){
                return false;
            }
        }
    }
    return true;
}

#undef SIZE_CHECK

template<typename T>
bool operator != (const mat<T>& a,const mat<T>& b){
    return !(a==b);
}

using fmat = mat<float>;    using dmat = mat<double>;
