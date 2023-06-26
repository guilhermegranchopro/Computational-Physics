#include "Functor.h"
#include <iostream>
#include <limits>
#include <cmath>
#include "IntegDeriv.h"

using namespace std;

void IntegDeriv::TrapezoidalRule(double xi, double xf, double& Integral, double& Error) {
    cout << endl;
    cout << "Trapezoidal Rule Integral:" << endl;
    double h = (xf - xi) / n;

    double product = (xf - xi) / (2 * n);
    double sum = 0;
    for (int a = 1; a <= n - 1; a++) {
        sum += F(xi + a * h);
    }

    Integral = product * (F(xi) + 2 * sum + F(xf));
    cout << "Integral: " << Integral << endl;

    double PError = ((pow(h, 2) * (xf - xi)) / 12);
    Error = PError * computeMax(xi, xf, &IntegDeriv::calculateSecondDerivative);
    cout << "Error: " << Error << endl;
    cout << endl;
}

void IntegDeriv::simpsonRule(double xi, double xf, double& Integral, double& Error) {
    cout << endl;
    cout << "Simpson Rule Integral:" << endl;
    double h = (xf - xi) / n;

    double Fsum = 0;
    for (int a = 1; a <= n / 2; a++) {
        Fsum += F(xi + (2 * a - 1) * h);
    }

    double Ssum = 0;
    for (int a = 1; a <= n / 2 - 1; a++) {
        Ssum += F(xi + 2 * a * h);
    }

    Integral = (1.0 / 3.0) * h * (F(xi) + 4 * Fsum + 2 * Ssum + F(xf));
    cout << "Integral: " << Integral << endl;

    double PError = (pow(h, 4) * (xf - xi)) / 180;
    Error = PError * abs(computeMax(xi, xf, &IntegDeriv::calculateFourthDerivative));
    cout << "Error: " << Error << endl;
    cout << endl;
}

double IntegDeriv::calculateSecondDerivative(double x) {
    return ((-F(x + 2*H) + 16 * F(x + H) - 30*F(x) + 16*F(x - H) - F(x - 2*H)) / ((pow(H, 2)*12)));
}

double IntegDeriv::calculateFourthDerivative(double x) {
    return ((F(x + 2 * H) - 4 * F(x + H) + 6 * F(x) - 4 * F(x - H) + F(x - 2 * H)) / (pow(H, 4)));
}

double IntegDeriv::computeMax(double a, double b, double (IntegDeriv::*derivate)(double)) {
    double maxFourthDerivative = numeric_limits<double>::min();

    for (double x = a; x <= b; x += H) {
        double fourthDerivative = (this->*derivate)(x);
        maxFourthDerivative = max(maxFourthDerivative, fourthDerivative);
    }

    return maxFourthDerivative;
}