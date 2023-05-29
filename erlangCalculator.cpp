
#include <stdio.h>        
#include <stdlib.h>       

//----- Function prototypes -------------------------------------------------
double E(int c, double a);      // Recursive function for Erlang-B

//===== Main program ========================================================
double erlangCalculator(double lambda,double mu,int num_servers)
{   
  double     a;             // Total offered load
  double     rho;           // Utilization per server
  double     prob_block;    // Erlang-B blocking probability
  double     prob_queue;    // Erlang-C queueing probability

  // Compute Erlang-B blocking probability using recursive function E()
  a = lambda / mu;
  prob_block = E(num_servers, a);

  // Directly compute Erlang-C queueing probability from Erlang-B probability
  rho = a / num_servers;
  if (rho < 1.0)
    prob_queue = prob_block / (1 - rho + (rho * prob_block));

  // Output input parameters, a, rho, Erlang-B, and Erlang-C
  printf("lambda      = %f \n", lambda);
  printf("mu          = %f \n", mu);
  printf("num_servervidores = %d \n", num_servers);
  if (rho < 1.0)
    printf("(Erlang-C) = %f \n", prob_queue);
  else {
    //printf("= Pr[queue] (Erlang-C) = undefined (utilization >= 1.0) \n");
    prob_queue = 1.0;
    printf("=(Erlang-C) = %f \n", 1.0);
  }
  printf("===============================================================\n");

  return prob_queue;
}

double E(int c, double a)
{
  double     e_last;        // Last E() value

  if (c == 0)
    return(1.0);
  else
    return((a * (e_last = E(c - 1, a))) / (c + a * e_last));
}