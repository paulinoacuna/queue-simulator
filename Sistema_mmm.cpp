#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath> // Incluir la biblioteca cmath para la función tgamma
#include "lcgrand.cpp" /* Encabezado para el generador de números aleatorios */


#include "erlangCalculator.cpp"  /* Calculadora erlang */
#include "./medidas/prom_clientes_cola.cpp"
#include "./medidas/prom_clientes_sistema.cpp"
#include "./medidas/prob_sistema_vacio.cpp"
#include "./medidas/tiempo_cliente_sistema.cpp"
#include "./medidas/tiempo_cliente_cola.cpp"

#define LIMITE_COLA 10000  /* Capacidad máxima de la cola */
#define OCUPADO      1    /* Indicador de servidor ocupado */
#define LIBRE        0    /* Indicador de servidor libre */

int sig_tipo_evento, num_entra_cola, num_clientes_espera, servidor_actual ,num_esperas_requerido, num_eventos, num_servidores, estado_servidor[10];
float area_num_entra_cola, area_estado_servidor, media_entre_llegadas, media_atencion,
      tiempo_simulacion, tiempo_llegada[LIMITE_COLA + 1], tiempo_ultimo_evento, tiempo_sig_evento[3],
      total_de_esperas;
FILE *parametros, *resultados;

//Implementacion C Erlang

float lambda;  // Tasa promedio de llegada de clientes al sistema
float miu;  // Tasa promedio de servicio por servidor
float C = 0.0;  // Fracción de tiempo en la que todos los servidores están ocupados


void inicializar(void); //alg 1
void controltiempo(void); //alg 2

void llegada(void); 
void salida(void); //alg 3 | func gestora de eventos tipo 1 y 2

                    //alg 4 funcion percentil que usa el teorema fundamental de la simulación

void reportes(void); 
void actualizar_estad_prom_tiempo(void); //alg 5 reportes finales

float funcionPercentilExpon(float mean);


int main(void)  /* Funcion Principal */
{
    /* Abre los archivos de entrada y salida */

    parametros = fopen("param.txt", "r");
    resultados = fopen("result.txt", "w");

    /* Especifica el numero de eventos para la funcion controltiempo. */

    num_eventos = 2;

    /* Lee los parametros de entrada. */

    fscanf(parametros, "%f %f %d %d", &media_entre_llegadas, &media_atencion, &num_esperas_requerido, &num_servidores);

    /* Escribe en el archivo de salida los encabezados del reporte y los parametros iniciales */

    fprintf(resultados, "Simulador de Sistema de Colas M/M/m\n\n");
    fprintf(resultados, "Tiempo promedio de llegada (lambda): %11.3f minutos\n\n", media_entre_llegadas);
    fprintf(resultados, "Tiempo promedio de atención (mu): %16.3f minutos\n\n", media_atencion);
    fprintf(resultados, "Número de clientes requeridos (n): %14d\n\n", num_esperas_requerido);
    fprintf(resultados, "Número de servidores (m): %14d\n\n", num_servidores);

    /* Inicializa la simulacion. */

    inicializar();

    /* Corre la simulacion mientras aún existan clientes requeridos */

    while (num_clientes_espera < num_esperas_requerido)
    {
        /* Determina el siguiente evento */

        controltiempo();

        /* Actualiza los acumuladores estadisticos de tiempo promedio */

        actualizar_estad_prom_tiempo();

        /* Invoca la funcion del evento adecuado. */

        switch (sig_tipo_evento)
        {
        case 1:
            llegada();
            break;
        case 2:
            salida();
            break;
        }
    }

    /* Invoca el generador de reportes y termina la simulacion. */

    reportes();

    fclose(parametros);
    fclose(resultados);

    return 0;
}


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



void controltiempo(void)  /* Funcion controltiempo */
{
    int i;
    float min_tiempo_sig_evento = 1.0e+29;

    sig_tipo_evento = 0;

    /* Determina el tipo de evento del evento que debe ocurrir. */

    for (i = 1; i <= num_eventos; ++i)
    {
        if (tiempo_sig_evento[i] < min_tiempo_sig_evento)
        {
            min_tiempo_sig_evento = tiempo_sig_evento[i];
            sig_tipo_evento = i;
        }
    }

    /* Revisa si la lista de eventos esta vacia. */

    if (sig_tipo_evento == 0)
    {
        /* La lista de eventos esta vacia, se detiene la simulacion. */

        fprintf(resultados, "\nLa lista de eventos esta vacia %f", tiempo_simulacion);
        exit(1);
    }

    /* La lista de eventos no esta vacia, adelanta el reloj de la simulacion. */

    tiempo_simulacion = min_tiempo_sig_evento;
}


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


void reportes(void)  /* Funcion generadora de reportes. */
{

    fprintf(resultados, "=============================================================\n\n");
    

double P0 = prob_sistema_vacio(lambda,media_atencion,num_servidores);
double Ls = prom_clientes_sistema(lambda,media_atencion,num_servidores,P0);
double Lq = prom_clientes_cola (lambda,media_atencion, Ls);
double Ws = tiempo_cliente_sistema(lambda,media_atencion,num_servidores,P0);
double Wq = tiempo_cliente_cola(Ws,media_atencion);

    /* Calcula y estima los estimados de las medidas deseadas de desempeño */

    //Erlang C
    fprintf(resultados, "C Erlang | Probabilidad de que todos los servidores estén ocupados%8.3f\n\n", C);
    
    fprintf(resultados, "P0       | Probabilidad de 0 clientes en el sistema: %11.3f \n\n",P0);

    fprintf(resultados, "Ls       | Numero promedio de clientes en el sistema%11.3f clientes\n\n",Ls);

    fprintf(resultados, "Lq       | Numero promedio de clientes en la cola%11.3f clientes\n\n",Lq);

    fprintf(resultados, "Ws       | Tiempo promedio de clientes en el sistema%11.3f minutos\n\n",Ws);

    fprintf(resultados, "Wq       | Tiempo promedio de clientes en la cola%11.3f minutos\n\n",Wq);

if((lambda/media_atencion)*10 >= 100){
    fprintf(resultados, "Utilización por servidor: %15.3f%%\n",100.0);

}else{
    fprintf(resultados, "Utilización por servidor: %15.3f%%\n",(lambda/media_atencion)*10);

}

    
    fprintf(resultados, "Utilización del Sistema: %15.3f%%\n\n",(1-P0)*100);

    fprintf(resultados, "Tiempo de terminacion de la simulacion%12.3f minutos\n\n", tiempo_simulacion);

}


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


float funcionPercentilExpon(float media)  /* Función generadora percentil exponencial */
{
    /* Retorna una variable aleatoria exponencial con media "media" */
    double aux = lcgrand(1);
    //printf("Numero aleatorio generado: %f \n", aux);
    return (-media * log(aux)/100);
}