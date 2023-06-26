#include "Functor.h"
#include "IntegDeriv.h"
#include "MyFunction.h"
#include "Rwalk1D.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TSystem.h"
#include "TApplication.h"
#include "TH1.h"
#include "TCanvas.h"

#include <vector>
#include <iostream>
#include <string>

using namespace std;

void PlotTrajectories(int nTraj, int nSteps, Rwalk1D& rw);
void CreateHistogram(int nTraj, int nSteps, Rwalk1D& rw, string name);

int main() {

    MyFunction function("My Function"); // Cria um objeto da classe MyFunction
    function.Draw(0, 2, 1000, "x", "y"); // Chama a função Draw() com os argumentos necessários

    double INT=0.0, Error = 0.0, xi = 0.0, xf = 2.0;
    IntegDeriv Integral(function);
    Integral.TrapezoidalRule(xi, xf, INT, Error); // Chama o método TrapezoidalRule() da classe IntegDeriv
    Integral.simpsonRule(xi, xf, INT, Error); // Chama o método simpsonRule() da classe IntegDeriv

    //a
    int Nparticles = 5, steps = 500;
    Rwalk1D rw1(Nparticles, 0., .5, .5, 1., 1.); // Simulate 200 particles starting at position 0, with equal probabilities and step sizes of 1

    rw1.Run(steps); // Simulate 100 steps

    PlotTrajectories(Nparticles, steps, rw1);

    //b & c
    Nparticles = 200;
    Rwalk1D rw2(Nparticles, 0., .5, .5, 1., 1.); // Simulate 200 particles starting at position 0, with equal probabilities and step sizes of 1

    steps = 10;
    rw2.Run(steps);
    double sum = 0;
    for (int a = 0; a<Nparticles; a++){
        sum += rw2.GetTrajectory(a)[steps];
    }
    double averageDisplacement10 = sum / Nparticles;
    cout<<endl;
    cout << "Average displacement after 10 steps: " << averageDisplacement10 << endl;
    cout<<endl;
    CreateHistogram(Nparticles, steps, rw2, "RW_2c_10.pdf");

    steps = 100;
    rw2.Run(steps);
    sum = 0;
    for (int a = 0; a<Nparticles; a++){
        sum += rw2.GetTrajectory(a)[steps];
    }
    double averageDisplacement100 = sum / Nparticles;
    cout<<endl;
    cout << "Average displacement after 100 steps: " << averageDisplacement100 << endl;
    cout<<endl;
    CreateHistogram(Nparticles, steps, rw2, "RW_2c_100.pdf");


    return 0;
}

void PlotTrajectories(int nTraj, int nSteps, Rwalk1D& rw){

    // Criar um objeto TMultiGraph
    TMultiGraph* multiGraph = new TMultiGraph();

    vector<int> colors = {632, 600, 416, 616, 800, 432, 400, 920, 822, 806, 418, 420, 618, 620, 802, 860};

    int k;
    double t,x;
    vector<TGraph*> Trajectories(nTraj);
    for(int a = 0; a<nTraj; a++){
        Trajectories[a] = new TGraph();
        multiGraph->Add(Trajectories[a]);
        for (int i = 0; i <= nSteps; i++) {
            t = i*rw.GetTimeStep();
            x = rw.GetTrajectory(a)[t];
            Trajectories[a]->SetPoint(i, t, x);
        }
        k = (a % colors.size());
        Trajectories[a]->SetLineColor(colors[k]);
    }

    multiGraph->SetTitle("Trajectory Plot");
    multiGraph->GetXaxis()->SetTitle("Time");
    multiGraph->GetYaxis()->SetTitle("Position");

    // Desenhar o TMultiGraph no TCanvas
    TCanvas* canvas = new TCanvas("canvas", "Trajectory Plot", 800, 600);
    multiGraph->Draw("ALP");
    canvas->Update();
    gSystem->ProcessEvents();
    canvas->WaitPrimitive();
    canvas->SaveAs("RW_2a.pdf");
}

void CreateHistogram(int nTraj, int nSteps, Rwalk1D& rw, string name){

    vector<double> deslocationValues(nTraj);

    double deslocation, Tfinal;
    for (int i = 0; i < nTraj; i++) {
        Tfinal = nSteps * rw.GetTimeStep();
        deslocation = rw.GetTrajectory(i)[Tfinal];
        deslocationValues[i] = deslocation;
    }

    // Encontrar o valor mínimo e máximo da deslocation
    double deslocationMin = *min_element(deslocationValues.begin(), deslocationValues.end());
    double deslocationMax = *max_element(deslocationValues.begin(), deslocationValues.end());

    // Definir automaticamente os valores de xMin e xMax
    double range = deslocationMax - deslocationMin;
    double xMin = deslocationMin - 0.1 * range;
    double xMax = deslocationMax + 0.1 * range;

    // Criar um objeto do tipo TH1 (histograma de uma dimensão) com os valores de xMin e xMax
    TH1D* histogram = new TH1D("histogram", "Histogram", nTraj, xMin, xMax);

    // Preencher o histograma com os dados do vetor de deslocationValues
    for (const auto& value : deslocationValues) {
        histogram->Fill(value);
    }

    // Configurar as características do histograma
    histogram->GetXaxis()->SetTitle("X");
    histogram->GetYaxis()->SetTitle("Count");
    histogram->SetFillColor(kBlue);
    histogram->SetFillStyle(3004); // Preenchimento com linhas diagonais

    // Criar um canvas e desenhar o histograma
    TCanvas* canvas = new TCanvas("canvas", "Histogram", 800, 600);
    histogram->Draw();
    canvas->Update();
    canvas->WaitPrimitive();
    canvas->SaveAs(name.c_str());
}