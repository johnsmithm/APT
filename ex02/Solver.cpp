#include <iostream>
#include <time.h>
//#include "Vector.h"
#include "Matrix.h"
#include "Stencil.h"

#define PI 3.141592653589793

template<typename T, size_t rows_>
using Vector = Matrix<T, rows_, 1>;

template<typename T, class Derived, size_t numPoints>
void solve (const MatrixLike<T, Derived ,numPoints,numPoints>& A, const Vector<T,numPoints>& b, Vector<T,numPoints>& u) {
	const size_t numGridPoints = u.size( );

	double initRes = (b - (A * u)).l2Norm( ); // determine the initial residual
	double curRes = initRes;
	std::cout << "Initial residual:\t\t" << initRes << std::endl;

	unsigned int curIt = 0; // store the current iteration index

	while (curRes > 1.e-5 * initRes) { // solve until the residual is reduced by a certain amount
		++curIt;
        assert(curIt<10000);
		u = u + A.inverseDiagonal( ) * (b - (A * u)); // Jacobi step
        
        //if(curIt==1)std::cout<<(A * u).size()<<'\n';
		
        curRes = (b - (A * u)).l2Norm( ); // update the residual

		if (0 == curIt % 500) // print some info every few steps
			std::cout << "Residual after iteration " << curIt << ":\t" << curRes << std::endl;
	}

	std::cout << "Residual after iteration " << curIt << ":\t" << curRes << std::endl << std::endl; // print the final number of iterations and the final residual
}

void testFullMatrix (const int numGridPoints) {
	const double hx = 1. / (numGridPoints - 1);
	const double hxSq = hx * hx;

	std::cout << "Starting full matrix solver for " << numGridPoints << " grid points" << std::endl;

	Matrix<double,numGridPoints,numGridPoints> A;
	Vector<double,numGridPoints> u;
	Vector<double,numGridPoints> b;

    A(0, 0) = 1.;
    for (int x = 1; x < numGridPoints - 1; ++x) {
        A(x, x - 1) = 1. / hxSq;
        A(x, x) = -2. / hxSq;
        A(x, x + 1) = 1. / hxSq;
    }
    A(numGridPoints - 1, numGridPoints - 1) = 1.;
    

	for (int x = 0; x < numGridPoints; ++x) {
		b(x) = sin(2. * PI * (x / (double)(numGridPoints - 1)));
	}
      //  std::cout<<A<<'\n'<<b;
    
	std::cout << "Initialization complete\n";
    
	// TODO: start timing
    clock_t t1,t2;
    t1=clock();
    //siwir::Timer timer;
	solve(A, b, u);
    //double time = timer.elapsed();
    t2=clock();
    float diff ((float)t2-(float)t1);
    float time = diff / CLOCKS_PER_SEC;
    std::cout << "time:" << time << '\n' ;
	// TODO: end timing and print elapsed time
}

void testStencil (const int numGridPoints) {
	// TODO: add stencil code
	// the stencil can be set up using
	//		Stencil<double> ASten({ { 0, 1. } }, { { -1, 1. / hxSq },{ 0, -2. / hxSq },{ 1, 1. / hxSq } });
    const double hx = 1. / (numGridPoints - 1);
	const double hxSq = hx * hx;

	std::cout << "Starting Stencil solver for " << numGridPoints << " grid points" << std::endl;

	Stencil<double,numGridPoints,numGridPoints> ASten({ { 0, 1. } }, { { -1, 1. / hxSq },{ 0, -2. / hxSq },{ 1, 1. / hxSq } });
	Vector<double,numGridPoints> u;
	Vector<double,numGridPoints> b( 
                                   [numGridPoints](int x)->double{return sin(2. * PI * (x / (double)(numGridPoints - 1)));});
    
    /*for (int x = 0; x < numGridPoints; ++x) {
		b(x) = sin(2. * PI * (x / (double)(numGridPoints - 1)));
	}*/
    
    std::cout << "Initialization complete\n";
    
	// TODO: start timing
    clock_t t1,t2;
    t1=clock();
    //siwir::Timer timer;
	solve(ASten, b, u);
    //double time = timer.elapsed();
    t2=clock();
    float diff ((float)t2-(float)t1);
    float time = diff / CLOCKS_PER_SEC;
    std::cout << "time:" << time << '\n' ;
    
}

int main(int argc, char** argv) {
    testFullMatrix( 29 );
    testStencil( 29 );
}