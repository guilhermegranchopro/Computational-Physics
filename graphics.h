#ifndef MINI_PROJETO_2_GRAPHICS_H
#define MINI_PROJETO_2_GRAPHICS_H

#include <vector>
#include <array>
#include <algorithm>
#include <ostream>
#include "TGraph.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TStyle.h"

using namespace std;

void CreateHistogram(const vector<array<float, 3>>& data, int ncols, int nrows, float sizeX, float sizeY, int Totalncell){
    TCanvas *c1 = new TCanvas();  // Create a canvas to display the histogram

    gStyle -> SetPalette(kRainBow);  // Set the color palette for the histogram

    TH2F *hist = new TH2F("hist", "Histogram", ncols, 0, sizeX, nrows, 0, sizeY);  // Create a 2D histogram object

    hist->SetStats(0);  // Disable the statistics box on the histogram

    int a;
    float x;
    float y;


    for (int j = 0; j < Totalncell; j++){  // Loop over the cells of the data
         a = abs(data[j][2]*1000000);  // Extract the third value from the data and calculate a scaling factor
         x = data[j][0];  // Extract the first value from the data
         y = data[j][1];  // Extract the second value from the data
         for (int k=0; k<a; k++){  // Fill the histogram with (x, y) values based on the scaling factor
             hist->Fill(x,y);
         }
    }

    hist->GetXaxis()->SetTitle("x [cm]");  // Set the X-axis title of the histogram
    hist->GetYaxis()->SetTitle("y [cm]");  // Set the Y-axis title of the histogram
    hist->GetZaxis()->SetTitle("Entries");  // Set the Z-axis title of the histogram

    hist->SetContour(1000);  // Set the number of contour levels for the histogram

    hist->Draw("colz");  // Draw the histogram with a color representation

    c1->SaveAs("FIG_LightMap_power.pdf");  // Save the canvas as a PDF file
}

void CreateGraph(const vector<pair<float, float>>& data, int ndata) {
    double x[ndata];
    double y[ndata];

    for (int a=0; a<ndata; a++){
        x[a] = static_cast<double>(data[a].first);  // Extract the first value from the data and cast it to double
        y[a] = static_cast<double>(data[a].second);   // Extract the second value from the data and cast it to double
    }

    TGraph *gr = new TGraph(ndata, x, y);  // Create a graph object with the (x, y) data

    gr->SetMarkerStyle(4);  // Set the marker style for the graph
    gr->SetMarkerSize(1);  // Set the marker size for the graph

    TCanvas *c1 = new TCanvas();  // Create a canvas to display the graph
    gr->Draw("ALP");  // Draw the graph as a line plot with markers

    gr->SetTitle("Grafico de Dispersao");  // Add a title to the graph
    gr->GetXaxis()->SetTitle("Tamanho das celulas [cm]");   // Add a title to the X-axis
    gr->GetYaxis()->SetTitle("Potencia total [Watts]");   // Add a title to the Y-axis

    c1->SaveAs("FIG_LightMap_totalpower.pdf");  // Save the canvas as a PDF file
}

#endif //MINI_PROJETO_2_GRAPHICS_H