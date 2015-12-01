#include <algorithm>
#include <cassert>
#include <iostream>
#include <functional>
#include <typeinfo>
#include <math.h>
#include "MatrixLike.h"
    
using namespace std;

template<typename T,size_t rows, size_t cols>
class Matrix : public MatrixLike<T, Matrix<T,rows,cols> , rows, cols >
{
public:    
    
    Matrix(){
             static_assert( cols > 0 && rows > 0 ,"Incorect rows and cols value"); 
		// can I avoid that?
		
            for(int i=0;i<rows;++i)
             for(int j=0;j<cols;++j)
                data_[j+cols*i] = 0;
    }    
   Matrix(  std::function<T(int)> f)
       
    {
        static_assert( rows >= 0 ,"Incorect");
        for(int i=0;i<rows;++i)data_[i] = f(i);
    }
  /*  
    Matrix( const Matrix<T,orows,ocols> & o )
        : sizex_( o.sizex_ ),sizey_( o.sizey_ )
    {
       data_ = new T[sizex_*sizey_];
       std::copy( o.data_, o.data_+sizex_*sizey_, data_ );
    }//how to copy cols and rows
    
    ~Matrix()
    {//how to delete
        //delete [] data_;
    }    */
    
    Matrix inverseDiagonal()const{
    //assert(sizex_==sizey_);
        Matrix o;
        for(int i=0;i<cols;++i)o.data_[i+cols*i]= 1.0/data_[i+cols*i];
        return o;
    }
    
    public :double l2Norm ( ) const {
        T norm = 0.;
        norm = std::accumulate(&data_[0],&data_[0]+rows,0.,[](T & sum,T b){
        return (sum+b*b);
        });
        return sqrt(norm);
    }

    Matrix<T,rows,1> operator* (const Matrix<T,rows,1> & o)const {
         //assert(el==rows);
         Matrix<T,rows,1> t;
         for(int i=0;i<rows;++i)
             for(int j=0;j<cols;++j)
                t.data_[i]=t.data_[i]+data_[j+cols*i]*o.data_[j];
         return t;
    }
        
    /*
    Matrix & operator= ( const Matrix & o )
    {       //how we can acces cols from o?
        //assert( sizex_ == o.sizex_ && sizey_ == o.sizey_ );
        assert(typeid(o.data_[0]).name()==typeid(data_[0]).name());
        Matrix tmp( o );
        std::swap ( data_, tmp.data_ );
        return *this;
    }
    
    */
    
   
    T & operator() ( int i ,int j) 
    {
        assert( i>=0 && i < rows && j>=0 && j < cols );
        return data_[i*cols+j];
    }//difference between these two??
    
    const T & operator() ( int i ,int j)const
    {
        assert( i>=0 && i < rows && j>=0 && j < cols );
        return data_[i*cols+j];
    }
    
     T & operator() ( int i )
    {
        assert( i>=0 && i < rows );
        return data_[i];
    }
    
    T operator()( int i ) const
    {
        assert( i>=0 && i < rows );
        return data_[i];
    } 
    
    
    Matrix<T,rows,1> operator+( const Matrix<T,rows,1> & o )
    {
        //assert( size_ == o.size_ );
        Matrix<T,rows,1> result;
        for(int i=0;i<rows;++i)
            result.data_[i]=data_[i]+o.data_[i];
        //std::transform( data_, data_ + rows, o.data_, result.data_, std::plus<T>() );
        return result;
    }
    //why does not work for curRes = (b - (A * u)).l2Norm( );
    Matrix<T,rows,1> operator-( const Matrix<T,rows,1> & o )
    {
        //assert( size_ == o.size_ );
        Matrix<T,rows,1> result;
        for(int i=0;i<rows;++i)
            result.data_[i]=data_[i] - o.data_[i];
        //std::transform( data_, data_ + rows, o.data_, result.data_, std::plus<T>() );
        return result;
    }
    
    
    int size() const { return rows;  }
    int sizex() const { return rows; }
    
    int sizey() const { return cols; }
    
    
private:
    std::array<T, rows*cols> data_;
};


template<typename T, size_t rows, size_t cols >
ostream & operator<<( ostream & os, const Matrix<T,rows,cols> & v )
{
    for( int i=0; i < rows; ++i )
    {
        for( int j=0; j < cols; ++j )
        {
            os << v(i,j) << " ";
        }
        os << '\n';
    }
    return os;
}
