#ifndef __INTEGDERIV__
#define __INTEGDERIV__

#include "Functor.h"
#include "MyFunction.h"
#include <iostream>
#include <limits>
#include <cmath>

using namespace std;

class IntegDeriv {
public:
    IntegDeriv(Functor& function) : F(function){}
    ~IntegDeriv() = default;

    // Métodos de integração
    virtual void TrapezoidalRule(double xi, double xf, double& Integral, double& Error);
    virtual void simpsonRule(double xi, double xf, double& Integral, double& Error);
    // Métodos de derivação
    virtual double calculateSecondDerivative(double x);
    virtual double calculateFourthDerivative(double x);
    // Método de identificação de um máximo relativo
    virtual double computeMax(double a, double b, double (IntegDeriv::*derivate)(double));

private:
    Functor& F;
    int n = 1e7;
    double H = 1e-3;
};

#endif