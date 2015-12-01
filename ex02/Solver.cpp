#include <iostream>
#include <time.h>
#include "Vector.h"
#include "Matrix.h"
#include "Stencil.h"
#include <functional>

#define PI 3.141592653589793
const int numGridPoints = 129;


template<typename T, class Derived, size_t numPoints>
void solve (const MatrixLike<T, Derived ,numPoints,numPoints>& A, const Vector<T,numPoints>& b, Vector<T,numPoints>& u) {


	double initRes = (b - (A * u)).l2Norm( ); // determine the initial residual
	double curRes = initRes;
	std::cout << "Initial residual:\t\t" << initRes << std::endl;// << (u + A.inverseDiagonal( ) * (b - (A * u)));
   
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


double measureTime (std::function<void( )> toMeasure) { 
    // TODO: start timing
    clock_t t1,t2;
    t1=clock();
    //siwir::Timer timer;
	toMeasure();
    //double time = timer.elapsed();
    t2=clock();
    double diff ((double)t2-(double)t1);
    double time = diff / CLOCKS_PER_SEC;
    return time;
}

void testFullMatrix (/*const int  numGridPoints*/) {
	const double hx = 1. / (numGridPoints - 1);
	const double hxSq = hx * hx;

	std::cout << "Starting full matrix solver for " << numGridPoints << " grid points" << std::endl;

	Matrix<double,numGridPoints,numGridPoints> A;
	Vector<double,numGridPoints> u;
	Vector<double,numGridPoints> b;

    A(0, 0) = 1.;
    for (size_t x = 1; x < numGridPoints - 1; ++x) {
        A(x, x - 1) = 1. / hxSq;
        A(x, x) = -2. / hxSq;
        A(x, x + 1) = 1. / hxSq;
    }
    A(numGridPoints - 1, numGridPoints - 1) = 1.;
    

	for (size_t x = 0; x < numGridPoints; ++x) {
		b(x) = sin(2. * PI * (x / (double)(numGridPoints - 1)));
	}
      //  std::cout<<A<<'\n'<<b;
    
	std::cout << "Initialization complete\n";
   
	double f  = measureTime(std::bind(solve<double, Matrix<double,numGridPoints,numGridPoints >, numGridPoints >,A,b,u));
	
	std::cout << "time with std::bind:" << f << '\n' ;
	
}

void testStencil () {
	

    const double hx = 1. / (numGridPoints - 1);
	const double hxSq = hx * hx;

	std::cout << "Starting Stencil solver for " << numGridPoints << " grid points" << std::endl;

    
    
	Stencil<double,numGridPoints,numGridPoints> ASten({ { 0, 1. } }, { { -1, 1. / hxSq },{ 0, -2. / hxSq },{ 1, 1. / hxSq } });
	Vector<double,numGridPoints> u;
    int numGridPoints1 = numGridPoints;
	Vector<double,numGridPoints> b([numGridPoints1](int x)->double{return sin(2. * PI * (x / (double)(numGridPoints1 - 1)));});
    
    
  
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
    testFullMatrix(  );
    testStencil(  );
}
