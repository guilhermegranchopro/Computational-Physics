#ifndef MINI_PROJETO_2_LIGHTMAP_H
#define MINI_PROJETO_2_LIGHTMAP_H

#include <array>
#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "graphics.h"

using namespace std;

struct cell {
    array<float,3> center_coo; // cell center coordinates (cm)
    float area; // cell area (cmˆ2)
    float power; // cell incident power (W)
};

struct lightsource {
    array<float,3> coo; // source coordinates (cm)
    float power; // source power (W)
};

class lightmap {
public:
// constructors
// ... number of cells along x and y and plane dimensions (size)

    lightmap() = default;
    lightmap(array<int,2> ncell, std::array<float,2> size);
    lightmap(lightsource S, array<int,2> ncell, array<float,2> size);
    lightmap(lightsource S, array<int,2> ncell, array<float,2> size, bool YN);
    lightmap(lightsource S, array<int,2> ncell, array<float,2> size, bool YN1, bool YN2);
    lightmap(vector<lightsource> vS, array<int,2> ncell, array<float,2> size);
    lightmap(vector<lightsource> vS, array<int,2> ncell, array<float,2> size, bool YN);
    lightmap(vector<lightsource> vS, array<int,2> ncell, array<float,2> size, bool YN1, bool YN2);

// getters
    pair<int,int> GetCellIndex(float x, float y) const; // return cell indices
    pair<float,float> GetCellCoo(int index_x, int index_y) const; //return cell center coo
    double GetCellPower(int index_x, int index_y) const; // return cell power Watts
    double GetCellPower(float x, float y) const; // return cell powerWatts
    int GetCellNx() const; // get number of cells along x
    int GetCellNy() const; // get number of cells along y
    const vector<vector<cell>>& GetCells() const; // return cells grid
    vector<vector<cell>>& GetCells(); // return cells grid
    array<float,3> GetNormal(); // return normal to plane

// other functions
    void AddLightSource(lightsource S); // add sources
    const cell& GetMaxCell(); // get cell with max power
    array <float, 4> distance2cell(array<float,3> SourceCoo, array<float,3> PointCoo);
    float Irradiance(array<float,3> PointCoo);
    float CellPower(const cell& C);
    void SortOFPower();
    float medianaOfPower();
    float totalPower();

// other methods
    float distanceCalculator(float x, float y, float z);
    float ResultCalculator();
    void histogram();

private:
    vector<vector<cell>> GRID; // cell grid
    int Totalncell, ncols, nrows;
    float TotalGridArea;
    vector<lightsource> S;
    vector<cell> vGRID;
    float L;
    float sizeX, sizeY;
};

// constructors
lightmap::lightmap(array<int,2> ncell, array<float,2> size){
    sizeX = size[0];
    sizeY = size[1];
    TotalGridArea = size[0]*size[1];
    Totalncell = ncell[0]*ncell[1];
    L = sqrt(TotalGridArea/Totalncell);
    vGRID = vector<cell>(Totalncell);
    ncols = ncell[0];
    nrows = ncell[1];
    GRID = vector<vector<cell>>(ncell[1], vector<cell>(ncell[0]));
}

lightmap::lightmap(lightsource S, array<int,2> ncell, array<float,2> size){
    pair <float,float> position;
    int a=0;

    sizeX = size[0];
    sizeY = size[1];
    TotalGridArea = size[0]*size[1];
    Totalncell = ncell[0]*ncell[1];
    L = sqrt(TotalGridArea/Totalncell);
    vGRID = vector<cell>(Totalncell);
    ncols = ncell[0];
    nrows = ncell[1];
    GRID = vector<vector<cell>>(ncell[1], vector<cell>(ncell[0]));
    AddLightSource(S);

    for (int i=0; i<nrows; i++){
        for (int j=0; j<ncols; j++){
            cell C;
            C.area =TotalGridArea/Totalncell;
            position = GetCellCoo(j, i);
            C.center_coo = {position.first, position.second, 0};
            C.power = CellPower(C);
            GRID[i][j] = C;
            vGRID[a] = C;
            a++;
        }
    }
    SortOFPower();

}

