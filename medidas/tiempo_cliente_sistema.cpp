#include <iostream>
#include <cmath>

double calculateWs(int m, double lambda, double mu, double p0) {

    double Ws = (mu * (std::pow(lambda / mu, m)) * p0) / (std::tgamma(m-1) * std::pow(m*mu - lambda, 2));
    return Ws + (1/mu);
}

double tiempo_cliente_sistema(double lambda, double mu, int m, double p0) {
    // Cálculo de Ls
    double Ws = calculateWs(m, lambda, mu, p0);
    return Ws;
}
