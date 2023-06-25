#include "ODEpoint.h"

using namespace std;

Xvar::Xvar(int nVariables){
    x.resize(nVariables);
}

Xvar::Xvar(vector<double> newX){
    x = newX;
}

Xvar::Xvar(const initializer_list<double>& v){
    x = v;
}

Xvar::Xvar(const Xvar& oldX){
    x = oldX.x;
}

Xvar& Xvar::operator=(const Xvar& other) {
    if (this != &other) { // Verifica se não é o mesmo objeto
        x = other.x; // Copia o valor do outro objeto
    }
    return *this; // Retorna uma referência para o próprio objeto
}

// Operador de adição
Xvar Xvar::operator+(const Xvar& other) {
    Xvar result;
    size_t size = x.size();

    if (size == other.x.size()) {
        result.x.resize(size);
        for (size_t i = 0; i < size; i++) {
            result.x[i] = x[i] + other.x[i];
        }
    }

    return result;
}

double& Xvar::operator[](int index) {
    return x[index];
}

Xvar operator*(double scalar, const Xvar& xvar) {
    vector<double> result;
    for (double value : xvar.x) {
        result.push_back(scalar * value);
    }
    return Xvar(result);
}

ostream& operator<<(ostream& os, const Xvar& xvar) {
    for (double value : xvar.x) {
        os << value << " ";
    }
    return os;
}

vector<double>& Xvar::X(){
    return x;
}

void ODEpoint::SetODEpoint(double t_, Xvar& p) {
    t = t_;
    static_cast<Xvar&>(*this) = p; // Atribuição do objeto Xvar
}

void ODEpoint::SetODEpoint(double t_, const initializer_list<double>& v) {
    t = t_;
    Xvar::x = v; // Atribuição do vetor x da classe Xvar
}

void ODEpoint::SetODEpoint(double t_, vector<double> v) {
    t = t_;
    Xvar::x = v; // Atribuição do vetor x da classe Xvar
}