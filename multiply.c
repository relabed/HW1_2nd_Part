/*
 * multiply.c
 *
 *  Created on: 18 février 2021
 *      Author: relabed
 */
#include "multiply.h"
#include<pthread.h>

struct thread_args{
	    int start;
	    int end;
	    int **matrixA;
	    int **matrixB;
	    int **matrixD;
	    int l;
	};

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *multiply(void * arg) {
	struct thread_args * range = (struct thread_args *) arg;
				for(int i = range->start; i < range->end; i++) {  // two-dimensional decomposition
					 for(int j = 0; j < range->l; j++) { // two-dimensional decomposition
					    long thread_private_tmp = 0;
					    	for(int k = 0; k < range->l; k++) { // two-dimensional decomposition
				          thread_private_tmp += range->matrixA[i][k] * range->matrixB[k][j];
				    					   }
				                        pthread_mutex_lock(&lock);
				                        range->matrixD[i][j] += thread_private_tmp;
				    					 pthread_mutex_unlock(&lock);
				    					    	    	    }
				    					    	    	}
			return 0;
				    					    	 }
