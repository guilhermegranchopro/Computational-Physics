#include "FCmatrixAlgo.h"
#include "EqSolver.h"

using namespace std;
using namespace Eigen;

int main() {
    // define matrix equations: A.X=b
    // create matrix A and vector of constants b
    Matrix<double, 3, 3> A;
    A << 1, 1, 1,
        1, 2, 1,
        1, 1, 3;

    Matrix<double, 3, 1> b;
    b << 1, 2, 3;

    EqSolver S(A,b);
    auto X = S.GaussSolver();
    cout << "Gauss simplification:" << endl;
    cout << S;
    cout << "Gauss solution: \n" << X << endl;
    cout << endl;

    EqSolver Q(A,b);
    auto Y = Q.GaussSolver(true);
    cout << "Gauss simplification with pivoting:" << endl;
    cout << Q;
    cout << "Gauss solution with pivoting: \n" << Y << endl;
    cout << endl;

    EqSolver T(A,b);
    cout << "LU decomposition (Doolitle) simplification:" << endl;
    auto Z = T.LUSolver();
    cout << "LU decomposition (Doolitle) solution: \n" << Z << endl;
    cout << endl;

    EqSolver C(A,b);
    cout << "LU decomposition (Doolitle) with pivoting simplification:" << endl;
    auto W = C.LUSolver(true);
    cout << "LU decomposition (Doolitle) with pivoting solution: \n" << W << endl;
    cout << endl;

    int itmax = 1000;
    double tol = 1.E-6;

    EqSolver M(A,b);
    Eigen::Matrix<double, Eigen::Dynamic, 1> zeros(A.rows()); // Vector of size 3 initialized with zeros
    zeros.setZero(); // Alternatively, you can use the setZero() function to set all elements to zero
    M.IterativeJacobiSolver(zeros, itmax, tol);
    cout << "Iterative Jacobi Solver: \n" << zeros << endl;

    cout<<endl;

    EqSolver N(A,b);
    Eigen::Matrix<double, Eigen::Dynamic, 1> zeros2(A.rows()); // Vector of size 3 initialized with zeros
    zeros2.setZero(); // Alternatively, you can use the setZero() function to set all elements to zero
    N.IterativeGaussSeidelSolver(zeros2, itmax, tol);
    cout << "Iterative Gauss Seidel Solver: \n" << zeros2 << endl;

    return 0;
}