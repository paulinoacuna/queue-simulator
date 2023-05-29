#ifndef LLEGADA_H
#define LLEGADA_H

#include <stdio.h>  // Para FILE

// Definición de constantes
#define OCUPADO 1
#define LIBRE 0
#define LIMITE_COLA 10000  // Tendrás que definir el valor correcto para tu situación.

// Variables externas
extern float media_entre_llegadas;
extern float media_atencion;
extern float tiempo_simulacion;
extern int num_entra_cola;
extern float tiempo_sig_evento[3];
extern int estado_servidor[10];
extern float tiempo_llegada[LIMITE_COLA + 1];
extern int num_clientes_espera;
extern int servidor_actual;
extern float total_de_esperas;
extern FILE *resultados;

// Funciones externas
extern float funcionPercentilExpon(float media);

// Función llegada
void llegada(void);

#endif  // LLEGADA_H
