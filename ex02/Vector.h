#include <algorithm>
#include <cassert>
#include <iostream>
#include <math.h>
    
using namespace std;

template<typename T>
class Vector
{
public:
    Vector( int size )
        : size_( size )
    {
        assert( size >= 0 );
        data_ = new T[size];
    }
    
    Vector( int size, const T & initValue )
        : size_( size )
    {
        assert( size >= 0 );
        data_ = new T[size];
        std::fill( data_, data_+size_, initValue );
    }
    
     Vector( int size, std::function<T(int)> f)
        : size_( size )
    {
        assert( size >= 0 );
        data_ = new T[size];
        for(int i=0;i<size;++i)data_[i] = f(i);
    }
    
    Vector( const Vector & o )
        : size_( o.size_ )
    {
       data_ = new T[size_];
       std::copy( o.data_, o.data_+size_, data_ );
    }
    
    ~Vector()
    {
        delete [] data_;
    }    
    
    public :double l2Norm ( ) const {
        double norm = 0.;
        /*for (int i = 0; i < size_; ++i)
        norm += data_[i] * data_[i];*/
        norm = std::accumulate(data_,data_+size_,0.,[](T & sum,T b){
        return (sum+b*b);
        });
        return sqrt(norm);
    }
    
    Vector operator+( const Vector & o )
    {
        assert( size_ == o.size_ );
        Vector result(size_);
        std::transform( data_, data_ + size_, o.data_, result.data_, std::plus<T>() );
        return result;
    }
    
    Vector operator-( const Vector & o )const
    {
        assert( size_ == o.size_ );
        Vector result(size_);
        std::transform( data_, data_ + size_, o.data_, result.data_, std::minus<T>() );
        return result;
    }
    
    Vector & operator= ( const Vector & o )
    {
        Vector tmp( o );
        std::swap ( data_, tmp.data_ );
        std::swap ( size_, tmp.size_ );
        return *this;
    }
    
    
    T & operator() ( int i )
    {
        assert( i>=0 && i < size_ );
        return data_[i];
    }
    
    T operator()( int i ) const
    {
        assert( i>=0 && i < size_ );
        return data_[i];
    }
    
    int size() const { return size_; }
    
    
private:
    T * data_;
    int size_;
};


template<typename T>
ostream & operator<<( ostream & os, const Vector<T> & v )
{
    for( int i=0; i < v.size(); ++i )
    {
        os << v(i) << " ";
        os<<"\n";
    }
    return os;
}

