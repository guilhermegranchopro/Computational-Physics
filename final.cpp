#include "lightmap.h"
#include "graphics.h"

array <int, 2> Getncell(array<float,2> size, float cellL);

int main(){
    lightsource S{};
    array<float,2> size = {200, 300};
    S.power = 100;

    S.coo = {100,150,100};
    lightmap LM10(S, Getncell(size, 10), size, true, true);

    int a = 0;
    S.coo = {100,150,25};
    vector <float> N = {20, 12.5, 6.25, 4, 2.5, 2, 1};
    vector<pair<float, float>> N_TP (N.size());
    for(float i : N){
        lightmap LM(S, Getncell(size, i), size);
        N_TP[a].first = i;
        N_TP[a].second = LM.totalPower();
        a++;
    }
    CreateGraph(N_TP, N.size());

    return 0;
}

array <int, 2> Getncell(array<float,2> size, float cellL){
    array<int,2> ncell{};

    for(int a = 0; a<2; a++){
        ncell[a] = static_cast<int>(size[a] / cellL);
    }

    return ncell;
}