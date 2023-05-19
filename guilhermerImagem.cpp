//
// Mini-projeto 1
//

// importar as bibliotecas
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

//iniciar a estrutura especifica para guardar toda a informação de uma imagem
struct Image{
    int WhiteValue = 0, ncols = 0, nrows = 0;//iniciar o valor branco máximo e o tamanho da imagem
    vector<vector<int>> M;//matriz onde se irá guardar os valores dos pixeis
};

//inicializa todas as funções usadas
Image ReadImage(const string& filename);
void Histograma(Image Original);
vector<double> MediaVariancia(Image Original);
void salvarImagemPGM(const string& nomeArquivo, vector<vector<int>> principal, int width, int height, int WhiteValue);
void InvertedImage(Image Original);
Image CancelamentodeRuido(Image Original);
Image CancelamentodeRuidoVersaoMelhorada(Image Original);

int main(){
    Image imagemOriginal = ReadImage("imagem_proj1.pgm");//retirar informação da imagem original
    cout << "Imagem original:" << endl;
    cout << endl;
    Histograma(imagemOriginal);//analise da recorrencia dos valores dos pixels
    vector<double> MV = MediaVariancia(imagemOriginal);//calcula a media e a variancia da imagem original
    InvertedImage(imagemOriginal);//cria uma imagem inversa à original
    Image imagemTratada = CancelamentodeRuido(imagemOriginal);//cria uma imagem tratada com o novo algoritmo
    cout << "Primeira imagem tratada:" << endl;
    cout << endl;
    vector<double> MVImagemTratada = MediaVariancia(imagemTratada);//calcula a media e a variancia da imagem tratada
    Image imagemTratadaComOSegundoMetodo = CancelamentodeRuidoVersaoMelhorada(imagemOriginal);//cria uma nova imagem tratada com o segundo algoritmo
    cout << "Segunda imagem tratada:" << endl;
    cout << endl;
    vector<double> MVImagemTratadaComOSegundoMetodo = MediaVariancia(imagemTratadaComOSegundoMetodo);//calcula a media e a variancia da nova imagem tratada
}

Image ReadImage(const string& filename){

    // 1. read image configuration (3 first lines)
    //... abrir o ficheiro da imagem
    ifstream FI(filename);
    string line;

    //... ler a primeira linha
    getline(FI, line);
    getline(FI, line);

    //inicializar a estrutura para guardar informação da imagem
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

    //fechar o ficheiro
    FI.close();
    return Original;
}

void Histograma(Image Original){

    //inicializa todos os vetores usados ao longo da função
    vector<double> ColourFreq(Original.WhiteValue+1), top(Original.WhiteValue+1), VirtualColourFreq(Original.WhiteValue+1), ColourFreqRel(Original.WhiteValue+1);

    //construimos o vector de frequencias absolutas das recorrencias das cores
    for(int i=0; i<Original.nrows; i++){
        for(int j=0; j<Original.ncols; j++){
            ColourFreq[Original.M[i][j]]++;
        }
    }

    cout << "Vetor de frequencias absolutas:" << endl;
    for (int u=0; u<ColourFreq.size(); u++){
        top[u] = u;//inicializa o vetor top
        cout << "Numero de recorrencias da cor " << u << ": " << ColourFreq[u] << endl;
        ColourFreqRel[u] = (ColourFreq[u] / (Original.nrows * Original.ncols)) * 100;// calcula o vetor de frequencias relativas
        VirtualColourFreq [u] = ColourFreq[u];//cria um vetor extra para ser usado mais à frente
    }
    cout << endl;

    //mostra o vetor de frequencia relativa
    cout << "Vetor de frequencias relativas:" << endl;
    for (size_t a=0; a<ColourFreq.size(); a++){
        cout << "Percentagem de recorrencias da cor " << a << " na imagem: " << ColourFreqRel[a] << "%" << endl;
    }
    cout << endl;

    //organiza por ordem crescente o vetor de frequencias absolutas em paralelo com o top
    sort(top.begin(), top.end(), [&VirtualColourFreq](size_t i, size_t j) { return VirtualColourFreq[i] < VirtualColourFreq[j]; });

    //mostra o vetor por ordem crescente com o valor das cores mais usadas
    cout << "Lista de cores mais usadas por ordem crescente:" << endl;
    for (size_t a=0; a<ColourFreq.size(); a++){
        cout << "Posicao " << ColourFreq.size()-a << ": cor " << top[a] << endl;
    }
    cout << endl;

}

