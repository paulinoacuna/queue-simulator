// actualizar_estad_prom_tiempo.h

#ifndef ACTUALIZAR_ESTAD_PROM_TIEMPO_H
#define ACTUALIZAR_ESTAD_PROM_TIEMPO_H

extern float tiempo_simulacion;
extern float tiempo_ultimo_evento;
extern int num_entra_cola;
extern float area_num_entra_cola;
extern float area_estado_servidor;
extern int estado_servidor[];
extern int num_servidores;

void actualizar_estad_prom_tiempo(void);

#endif // ACTUALIZAR_ESTAD_PROM_TIEMPO_H
