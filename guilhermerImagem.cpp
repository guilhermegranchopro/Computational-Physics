//////////////////// PROJECT 1 ////////////////////

// Done on May 27th


////////// Part A //////////


// Imported libraries

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;


// Iniciar a estrutura específica para guardar toda a informação de uma imagem

struct Image{
    int WhiteValue = 0, ncols = 0, nrows = 0; // Iniciar o valor branco máximo e o tamanho da imagem
    vector<vector<int>> matriz; // matriz onde se irá guardar os valores dos pixeis
};


// Definição de todas as funções usadas

Image ReadImage(const string& filename);
void Histogram(Image Original);
vector<double> AverageVariance(Image Original);
void SaveImagePGM(const string& FileName, vector<vector<int>> principal, int width, int height, int WhiteValue);
void InvertedImage(Image Original);
Image NoiseCancellation(Image Original);
Image BoxFiltering(Image Original);

int main(){
    Image imagemOriginal = ReadImage("imagem.pgm");// Retirar informação da imagem original

    cout << "Imagem original:" << endl << endl;

    Histogram(imagemOriginal);// Analise da recorrencia dos valores dos pixels
    vector<double> MV = AverageVariance(imagemOriginal);// Calcula a media e a variancia da imagem original

    InvertedImage(imagemOriginal);// Cria uma imagem inversa à original

    Image imagemTratada = NoiseCancellation(imagemOriginal);// Cria uma imagem tratada com o novo algoritmo
    cout << "Primeira imagem tratada:" << endl;
    cout << endl;
    vector<double> MVImagemTratada = AverageVariance(imagemTratada);// Calcula a media e a variancia da imagem tratada

    Image imagemTratadaComOSegundoMetodo = BoxFiltering(imagemOriginal);// Cria uma nova imagem tratada com o segundo algoritmo
    cout << "Segunda imagem tratada:" << endl;
    cout << endl;
    vector<double> MVImagemTratadaComOSegundoMetodo = AverageVariance(imagemTratadaComOSegundoMetodo);// Calcula a media e a variancia da nova imagem tratada
}

Image ReadImage(const string& filename){

    // 1. Ler a configuração da imagem (apenas as 3 primeiras linhas)

    //... Abrir o ficheiro da imagem
    ifstream FI(filename);
    string line;

    //... Ler a primeira linha
    getline(FI, line);
    getline(FI, line);

    // Inicializar a estrutura para guardar informação da imagem
    Image Original;

    //... Ler a 2a linha e recuperar o numero de linhas e colunas
    istringstream iss(line);
    iss >> Original.ncols >> Original.nrows;

    //... Ler a 3a linha e recuperar o código máximo de cor
    getline(FI, line);
    iss.clear();
    iss.str(line);
    iss >> Original.WhiteValue;

    // 2. Criação da matriz [nrows, ncols]
    Original.matriz = vector<vector<int>>(Original.nrows, vector<int>(Original.ncols));

    // 3. Ler a imagem numa matriz
    for(int i=0; i<Original.nrows; i++){
        for(int j=0; j<Original.ncols; j++){
            FI >> Original.matriz[i][j];
        }
    }

    // Fechar o ficheiro
    FI.close();
    return Original;
}

void Histogram(Image Original){

    // Inicializa todos os vetores usados ao longo da função
    vector<double> ColourFreq(Original.WhiteValue+1), top(Original.WhiteValue+1), VirtualColourFreq(Original.WhiteValue+1), ColourFreqRel(Original.WhiteValue+1);

    // Construimos o vector de frequencias absolutas das recorrencias das cores
    for(int i=0; i<Original.nrows; i++){
        for(int j=0; j<Original.ncols; j++){
            ColourFreq[Original.matriz[i][j]]++;
        }
    }

    cout << "Vetor de frequencias absolutas:" << endl;
    for (int u=0; u<ColourFreq.size(); u++){
        top[u] = u;// Inicializa o vetor top
        cout << "Numero de recorrencias da cor " << u << ": " << ColourFreq[u] << endl;
        ColourFreqRel[u] = (ColourFreq[u] / (Original.nrows * Original.ncols)) * 100;// Calcula o vetor de frequencias relativas
        VirtualColourFreq [u] = ColourFreq[u];// Cria um vetor extra para ser usado mais à frente
    }
    cout << endl;

    // Mostra o vetor de frequencia relativa
    cout << "Vetor de frequencias relativas:" << endl;
    for (size_t a=0; a<ColourFreq.size(); a++){
        cout << "Percentagem de recorrencias da cor " << a << " na imagem: " << ColourFreqRel[a] << "%" << endl;
    }
    cout << endl;

    // Organiza por ordem crescente o vetor de frequencias absolutas em paralelo com o top
    sort(top.begin(), top.end(), [&VirtualColourFreq](size_t i, size_t j) { return VirtualColourFreq[i] < VirtualColourFreq[j]; });

    // Mostra o vetor por ordem crescente com o valor das cores mais usadas
    cout << "Lista de cores mais usadas por ordem crescente:" << endl;
    for (size_t a=0; a<ColourFreq.size(); a++){
        cout << "Posicao " << ColourFreq.size()-a << ": cor " << top[a] << endl;
    }
    cout << endl;

}

