#pragma once

#include <cassert>
#include <utility> //std::swap
#include <algorithm> // std::find_if
#include <vector>
#include<iostream>
#include <iterator>

#include "MatrixLike.h"

template<typename T>
using StencilEntry = std::pair<int, T>; // convenience type for stencil entries

template<typename T, size_t rows, size_t cols >
class Stencil : public MatrixLike<T, Stencil<T,rows,cols> >/* TODO: inherit MatrixLike */ {
public:
	Stencil(const std::vector<StencilEntry<T> >& boundaryEntries, const std::vector<StencilEntry<T> >& innerEntries)
		: boundaryStencil_(boundaryEntries), innerStencil_(innerEntries) { 
            //for(int i=0;i<innerStencil_.size();++i)
              //  std::cout<<innerStencil_[i].first<<" "<<innerStencil_[i].second<<'\n' ;
        }
	Stencil(const std::vector<StencilEntry<T> >& innerEntries)	// c'tor for stencils w/o explicit boundary handling
		: boundaryStencil_(innerEntries), innerStencil_(innerEntries) { }

	Stencil(const Stencil & o)
       : boundaryStencil_(o.boundaryStencil_), innerStencil_(o.innerStencil_){};
    
	Stencil(Stencil && o)
       : boundaryStencil_(o.boundaryStencil_), innerStencil_(o.innerStencil_){};

	~Stencil( ) noexcept  { }

	Stencil& operator=(const Stencil & o)
    {
        Stencil tmp( o );
        std::swap ( boundaryStencil_, tmp.boundaryStencil_ );
        std::swap ( innerStencil_   , tmp.innerStencil_ );
        return *this;
    };
	Stencil& operator=(Stencil && o) 
    {
        Stencil tmp( o );
        std::swap ( boundaryStencil_, tmp.boundaryStencil_ );
        std::swap ( innerStencil_   , tmp.innerStencil_ );
        return *this;
    };

	// HINT: stencil entries are stored as offset/coefficient pair, that is the offset specifies which element of a
	// vector, relative to the current index, is to be regarded. It is then multiplied with the according coefficient.
	// All of these expressions are evaluated and then summed up to get the final result.
	Vector<T,rows> operator* (const Vector<T,rows> & o) const {        
        Vector<T,rows> t;
        
        for(int i=0;i<boundaryStencil_.size();++i)
        t(0,0)+=o(0+boundaryStencil_[i].first,0)*boundaryStencil_[i].second; //A(0, 0) = 1.;
        
        for (int x = 1; x < o.size() - 1; ++x) {
            for(int i=0;i<innerStencil_.size();++i)
                t(x,0) += o(x+innerStencil_[i].first,0)*innerStencil_[i].second ;
            //A(x, x - 1) = 1. / hxSq;
            //A(x, x) = -2. / hxSq;
            //A(x, x + 1) = 1. / hxSq;
        }
        
        for(int i=0;i<boundaryStencil_.size();++i)
        t(o.size()-1,0)+=o(o.size()-1-boundaryStencil_[i].first,0)*boundaryStencil_[i].second;//A(numGridPoints - 1, numGridPoints - 1) = 1.;
        
        return t;
    };

	Stencil<T,rows,cols> inverseDiagonal( ) const { 
        
        StencilEntry<T> ZeroOffsetInner={0,0.};       
        StencilEntry<T> ZeroOffsetbound={0,0.};     
        
        auto ptr = std::find_if(std::begin(innerStencil_), std::end(innerStencil_), 
        []( StencilEntry<T> const& item)->bool{
            if(item.first==0)return true;
            return false;
        });
        //std::cout<<(*pr).second<<'\n'<<ZeroOffset.second<<'\n';        
        ZeroOffsetInner.second = 1./(*ptr).second;   
        
        auto ptrb = std::find_if(std::begin(boundaryStencil_), std::end(boundaryStencil_), 
        []( StencilEntry<T> const& item)->bool{
            if(item.first==0)return true;
            return false;
        });
        ZeroOffsetbound.second = 1./(*ptrb).second; 
        
        Stencil o({ZeroOffsetbound},{ZeroOffsetInner});
        return o;
    };

protected:
	// containers for the stencil entries -> boundary stencils represent the first and last rows of a corresponding
	// matrix and are to be applied to the first and last element of a target vector; inner stencils correspond to
	// the remaining rows of the matrix
	std::vector<StencilEntry<T> > boundaryStencil_;	// feel free to change the datatype if convenient
	std::vector<StencilEntry<T> > innerStencil_;	// feel free to change the datatype if convenient
};
