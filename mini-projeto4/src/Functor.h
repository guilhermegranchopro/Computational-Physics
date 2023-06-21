#ifndef __FUNCTOR__
#define __FUNCTOR__

#include <string>
#include "TCanvas.h"
#include "TGraph.h"

using namespace std;

class Functor {

public:

    Functor(string s="Functor");
    ~Functor() = default;

    virtual double operator()(double x);

    // args:
    // xi, xf ........... xmin and xmax limits for function display
    // num .............. number of sampling points to be used o TGraph
    // xtitle, ytitle ... axis titles
    virtual void Draw(double xi, double xf, int num, string xtitle="x", string ytitle="y");

protected:
    static TCanvas *c;
    string name;

};

#endif