vector<double> MediaVariancia(Image Original){

    double media, variancia, sum, npixels;

    npixels = Original.nrows * Original.ncols;//calcula o numero de pixels da imagem

    //faz um somatorio dos valores da matriz
    for(int i=0; i<Original.nrows; i++){
        for(int j=0; j<Original.ncols; j++){
            sum = sum + Original.M[i][j];
        }
    }

    //calcula a media
    media = sum / npixels;
    cout << "media da imagem: " <<  media << endl;

    sum = 0;

    //realiza os calculos necessarios para a variancia
    for(int i=0; i<Original.nrows; i++){
        for(int j=0; j<Original.ncols; j++){
            sum = sum + pow((Original.M[i][j] - media), 2);
        }
    }

    //acaba de calcular a variancia
    variancia = sum / (npixels - 1);
    cout << "variancia da imagem: " << variancia << endl;
    cout << endl;

    //guarda os valores da media e da variancia
    vector<double> MV = {media, variancia};

    return MV;
}

//guarda novas imagens PGM criadas
void salvarImagemPGM(const string& nomeArquivo, vector<vector<int>> principal, int width, int height, int WhiteValue){

    ofstream arquivo(nomeArquivo, std::ios::binary); //abre um novo arquivo para escrever o codigo da imagem PGM
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

    arquivo.close();//fecha o arquivo
}

void InvertedImage(Image Original){

    //inicializa a estrutura da imagem invertida
    Image imagemInverted;
    imagemInverted.nrows = Original.nrows;
    imagemInverted.ncols = Original.ncols;
    imagemInverted.WhiteValue = Original.WhiteValue;
    imagemInverted.M = vector<vector<int>>(Original.nrows, vector<int>(Original.ncols));

    //inicializa a estrutura da imagem original somada com a invertida
    Image imagemSum;
    imagemSum.nrows = Original.nrows;
    imagemSum.ncols = Original.ncols;
    imagemSum.WhiteValue = Original.WhiteValue;
    imagemSum.M = vector<vector<int>>(Original.nrows, vector<int>(Original.ncols));

    //Calcula a imagem invertida e o somatorio da original com a invertida
    for(int i=0; i<Original.nrows; i++){
        for(int j=0; j<Original.ncols; j++){
            imagemInverted.M[i][j] = Original.WhiteValue - Original.M[i][j];
            imagemSum.M[i][j] = imagemInverted.M[i][j] + Original.M[i][j];
        }
    }

    //guarda no pc as duas imagens criadas como ficheiros PGM
    salvarImagemPGM("glassware_noisy_inverted.ascii.pgm", imagemInverted.M, Original.ncols, Original.nrows, Original.WhiteValue);
    salvarImagemPGM("Somatorio_da_imagem_invertida_com_a_original.pgm", imagemSum.M, Original.ncols, Original.nrows, Original.WhiteValue);

}

Image CancelamentodeRuido(Image Original){

    //inicializa a imagem tratada
    Image imagemTratada;
    imagemTratada.nrows = Original.nrows;
    imagemTratada.ncols = Original.ncols;
    imagemTratada.WhiteValue = Original.WhiteValue;
    imagemTratada.M = vector<vector<int>>(Original.nrows, vector<int>(Original.ncols));

    //algoritmo de tratamento de imagem
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

    //guarda a imagem
    salvarImagemPGM("glassware_reduced_noise.ascii.pgm", imagemTratada.M, Original.ncols, Original.nrows, Original.WhiteValue);

    return imagemTratada;
}

Image CancelamentodeRuidoVersaoMelhorada(Image Original){

    //inicializa a imagem tratada
    Image imagemTratadaComOSegundoMetodo;
    imagemTratadaComOSegundoMetodo.nrows = Original.nrows;
    imagemTratadaComOSegundoMetodo.ncols = Original.ncols;
    imagemTratadaComOSegundoMetodo.WhiteValue = Original.WhiteValue;
    imagemTratadaComOSegundoMetodo.M = vector<vector<int>>(Original.nrows, vector<int>(Original.ncols));

    //algoritmo de tratamento de imagem
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

            imagemTratadaComOSegundoMetodo.M[i][j] = static_cast<int>(round(sum / 9.0));
        }
    }

    //guarda a imagem
    salvarImagemPGM("glassware_box_blur.ascii.pgm", imagemTratadaComOSegundoMetodo.M, Original.ncols, Original.nrows, Original.WhiteValue);

    return imagemTratadaComOSegundoMetodo;
}
