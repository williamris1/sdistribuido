/*
 ============================================================================
 Name        : EulerMPI.c
 Author      : William Remigio Montufar Chata Version     : 1.0 MPI
 Copyright   : Your copyright notice
 Description : Compute Pi in MPI C++
 Compile Command:
	$ mpiexec -n 5 ./EulerMPI
 ============================================================================
 */
#include <iostream>
#include <iomanip>
#include <cmath>
#include "mpi.h"   // Biblioteca de MPI
#include <cstdlib> // Incluido para el uso de atoi

using namespace std;
int main(int argc, char *argv[])
{

	int n, // Numero de iteraciones
		rank, // Identificador de proceso
		size; // Numero de procesos
   const double EPSILON = 2.718281828459045235360287471352;
   unsigned long long fact = 1;
   double ep_total,h,sum;
   double local_ep; // Valor local de epsilon

   	   MPI_Init(&argc, &argv); // Inicializamos los procesos
       MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtenemos el numero total de procesos
       MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtenemos el valor de nuestro identificador

   // Solo el proceso 0 va a conocer el numero de iteraciones que vamos a
   // ejecutar para la aproximacion de PI
   	if (rank == 0) {
   		cout<<"introduce la precision del calculo (n >= 0): ";
   		cin>>n;
   	}

	// El proceso 0 reparte al resto de procesos el numero de iteraciones
	// que calcularemos para la aproximacion de EP
	MPI_Bcast(&n, // Puntero al dato que vamos a enviar
			  1,  // Numero de datos a los que apunta el puntero
			  MPI_INT, // Tipo del dato a enviar
			  0, // Identificacion del proceso que envia el dato
			  MPI_COMM_WORLD);

	if (n <= 0){
		MPI_Finalize();
		exit(0);
	}


	else {
		h = 1.0 / (double) n;
		sum = 0;
		local_ep=0;

		for (int i = rank + 1; i <= n; i += size) {

			int j=i;
			int temp=1;
			fact=1;
			  do {
				   fact=fact*temp;
				   temp++;
			   }
			   while (j >= temp);
			 sum +=(1.0/fact)+h;
		}

	   local_ep=sum;

	   //double mypi, // Valor local de PI
	   // Todos los procesos ahora comparten su valor local de e,
	   // lo hacen reduciendo su valor local a un proceso
	   // seleccionada a traves de una operacion aritmetica.
	   		MPI_Reduce(&local_ep, // Valor local de PI
	   					&ep_total,  // Resultado total de Epsilon
	   					1,	  // Numero de datos que vamos a reducir
	   					MPI_DOUBLE,  // Tipo de dato que vamos a reducir
	   					MPI_SUM,  // Operacion que aplicaremos
	   					0, // proceso que va a recibir el dato reducido
	   					MPI_COMM_WORLD);



		// Solo el proceso 0 imprime el mensaje, ya que es la unica que
		// conoce el valor de e aproximado.
		if (rank == 0)
		{
			printf("Valor de Epsilon con 30 Digitos: %.30f \n", EPSILON);
			printf("Valor de Epsilon segun la operaciòn del algoritmo %.30f \n", ep_total);
			printf("Margen error de %.30f \n", fabs(ep_total - EPSILON));
		}

   }

   MPI_Finalize();
   return 0;
}

