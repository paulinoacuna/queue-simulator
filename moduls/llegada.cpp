#include "llegada.h"
#include <stdio.h>   // Para fprintf
#include <stdlib.h>  // Para exit


void llegada(void)  /* Funcion de llegada */
{
    
    float espera;

    /* Programa la siguiente llegada */
    
    tiempo_sig_evento[1] = tiempo_simulacion + funcionPercentilExpon(media_entre_llegadas);

    //printf("num exp%15.3f",funcionPercentilExpon(media_entre_llegadas));
    /* Revisa si todos los servidores están OCUPADOS */
    int i;
    for (i = 0; i < 10; i++) {
        if (estado_servidor[i] == LIBRE) {
            break;
        }
    }

    if (i == 10) {
        /* Todos los servidores están OCUPADOS, aumenta el número de clientes en cola */

        ++num_entra_cola;

        /* Verifica si hay condición de desbordamiento */
        if (num_entra_cola > LIMITE_COLA)
        {
            /* Se ha desbordado la cola, detiene la simulación */
            fprintf(resultados, "\nDesbordamiento del arreglo tiempo_llegada a la hora");
            fprintf(resultados, "%f", tiempo_simulacion);
            exit(2);
        }

        /* Todavía hay espacio en la cola, se almacena el tiempo de llegada del cliente en el ( nuevo ) fin de tiempo_llegada */
        tiempo_llegada[num_entra_cola] = tiempo_simulacion;
    }
    else {
        /* Hay al menos un servidor LIBRE */

        /* Elige el primer servidor LIBRE */
        servidor_actual = i;

        /* El servidor está ahora OCUPADO, por lo que el cliente que llega no espera */
        espera = 0.0;
        total_de_esperas += espera;

        /* Incrementa el número de clientes en espera y pasa el servidor a OCUPADO */
        ++num_clientes_espera;
        estado_servidor[servidor_actual] = OCUPADO;

        /* Programa una salida (servicio terminado) */
        tiempo_sig_evento[2] = tiempo_simulacion + funcionPercentilExpon(media_atencion);
    }
}