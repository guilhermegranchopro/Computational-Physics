// PROJETO 1

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

vector<vector<int>> ReadImage(string filename);

auto Histograma(string filename);

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

auto Histograma(string filename){

    // Criar vector de inteiros, ColourFreq, com a dimensao do numero de cores n+1, da cor preta a cor branca passando pelos cinzentos

    vector<int> ColourFreq(N+1);

    for (int i = 0; i < ReadImage(filename).size(); i++) { // loop externo percorre as linhas da matriz
        for (int j = 0; j < ReadImage(filename).size(); j++) { // loop interno percorre as colunas da linha atual
            ColourFreq[ReadImage(filename)[i][j]] += 1;
        }
    }
    cout << ColourFreq << endl;

    // Determinar a cor mais frequente

    int maximo = max_element(ColourFreq.begin(), ColourFreq.end());
    int indice_maximo = distance(ColourFreq.begin(), maximo);
    cout << "A cor mais frequente é " << indice_maximo;

    // Somar todos os valores do vetor ColourFreq

    int total = 0;
    for (int i = 0; i < ColourFreq.size(); i++) {
        total += ColourFreq[i];
    }

    // Criar um novo vetor para armazenar as frequências relativas em porcentagem

    vector<double> freq_relativa(ColourFreq.size());

    // Calcular as frequências relativas em percentagem
    
    for (int i = 0; i < ColourFreq.size(); i++) {
        freq_relativa[i] = ColourFreq[i] / total * 100;
    }
    cout << "Frequências Relativas: "<< freq_relativa << endl;
}