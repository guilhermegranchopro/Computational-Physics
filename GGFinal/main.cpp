#include <iostream>
#include "EqSolver.h"
#include "FCmatrixAlgo.h"

using namespace std;
using namespace Eigen;


int main() {
    // define matrix equations: A.X=b
    // create matrix A and vector of constants b
    Matrix<double, 3, 3> A;
    A << 2, -2, 6,
        -2, 4, 3,
        -1, 8, 4;

    Matrix<double, 3, 1> b;
    b << 16, 0, -1;

    // solution with Gauss (no pivoting)
    EqSolver S(A,b);
    auto X = S.GaussSolver(false);
    cout << "Gauss (no pivoting): ";
    for (int i = 0; i < X.size(); i++) {
        cout << X[i] << " ";
    }
    cout << endl;

    // solution with Gauss pivoting
    EqSolver SG(A,b);
    auto Y = SG.GaussSolver(true);
    cout << "Gauss pivoting: ";
    for (int i = 0; i < Y.size(); i++) {
        cout << Y[i] << " ";
    }
    cout << endl;

    // solution with LU decomposition
    EqSolver SLUD(A,b);
    auto Z = SLUD.LUSolver(false);;
    cout << "LU decomposition: ";
    for (int i = 0; i < Z.size(); i++) {
        cout << Z[i] << " ";
    }
    cout << endl;

    // Solution with Jacobi iterative method
    EqSolver SJI(A,b);
    Matrix<double, Dynamic, 1> startingSolution = VectorXd::Zero(3);
    int itmax = 10;
    double tol = 1.0E-4;
    auto W = SJI.IterativeJacobiSolver(startingSolution, itmax, tol);
    cout << "Jacobi iterative method: ";
    for (int i = 0; i < W.size(); i++) {
        cout << W[i] << " ";
    }
    cout << endl;

    // Solution with Gauss-Seidel iterative method
    EqSolver SGS(A,b);
    auto T = SGS.IterativeGaussSeidelSolver(startingSolution, itmax, tol);
    cout << "Gauss-Seidel iterative method: ";
    for (int i = 0; i < T.size(); i++) {
        cout << T[i] << " ";
    }
    cout << endl;

    return 0;
}
