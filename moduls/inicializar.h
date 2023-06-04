// inicializar.h

#ifndef INICIALIZAR_H
#define INICIALIZAR_H

extern float media_entre_llegadas;
extern float media_atencion;
extern float lambda;
extern float C;
extern float tiempo_simulacion;
extern int num_entra_cola;
extern float tiempo_ultimo_evento;
extern int servidor_actual;
extern int num_clientes_espera;
extern float total_de_esperas;
extern float area_num_entra_cola;
extern float area_estado_servidor;
extern float tiempo_sig_evento[3];
extern int num_esperas_requerido;
extern int num_servidores;

//extern float erlangCalculator(float lambda, float media, int num_servidores);
extern float funcionPercentilExpon(float media);

void inicializar(void);

#endif  // INICIALIZAR_H
