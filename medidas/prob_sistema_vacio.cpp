#include <iostream>
#include <cmath>

double calculateP0(int m, double lambda, double mu) {
    double rho = lambda / (m * mu);
    double sum = 0.0;

    for (int n = 0; n <= m - 1; n++) {
        double numerator = std::pow(rho, n) / std::tgamma(n + 1);
        sum += numerator;
    }

    double denominator = std::pow(rho, m) / std::tgamma(m) * (1.0 - rho / m);
    double p0 = 1.0 / (sum + denominator);

    return p0;
}

double prob_sistema_vacio(double lambda, double mu,int m) {

    double p0 = calculateP0(m, lambda, mu);
    return p0;
}
