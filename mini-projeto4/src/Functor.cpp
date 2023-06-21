#include <string>
#include "TGraph.h"
#include "Functor.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TAxis.h"
#include "TSystem.h"


using namespace std;

Functor::Functor(string s) : name(s) {}

double Functor::operator()(double x) {
    return 0;
}

void Functor::Draw(double xi, double xf, int num, string xtitle, string ytitle){

    /*
    we are going to use the Draw method of the Base class (Functor) to draw
    any function defined in classes inheriting from Functor
    */

    printf("[%s] range=[%.2f, %.2f] num=%d titles=[%s,%s]\n", __PRETTY_FUNCTION__, xi, xf, num, xtitle.c_str(), ytitle.c_str());

    // make graph object
    vector<double> x(num), y(num);
    double h = (xf - xi) / (num - 1);
    for (int i = 0; i < num; ++i) {
        x[i] = xi + i * h;
        y[i] = (*this)(x[i]);
    }

    TGraph *g = new TGraph(x.size(), x.data(), y.data());
    g->GetXaxis()->SetTitle(xtitle.c_str());
    g->GetYaxis()->SetTitle(ytitle.c_str());
    g->SetLineColor(kBlue - 2);
    g->SetLineWidth(4);
    g->SetTitle(name.c_str());

    printf("TGraph done...numb of points=%d\n", g->GetN());

    // display
    TCanvas* c = new TCanvas(("c_" + name).c_str(), "Function Plot", 800, 600);
    g->Draw("ALP");
    c->Update();
    gSystem->ProcessEvents();
    c->WaitPrimitive();
    c->SaveAs("Function_Plot.pdf");
}