lightmap::lightmap(lightsource S, array<int,2> ncell, array<float,2> size, bool YN){
    pair <float,float> position;
    int a = 0;

    sizeX = size[0];
    sizeY = size[1];
    TotalGridArea = size[0]*size[1];
    Totalncell = ncell[0]*ncell[1];
    L = sqrt(TotalGridArea/Totalncell);
    vGRID = vector<cell>(Totalncell);
    ncols = ncell[0];
    nrows = ncell[1];
    GRID = vector<vector<cell>>(ncell[1], vector<cell>(ncell[0]));
    AddLightSource(S);

    for (int i=0; i<nrows; i++){
        for (int j=0; j<ncols; j++){
            cell C;
            C.area =TotalGridArea/Totalncell;
            position = GetCellCoo(j, i);
            C.center_coo = {position.first, position.second, 0};
            C.power = CellPower(C);
            GRID[i][j] = C;
            vGRID[a] = C;
            a++;
        }
    }
    SortOFPower();

    if (YN){
        cout<<endl;
        cout<<"Quociente entre a potencia maxima e a potencia da celula correspondente a mediana das potencias:"<<endl;
        cout<<ResultCalculator()<<endl;
        cout<<endl;
        cout<<"Potencia total incidente no plano:"<<endl;
        cout<<totalPower()<<endl;
        cout<<endl;
    }

}

lightmap::lightmap(lightsource S, array<int,2> ncell, array<float,2> size, bool YN1, bool YN2){
    pair <float,float> position;
    int a = 0;

    sizeX = size[0];
    sizeY = size[1];
    TotalGridArea = size[0]*size[1];
    Totalncell = ncell[0]*ncell[1];
    L = sqrt(TotalGridArea/Totalncell);
    vGRID = vector<cell>(Totalncell);
    ncols = ncell[0];
    nrows = ncell[1];
    GRID = vector<vector<cell>>(ncell[1], vector<cell>(ncell[0]));
    AddLightSource(S);

    for (int i=0; i<nrows; i++){
        for (int j=0; j<ncols; j++){
            cell C;
            C.area =TotalGridArea/Totalncell;
            position = GetCellCoo(j, i);
            C.center_coo = {position.first, position.second, 0};
            C.power = CellPower(C);
            GRID[i][j] = C;
            vGRID[a] = C;
            a++;
        }
    }
    SortOFPower();

    if (YN1){
        cout<<endl;
        cout<<"Quociente entre a potencia maxima e a potencia da celula correspondente a mediana das potencias:"<<endl;
        cout<<ResultCalculator()<<endl;
        cout<<endl;
        cout<<"Potencia total incidente no plano:"<<endl;
        cout<<totalPower()<<endl;
        cout<<endl;
    }

    if (YN2){
        histogram();
    }

}

lightmap::lightmap(vector<lightsource> vS, array<int,2> ncell, array<float,2> size){
    pair <float,float> position;
    int a=0;

    sizeX = size[0];
    sizeY = size[1];
    TotalGridArea = size[0]*size[1];
    Totalncell = ncell[0]*ncell[1];
    L = sqrt(TotalGridArea/Totalncell);
    vGRID = vector<cell>(Totalncell);
    ncols = ncell[0];
    nrows = ncell[1];
    GRID = vector<vector<cell>>(ncell[1], vector<cell>(ncell[0]));
    S = std::move(vS);

    for (int i=0; i<nrows; i++){
        for (int j=0; j<ncols; j++){
            cell C;
            C.area =TotalGridArea/Totalncell;
            position = GetCellCoo(j, i);
            C.center_coo = {position.first, position.second, 0};
            C.power = CellPower(C);
            GRID[i][j] = C;
            vGRID[a] = C;
            a++;
        }
    }
    SortOFPower();

};

