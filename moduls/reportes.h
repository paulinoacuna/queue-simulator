// reportes.h

#ifndef REPORTES_H
#define REPORTES_H

#include <stdio.h>  // Para fprintf

extern FILE* resultados;
extern float lambda;
extern float media_atencion;
extern int num_servidores;
extern float tiempo_simulacion;
extern float C;  // Ahora, 'C' está incluido

extern double prob_sistema_vacio(double lambda, double media, int num_servidores);
extern double prom_clientes_sistema(double lambda, double media, int num_servidores, double P0);
extern double prom_clientes_cola(double lambda, double media, double Ls);
extern double tiempo_cliente_sistema(double lambda, double media, int num_servidores, double P0);
extern double tiempo_cliente_cola(double Ws, double media);

void reportes(void);

#endif  // REPORTES_H
