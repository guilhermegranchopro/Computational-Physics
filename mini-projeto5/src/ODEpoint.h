#ifndef __ODEPOINT__
#define __ODEPOINT__

#include <vector>
#include <ostream>

using namespace std;

class Xvar {
public:
    Xvar() = default;
    Xvar(int); // number of dependent variables
    Xvar(vector<double>);// passing vector
    // using initializer list to build object: X({1,2})
    Xvar(const initializer_list<double>& v);
    ~Xvar() = default;
    Xvar(const Xvar&); // copy constructor

    Xvar& operator=(const Xvar&);// assignment operator
    Xvar operator+(const Xvar&); // operator+
    double& operator[](int); // x[i]

    friend Xvar operator*(double, const Xvar&);// scalar*X
    friend ostream& operator<< (ostream&, const Xvar&);

    vector<double>& X(); // accessor to x

    protected:
    vector<double> x;
};

class ODEpoint : public Xvar {

private:
    double t; //time

public:
    ODEpoint() : t(-1) {}
    ODEpoint(double t_, Xvar a_) : t(t_), Xvar(a_) {}
    ODEpoint(double t_, const vector<double>& v) : t(t_), Xvar(v) {}
    ODEpoint(double t_, const initializer_list<double>& v) : t(t_), Xvar(v) {}

    void SetODEpoint(double t_, Xvar& p);
    void SetODEpoint(double t_, const initializer_list<double>& v);
    void SetODEpoint(double t_, vector<double> v);
    double& T() { return t;} // accessor to time
};

#endif