lightmap::lightmap(vector<lightsource> vS, array<int,2> ncell, array<float,2> size, bool YN){
    pair <float,float> position;
    int a = 0;

    sizeX = size[0];
    sizeY = size[1];
    TotalGridArea = size[0]*size[1];
    Totalncell = ncell[0]*ncell[1];
    L = sqrt(TotalGridArea/Totalncell);
    vGRID = vector<cell>(Totalncell);
    ncols = ncell[0];
    nrows = ncell[1];
    GRID = vector<vector<cell>>(ncell[1], vector<cell>(ncell[0]));
    S = std::move(vS);

    for (int i=0; i<nrows; i++){
        for (int j=0; j<ncols; j++){
            cell C;
            C.area =TotalGridArea/Totalncell;
            position = GetCellCoo(j, i);
            C.center_coo = {position.first, position.second, 0};
            C.power = CellPower(C);
            GRID[i][j] = C;
            vGRID[a] = C;
            a++;
        }
    }
    SortOFPower();

    if (YN){
        cout<<endl;
        cout<<"Quociente entre a potencia maxima e a potencia da celula correspondente a mediana das potencias:"<<endl;
        cout<<ResultCalculator()<<endl;
        cout<<endl;
        cout<<"Potencia total incidente no plano:"<<endl;
        cout<<totalPower()<<endl;
        cout<<endl;
    }

};

lightmap::lightmap(vector<lightsource> vS, array<int,2> ncell, array<float,2> size, bool YN1, bool YN2){
    pair <float,float> position;
    int a = 0;

    sizeX = size[0];
    sizeY = size[1];
    TotalGridArea = size[0]*size[1];
    Totalncell = ncell[0]*ncell[1];
    L = sqrt(TotalGridArea/Totalncell);
    vGRID = vector<cell>(Totalncell);
    ncols = ncell[0];
    nrows = ncell[1];
    GRID = vector<vector<cell>>(ncell[1], vector<cell>(ncell[0]));
    S = std::move(vS);

    for (int i=0; i<nrows; i++){
        for (int j=0; j<ncols; j++){
            cell C;
            C.area =TotalGridArea/Totalncell;
            position = GetCellCoo(j, i);
            C.center_coo = {position.first, position.second, 0};
            C.power = CellPower(C);
            GRID[i][j] = C;
            vGRID[a] = C;
            a++;
        }
    }
    SortOFPower();

    if (YN1){
        cout<<endl;
        cout<<"Quociente entre a potencia maxima e a potencia da celula correspondente a mediana das potencias:"<<endl;
        cout<<ResultCalculator()<<endl;
        cout<<endl;
        cout<<"Potencia total incidente no plano:"<<endl;
        cout<<totalPower()<<endl;
        cout<<endl;
    }

    if (YN2){
        histogram();
    }

};

// getters
pair<int,int> lightmap::GetCellIndex(float x, float y) const{
    pair<int,int> CellIndex;

    CellIndex.first = static_cast<int>( ( x - (L/2) ) * ( (ncols-1) / (sizeX-L) ) );
    CellIndex.second = static_cast<int>( ( y - (L/2) ) * ( (nrows-1) / (sizeY-L) ) );

    return CellIndex;
}

pair<float,float> lightmap::GetCellCoo(int index_x, int index_y) const{
    pair<float,float> CellPosition;

    CellPosition.first = static_cast<float>( ( ( (sizeX-L) / (ncols-1) )*index_x ) + (L/2) );
    CellPosition.second = static_cast<float>( ( ( (sizeY-L) / (nrows-1) )*index_y ) + (L/2) );

    return CellPosition;
}

double lightmap::GetCellPower(int index_x, int index_y) const{
    return GRID[index_y][index_x].power;
}

double lightmap::GetCellPower(float x, float y) const{
    pair<int,int> index;
    index = GetCellIndex(x, y);
    return GetCellPower(index.first,index.second);
}

