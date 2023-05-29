#include <iostream>
#include <cmath>

double tiempo_cliente_cola(double Ws,double mu){

double Wq = Ws - (1/mu);
return Wq;
}