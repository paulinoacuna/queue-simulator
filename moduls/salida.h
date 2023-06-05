// salida.h

#ifndef SALIDA_H
#define SALIDA_H
#define LIBRE        0    /* Indicador de servidor libre */

extern int num_entra_cola;
extern int estado_servidor[];
extern int servidor_actual;
extern float tiempo_sig_evento[3];
extern float tiempo_simulacion;
extern float tiempo_llegada[];
extern float total_de_esperas;
extern int num_clientes_espera;
extern float media_atencion;
extern float funcionPercentilExpon(float media);

void salida(void);

#endif  // SALIDA_H
