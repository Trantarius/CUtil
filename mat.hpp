#pragma once
#include "vec.hpp"

#ifdef MAT_OPTIMIZE
#pragma GCC push_options
#pragma GCC optimize ("O3")
#endif

template<typename T>
class mat{
    T* data=nullptr;
    size_t _rows=0;
    size_t _cols=0;

    struct mat_row_access{
        T* ptr;
        size_t limit;
        T& operator[](size_t n){
            if(n>limit){
                throw std::logic_error("mat idx out of bounds");
            }
            return ptr[n];
        }
    };

    struct const_mat_row_access{
        const T* ptr;
        size_t limit;
        const T& operator[](size_t n) const{
            if(n>limit){
                throw std::logic_error("mat idx out of bounds");
            }
            return ptr[n];
        }
    };

public:
    static constexpr bool debug_enabled=
#ifdef MAT_DEBUG
        true;
#else
        false;
#endif
    static constexpr bool optimize_enabled=
#ifdef MAT_OPTIMIZE
        true;
#else
        false;
#endif

    mat(){}
    mat(size_t r,size_t c):data(new T[r*c]),_rows(r),_cols(c){}
    mat(size_t s):mat(s,s){}
    template<typename...Ts>
    mat(size_t r,size_t c,T a,Ts...bs):data(new T[r*c]{a,bs...}),_rows(r),_cols(c){}
    mat(const mat<T>& b):data(new T[b._cols*b._rows]),_rows(b._rows),_cols(b._cols){
        memcpy(data,b.data,sizeof(T)*_rows*_cols);
    }
    mat(mat<T>&& b):data(b.data),_rows(b._rows),_cols(b._cols){
        b.data=nullptr;
        b._rows=0;
        b._cols=0;
    }

    ~mat(){
        if(data!=nullptr){
            delete [] data;
        }
    }
#ifdef MAT_DEBUG
    mat_row_access operator[](size_t r){
        if(r>_rows){
            throw std::logic_error("mat idx out of bounds");
        }
        mat_row_access ret;
        ret.ptr=data+r*_cols;
        ret.limit=_cols;
        return ret;
    }
    const_mat_row_access operator[](size_t r) const{
        if(r>_rows){
            throw std::logic_error("mat idx out of bounds");
        }
        const_mat_row_access ret;
        ret.ptr=data+r*_cols;
        ret.limit=_cols;
        return ret;
    }
#else
    T* operator[](size_t r){
        return data+r*_cols;
    }
    const T* operator[](size_t r) const {
        return data+r*_cols;
    }
#endif

    static mat<T> identity(size_t r,size_t c){
        mat<T> ret(r,c);
        for(size_t n=0;n<r;n++){
            for(size_t m=0;m<c;m++){
                ret[n][m]=(char)(n==m);
            }
        }
        return ret;
    }
    static mat<T> identity(size_t s){
        return identity(s,s);
    }

