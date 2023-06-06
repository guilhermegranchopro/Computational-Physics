#ifndef MINI_PROJETO_2_GRAPHICS_H
#define MINI_PROJETO_2_GRAPHICS_H

#include <vector>
#include <array>
#include <algorithm>
#include "TGraph.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TStyle.h"

using namespace std;

void CreateHistogram(const vector<array<float, 3>>& data, int ncols, int nrows, float sizeX, float sizeY, int Totalncell){
    TCanvas *c1 = new TCanvas();

    gStyle -> SetPalette(kRainBow);

    TH2F *hist = new TH2F("hist", "Histogram", ncols, 0, sizeX, nrows, 0, sizeY);

    hist->SetStats(0);

    int a;
    float x, y;

    for (int j=0; j<Totalncell; j++){
         a = abs(data[j][2]*1000000);
         x = data[j][0];
         y = data[j][1];
         for (int k=0; k<a; k++){
             hist->Fill(x,y);
         }
    }

    hist->GetXaxis()->SetTitle("x [cm]");
    hist->GetYaxis()->SetTitle("y [cm]");
    hist->GetZaxis()->SetTitle("Entries");

    hist->SetContour(1000);

    hist->Draw("colz");

    c1->SaveAs("FIG_LightMap_power.pdf");
}

void CreateGraph(const vector<pair<float, float>>& data, int ndata) {
    double x[ndata];
    double y[ndata];

    for (int a=0; a<ndata; a++){
        x[a] = static_cast<double>(data[a].first);
        y[a] = static_cast<double>(data[a].second);
    }

    TGraph *gr = new TGraph(ndata, x, y);

    gr->SetMarkerStyle(4);
    gr->SetMarkerSize(1);

    TCanvas *c1 = new TCanvas();
    gr->Draw("ALP");

    gr->SetTitle("Grafico de Dispersao"); // Adicione um título ao gráfico
    gr->GetXaxis()->SetTitle("Tamanho das celulas [cm]");   // Adicione um título ao eixo X
    gr->GetYaxis()->SetTitle("Potencia total [Watts]");   // Adicione um título ao eixo Y

    c1->SaveAs("FIG_LightMap_totalpower.pdf");
}

#endif //MINI_PROJETO_2_GRAPHICS_H