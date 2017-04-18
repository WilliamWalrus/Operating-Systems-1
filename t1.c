#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

unsigned int nthreads = 30;
#define SIZE 9

int load_grid(int grid[][SIZE], char *filename);
int grid[9][9];
int nextRow = 0;
int nextCol = 0;
int nextReg = 0;
int erros = 0;

pthread_mutex_t mutexRow, mutexCol, mutexReg, mutexErr;

void checkCol(int col) {
	//pthread_t tid = pthread_self();
	//printf("coluna %d e thread %u\n", col + 1, (unsigned int)tid);
	
	for (int i = 0; i < SIZE; i++) {
		for (int j = i + 1; j < SIZE; j++) {
			if (grid[i][col] == grid[j][col]) {
				printf("Erro na coluna %d!\n", col + 1);
				
				pthread_mutex_lock(&mutexErr);
				erros++;
				pthread_mutex_unlock(&mutexErr);
				return;
			}
		}
	}
}

void checkRow(int row) {
	//pthread_t tid = pthread_self();
	//printf("linha %d e thread %u\n", row + 1, (unsigned int)tid);
	
	for (int i = 0; i < SIZE; i++) {
		for (int j = i + 1; j < SIZE; j++) {
			if (grid[row][i] == grid[row][j]) {
				printf("Erro na linha %d!\n", row + 1);
				
				pthread_mutex_lock(&mutexErr);
				erros++;
				pthread_mutex_unlock(&mutexErr);
				return;
			}
		}
	}
}

void checkReg(int reg) {
	//pthread_t tid = pthread_self();
	//printf("região %d e thread %u\n", reg + 1, (unsigned int)tid);
	
	int rowBase = (reg / 3) * 3;
	int rowLimit = ((reg + 3)/ 3) * 3;
	int colBase = (reg % 3) * 3;
	int colLimit = ((reg % 3) + 1) * 3;
	int regVector[SIZE]; 
	int k = -1;
	
	for (int i = rowBase; i < rowLimit; i++) {
		for (int j = colBase; j < colLimit; j++) {
			regVector[++k] = grid[i][j];
		}
	}
	
	for (int i = 0; i < SIZE; i++) {
		for (int j = i + 1; j < SIZE; j++) {
			if (regVector[i] == regVector[j]) {
				printf("Erro na região %d!\n", reg + 1);
				
				pthread_mutex_lock(&mutexErr);
				erros++;
				pthread_mutex_unlock(&mutexErr);
				return;
			}
		}
	}
}

void *check(void* arg) {

	while (true) {
		pthread_mutex_lock(&mutexCol);
		if (nextCol > 8) {
			pthread_mutex_unlock(&mutexCol);
		} else {
			int col = nextCol;
			nextCol++;
			pthread_mutex_unlock(&mutexCol);
			checkCol(col);
		}
		
		pthread_mutex_lock(&mutexRow);
		if (nextRow > 8) {
			pthread_mutex_unlock(&mutexRow);
		} else {
			int row = nextRow;
			nextRow++;
			pthread_mutex_unlock(&mutexRow);
			checkRow(row);
		}

		pthread_mutex_lock(&mutexReg);
		if (nextReg > 8) {
			pthread_mutex_unlock(&mutexReg);
		} else {
			int reg = nextReg;
			nextReg++;
			pthread_mutex_unlock(&mutexReg);
			checkReg(reg);
		}
		
		pthread_mutex_lock(&mutexCol);
		pthread_mutex_lock(&mutexRow);
		pthread_mutex_lock(&mutexReg);
		if (nextCol > 8 && nextRow > 8 && nextReg > 8) break;
		pthread_mutex_unlock(&mutexCol);
		pthread_mutex_unlock(&mutexRow);
		pthread_mutex_unlock(&mutexReg);
	}
	pthread_mutex_unlock(&mutexCol);
	pthread_mutex_unlock(&mutexRow);
	pthread_mutex_unlock(&mutexReg);
	return NULL;
}

int main() {
	if(nthreads > 27) nthreads = 27;
	
	load_grid(grid, "input_grid_errado.txt");
	
	pthread_mutex_init(&mutexRow, NULL);
	pthread_mutex_init(&mutexCol, NULL);
	pthread_mutex_init(&mutexReg, NULL);
	pthread_mutex_init(&mutexErr, NULL);
	
	pthread_t thread[nthreads];
	for (int i = 0; i < nthreads; i++) {
		pthread_create(&thread[i], NULL, check, NULL);
	}
	
	for (int i = 0; i < nthreads; i++) {
		pthread_t t = thread[i];
		pthread_join(t, NULL);
	}
	
	printf("%d erros.\n", erros);
	
	pthread_mutex_destroy(&mutexRow);
	pthread_mutex_destroy(&mutexCol);
	pthread_mutex_destroy(&mutexReg);
	pthread_mutex_destroy(&mutexErr);
	
	pthread_exit(NULL);
}




/* Funcao que le um grid do arquivo "filename" e o armazena em uma matriz */
int load_grid(int grid[][SIZE], char *filename) {
	FILE *input_file = fopen(filename, "r");

	if (input_file != NULL) {
		for(int i = 0; i < SIZE; i++)
			for(int j = 0; j < SIZE; j++)
				fscanf(input_file, "%d", &grid[i][j]);
		fclose(input_file);
		return 1;
	}
	return 0;
}
