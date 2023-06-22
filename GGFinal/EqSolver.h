#ifndef GGFINAL_EQSOLVER_H
#define GGFINAL_EQSOLVER_H

#include "FCmatrixAlgo.h"

using namespace std;
using namespace Eigen;

class EqSolver {
public:
// constructors and destructor
    EqSolver();
    EqSolver(const Matrix<double, Dynamic, Dynamic>& matrixCoeffs, const Matrix<double, Dynamic, 1>& vectorConstants);
    ~EqSolver() = default;

// solvers
    const vector<double> GaussSolver(bool pivot = false);
    const vector<double> LUSolver(bool pivot = false);
    const Matrix<double, Dynamic, 1> IterativeJacobiSolver(Matrix<double, Dynamic, 1>& startingSolution, int& itmax, double tol);
    const Matrix<double, Dynamic, 1> IterativeGaussSeidelSolver(Matrix<double, Dynamic, 1>& startingSolution, int& itmax, double tol);

private:
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> M; //coefficients matrix
    Eigen::Matrix<double,Eigen::Dynamic,1> b; // constants vector
};

EqSolver::EqSolver(const Matrix<double, Dynamic, Dynamic>& matrixCoeffs, const Matrix<double, Dynamic, 1>& vectorConstants) : M(matrixCoeffs), b(vectorConstants) {
}

const vector<double> EqSolver::GaussSolver(bool pivot) {
    const int n = M.rows();
    Matrix<double, Dynamic, Dynamic> U = M;
    Matrix<double, Dynamic, 1> c = b;
    FCmatrixAlgo MAlgo;

    if (pivot) {
        // Apply pivoting if requested
        FCmatrixAlgo::GaussEliminationPivot(U, c);
    }
    else {
        FCmatrixAlgo::GaussElimination(U, c);
    }

    U = MAlgo.GetCoeffs();
    c = MAlgo.GetConstants();

    vector<double> x(n);

    for (int k = n - 1; k >= 0; k--) {
        double sumX = 0;

        for (int j = k + 1; j < n; j++) {
            sumX += x[j] * U(k, j);
        }

        x[k] = (c[k] - sumX) / U(k, k);
    }

    return x;
}

const vector<double> EqSolver::LUSolver(bool pivot) {
    int n = M.rows(); // Assuming M is an n x n matrix
    Matrix<double, Dynamic, Dynamic> U = M;
    Matrix<double, Dynamic, 1> c = b;
    FCmatrixAlgo MAlgo;
    Matrix<int, Dynamic, 1> rowOrder(n);
    for (int i = 0; i < n; i++) {
        rowOrder(i) = i;
    }

    FCmatrixAlgo::LUdecomposition(U, rowOrder, false);

    U = MAlgo.GetCoeffs();

    vector<double> y(n);
    // Forward solution (Ly = b)
    for (int k = 0; k < n; k++) {
        double sumC = 0.0;
        for (int i = 0; i < k; i++) {
            sumC += y[i] * U(k, i);
        }
        y[k] = c[k] - sumC;
    }

    vector<double> x(n);
    // Backward solution (Ux = y)
    for (int k = n - 1; k >= 0; k--) {
        double sumC = 0.0;
        for (int i = k + 1; i < n; i++) {
            sumC += x[i] * U(k, i);
        }
        x[k] = (y[k] - sumC) / U(k, k);
    }

    return x;
}


const Matrix<double, Dynamic, 1> EqSolver::IterativeJacobiSolver(Matrix<double, Dynamic, 1>& startingSolution, int& itmax, double tol) {
    int m = startingSolution.size();
    Matrix<double, Dynamic, Dynamic> A = M; // Assuming M is the matrix A
    Matrix<double, Dynamic, 1> b = this->b;
    Matrix<double, Dynamic, 1> x = startingSolution;
    Matrix<double, Dynamic, 1> x_aux = x;
    bool btol = false;
    int it = 0;
    double eps = tol;

    while (!btol && (it++ < itmax)) {
        x_aux = x;

        for (int i = 0; i < m; i++) {
            double sum = 0.0;

            for (int j = 0; j < m; j++) {
                if (i != j) {
                    sum += -A(i, j) * x_aux[j];
                }
            }

            x[i] = (b[i] + sum) / A(i, i);
        }

        btol = true; // Assume convergence

        for (int i = 0; i < m; i++) {
            if (fabs(x[i] - x_aux[i]) >= eps) {
                btol = false; // Not converged
                break;
            }
        }
    }

    return x;
}

const Matrix<double, Dynamic, 1> EqSolver::IterativeGaussSeidelSolver(Matrix<double, Dynamic, 1>& startingSolution, int& itmax, double tol) {
    int m = startingSolution.size();
    Matrix<double, Dynamic, Dynamic> A = M; // Assuming M is the matrix A
    Matrix<double, Dynamic, 1> b = this->b;
    Matrix<double, Dynamic, 1> x = startingSolution;
    Matrix<double, Dynamic, 1> x_aux = x;
    bool btol = false;
    int it = 0;
    double eps = tol;

    while (!btol && (it++ < itmax)) {
        x_aux = x;

        for (int i = 0; i < m; i++) {
            x[i] = 0.0;

            for (int j = 0; j < m; j++) {
                if (i != j) {
                    x[i] += -A(i, j) * x[j];
                }
            }

            x[i] += b[i];
            x[i] /= A(i, i);
        }

        btol = true; // Assume convergence

        for (int i = 0; i < m; i++) {
            if (fabs(x[i] - x_aux[i]) >= eps) {
                btol = false; // Not converged
                break;
            }
        }
    }

    return x;
}

#endif //GGFINAL_EQSOLVER_H
