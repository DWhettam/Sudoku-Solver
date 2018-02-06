#include "SudokuPuzzle.h"

// This is a basic starting point for you to use to implement your Sudoku solver
int main(int argc, char **) {
	SudokuPuzzle puzzle;	
	puzzle.Solve("sudoku_puzzle4.txt"); 

	system("pause");

	return 0;
}
