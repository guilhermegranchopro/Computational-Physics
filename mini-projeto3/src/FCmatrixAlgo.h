#ifndef __FCMATRIXALGO__
#define __FCMATRIXALGO__

#include "EqSolver.h"

#include <Eigen/Core>

using namespace std;
using namespace Eigen;

class FCmatrixAlgo {
public:
    FCmatrixAlgo() = default;
    FCmatrixAlgo(
            const Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>&,// matrix coeffs
            const Eigen::Matrix<double,Eigen::Dynamic,1>& // vector ofconstants
    );
    ~FCmatrixAlgo() = default;

/*
Implements Gauss elimination
*/
    void GaussElimination(
            Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>&,// matrix coeffs
            Eigen::Matrix<double,Eigen::Dynamic,1>& //vector of constants
    ); //no pivoting
    void GaussEliminationPivot(
            Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>&,// matrix coeff
            Eigen::Matrix<double,Eigen::Dynamic,1>&, //vector of constants
            Eigen::Matrix<double,Eigen::Dynamic,1>& // row order indexing
    ); //make pivoting

/*
Implements LU decomposition (Doolitle)
*/
    void LUdecomposition(
            Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>&,// matrix coeff
            Eigen::Matrix<int,Eigen::Dynamic,1>&, // row order indexing
            bool bpivot=false // activate pivoting
    );

//getters
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> GetM();
    Eigen::Matrix<double,Eigen::Dynamic,1> Getb();
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> GetU();
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> GetP();
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> GetL();

private:
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> M; //coefficients matrix
    Eigen::Matrix<double,Eigen::Dynamic,1> b; // constants vector
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> U;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> P;
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> L;
};

#endif