vector<double> AverageVariance(Image Original){

    double media, variancia, sum, npixels;

    npixels = Original.nrows * Original.ncols;// Calcula o numero de pixels da imagem

    // Faz um somatorio dos valores da matriz
    for(int i=0; i<Original.nrows; i++){
        for(int j=0; j<Original.ncols; j++){
            sum = sum + Original.matriz[i][j];
        }
    }

    // Calcula a media
    media = sum / npixels;
    cout << "media da imagem: " <<  media << endl;

    sum = 0;

    // Realiza os calculos necessarios para a variancia
    for(int i=0; i<Original.nrows; i++){
        for(int j=0; j<Original.ncols; j++){
            sum = sum + pow((Original.matriz[i][j] - media), 2);
        }
    }

    // Acaba de calcular a variancia
    variancia = sum / (npixels - 1);
    cout << "variancia da imagem: " << variancia << endl;
    cout << endl;

    // Guarda os valores da media e da variancia
    vector<double> MV = {media, variancia};

    return MV;
}

// Guarda novas imagens PGM criadas
void SaveImagePGM(const string& FileName, vector<vector<int>> principal, int width, int height, int WhiteValue){

    ofstream arquivo(FileName, std::ios::binary); // Abre um novo arquivo para escrever o codigo da imagem PGM
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

    arquivo.close();// Fecha o arquivo
}

void InvertedImage(Image Original){

    // Inicializa a estrutura da imagem invertida
    Image imagemInverted;
    imagemInverted.nrows = Original.nrows;
    imagemInverted.ncols = Original.ncols;
    imagemInverted.WhiteValue = Original.WhiteValue;
    imagemInverted.matriz = vector<vector<int>>(Original.nrows, vector<int>(Original.ncols));

    // Inicializa a estrutura da imagem original somada com a invertida
    Image imagemSum;
    imagemSum.nrows = Original.nrows;
    imagemSum.ncols = Original.ncols;
    imagemSum.WhiteValue = Original.WhiteValue;
    imagemSum.matriz = vector<vector<int>>(Original.nrows, vector<int>(Original.ncols));

    // Calcula a imagem invertida e o somatorio da original com a invertida
    for(int i=0; i<Original.nrows; i++){
        for(int j=0; j<Original.ncols; j++){
            imagemInverted.matriz[i][j] = Original.WhiteValue - Original.matriz[i][j];
            imagemSum.matriz[i][j] = imagemInverted.matriz[i][j] + Original.matriz[i][j];
        }
    }

    // Guarda no pc as duas imagens criadas como ficheiros PGM
    SaveImagePGM("peixe_noise10_inverted.ascii.pgm", imagemInverted.matriz, Original.ncols, Original.nrows, Original.WhiteValue);
    SaveImagePGM("somatorio_da_imagem_invertida_com_a_original.pgm", imagemSum.matriz, Original.ncols, Original.nrows, Original.WhiteValue);

}

Image NoiseCancellation(Image Original){

    // Inicializa a imagem tratada
    Image imagemTratada;
    imagemTratada.nrows = Original.nrows;
    imagemTratada.ncols = Original.ncols;
    imagemTratada.WhiteValue = Original.WhiteValue;
    imagemTratada.matriz = vector<vector<int>>(Original.nrows, vector<int>(Original.ncols));

    // Algoritmo de tratamento de imagem
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

                    sum += Original.matriz[a][b];
                }
            }

            sum -= Original.matriz[i][j];
            imagemTratada.matriz[i][j] = static_cast<int>(round(sum / 8.0));
        }
    }

    // Guarda a imagem
    SaveImagePGM("peixe_reduced_noise.asciiw.pgm", imagemTratada.matriz, Original.ncols, Original.nrows, Original.WhiteValue);

    return imagemTratada;
}

Image BoxFiltering(Image Original){

    // Inicializa a imagem tratada
    Image imagemTratadaComOSegundoMetodo;
    imagemTratadaComOSegundoMetodo.nrows = Original.nrows;
    imagemTratadaComOSegundoMetodo.ncols = Original.ncols;
    imagemTratadaComOSegundoMetodo.WhiteValue = Original.WhiteValue;
    imagemTratadaComOSegundoMetodo.matriz = vector<vector<int>>(Original.nrows, vector<int>(Original.ncols));

    // Algoritmo de tratamento de imagem
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

                    sum += Original.matriz[a][b];
                }
            }

            imagemTratadaComOSegundoMetodo.matriz[i][j] = static_cast<int>(round(sum / 9.0));
        }
    }

    // Guarda a imagem
    SaveImagePGM("peixe_box_blur.ascii.pgm", imagemTratadaComOSegundoMetodo.matriz, Original.ncols, Original.nrows, Original.WhiteValue);

    return imagemTratadaComOSegundoMetodo;
}