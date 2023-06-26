#include "EqSolver.h"

using namespace std;
using namespace Eigen;

EqSolver::EqSolver(const Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& Cmatrix, const Eigen::Matrix<double,Eigen::Dynamic,1>& Cvector){
    M = Cmatrix;
    b = Cvector;
    x.resize(b.rows());
    x.setZero();
}

ostream& operator<<(ostream& os, const EqSolver& eqSolver) {
    os << "M:\n" << eqSolver.M << "\n";
    cout << endl;
    os << "b:\n" << eqSolver.b << "\n";
    cout << endl;
    return os;
}

const Eigen::Matrix<double,Eigen::Dynamic,1>& EqSolver::GaussSolver(bool pivot){

    FCmatrixAlgo AlgoG(M, b);

    if (pivot){
        int size = M.rows();
        // Create a vector where each value is the index number
        Matrix<double, Eigen::Dynamic, 1> v(size);
        v.setLinSpaced(size, 0, size - 1);
        AlgoG.GaussEliminationPivot(M, b, v);
    }
    else{
        AlgoG.GaussElimination(M, b);
    }

    SetM(AlgoG.GetM());
    Setb(AlgoG.Getb());

    int n = M.rows();

    x(n - 1) = b(n - 1) / M(n - 1, n - 1);
    for (int i = n - 2; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) {
            sum += M(i, j) * x(j);
        }
        x(i) = (b(i) - sum) / M(i, i);
    }

    return x;

}

const Eigen::Matrix<double,Eigen::Dynamic,1>& EqSolver::LUSolver(bool pivot){

    int size = M.rows();
    // Create a vector where each value is the index number
    Matrix<int, Eigen::Dynamic, 1> v(size);
    v.setLinSpaced(size, 0, size - 1);
    FCmatrixAlgo AlgoLU(M, b);
    AlgoLU.LUdecomposition(M, v, pivot);

    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> U = AlgoLU.GetU();
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> P = AlgoLU.GetP();
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> L = AlgoLU.GetL();

    int n = L.rows();
    Eigen::Matrix<double,Eigen::Dynamic,1> Y = P * b;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            Y(i) -= L(i, j) * Y(j);
        }
        Y(i) /= L(i, i);
    }

    n = U.rows();
    Setx(Y);

    for (int i = n - 1; i >= 0; i--) {
        for (int j = i + 1; j < n; j++) {
            x(i) -= U(i, j) * x(j);
        }
        x(i) /= U(i, i);
    }

    return x;
}

void EqSolver::IterativeJacobiSolver(Eigen::Matrix<double,Eigen::Dynamic,1>& zero, int& itmax, double tol){

    int n = M.rows();

    // Create diagonal matrix D and inverse of D
    MatrixXd D = M.diagonal().asDiagonal();
    MatrixXd D_inv = D.inverse();

    int iteration = 0;
    double residual = (M * zero - b).norm();  // Initial residual

    while (residual > tol && iteration < itmax) {
        VectorXd x_new = D_inv * (b - (M - D) * zero);  // Jacobi update
        residual = (M * x_new - b).norm();
        zero = x_new;
        iteration++;
    }

    cout << "Converged in " << iteration << " iterations." << endl;

}

void EqSolver::IterativeGaussSeidelSolver(Eigen::Matrix<double,Eigen::Dynamic,1>& zero, int& itmax, double tol){

    int n = M.rows();

    int iteration = 0;
    double residual = (M * zero - b).norm();  // Initial residual

    while (residual > tol && iteration < itmax) {
        for (int i = 0; i < n; i++) {
            double sum = 0.0;
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    sum += M(i, j) * zero(j);
                }
            }
            zero(i) = (b(i) - sum) / M(i, i);
        }

        residual = (M * zero - b).norm();
        iteration++;
    }

    cout << "Converged in " << iteration << " iterations." << endl;

}

Eigen::Matrix<double,Eigen::Dynamic,1> EqSolver::Getx(){
    return x;
}

void EqSolver::SetM(Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> newM){
    M = newM;
}

void EqSolver::Setb(Eigen::Matrix<double,Eigen::Dynamic,1> newb){
    b = newb;
}

void EqSolver::Setx(Eigen::Matrix<double,Eigen::Dynamic,1> newx){
    x = newx;
}