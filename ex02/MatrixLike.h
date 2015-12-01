#pragma once

// forward declarations
//template<typename T>
//class Vector;

template<typename T,size_t rows, size_t cols>
class Matrix;
/*
template<typename T, size_t rows_>
using Vector = Matrix<T, rows_, 1>;*/

template<typename T, class Derived, size_t rows, size_t cols >
class MatrixLike {
public:
	/// c'tor/ d'tor
	virtual ~MatrixLike () noexcept = 0; // pure virtual destructor

	/// virtual operators
	virtual Matrix<T,rows,1> operator* (const Matrix<T,rows,1> & o) const = 0;
	// feel free to extend as required

	// TODO: optimize the () operator
//	virtual const T & operator() (int r, int c) const = 0;
//	virtual T & operator() (int r, int c) = 0;

	/// other functions
	virtual Derived inverseDiagonal( ) const = 0;
	// feel free to extend as required
    virtual double l2Norm ( ) const {return 0.;};
    

protected:
};

// MatrixLike d'tor implementation
template<typename T, class Derived, size_t rows, size_t cols>
inline MatrixLike<T, Derived, rows, cols>::~MatrixLike () noexcept { }
