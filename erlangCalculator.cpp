//===================================================== file = erlang.c =====
//=  Program to solve for Erlang-B and Erlang-C probabilities               =
//===========================================================================
//=  Notes: 1) Input from command line and output to stdout                 =
//=             * Input is three values, arrival rate lambda, service       =
//=               rate mu (per server), and the number of servers           =
//=             * Ouput is Erlang-B blocking probability (blocked calls     =
//=               cleared) and Erlang-C (blocked calls queued) queueing     =
//=               probability                                               =
//=             * Note that Erlang-C queueing probability is undefined if   =
//=               rho = (lambda / (mu * num_servers)) >= 1.0                =
//=         2) Uses a recursive method to solve for Erlang-B to insure      =
//=            numerical stability                                          =
//=             * See "Teletraffic Engineering" by Ramses Mina (p 29)       =
//=         3) Solves for Erlang-C directly from Erlang-B                   =
//=             * See "The Practical Performance Analyst" by Gunther (p 65) =
//=-------------------------------------------------------------------------=
//=  Example execution: (command line is erlang 600.0 100.0 10)             =
//=                                                                         =
//=   ================================================ erlang.c =====       =
//=   = lambda      = 600.000000                                            =
//=   = mu          = 100.000000                                            =
//=   = num_servers = 10                                                    =
//=   = a           = 6.000000                                              =
//=   = rho         = 0.600000                                              =
//=   ---------------------------------------------------------------       =
//=   = Pr[block] (Erlang-B) = 0.043142                                     =
//=   = Pr[queue] (Erlang-C) = 0.101299                                     =
//=   ===============================================================       =
//=-------------------------------------------------------------------------=
//=  Build: bcc32 erlang.c, cl erlang.c, gcc erlang.c                       =
//=-------------------------------------------------------------------------=
//=  Execute: erlang lambda mu num_servers                                  =
//=-------------------------------------------------------------------------=
//=  Author: Ken Christensen                                                =
//=          University of South Florida                                    =
//=          WWW: http://www.csee.usf.edu/~christen                         =
//=          Email: christen@csee.usf.edu                                   =
//=-------------------------------------------------------------------------=
//=  History: KJC (03/06/99) - Genesis                                      =
//=           KJC (07/06/00) - Removed extraneous variable to fix warning   =
//=           KJC (02/17/05) - Fixed a %ld to %d in a printf()              =
//=           KJC (08/26/17) - Minor clean-up and extended output           =
//===========================================================================
//----- Include files -------------------------------------------------------
#include <stdio.h>          // Needed for printf()
#include <stdlib.h>         // Needed for exit(), atoi(), and atof()

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
  printf("================================================ erlang.c =====\n");
  printf("= lambda      = %f \n", lambda);
  printf("= mu          = %f \n", mu);
  printf("= num_servers = %d \n", num_servers);
  printf("= a           = %f \n", a);
  printf("= rho         = %f \n", rho);
  printf("---------------------------------------------------------------\n");
  printf("= Pr[block] (Erlang-B) = %f \n", prob_block);
  if (rho < 1.0)
    printf("= Pr[queue] (Erlang-C) = %f \n", prob_queue);
  else {
    //printf("= Pr[queue] (Erlang-C) = undefined (utilization >= 1.0) \n");
    prob_queue = 1.0;
    printf("= Pr[queue] (Erlang-C) = %f \n", 1.0);
  }
  printf("===============================================================\n");

  return prob_queue;
}

//===========================================================================
//=  Recursive function to solve for Erlang-B blocking probability          =
//=   - Input is c = number of servers and a = total offered load           =
//=   - Output returned is Erlang-B blocking probability                    =
//===========================================================================
double E(int c, double a)
{
  double     e_last;        // Last E() value

  if (c == 0)
    return(1.0);
  else
    return((a * (e_last = E(c - 1, a))) / (c + a * e_last));
}