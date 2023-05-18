//
// Created by guilh on 12/05/2023.
//

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

struct Image{
    int WhiteValue = 0, ncols = 0, nrows = 0;
    vector<vector<int>> M;
};

Image ReadImage(const string& filename);
void Histograma(Image Original);
vector<double> MediaVariancia(Image Original);
void salvarImagemPGM(const string& nomeArquivo, vector<vector<int>> principal, int width, int height, int WhiteValue);
void InvertedImage(Image Original);
Image CancelamentodeRuido(Image Original);
Image CancelamentodeRuidoVersaoMelhorada(Image Original);

int main(){
    Image imagemOriginal = ReadImage("imagem_proj1.pgm");
    Histograma(imagemOriginal);
    vector<double> MV = MediaVariancia(imagemOriginal);
    InvertedImage(imagemOriginal);
    Image imagemTratada = CancelamentodeRuido(imagemOriginal);
    vector<double> MVImagemTratada = MediaVariancia(imagemTratada);
    Image imagemTratadaComOSegundoMetodo = CancelamentodeRuidoVersaoMelhorada(imagemOriginal);
    vector<double> MVImagemTratadaComOSegundoMetodo = MediaVariancia(imagemTratadaComOSegundoMetodo);
}

Image ReadImage(const string& filename){

    // 1. read image configuration (3 first lines)
    //... abrir o ficheiro da imagem
    ifstream FI(filename);
    string line;

    //... ler a primeira linha
    getline(FI, line);
    getline(FI, line);

    Image Original;

    //... ler a 2a linha e recuperar o numero de linhas e colunas
    istringstream iss(line);
    iss >> Original.ncols >> Original.nrows;

    //... ler a 3a linha e recuperar o codigo maximo de cor
    getline(FI, line);
    iss.clear();
    iss.str(line);
    iss >> Original.WhiteValue;

    // 2. creation of matrix M [nrows, ncols]
    Original.M = vector<vector<int>>(Original.nrows, vector<int>(Original.ncols));

    // 3. read image to matrix
    for(int i=0; i<Original.nrows; i++){
        for(int j=0; j<Original.ncols; j++){
            FI >> Original.M[i][j];
        }
    }

    FI.close();
    return Original;
}

void Histograma(Image Original){

    vector<double> ColourFreq(Original.WhiteValue+1), top(Original.WhiteValue+1), VirtualColourFreq(Original.WhiteValue+1), ColourFreqRel(Original.WhiteValue+1);

    for(int i=0; i<Original.nrows; i++){
        for(int j=0; j<Original.ncols; j++){
            ColourFreq[Original.M[i][j]]++;
        }
    }

    //cout << "Vetor de frequencias absolutas:" << endl;
    for (int u=0; u<ColourFreq.size(); u++){
        top[u] = u;
        //cout << ColourFreq[u] << endl;
        ColourFreqRel[u] = (ColourFreq[u] / (Original.nrows * Original.ncols)) * 100;
        VirtualColourFreq [u] = ColourFreq[u];
    }

    /*cout << "Vetor de frequencias relativas:" << endl;
    for (size_t a=0; a<ColourFreq.size(); a++){
        cout << ColourFreqRel[a] << "%" << endl;
    }*/

    sort(top.begin(), top.end(), [&VirtualColourFreq](size_t i, size_t j) { return VirtualColourFreq[i] < VirtualColourFreq[j]; });

    /*cout << "Lista de cores mais usadas por ordem crescente:" << endl;
    for (size_t a=0; a<ColourFreq.size(); a++){
        cout << top[a] << endl;
    }*/

}

