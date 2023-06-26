#include "ODEpoint.h"
#include "ODEsolver.h"

#include <cmath>

using namespace std;

void DrawGraphics(vector <ODEpoint>, int, string, string, string);

// Parâmetros do pêndulo
const double g = 9.80665;  // Aceleração da gravidade (m/s^2)
const double L = 4.0;   // Comprimento do pêndulo (m)

// Funções de equações diferenciais
double dtheta_dt(double omega) {
    return omega;
}

double domega_dt(double theta) {
    return -g / L * sin(theta);
}

vector<function <double(double)>> functions = {dtheta_dt, domega_dt};


int main() {

    ODEsolver Pendulum(functions);
    ODEpoint i(0, {M_PI/4, 0});
    double step = 1e-2, Xmax = 10;

    Pendulum.Euler(i, step, Xmax);
    Pendulum.PredictorCorrector(i, step, Xmax);
    Pendulum.LeapFrog(i, step, Xmax);
    Pendulum.RK2(i, step, Xmax);
    Pendulum.RK4(i, step, Xmax);

    Pendulum.printMS();

    return 0;
}
