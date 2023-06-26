#ifndef __MYFUNCTION__
#define __MYFUNCTION__

#include "Functor.h"
#include <cmath>

using namespace std;

class MyFunction : public Functor {

public:
    MyFunction(string s="MyFunction");
    ~MyFunction() = default;

    double operator()(double x);

};

#endif