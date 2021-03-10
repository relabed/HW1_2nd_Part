/*
 * hw1-multiply-random-square-matrix (sequential & parallel using pthread)
 *
 *  Created on: 25 janv. 2021
 *      Author: relabed
 */
#include<stdio.h>
#include<math.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include <stdint.h>
#include <sys/time.h>

#include "initMatrix.h"
#include "multiply.h"

struct thread_args{
	    int start;
	    int end;
	    int **matrixA;
	    int **matrixB;
	    int **matrixD;
	    int l;
	};

int main()
{

	char mode;
	printf("Entrez le mode d'exécution du programme (p pour parallèle et s pour séquentiel):\n");
			    scanf("%c",&mode); /* on récupère le mode d'exécution que l'utilisateur tape */

	if (mode == 's') { // si s exécution séquentiel seulement

		printf("Sequential execution\n");

	int n;  	// MATRIX DIMENSION
	int i;

		printf("Entrez la dimension de la matrice (un nombre entier):\n");
		    scanf("%d",&n); /* on récupère la dimension de la matrice que l'utilisateur tape */


		int **A = (int **)malloc(n * sizeof(int *)); /* on alloue de l'espace pour une matrice */
		for (i = 0; i < n; i++)
			A[i] = (int *)malloc(n * sizeof(int));

		generateRandomMatrix(A, n, 10); /* on affecte des valeurs aléatoires à la matrice A*/


		int **B = (int **)malloc(n * sizeof(int *)); /* on alloue de l'espace pour une matrice */
		for (i = 0; i < n; i++)
			B[i] = (int *)malloc(n * sizeof(int));

		generateRandomMatrix(B, n, 10); /* on affecte des valeurs aléatoires à la matrice B */


	float temps;
	clock_t t1, t2;

		int j,k;
		int **C = (int **)malloc(n * sizeof(int *)); /* on alloue de l'espace pour une matrice */
			for (i = 0; i < n; i++)
				C[i] = (int *)malloc(n * sizeof(int));

		t1 = clock(); /* ici on commence à chronometrer le temps */
		for(i = 0; i < n; i++)
		{
			for(j = 0; j < n; j++)
			{
				C[i][j]=0;
				for(k = 0; k < n; k++)
				{
					C[i][j] +=  A[i][k] * B[k][j]; /* ici ce fait la multiplication et on enregistre les résultats dans la matrice C*/
				}
			}
		}


		t2 = clock(); /* ici on arrête de chronometrer le temps */
		temps = (float)(t2-t1)/CLOCKS_PER_SEC;

		    printf("temps d'execution en secondes = %f\n", temps);

	}

	else if (mode == 'p')  	{  // si p mode séquentiel et parallèle avec comparaison du temps d'exécution et du résultat
		float temps_p,temps_s;
			clock_t t1, t2, t3,t4;


		printf("Sequential & Parallal execution\n");

		int DIM;
			printf("Entrez la dimension de la matrice (un nombre entier):\n");
		    scanf("%d",&DIM); // on récupère la dimension de la matrice que l'utilisateur choisi

		int NUM_THREADS;
			printf("Entrez le nombre de threads:\n");
		    scanf("%d",&NUM_THREADS); // on récupère le nombre de thread que l'utilisateur veut utiliser

		 int i;

		 int **A = (int **)malloc(DIM * sizeof(int *)); // on alloue de l'espace pour une matrice
		 for (i = 0; i < DIM; i++)
		 A[i] = (int *)malloc(DIM * sizeof(int));

		 generateRandomMatrix(A, DIM, 10); // on affecte des valeurs aléatoires à la matrice A

		 int **B = (int **)malloc(DIM * sizeof(int *)); // on alloue de l'espace pour une matrice
		 for (i = 0; i < DIM; i++)
		 B[i] = (int *)malloc(DIM * sizeof(int));

		 generateRandomMatrix(B, DIM, 10); // on affecte des valeurs aléatoires à la matrice B

		 int **C = (int **)malloc(DIM * sizeof(int *)); // on alloue de l'espace pour la matrice de la multiplication séquentielle
		 for (i = 0; i < DIM; i++)
		 C[i] = (int *)malloc(DIM * sizeof(int));

		 int **D = (int **)malloc(DIM * sizeof(int *)); // on alloue de l'espace pour la matrice de la multiplication parallèle
		 for (i = 0; i < DIM; i++)
		 D[i] = (int *)malloc(DIM * sizeof(int));

		 int j,k;

		 t3 = clock(); /* ici on commence à chronometrer le temps du séquentiel */
		 for(i = 0; i < DIM; i++)
		    		{
		    			for(j = 0; j < DIM; j++)
		    			{
		    				C[i][j]=0;
		    				for(k = 0; k < DIM; k++)
		    				{
		    					C[i][j] +=  A[i][k] * B[k][j]; /* ici ce fait la multiplication et on enregistre les résultats dans la matrice C*/
		    				}
		    			}
		    		}
	/*	 printf("La Matrice C (Résultat Multiplication séquentiel) : \n");
		 		 for (i = 0; i < DIM; ++i) {
		 		 		 		for (j = 0; j < DIM; ++j) {
		 		 		 			printf("%d\t", C[i][j]);
		 		 		 		}
		 		 		 		printf("\n");
		 		 		 	}*/

		    		t4 = clock(); /* ici on arrête de chronometrer le temps du séquentiel */
		    		temps_s = (float)(t4-t3)/CLOCKS_PER_SEC;

		    		    printf("temps d'execution en mode séquentiel en secondes = %f\n", temps_s);

			    pthread_t child_threads[NUM_THREADS];
			    struct thread_args work_ranges[NUM_THREADS];
			    int current_start, range;
			    current_start = 0;
			    range = (DIM / DIM / NUM_THREADS);  // two-dimensional decomposition
			    for(int i = 0; i < NUM_THREADS; i++) { // two-dimensional decomposition
			    		work_ranges[i].start = current_start;
			    		work_ranges[i].end = current_start + range;
			    	    current_start += range;
			    }
			    work_ranges[NUM_THREADS-1].end = DIM; // two-dimensional decomposition
			    t1 = clock(); // ici on commence à chronometrer le temps du parallèle
			    for(int i = 0; i < NUM_THREADS; i++) { // two-dimensional decomposition
			    	work_ranges[i].matrixA = A;
			    	work_ranges[i].matrixB = B;
			    	work_ranges[i].matrixD = D;
			    	work_ranges[i].l = DIM;
			    	pthread_create(&child_threads[i], NULL, multiply, &work_ranges[i]);
			    }
			    t2 = clock(); // ici on arrête de chronometrer le temps du parallèle
			    for(int i = 0; i < NUM_THREADS; i++) { // two-dimensional decomposition
			    		pthread_join(child_threads[i], NULL);
			    }

temps_p = (float)(t2-t1)/CLOCKS_PER_SEC;
/*printf("La Matrice D (Résultat Multiplication parallèle) : \n");
		 		 for (i = 0; i < DIM; ++i) {
		 		 		 		for (j = 0; j < DIM; ++j) {
		 		 		 			printf("%d\t", D[i][j]);
		 		 		 		}
		 		 		 		printf("\n");
		 		 		 	}*/
printf("temps d'execution en mode parallèle en secondes = %f\n", temps_p);

printf("Accélération de Calcul = %f\n", temps_s/temps_p);

int **E = (int **)malloc(DIM * sizeof(int *)); // on alloue de l'espace pour la matrice de vérification des résultats parallèls et sequentiels
for (i = 0; i < DIM; i++)
E[i] = (int *)malloc(DIM * sizeof(int));

for (i = 0; i < DIM; ++i) {
		for (j = 0; j < DIM; ++j) {
			E[i][j]=D[i][j]-C[i][j]; // la matrice E contient la soustraction entre le résultat de la multiplication parallèle et le résultat de la multiplication séquentiel
		}
	}

if (E[DIM-1][DIM-1] == 0){ // si la valeur est égal à zéro alors les deux matrices (multiplication parallèle et multiplication séquentiel sont identiques)
	printf("les Matrices résultantes de la multiplication en mode séquentiel et parallèle sont identiques");
}
else {
	printf("il y a eu une erreur de calcul dans le mode parallèle");
}
	}
	else {
		printf("erreur de saisie, veuillez relancer le programme");
	}
return 0;
}
