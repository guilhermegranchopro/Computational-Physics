#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <utility>
#include <algorithm>

using namespace Eigen;
using namespace std;

static void GaussEliminationPivot(Matrix<double,Dynamic,Dynamic>& matrixCoeff, Matrix<double,Dynamic,1>& vectorOfConstants, Matrix<double,Dynamic,1>& rowOrder) {
    Matrix < double, Dynamic, Dynamic > augmentedMatrix(matrixCoeff.rows(), matrixCoeff.cols() + 1);
    augmentedMatrix << matrixCoeff, vectorOfConstants;

    int nrows = augmentedMatrix.rows();
    int ncols = augmentedMatrix.cols();

    vector<double> VCoeffMax(nrows);

    double factor;

    for (int a = 0; a < nrows; a++) {
        VCoeffMax[a] = matrixCoeff.row(a).maxCoeff();
    }

    for (int j = 0; j < ncols - 2; j++) {
        vector <pair<double, double>> VPivoteNumber(nrows - j);
        double rel_size_cand = augmentedMatrix(j, j) / VCoeffMax[j];

        for (int a = j; a < nrows; a++) {
            VPivoteNumber[a - j].first = a;
            VPivoteNumber[a - j].second = abs(augmentedMatrix(a, j) / VCoeffMax[a]);
        }

        sort(VPivoteNumber.begin(), VPivoteNumber.end(),
             [](const pair<double, double> &a, const pair<double, double> &b) {
                 return a.second < b.second;
             });

        if (rel_size_cand < VPivoteNumber[0].second) {
            swap(augmentedMatrix.row(j), augmentedMatrix.row(VPivoteNumber[0].first));
            double copia = VCoeffMax[j];
            VCoeffMax[j] = VCoeffMax[VPivoteNumber[0].first];
            VCoeffMax[VPivoteNumber[0].first] = copia;
        }

        for (int a = j + 1; a < nrows; a++) {
            factor = augmentedMatrix(a, j) / augmentedMatrix(j, j);
            augmentedMatrix.row(a) = augmentedMatrix.row(a) - augmentedMatrix.row(j) * factor;
        }
    }
}

int main() {
    // Definição da matriz de coeficientes e vetor de constantes
    Matrix<double, 3, 3> matrixCoeff;
    matrixCoeff << 2, 1, -1,
            -3, -1, 2,
            -2, 1, 2;

    Matrix<double, 3, 1> vectorOfConstants;
    vectorOfConstants << 8, -11, -3;

    // Criação da matriz de ordem das linhas
    Matrix<double, 3, 1> rowOrder;
    rowOrder << 0, 1, 2;

    // Aplicação do método de eliminação de Gauss com pivoteamento
    GaussEliminationPivot(matrixCoeff, vectorOfConstants, rowOrder);

    return 0;
}