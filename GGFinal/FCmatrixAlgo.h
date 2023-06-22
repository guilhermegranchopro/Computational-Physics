#ifndef GGFINAL_FCMATRIXALGO_H
#define GGFINAL_FCMATRIXALGO_H

#include <Eigen/Dense>
#include <iostream>

using namespace std;
using namespace Eigen;

class FCmatrixAlgo {
public:
    //constructors
    FCmatrixAlgo() = default;
    ~FCmatrixAlgo() = default;

    //getters
    Matrix<double, Dynamic, Dynamic> GetCoeffs();
    Matrix<double, Dynamic, 1> GetConstants();

/*
Implements Gauss elimination
*/
    static void GaussElimination(Matrix<double, Dynamic, Dynamic>& coeffs, Matrix<double, Dynamic, 1>& constants); // no pivoting
    static void GaussEliminationPivot(Matrix<double, Dynamic, Dynamic>& matrixCoeff, Matrix<double, Dynamic, 1>& vectorOfConstants); // make pivoting

/*
Implements LU decomposition (Doolittle)
*/
    static void LUdecomposition(Matrix<double, Dynamic, Dynamic>& coeff, Matrix<int, Dynamic, 1>& rowOrder, bool bpivot = false);

private:
    static Matrix<double, Dynamic, Dynamic> M;
    static Matrix<double, Dynamic, 1> b;
};

// Definição dos membros estáticos da classe FCmatrixAlgo
Matrix<double, Dynamic, Dynamic> FCmatrixAlgo::M;
Matrix<double, Dynamic, 1> FCmatrixAlgo::b;


//implementations

Matrix<double, Dynamic, Dynamic> FCmatrixAlgo::GetCoeffs() {
    return M;
}

Matrix<double, Dynamic, 1> FCmatrixAlgo::GetConstants() {
    return b;
}

void FCmatrixAlgo::GaussElimination(Matrix<double, Dynamic, Dynamic>& coeffs, Matrix<double, Dynamic, 1>& constants) {
    int n = coeffs.rows();

    for (int k = 0; k < n - 1; k++) {
        for (int i = k + 1; i < n; i++) {
            double lambda = coeffs(i, k) / coeffs(k, k);
            for (int j = k + 1; j < n; j++) {
                coeffs(i, j) -= lambda * coeffs(k, j);
            }
            constants(i) -= lambda * constants(k);
            coeffs(i, k) = 0.0;  // Set the lower triangular elements to 0
        }
    }


    M = coeffs;
    b = constants;
}

void FCmatrixAlgo::GaussEliminationPivot(Matrix<double, Dynamic, Dynamic>& matrixCoeff, Matrix<double, Dynamic, 1>& vectorOfConstants) {
    int nrows = matrixCoeff.rows();
    int ncols = matrixCoeff.cols();
    Matrix<double, Dynamic, Dynamic> augmentedMatrix(nrows, ncols + 1);
    augmentedMatrix << matrixCoeff, vectorOfConstants;

    vector<double> VCoeffMax(nrows);

    for (int a = 0; a < nrows; a++) {
        VCoeffMax[a] = augmentedMatrix.row(a).head(ncols).cwiseAbs().maxCoeff();
    }

    for (int j = 0; j < ncols - 1; j++) {
        vector<pair<double, double>> VPivoteNumber(nrows - j);
        double rel_size_cand = augmentedMatrix(j, j) / VCoeffMax[j];

        for (int a = j; a < nrows; a++) {
            VPivoteNumber[a - j].first = a;
            VPivoteNumber[a - j].second = abs(augmentedMatrix(a, j) / VCoeffMax[a]);
        }

        sort(VPivoteNumber.begin(), VPivoteNumber.end(),
             [](const pair<double, double>& a, const pair<double, double>& b) {
                 return a.second > b.second;
             });

        if (rel_size_cand < VPivoteNumber[0].second) {
            augmentedMatrix.row(j).swap(augmentedMatrix.row(VPivoteNumber[0].first));
            double copia = VCoeffMax[j];
            VCoeffMax[j] = VCoeffMax[VPivoteNumber[0].first];
            VCoeffMax[VPivoteNumber[0].first] = copia;
        }

        for (int a = j + 1; a < nrows; a++) {
            double factor = augmentedMatrix(a, j) / augmentedMatrix(j, j);
            augmentedMatrix.row(a) = augmentedMatrix.row(a) - augmentedMatrix.row(j) * factor;
        }
    }

    M = augmentedMatrix.leftCols(ncols);
    b = augmentedMatrix.col(ncols);
}

void FCmatrixAlgo::LUdecomposition(Matrix<double, Dynamic, Dynamic>& coeff, Matrix<int, Dynamic, 1>& rowOrder, bool bpivot) {
    int n = coeff.rows();

    for (int k = 0; k < n - 1; k++) {
        if (bpivot) {
            // Pivot row with the largest absolute value in the current column
            int pivotRow = k;
            double maxAbsValue = std::abs(coeff(rowOrder(k), k));

            for (int i = k + 1; i < n; i++) {
                double absValue = std::abs(coeff(rowOrder(i), k));
                if (absValue > maxAbsValue) {
                    maxAbsValue = absValue;
                    pivotRow = i;
                }
            }

            // Swap rows
            if (pivotRow != k) {
                rowOrder.row(k).swap(rowOrder.row(pivotRow));
            }
        }

        for (int i = k + 1; i < n; i++) {
            double multiplier = coeff(rowOrder(i), k) / coeff(rowOrder(k), k);
            coeff(rowOrder(i), k) = multiplier;

            for (int j = k + 1; j < n; j++) {
                coeff(rowOrder(i), j) -= multiplier * coeff(rowOrder(k), j);
            }
        }
    }

    M = coeff;
}

#endif //GGFINAL_FCMATRIXALGO_H
