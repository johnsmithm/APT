#include <algorithm>
#include <cassert>
#include <iostream>
#include <math.h>
#include <functional>
    
using namespace std;

template<typename T, size_t size_>
class Vector
{
public:
    Vector()
      //  : size_( size )
    {
      //  assert( size >= 0 );
      //  data_ = new T[size];
        for (size_t i = 0; i < size_; ++i)
            data_[i]=0;
    }
    
  /*  Vector( int size, const T & initValue )
       // : size_( size )
    {
     //   assert( size >= 0 );
        data_ = new T[size];
        std::fill( data_, data_+size_, initValue );
    }
    */
     Vector(  std::function<T(size_t)> f)
     //   : size_( size )
    {
       // assert( size >= 0 );
        //data_ = new T[size];
        for(size_t i=0;i<size_;++i)data_[i] = f(i);
    }
    
   /* Vector( const Vector & o )
      //  : size_( o.size_ )
    {
       data_ = new T[size_];
       std::copy( o.data_, o.data_+size_, data_ );
    }
    
    ~Vector()
    {
     //   delete [] data_;
    }    
    */
    public :double l2Norm ( ) const {
        T norm = 0.;
        //for (int i = 0; i < size_; ++i)
        //norm += data_[i] * data_[i];
        norm = std::accumulate(&data_[0],&data_[0]+size_,0.,[](T & sum,T b){
        return (sum+b*b);
        });
        return sqrt(norm);
    }
    
    Vector operator+( const Vector & o ) const
    {
      Vector result;
        for(size_t i=0;i<size_;++i)
            result.data_[i] = data_[i]+o.data_[i];
        return result;
    }
    
    Vector operator-( const Vector & o ) const
    {
        Vector result;
        for(size_t i=0;i<size_;++i)
            result.data_[i] = data_[i]-o.data_[i];
        return result;
        
    }
    
   /* Vector & operator= ( const Vector & o )
    {
        Vector tmp( o );
        std::swap ( data_, tmp.data_ );
       // std::swap ( size_, tmp.size_ );
        return *this;
    }
    */
    
    T & operator() ( size_t i )
    {
        assert( i>=0 && i < size_ );
        return data_[i];
    }
    
    T operator()( size_t i ) const
    {
        assert( i>=0 && i < size_ );
        return data_[i];
    }
    
    int size() const { return size_; }
    
    
private:
     std::array<T, size_> data_;
};


template<typename T, size_t size_>
ostream & operator<<( ostream & os, const Vector<T,size_> & v )
{
    for( size_t i=0; i < v.size(); ++i )
    {
        os << v(i) << " ";
        os<<"\n";
    }
    return os;
}

