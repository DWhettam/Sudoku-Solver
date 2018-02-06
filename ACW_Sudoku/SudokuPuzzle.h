#pragma once

#include <iostream>
#include <fstream>
#include "CellGroup.h"
using namespace std;

class SudokuPuzzle
{
public:
	SudokuPuzzle(void);
	~SudokuPuzzle(void);

	enum class Context {
		block,
		column,
		row
	};

	
	void Solve(char filenameIn[]);
		


private:
	CellGroup gridRows[9];
	CellGroup gridColumns[9];
	CellGroup gridBlocks[9];

	void Output() const;
	void InitialiseGrid(char filenameIn[]);

	int candidateListComparisonCount;
	void incrementCandidateListComparisonCount();
	bool propogateCandidateLists() const;
	void updateCandidateListsForGivenCellsRowBlockColumn(Context const &context, int contextIndex, int index, int value);
	bool findHiddenSingles(CellGroup *context, Context contextIdentifier);
	bool findHiddenSingles();
};