vector<double> MediaVariancia(Image Original){

    double media, variancia, sum, npixels;

    npixels = Original.nrows * Original.ncols;

    for(int i=0; i<Original.nrows; i++){
        for(int j=0; j<Original.ncols; j++){
            sum = sum + Original.M[i][j];
        }
    }

    media = sum / npixels;
    //cout << media << endl;

    sum = 0;

    for(int i=0; i<Original.nrows; i++){
        for(int j=0; j<Original.ncols; j++){
            sum = sum + pow((Original.M[i][j] - media), 2);
        }
    }

    variancia = sum / (npixels - 1);
    //cout << variancia << endl;

    vector<double> MV = {media, variancia};

    return MV;
}

void salvarImagemPGM(const string& nomeArquivo, vector<vector<int>> principal, int width, int height, int WhiteValue){

    ofstream arquivo(nomeArquivo, std::ios::binary);
    int a = 0;

    // Escreve o cabeçalho PGM no arquivo
    arquivo << "P2" << std::endl;
    arquivo << width << " " << height << std::endl;
    arquivo << WhiteValue << std::endl;

    // Escreve os pixels da imagem no arquivo
    for (unsigned y = 0; y < height; y++) {
        for (unsigned x = 0; x < width; x++) {
            arquivo << principal[y][x] << " ";
            a++;
            if (a==20){
                arquivo << endl;
                a = 0;
            }
        }
    }

    arquivo.close();
}

void InvertedImage(Image Original){

    vector<vector<int>> MInverted(Original.nrows, vector<int>(Original.ncols)), Sum(Original.nrows, vector<int>(Original.ncols));

    for(int i=0; i<Original.nrows; i++){
        for(int j=0; j<Original.ncols; j++){
            MInverted[i][j] = Original.WhiteValue - Original.M[i][j];
            Sum[i][j] = MInverted[i][j] + Original.M[i][j];
            //cout << Sum[i][j] << " ";
        }
    }

    salvarImagemPGM("glassware_noisy_inverted.ascii.pgm", MInverted, Original.ncols, Original.nrows, Original.WhiteValue);

}

Image CancelamentodeRuido(Image Original){

    Image imagemTratada;
    imagemTratada.nrows = Original.nrows;
    imagemTratada.ncols = Original.ncols;

    for (int i = 0; i < Original.nrows; i++) {
        for (int j = 0; j < Original.ncols; j++) {
            double sum = 0.0;

            for (int a = i - 1; a <= i + 1; a++) {
                if (a < 0 || a >= Original.nrows) {
                    continue;
                }

                for (int b = j - 1; b <= j + 1; b++) {
                    if (b < 0 || b >= Original.ncols) {
                        continue;
                    }

                    sum += Original.M[a][b];
                }
            }

            sum -= Original.M[i][j];
            imagemTratada.M[i][j] = static_cast<int>(round(sum / 8.0));
        }
    }


    salvarImagemPGM("glassware_reduced_noise.ascii.pgm", imagemTratada.M, Original.ncols, Original.nrows, Original.WhiteValue);

    return imagemTratada;
}

Image CancelamentodeRuidoVersaoMelhorada(Image Original){

    Image imagemTratadaComOSegundoMetodo;
    imagemTratadaComOSegundoMetodo.nrows = Original.nrows;
    imagemTratadaComOSegundoMetodo.ncols = Original.ncols;
    double sum = 0;

    for(int i=0; i<Original.nrows; i++){
        for(int j=0; j<Original.ncols; j++){
            for(int a=i-1; i+1<a; a++){
                if (0<a || a>=Original.nrows){
                    break;
                }
                for(int b=j-1; j+1<b; b++){
                    if (b<0 || b>=Original.ncols){
                        break;
                    }
                    sum = sum + (Original.M[a][b]*(1.0/9));
                }
            }
            imagemTratadaComOSegundoMetodo.M[i][j] = static_cast<int>(round(sum));
            sum = 0;
        }
    }

    salvarImagemPGM("glassware_box_blur.ascii.pgm", imagemTratadaComOSegundoMetodo.M, Original.ncols, Original.nrows, Original.WhiteValue);

    return imagemTratadaComOSegundoMetodo;
}

