#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath> // Incluir la biblioteca cmath para la función tgamma
#include "lcgrand.cpp" /* Encabezado para el generador de números aleatorios */


//#include "erlangCalculator.cpp"  /* Calculadora erlang */
#include "./medidas/prom_clientes_cola.cpp"
#include "./medidas/prom_clientes_sistema.cpp"
#include "./medidas/prob_sistema_vacio.cpp"
#include "./medidas/tiempo_cliente_sistema.cpp"
#include "./medidas/tiempo_cliente_cola.cpp"

#include "./moduls/inicializar.h"
#include "./moduls/controltiempo.h"
#include "./moduls/llegada.h"
#include "./moduls/salida.h"
#include "./moduls/reportes.h"
#include "./moduls/actualizar_estad_prom_tiempo.h"

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


int main(int argc, char* argv[])  /* Funcion Principal */
{
    /* Abre los archivos de entrada y salida */

    if (argc != 3)
    {
        printf("Uso: %s <archivo_parametros> <archivo_resultados>\n", argv[0]);
        exit(1);
    }
    parametros = fopen(argv[1], "r");
    resultados = fopen(argv[2], "w");
    
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


float funcionPercentilExpon(float media)  /* Función generadora percentil exponencial */
{
    /* Retorna una variable aleatoria exponencial con media "media" */
    double aux = lcgrand(1);
    //printf("Numero aleatorio generado: %f \n", aux);
    return (-media * log(aux)/100);
}