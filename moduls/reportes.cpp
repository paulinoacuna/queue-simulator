#include "reportes.h"

void reportes(void)  /* Funcion generadora de reportes. */
{

    C = 0.0;

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