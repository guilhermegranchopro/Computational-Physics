// PROJETO 1

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <sstream>
#include "imagem_proj1.pgm"

using namespace std;

vector<vector<int>> ReadImage(string filename);

vector Histograma(string filename);

auto Media (vector<vector<int>> M);

auto Negativo ();

auto EliminarRuido ();

auto Variancia ();

int main(){
    ReadImage("imagem_proj1.pgm");
    Histograma("imagem_proj1.pgm");
    Media();
    Negativo();
    EliminarRuido();
    Variancia();
}

vector<vector<int>> ReadImage(string filename){
    // Ler configuracao do ficheiro nas primeiras 3 linhas
    // Abrir o ficheiro da imagem

    ifstream FI(filename);
    string line;

    // Ler primeira linha

    getline(FI, line);

    // cout << line << endl;
    // Ler segunda linha e recuperar numero de linhas e colunas

    getline(FI, line);

    // cout << line << endl;

    istringstream iss(line);
    int nrows, ncols;
    iss >> nrows >> ncols;

    // Ler terceira linha e recuperar valor maximo, ou seja, codigo maximo de cor

    getline(FI, line);

    // cout << line << endl;

    iss.str(line);
    int WhiteValue;
    iss >> WhiteValue;

    // Criar matriz de inteiros, M, com o tamanho das nrows e ncols

    vector<vector<int>> M(nrows, vector<int>(ncols));

    // Ler o resto da imagem e guardar na matriz M

    for(int i=0; i<nrows; i++){
        for(int j=0; j<ncols; j++){
            FI >> M[i][j];
            // cout << M[i][j] << " ";
        }
        // cout << endl;
    }
    return M;
}

vector Histograma(string filename){
    // Criar vector de inteiros, ColourFreq, com a dimensao do numero de cores n+1, da cor preta a cor branca passando pelos cinzentos

    vector<int> ColourFreq(N+1);

    for (int i = 0; i < ReadImage(filename).size(); i++) { // loop externo percorre as linhas da matriz
        for (int j = 0; j < ReadImage(filename).size(); j++) { // loop interno percorre as colunas da linha atual
            ColourFreq[ReadImage(filename)[i][j]] += 1;
        }
    }
    cout << ColourFreq;

    int maximo = *max_element(ColourFreq.begin(), ColourFreq.end());
    cout << maximo;
}

//Calcular valor médio e variância da imagem

auto Media (vector<vector<int>> M){
    // Calcular a cor média da imagem

    int sum = 0;
    int count = 0;
    for(int i=0; i<nrows; i++){
        for(int j=0; j<ncols; j++){
            sum += M[i][j];
            count++;
        }
    }
    int media = sum/count;
    //cout << "A cor média da imagem é: " << media << endl;

    // Calcular a variância da imagem

    int sum2 = 0;
    for(int i=0; i<nrows; i++){
        for(int j=0; j<ncols; j++){
            sum2 += pow((M[i][j] - media), 2);
        }
    }
    int variancia = sum2/count;
    //cout << "A variância da imagem é: " << variancia << endl;
}

// Criar uma imagem com o negativo da imagem original

auto Negativo (vector<vector<int>> M){
    // Criar matriz de inteiros, M2, com o tamanho das nrows e ncols

    vector<vector<int>> M2(nrows, vector<int>(ncols));

    // Calcular o negativo da imagem

    for(int i=0; i<nrows; i++){
        for(int j=0; j<ncols; j++){
            M2[i][j] = 255 - M[i][j];
        }
    }

    // Escrever a imagem negativa num ficheiro

    ofstream FO("glassware_noisy_inverted.ascii.pgm");
}

// Eliminar o ruído da imagem

auto EliminarRuido (vector<vector<int>> M){
    // Calcular média dos 8 pixeis em redor de cada pixel e trocar o valor do pixel pela média

    for(int i=0; i<nrows; i++){
        for(int j=0; j<ncols; j++){
            int sum = 0;
            int count = 0;
            for(int k=-1; k<2; k++){
                for(int l=-1; l<2; l++){
                    sum += M[i+k][j+l];
                    count++;
                }
            }
            int media = sum/count;
            M[i][j] = media;
        }
    }

    // Escrever a imagem sem ruído num ficheiro

    ofstream FO("glassware_reduced_noise.ascii.pgm");

}

// Calcular a variância da imagem sem ruído

auto Variancia (){
    
    ReadImage("glassware_reduced_noise.ascii.pgm");

    // Calcular a variância da imagem

    int sum2 = 0;
    for(int i=0; i<nrows; i++){
        for(int j=0; j<ncols; j++){
            sum2 += pow((M[i][j] - media), 2);
        }
    }
    int variancia = sum2/count;
    //cout << "A variância da imagem é: " << variancia << endl;
}