int lightmap::GetCellNx() const{
    return ncols;
} // get number of cells along x

int lightmap::GetCellNy() const{
    return nrows;
} // get number of cells along y

const vector<vector<cell>>& lightmap::GetCells() const{
    return GRID;
}

vector<vector<cell>>& lightmap::GetCells(){
    return GRID;
}

array<float,3> lightmap::GetNormal(){
    return {0,0,1};
}

// other functions
void lightmap::AddLightSource(lightsource SAdd){
    S.push_back(SAdd);
}

const cell& lightmap::GetMaxCell(){
    return vGRID[0];
} // get cell with max power

array <float, 4> lightmap::distance2cell(array<float,3> SourceCoo, array<float,3> PointCoo){
    array<float,4> vetorLStoCell = {0,0,0,0};

    for (int a=0; a<3; a++){
        vetorLStoCell[a] = (SourceCoo[a]-PointCoo[a]);
    }

    vetorLStoCell[3] = distanceCalculator(vetorLStoCell[0], vetorLStoCell[1], vetorLStoCell[2]);
    return vetorLStoCell;
}

float lightmap::Irradiance(array<float,3> PointCoo){
    array<float,4> vetorLStoCell;
    array<float,3> r;
    array<float,3> ProduteOfCoo;
    float Irrad, ProdutoInterno, SumIrrad = 0;
    array<float,3> n = GetNormal();

    for (int b = 0; b<S.size(); b++){
        vetorLStoCell = distance2cell(S[b].coo, PointCoo);
        r = {0,0,0};
        ProduteOfCoo = {0,0,0};
        ProdutoInterno = 0;
        for (int a=0; a<3; a++){
            r[a] = vetorLStoCell[a]/vetorLStoCell[3];
            ProduteOfCoo[a] = (r[a]*GetNormal()[a]);
            ProdutoInterno += ProduteOfCoo[a];
        }
        Irrad = static_cast<float>(( (S[b].power) / (4 * M_PI) ) * ( ProdutoInterno / pow(vetorLStoCell[3],2) ));
        SumIrrad += Irrad;
    }

    return SumIrrad;
}

float lightmap::CellPower(const cell& C){
    return Irradiance(C.center_coo)*C.area;
}

void lightmap::SortOFPower(){
    // Ordenar em ordem decrescente usando uma função lambda
    sort(vGRID.begin(), vGRID.end(),
         [](const cell& a, const cell& b) -> bool {
             return a.power > b.power;
         }
    );
}

float lightmap::medianaOfPower(){
    int V1, V2;
    int size = Totalncell;
    float result;

    if (size % 2 == 0){
        V1 = size/2;
        V2 = (size/2)-1;
        result = ((vGRID[V1].power + vGRID[V2].power)/2);
        return result;
    }
    else{
        V1 = (size-1)/2;
        result = vGRID[V1].power;
        return result;
    }

}

float lightmap::totalPower(){
    float Sum = 0;

    for (int a = 0; a<Totalncell; a++){
        Sum += vGRID[a].power;
    }

    return Sum;
}

// other methods
float lightmap::distanceCalculator(float x, float y, float z){
    return static_cast<float>(sqrt(pow(x,2)+pow(y,2)+pow(z,2)));
}

float lightmap::ResultCalculator(){
    return (GetMaxCell().power / medianaOfPower());
}

void lightmap::histogram(){
    vector<array<float, 3>> data(Totalncell);
    int a = 0;

    for (int i=0; i<nrows; i++){
        for (int j=0; j<ncols; j++){
            data[a][2] = GRID[i][j].power;
            data[a][0] = GetCellCoo(j, i).first;
            data[a][1] = GetCellCoo(j, i).second;
            a++;
        }
    }

    CreateHistogram(data, ncols, nrows, sizeX, sizeY, Totalncell);
}

#endif //MINI_PROJETO_2_LIGHTMAP_H
