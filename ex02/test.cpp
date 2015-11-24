#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
using namespace std;
// The Curiously Recurring Template Pattern (CRTP)
template<typename T,class T1>
class Base
{
    // methods within Base can use template to access members of Derived
};
template<typename T >
class Derived : public Base<T, Derived<T> >
{
    // ...
};

int main(){
Derived<int> fast;
    int n1 = 3;
    int n2 = 5;
 
    std::vector<pair<int,int>> v{{1,2},{2,3}};
    auto result2 = std::find_if(std::begin(v), std::end(v), [n1](pair<int,int> l)->bool{
    if(l.second==n1)return true;
        return false;
    });
    cout<<(*result2).first;
}