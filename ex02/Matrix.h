#include <algorithm>
#include <cassert>
#include <iostream>
#include <typeinfo>
#include <math.h>
#include "MatrixLike.h"
    
using namespace std;

template<typename T,size_t rows, size_t cols>
class Matrix : public MatrixLike<T, Matrix<T,rows,cols> >
{
public:    
    
    Matrix(){
             static_assert( cols >= 0 && rows>=0 ,"Incorect rows and cols value");             
    }    
    
    /*
    Matrix( const Matrix<T,orows,ocols> & o )
        : sizex_( o.sizex_ ),sizey_( o.sizey_ )
    {
       data_ = new T[sizex_*sizey_];
       std::copy( o.data_, o.data_+sizex_*sizey_, data_ );
    }//how to copy cols and rows
    */
    ~Matrix()
    {//how to delete
        //delete [] data_;
    }    
    
    Matrix inverseDiagonal()const{
    assert(sizex_==sizey_);
        Matrix<T,rows,cols> o;
        for(int i=0;i<cols;++i)o(i,i)= 1.0/data_[i+sizex_*i];
    return o;
    }
    
    public :double l2Norm ( ) const {
        double norm = 0.;
        norm = std::accumulate(&data_[0],&data_[0]+rows,0.,[](T & sum,T b){
        return (sum+b*b);
        });
        return sqrt(norm);
    }
    
    Vector<T,el> operator* (const Vector<T,el> & o)const {
         assert(el==rows);
         Vector<T,el> t;
         for(int i=0;i<sizey_;++i)
             for(int j=0;j<sizex_;++j)
                t(i,0)=t(i,0)+data_[j+sizex_*i]*o(j,0);
         return t;
    }
        
    
    Matrix & operator= ( const Matrix<T,orows,ocols> & o )
    {       //how we can acces cols from o?
        //assert( sizex_ == o.sizex_ && sizey_ == o.sizey_ );
        assert(typeid(o.data_[0]).name()==typeid(data_[0]).name());
        Matrix tmp( o );
        std::swap ( data_, tmp.data_ );
        return *this;
    }
    
   
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
    /*
    T operator()( int i, int j ) const
    {
        assert( i>=0 && i < sizey_ && j>=0 && j < sizex_ );
        return data_[i*sizex_+j];
    }*/
    
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
