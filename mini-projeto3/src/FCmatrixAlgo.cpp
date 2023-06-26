#include "FCmatrixAlgo.h"

using namespace std;
using namespace Eigen;

FCmatrixAlgo::FCmatrixAlgo(const Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& Cmatrix, const Eigen::Matrix<double,Eigen::Dynamic,1>& Cvector){
    M = Cmatrix;
    b = Cvector;
}

void FCmatrixAlgo::GaussElimination(Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& A, Eigen::Matrix<double,Eigen::Dynamic,1>& c){

    int n = M.rows();

    for (int k = 0; k < n - 1; k++) {
        for (int i = k + 1; i < n; i++) {
            double factor = M(i, k) / M(k, k);
            b(i) -= factor * b(k);
            for (int j = k; j < n; j++) {
                M(i, j) -= factor * M(k, j);
            }
        }
    }

}

void FCmatrixAlgo::GaussEliminationPivot(Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>& A, Eigen::Matrix<double,Eigen::Dynamic,1>& c, Eigen::Matrix<double,Eigen::Dynamic,1>& v){

    int n = M.rows();

    for (int k = 0; k < n - 1; k++) {
        // Find the pivot row
        int pivotRow = k;
        double maxElement = std::abs(M(k, k));
        for (int i = k + 1; i < n; i++) {
            double currentElement = std::abs(M(i, k));
            if (currentElement > maxElement) {
                maxElement = currentElement;
                pivotRow = i;
            }
        }

        // Swap rows to bring the pivot row to the current row position
        if (pivotRow != k) {
            M.row(k).swap(M.row(pivotRow));
            std::swap(b(k), b(pivotRow));
        }

        for (int i = k + 1; i < n; i++) {
            double factor = M(i, k) / M(k, k);
            b(i) -= factor * b(k);
            for (int j = k; j < n; j++) {
                M(i, j) -= factor * M(k, j);
            }
        }
    }

}

void FCmatrixAlgo::LUdecomposition(Eigen::Matrix<double,Eigen::Dynamic, Eigen::Dynamic>& A,Eigen::Matrix<int,Eigen::Dynamic,1>& v, bool bpivot){

    int n = M.rows();
    L = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>::Identity(n, n);
    U = M;
    P = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>::Identity(n, n);

    if (bpivot) {
        for (int k = 0; k < n - 1; k++) {
            int pivotRow = k;
            double maxPivot = std::abs(U(k, k));

            // Find the pivot element with maximum absolute value in the current column
            for (int i = k + 1; i < n; i++) {
                double absValue = std::abs(U(i, k));
                if (absValue > maxPivot) {
                    maxPivot = absValue;
                    pivotRow = i;
                }
            }

            // Swap rows in U
            U.row(k).swap(U.row(pivotRow));

            // Swap rows in L (except the first k-1 elements)
            for (int i = 0; i < k; i++) {
                std::swap(L(k, i), L(pivotRow, i));
            }

            // Swap rows in P
            P.row(k).swap(P.row(pivotRow));

            for (int i = k + 1; i < n; i++) {
                double factor = U(i, k) / U(k, k);
                L(i, k) = factor;
                for (int j = k; j < n; j++) {
                    U(i, j) -= factor * U(k, j);
                }
            }
        }
    } else {
        // Perform LU decomposition without pivoting
        for (int k = 0; k < n - 1; k++) {
            for (int i = k + 1; i < n; i++) {
                double factor = U(i, k) / U(k, k);
                L(i, k) = factor;
                for (int j = k; j < n; j++) {
                    U(i, j) -= factor * U(k, j);
                }
            }
        }
    }

    std::cout << "L:\n";
    for (int i = 0; i < L.rows(); ++i) {
        for (int j = 0; j < L.cols(); ++j) {
            std::cout << L(i, j) << " ";
        }
        std::cout << "\n";
    }
    cout<<endl;
    std::cout << "U:\n";
    for (int i = 0; i < U.rows(); ++i) {
        for (int j = 0; j < U.cols(); ++j) {
            std::cout << U(i, j) << " ";
        }
        std::cout << "\n";
    }
    cout<<endl;

}


Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> FCmatrixAlgo::GetM(){
    return M;
}

Eigen::Matrix<double,Eigen::Dynamic,1> FCmatrixAlgo::Getb(){
    return b;
}

Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> FCmatrixAlgo::GetU(){
    return U;
}

Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> FCmatrixAlgo::GetP(){
    return P;
}

Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> FCmatrixAlgo::GetL(){
    return L;
}