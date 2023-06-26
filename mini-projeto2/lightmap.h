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

// Structure representing a cell in the lightmap

struct cell {
    array<float,3> center_coo; // cell center coordinates (cm)
    float area; // cell area (cmˆ2)
    float power; // cell incident power (W)
};


// Structure representing a light source

struct lightsource {
    array<float,3> coo; // Source coordinates (cm)
    float power; // Source power (W)
};



// Class representing a lightmap

class lightmap {
public:

    
////////////////////////////////// IMPLEMENTAÇÃO ////////////////////////////////

    
////////// Constructors //////////

// Allow you to create a light map with different settings

    lightmap() = default;
    lightmap(array<int,2> ncell, std::array<float,2> size);
    lightmap(lightsource LS, array<int,2> ncell, array<float,2> size);
    lightmap(lightsource LS, array<int,2> ncell, array<float,2> size, bool YN);
    lightmap(lightsource LS, array<int,2> ncell, array<float,2> size, bool YN1, bool YN2);
    lightmap(vector<lightsource> vS, array<int,2> ncell, array<float,2> size);
    lightmap(vector<lightsource> vS, array<int,2> ncell, array<float,2> size, bool YN);
    lightmap(vector<lightsource> vS, array<int,2> ncell, array<float,2> size, bool YN1, bool YN2);


/////////// Getters //////////

// Allow you to access light map specific information

    pair<int,int> GetCellIndex(float x, float y) const;  // return cell indices
    pair<float,float> GetCellCoo(int index_x, int index_y) const;  //return cell center coo
    double GetCellPower(int index_x, int index_y) const;  // return cell power Watts
    double GetCellPower(float x, float y) const;  // return cell powerWatts
    int GetCellNx() const;  // get number of cells along x
    int GetCellNy() const;  // get number of cells along y
    const vector<vector<cell>>& GetCells() const;  // return cells grid
    vector<vector<cell>>& GetCells();  // return cells grid
    array<float,3> GetNormal();  // return normal to plane


////////// Other functions //////////

// Allow you to get the results of certain calculus or add features to the light map

    void AddLightSource(lightsource LS);  // Add a light source
    const cell& GetMaxPowerCell();  // Get cell with max power
    array<float, 4> DistanceToCell(array<float,3> SourceCoo, array<float,3> PointCoo);  // Calculate distance between the lightsource and some cell
    float Irradiance(array<float,3> PointCoo);  // Calculate irradiance at a given point
    float CellPower(const cell& C);  // Calculate power of a cell
    void SortPower();  // Sort the cells based on their power
    float MedianPower();  // Calculate the median power of the cells
    float TotalPower();  // Calculate the total incident power on the plane


////////// Other methods //////////

    float DistanceCalculator(float x, float y, float z);  // Calculate distance between two points
    float ResultCalculator();  // Calculate the ratio between maximum power and power at the cell corresponding to the median power
    void Histogram();  // Generate a histogram of cell powers


////////// Private Variables //////////

private:

    vector<vector<cell>> GRID;  // 2D grid of cells
    int Totalncell;  // Total number of cells
    int ncols;  // Number of cells in x axis
    int nrows;  // Number of cells in y axis
    float TotalGridArea;  // Total area of the grid
    vector<lightsource> S;  // Vector of light sources
    vector<cell> vGRID;  // Vector with cell's power
    float L;
    float sizeX, sizeY; // Size of the x and y axes, respectively
};


////////////////////////////// DECLARAÇÃO ///////////////////////////////////


////////// Constructors //////////


// Base constructor that creates the light map

lightmap::lightmap(array<int,2> ncell, array<float,2> size){
    sizeX = size[0];
    sizeY = size[1];
    TotalGridArea = size[0] * size[1];
    Totalncell = ncell[0] * ncell[1];
    L = sqrt(TotalGridArea/Totalncell);
    vGRID = vector<cell>(Totalncell);
    ncols = ncell[0];
    nrows = ncell[1];
    GRID = vector<vector<cell>>(ncell[1], vector<cell>(ncell[0]));
}


// Adds a light source LS to the previous base constructor, calculates the power of the cells and sorts them from most power to least power

