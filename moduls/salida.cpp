#include "salida.h"

void salida(void)  /* Funcion de Salida. */
{
    int i;
    float espera;

    /* Revisa si la cola está vacía */
    if (num_entra_cola == 0) {
        /* La cola está vacía, pasa el servidor actual a LIBRE y no considera el evento de salida */
        estado_servidor[servidor_actual] = LIBRE;
        tiempo_sig_evento[2] =  1.0e+30;
    } else {
        /* La cola no está vacía, disminuye el número de clientes en cola. */
        --num_entra_cola;

        /* Calcula la espera del cliente que está siendo atendido y actualiza el acumulador de espera */
        espera = tiempo_simulacion - tiempo_llegada[1];
        total_de_esperas += espera;

        /* Incrementa el número de clientes en espera y programa la salida. */
        ++num_clientes_espera;
        tiempo_sig_evento[2] = tiempo_simulacion + funcionPercentilExpon(media_atencion);

        /* Mueve cada cliente en la cola (si los hay) una posición hacia adelante */
        for (i = 1; i <= num_entra_cola; ++i) {
            tiempo_llegada[i] = tiempo_llegada[i + 1];
        }
    }



}