    mat<T>& operator = (const mat<T>& b){
        if(_rows*_cols!=b._rows*b._cols){
            if(data!=nullptr){
                delete [] data;
            }
            data=new T[_rows*_cols];
        }
        _rows=b._rows;
        _cols=b._cols;
        memcpy(data,b.data,sizeof(T)*_rows*_cols);
        return *this;
    }
    mat<T>& operator = (mat<T>&& b){
        if(data!=nullptr){
            delete [] data;
        }
        data=b.data;
        _rows=b._rows;
        _cols=b._cols;
        b.data=nullptr;
        b._rows=0;
        b._cols=0;
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

    size_t rows() const {return _rows;}
    size_t cols() const {return _cols;}

    static mat<T>* new_array(size_t r,size_t c,size_t count){
        mat<T>* ptr=new mat<T>[count];
        for(size_t n=0;n<count;n++){
            ptr[n]=mat<T>(r,c);
        }
        return ptr;
    }

    static mat<T>* new_array(size_t s,size_t count){
        return new_array(s,s,count);
    }

    static void delete_array(mat<T>* ptr){
        delete [] ptr;
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

    mat<T>& fill(T a){
        for(size_t n=0;n<_rows;n++){
            for(size_t m=0;m<_cols;m++){
                (*this)[n][m]=a;
            }
        }
        return *this;
    }
};


template<typename T>
mat<T> operator -(const mat<T>& b){
    mat<T> ret(b.rows(),b.cols());
    for(size_t n=0;n<b.size();n++){
        ret[n] = -b[n];
    }
    return ret;
}
template<typename T>
mat<T>&& operator -(mat<T>&& b){
    for(size_t n=0;n<b.size();n++){
        b[n] = -b[n];
    }
    return std::move(b);
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
mat<T>&& operator + (mat<T>&& a,const mat<T>& b){
    SIZE_CHECK
    for(size_t n=0;n<a.rows();n++){
        for(size_t m=0;m<a.cols();m++){
            a[n][m] = a[n][m] + b[n][m];
        }
    }
    return std::move(a);
}
template<typename T>
mat<T>&& operator + (const mat<T>& a,mat<T>&& b){
    SIZE_CHECK
    for(size_t n=0;n<a.rows();n++){
        for(size_t m=0;m<a.cols();m++){
            b[n][m] = a[n][m] + b[n][m];
        }
    }
    return std::move(b);
}
template<typename T>
mat<T>&& operator + (mat<T>&& a,mat<T>&& b){
    SIZE_CHECK
    for(size_t n=0;n<a.rows();n++){
        for(size_t m=0;m<a.cols();m++){
            a[n][m] = a[n][m] + b[n][m];
        }
    }
    return std::move(a);
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
mat<T>&& operator - (mat<T>&& a,const mat<T>& b){
    SIZE_CHECK
    for(size_t n=0;n<a.rows();n++){
        for(size_t m=0;m<a.cols();m++){
            a[n][m] = a[n][m] - b[n][m];
        }
    }
    return std::move(a);
}
template<typename T>
mat<T>&& operator - (const mat<T>& a,mat<T>&& b){
    SIZE_CHECK
    for(size_t n=0;n<a.rows();n++){
        for(size_t m=0;m<a.cols();m++){
            b[n][m] = a[n][m] - b[n][m];
        }
    }
    return std::move(b);
}
template<typename T>
mat<T>&& operator - (mat<T>&& a,mat<T>&& b){
    SIZE_CHECK
    for(size_t n=0;n<a.rows();n++){
        for(size_t m=0;m<a.cols();m++){
            a[n][m] = a[n][m] - b[n][m];
        }
    }
    return std::move(a);
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
        throw std::logic_error("vec*mat size mismatch: "+std::to_string(a.size())+" * "+std::to_string(b.rows())+"x"+std::to_string(b.cols()));
    }
#endif
    vec<T> ret(b.cols());
    for(size_t m=0;m<b.cols();m++){
        ret[m]=0;
        for(size_t i=0;i<b.rows();i++){
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
            ret[n][m]=a[n][m]*b;
        }
    }
    return ret;
}
template<typename T>
mat<T>&& operator *(mat<T>&& a,T b){
    for(size_t n=0;n<a.rows();n++){
        for(size_t m=0;m<a.cols();m++){
            a[n][m]=a[n][m]*b;
        }
    }
    return std::move(a);
}

template<typename T>
mat<T> operator *(T b,const mat<T>& a){
    return a*b;
}
template<typename T>
mat<T>&& operator *(T b,mat<T>&& a){
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



template<typename T>
mat<T> transpose(const mat<T>& in){
    mat<T> ret(in.cols(),in.rows());
    for(size_t n=0;n<ret.rows();n++){
        for(size_t m=0;m<ret.cols();m++){
            ret[n][m]=in[m][n];
        }
    }
    return ret;
}



#ifdef MAT_OPTIMIZE
#pragma GCC pop_options
#endif
