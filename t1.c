#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

unsigned int nthreads = 5;
#define SIZE 9

int load_grid(int grid[][SIZE], char *filename);
int grid[9][9];
int nextRow;
int nextCol;
int nextReg;

void checkRow(int row) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = i + 1; j < SIZE; j++) {
			if (grid[row][i] == grid[row][j]) printf("Erro na linha %d!\n", row + 1);
		}
	}
}

void checkCol(int col) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = i + 1; j < SIZE; j++) {
			if (grid[i][col] == grid[j][col]) printf("Erro na coluna %d!\n", col + 1);
		}
	}
}

void checkReg(int reg) {
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
			if (regVector[i] == regVector[j]) printf("Erro na região %d!\n", reg + 1);
		}
	}
}

int main() {
	load_grid(grid, "input_grid_errado.txt");
	for(int i = 0; i < SIZE; i++) checkRow(i);
	for(int i = 0; i < SIZE; i++) checkCol(i);
	for(int i = 0; i < SIZE; i++) checkReg(i);
	return 0;
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