lightmap::lightmap(lightsource LS, array<int,2> ncell, array<float,2> size){
    pair <float,float> position;
    int a = 0;

    sizeX = size[0];
    sizeY = size[1];
    TotalGridArea = size[0] * size[1];
    Totalncell = ncell[0] * ncell[1];
    L = sqrt(TotalGridArea/Totalncell);
    vGRID = vector<cell>(Totalncell);
    ncols = ncell[0];
    nrows = ncell[1];
    GRID = vector<vector<cell>>(ncell[1], vector<cell>(ncell[0]));
    AddLightSource(LS);

    for (int i = 0; i < nrows; i++){
        for (int j = 0; j < ncols; j++){
            cell C;

            C.area = TotalGridArea/Totalncell;
            position = GetCellCoo(j, i);
            C.center_coo = {position.first, position.second, 0};
            C.power = CellPower(C);
            GRID[i][j] = C;
            vGRID[a] = C;
            a++;
        }
    }
    SortPower();

}


// Similar to the previous one, but has an additional parameter YN that controls the printing of some information

lightmap::lightmap(lightsource LS, array<int,2> ncell, array<float,2> size, bool YN){
    pair <float,float> position;
    int a = 0;

    sizeX = size[0];
    sizeY = size[1];
    TotalGridArea = size[0] * size[1];
    Totalncell = ncell[0] * ncell[1];
    L = sqrt(TotalGridArea/Totalncell);
    vGRID = vector<cell>(Totalncell);
    ncols = ncell[0];
    nrows = ncell[1];
    GRID = vector<vector<cell>>(ncell[1], vector<cell>(ncell[0]));

    AddLightSource(LS);

    for (int i = 0; i < nrows; i++){
        for (int j = 0; j < ncols; j++){
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
    SortPower();

    if (YN){
        cout<< endl;
        cout<< "Ratio between the maximum power and the power of the cell corresponding to the median power:" << endl;
        cout<< ResultCalculator() << endl << endl;
        cout<< "Total incident power on the plane:" << endl;
        cout<< TotalPower()<< endl << endl;
    }

}


// Similar to the previous one, but has two additional parameters YN1 and YN2    that controls the printing of some information

lightmap::lightmap(lightsource LS, array<int,2> ncell, array<float,2> size, bool YN1, bool YN2){
    pair <float,float> position;
    int a = 0;

    sizeX = size[0];
    sizeY = size[1];
    TotalGridArea = size[0] * size[1];
    Totalncell = ncell[0] * ncell[1];
    L = sqrt(TotalGridArea/Totalncell);
    vGRID = vector<cell>(Totalncell);
    ncols = ncell[0];
    nrows = ncell[1];
    GRID = vector<vector<cell>>(ncell[1], vector<cell>(ncell[0]));

    AddLightSource(LS);

    for (int i = 0; i < nrows; i++){
        for (int j = 0; j < ncols; j++){
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
    SortPower();

    if (YN1){
        cout<< endl;
        cout<< "Ratio between the maximum power and the power of the cell corresponding to the median power:" << endl;
        cout<< ResultCalculator() << endl << endl;
        cout<< "Total incident power on the plane:" << endl;
        cout<< TotalPower()<< endl << endl;
    }

    if (YN2){
        Histogram();
    }

}


// This constructor receives a vector of light sources vS instead of a single one, then calls the basic constructor and adds the vector vS to the base S one

lightmap::lightmap(vector<lightsource> vS, array<int,2> ncell, array<float,2> size){
    pair <float,float> position;
    int a = 0;

    sizeX = size[0];
    sizeY = size[1];
    TotalGridArea = size[0] * size[1];
    Totalncell = ncell[0] * ncell[1];
    L = sqrt(TotalGridArea/Totalncell);
    vGRID = vector<cell>(Totalncell);
    ncols = ncell[0];
    nrows = ncell[1];
    GRID = vector<vector<cell>>(ncell[1], vector<cell>(ncell[0]));
    S = move(vS);

    for (int i = 0; i < nrows; i++){
        for (int j = 0; j < ncols; j++){
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
    SortPower();

};


// Similar to the previous one, but has an additional parameter YN that controls the printing of some information

lightmap::lightmap(vector<lightsource> vS, array<int,2> ncell, array<float,2> size, bool YN){
    pair <float,float> position;
    int a = 0;

    sizeX = size[0];
    sizeY = size[1];
    TotalGridArea = size[0] * size[1];
    Totalncell = ncell[0] * ncell[1];
    L = sqrt(TotalGridArea/Totalncell);
    vGRID = vector<cell>(Totalncell);
    ncols = ncell[0];
    nrows = ncell[1];
    GRID = vector<vector<cell>>(ncell[1], vector<cell>(ncell[0]));
    S = move(vS);

    for (int i = 0; i < nrows; i++){
        for (int j = 0; j < ncols; j++){
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
    SortPower();

    if (YN){
        cout<< endl;
        cout<< "Ratio between the maximum power and the power of the cell corresponding to the median power:" << endl;
        cout<< ResultCalculator() << endl << endl;
        cout<< "Total incident power on the plane:" << endl;
        cout<< TotalPower()<< endl << endl;
    }

};


// Similar to the previous one, but has two additional parameters YN1 and YN2    that controls the printing of some information

lightmap::lightmap(vector<lightsource> vS, array<int,2> ncell, array<float,2> size, bool YN1, bool YN2){
    pair <float,float> position;
    int a = 0;

    sizeX = size[0];
    sizeY = size[1];
    TotalGridArea = size[0] * size[1];
    Totalncell = ncell[0] * ncell[1];
    L = sqrt(TotalGridArea/Totalncell);
    vGRID = vector<cell>(Totalncell);
    ncols = ncell[0];
    nrows = ncell[1];
    GRID = vector<vector<cell>>(ncell[1], vector<cell>(ncell[0]));
    S = move(vS);

    for (int i = 0; i < nrows; i++){
        for (int j = 0; j<ncols; j++){
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
    SortPower();

    if (YN1){
        cout<< endl;
        cout<< "Ratio between the maximum power and the power of the cell corresponding to the median power:" << endl;
        cout<< ResultCalculator() << endl << endl;
        cout<< "Total incident power on the plane:" << endl;
        cout<< TotalPower()<< endl << endl;
    }

    if (YN2){
        Histogram();
    }

};


////////// Getters //////////


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




////////// Other functions //////////


void lightmap::AddLightSource(lightsource LS){
    S.push_back(LS);
}

const cell& lightmap::GetMaxPowerCell(){
    return vGRID[0];
} // get cell with max power

array <float, 4> lightmap::DistanceToCell(array<float,3> SourceCoo, array<float,3> PointCoo){
    array<float,4> vetorLStoCell = {0,0,0,0};

    for (int a=0; a<3; a++){
        vetorLStoCell[a] = (SourceCoo[a]-PointCoo[a]);
    }

    vetorLStoCell[3] = DistanceCalculator(vetorLStoCell[0], vetorLStoCell[1], vetorLStoCell[2]);
    return vetorLStoCell;
}

float lightmap::Irradiance(array<float,3> PointCoo){
    array<float,4> vetorLStoCell;
    array<float,3> r;
    array<float,3> ProduteOfCoo;
    float Irrad, ProdutoInterno, SumIrrad = 0;
    array<float,3> n = GetNormal();

    for (int b = 0; b<S.size(); b++){
        vetorLStoCell = DistanceToCell(S[b].coo, PointCoo);
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

void lightmap::SortPower(){
    // Ordenar em ordem decrescente usando uma função lambda
    sort(vGRID.begin(), vGRID.end(),
         [](const cell& a, const cell& b) -> bool {
             return a.power > b.power;
         }
    );
}

float lightmap::MedianPower(){
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

float lightmap::TotalPower(){
    float Sum = 0;

    for (int a = 0; a<Totalncell; a++){
        Sum += vGRID[a].power;
    }

    return Sum;
}

// other methods
float lightmap::DistanceCalculator(float x, float y, float z){
    return static_cast<float>(sqrt(pow(x,2)+pow(y,2)+pow(z,2)));
}

float lightmap::ResultCalculator(){
    return (GetMaxPowerCell().power / MedianPower());
}

void lightmap::Histogram(){
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
