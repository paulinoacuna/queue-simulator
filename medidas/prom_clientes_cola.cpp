#include <iostream>
#include <cmath>

double prom_clientes_cola (double lambda, double mu, double Ls){

double Lq = Ls - (lambda/mu);

return Lq;
}