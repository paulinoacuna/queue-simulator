#include "actualizar_estad_prom_tiempo.h"

void actualizar_estad_prom_tiempo(void)  /* Actualiza los acumuladores de área para las estadísticas de tiempo promedio. */
{
    float time_since_last_event;

    /* Calcula el tiempo desde el último evento y actualiza el marcador del último evento */

    time_since_last_event = tiempo_simulacion - tiempo_ultimo_evento;
    tiempo_ultimo_evento = tiempo_simulacion;

    /* Actualiza el área bajo la función del número en cola */
    area_num_entra_cola += num_entra_cola * time_since_last_event;

    /* Actualiza el área bajo la función indicadora de estado del servidor */
    for (int i = 0; i < num_servidores; i++)
    {
        area_estado_servidor += estado_servidor[i] * time_since_last_event;
    }

}
