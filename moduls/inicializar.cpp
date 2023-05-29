// inicializar.cpp

#include "../erlangCalculator.cpp"
#include "inicializar.h"

void inicializar(void)  /* Funcion de inicializacion. */
{

    //Adecuacion de C erlang
    lambda = media_entre_llegadas;  // Tasa promedio de llegada = 1 / media_entre_llegadas
    
    if(num_esperas_requerido >= 0 && num_esperas_requerido <= num_servidores){
        media_atencion = num_esperas_requerido*media_atencion;
    }else{
         media_atencion = num_servidores*media_atencion;
    }

    C = erlangCalculator(lambda, media_atencion,num_servidores);

    /* Inicializa el reloj de la simulacion. */

    tiempo_simulacion = 0.0;

    /* Inicializa las variables de estado */

    num_entra_cola = 0;
    tiempo_ultimo_evento = 0.0;
    servidor_actual = 0;

    /* Inicializa los contadores estadisticos. */

    num_clientes_espera = 0;
    total_de_esperas = 0.0;
    area_num_entra_cola = 0.0;
    area_estado_servidor = 0.0;

    /* Inicializa la lista de eventos. */

    tiempo_sig_evento[1] = tiempo_simulacion + funcionPercentilExpon(media_entre_llegadas);
    tiempo_sig_evento[2] = 1.0e+30;
}

