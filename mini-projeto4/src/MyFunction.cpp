#include "Functor.h"
#include <cmath>
#include "MyFunction.h"

using namespace std;

MyFunction::MyFunction(string s) : Functor(s){}

double MyFunction::operator()(double x) {
    return ( pow(x, 4) * log( x + sqrt( pow(x,2) + 1 ) ) );
}
