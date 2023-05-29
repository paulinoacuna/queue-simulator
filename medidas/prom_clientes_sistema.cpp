#include <iostream>
#include <cmath>

double calculateLs(int m, double lambda, double mu, double p0) {
    double rho = lambda / (m * mu); 
    double ls = (lambda * mu * (std::pow(lambda / mu, m)) * p0) / (std::tgamma(m-1) * std::pow(m*mu - lambda, 2));
    return (ls + (lambda/mu))/10;
}

double prom_clientes_sistema(double lambda, double mu, int m, double p0) {
    // Cálculo de Ls
    double ls = calculateLs(m, lambda, mu, p0);
    return ls;
}
