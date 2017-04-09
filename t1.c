#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

unsigned int nthreads = 5;
#define SIZE 9

int load_grid(int grid[][SIZE], char *filename);
int grid[9][9];

void checkCellLine(unsigned cell) {
	
	uint8_t row = (cell - 1) / (SIZE);
	uint8_t col = (cell - 1) % SIZE;
	uint8_t lineLimit = SIZE - col - 1;
	uint8_t n = grid[row][col];
	
	printf("N: %d - Limite: %d\n", grid[row][col], lineLimit);
	
	//for(uint8_t i = cell; i <= limiteLine; i++) printf("%d", grid[][]); 
}

void checkCellColumn(unsigned cell){}

void checkCell(unsigned cell) {
		
}
void *checkAll(void *arg) {
	
}

int main() {
	
	load_grid(grid, "input_grid_errado.txt");
	for(int i = 1; i < 82; i++) checkCellLine(i);
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
