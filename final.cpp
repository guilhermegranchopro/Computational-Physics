#include "lightmap.h"
#include "graphics.h"

array <int, 2> Getncell(array<float,2> size, float cell_lenght);

int main(){
    array<float,2> size = {200, 300};

    lightsource S{};  // Create a lightsource object S with default values
    S.power = 100;

    lightsource S2{};  // Create another lightsource object S2 with default values
    S.coo = {100, 150, 100};

    lightmap LM10(S, Getncell(size, 10), size, true, true);  // Create a lightmap object LM10 with specified parameters

    int a = 0;
    S.coo = {100,150,25};
    vector <float> N = {20, 12.5, 6.25, 4, 2.5, 2, 1};
    vector<pair<float, float>> N_TP (N.size());

    for(float i : N){
        lightmap LM(S, Getncell(size, i), size);  // Create a lightmap object LM with specified parameters

        N_TP[a].first = i;
        N_TP[a].second = LM.TotalPower();

        a++;
    }
    CreateGraph(N_TP, N.size());  // Call the CreateGraph function with vector N_TP and the size of vector N as arguments

    return 0;
}

array <int, 2> Getncell(array<float,2> size, float cell_lenght){
    array<int,2> ncell{};

    for(int a = 0; a<2; a++){
        ncell[a] = static_cast<int>(size[a] / cell_lenght);
    }

    return ncell;
}