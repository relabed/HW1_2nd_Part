/*
 * init-matrix.c
 *
 *  Created on: 25 janv. 2021
 *      Author: relabed
 */

#include <stdlib.h>

#include "initMatrix.h"

void generateRandomMatrix (int **matrix, int dim, int upperValue) {
	int i, j;
	for (i = 0; i < dim; ++i) {
		for (j = 0; j < dim; ++j) {
			matrix[i][j] = rand() % (upperValue);
		}
	}

}
