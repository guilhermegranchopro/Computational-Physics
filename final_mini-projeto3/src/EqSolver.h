#ifndef __EQSOLVER__
#define __EQSOLVER__

#include "FCmatrixAlgo.h"

#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

class EqSolver {
public:
// constructors and destructor
    EqSolver() = default;
    EqSolver(
            const Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>&,// matrix coeffs
            const Eigen::Matrix<double,Eigen::Dynamic,1>& // vector ofconstants
    );
    ~EqSolver() = default;

// output (optional)
    friend ostream& operator<<(ostream&, const EqSolver&);

// solvers
    const Eigen::Matrix<double,Eigen::Dynamic,1>& GaussSolver(bool pivot=false);
    const Eigen::Matrix<double,Eigen::Dynamic,1>& LUSolver(bool pivot=false);
    void IterativeJacobiSolver(
            Eigen::Matrix<double,Eigen::Dynamic,1>&, // starting solution
            int& itmax, //nb of max iterations
            double tol=1.E-3// tolerance on convergence
    );
    void IterativeGaussSeidelSolver(
            Eigen::Matrix<double,Eigen::Dynamic,1>&,
            int& itmax,
            double tol=1.E-3
    );

//getter
    Eigen::Matrix<double,Eigen::Dynamic,1> Getx();

//setters
    void SetM(Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>);
    void Setb(Eigen::Matrix<double,Eigen::Dynamic,1>);
    void Setx(Eigen::Matrix<double,Eigen::Dynamic,1>);

private:
    Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> M; //coefficients matrix
    Eigen::Matrix<double,Eigen::Dynamic,1> b; // constants vector
    Eigen::Matrix<double,Eigen::Dynamic,1> x; //solution
};

#endif
