/*
 ============================================================================
 Name        : EulerOpenMP.c
 Author      : William Remigio Montufar Chata
 Version     : V1
 Copyright   : Your copyright notice
 Description : Hello OpenMP World in C
 ============================================================================
 */
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {


	int n;
	int temp,fact;
	double ep_total=0;

	printf("introduce la precision del calculo (n >= 0 y multiplo de 4) n: ");
	scanf ("%d", &n); //maximo 32

	#pragma omp parallel num_threads(4)
	{

		int id = omp_get_thread_num();
		int nt = omp_get_num_threads();
		int size = n / nt;
		int ini = id * size;
		int fin = ini + size - 1;
		int j,k=0,l;

		for (j = ini; j <= fin; j++) {
			fact=1;
			temp=1;
			k=0;
			l=j;
			for (k = 0; k <= l; k++){
				fact=fact*temp;
				temp++;

			}
			ep_total +=(1.0/fact)+(1.0/n);

		}
	}

	printf("Valor de Epsilon %.30f \n \n", ep_total);

	return 0;
}


