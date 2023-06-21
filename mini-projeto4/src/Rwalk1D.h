#ifndef __RWALK1D__
#define __RWALK1D__

#include <vector>
#include <map>
#include <random>

using namespace std;

class Rwalk1D {

public:

    Rwalk1D(int N=1, double x=0., // N=nb of particles, x=x(0)
            double pL=0.5, double pR=0.5, // probabilities Left Right
            double dt=1, double dx=1 // time and space steps
    );
    ~Rwalk1D();

    // particle simulation
    virtual void Run(int nsteps); // number of steps
    // getters
    virtual const vector<double>& GetTrajectory(int n=1); // particle number
    virtual double GetTimeStep();
    virtual double GetSpaceStep();
    virtual int RandomProbability();

private:
    double x0; // init coo
    int N; // number of particles
    double pL, pR; // probabilities (left, same, righ)
    double dt, dx; // steps (time, space)
    map<int, vector<double> > mT; // trajectories
    default_random_engine randomEngine; // Declare a random number generator
};


#endif
