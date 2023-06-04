// controltiempo.h

#ifndef CONTROLTIEMPO_H
#define CONTROLTIEMPO_H

#include <stdio.h>  // Para fprintf.
#include <stdlib.h>  // Para exit.

extern int num_eventos;
extern float tiempo_sig_evento[];
extern int sig_tipo_evento;
extern float tiempo_simulacion;
extern FILE *resultados;

void controltiempo(void);

#endif  // CONTROLTIEMPO_H
