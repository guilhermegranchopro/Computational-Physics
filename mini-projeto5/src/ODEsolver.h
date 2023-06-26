#ifndef __ODESOLVER__
#define __ODESOLVER__

#include "ODEpoint.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TSystem.h"

#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <map>

using namespace std;

class ODEsolver {
public:
    ODEsolver(const vector<function<double(double)>>& func) : F(func){}
    ~ODEsolver() = default;

    // set functions
    void SetODEfunc(const vector<function<double(double)>>& func){F = func;}

    // solver methods
    const vector<ODEpoint>& Euler(ODEpoint i, double step, double T);
    const vector<ODEpoint>& PredictorCorrector(ODEpoint i, double step, double T);
    const vector<ODEpoint>& LeapFrog(ODEpoint i, double step, double T);
    const vector<ODEpoint>& RK2(ODEpoint i, double step, double T);
    const vector<ODEpoint>& RK4(ODEpoint i, double step, double T);

    //print MS
    const void printMS(string key = "");
    void DrawGraphicst_x(vector <ODEpoint>, int, string, string, string);
    void DrawGraphicsx1_x2(vector <ODEpoint>, int, int, string, string, string);

    //getter
    map<string, vector<ODEpoint> > GetMS();

private:
    vector<function<double(double)>> F;
    map<string, vector<ODEpoint> > MS; // key: "euler", "trapezoidal", ...
    vector<string> FullKey;
};

#endif
