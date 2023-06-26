#include "Rwalk1D.h"
#include <vector>
#include <map>
#include <random>
#include <iostream>

using namespace std;

Rwalk1D::Rwalk1D(int N, double x, double pL, double pR, double dt, double dx) : x0(x), N(N), pL(pL), pR(pR), dt(dt), dx(dx) {
    // Initialize the random number generator
    random_device rd;
    randomEngine = default_random_engine(rd());
}

Rwalk1D::~Rwalk1D() {
    // Se houver necessidade de alguma ação específica antes de destruir o objeto,
    // você pode implementar aqui.
}


void Rwalk1D::Run(int nsteps) {
    mT.clear();
    for (int i = 0; i<N; i++) {
            mT[i]= vector<double>(nsteps, 0.0);
    }

    for (int i = 0; i<N; i++) {
        mT[i][0] = x0;
        for (int a = 1; a <= nsteps; a++) {
            mT[i][a * dt] = RandomProbability() + mT[i][a * dt - dt];
        }
    }

}

const vector<double>& Rwalk1D::GetTrajectory(int n) {
    return mT[n];
}

double Rwalk1D::GetTimeStep() {
    return dt;
}

double Rwalk1D::GetSpaceStep() {
    return dx;
}

int Rwalk1D::RandomProbability() {
    uniform_real_distribution<double> distribution(0.0, 1.0);

    double randomNumber = distribution(randomEngine);
    int result = (randomNumber < pL) ? -dx : dx;
    return result;
}
