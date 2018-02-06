#include "SudokuPuzzle.h"
#include <Windows.h>
#include <unordered_set>

//Default constructor + destructor
SudokuPuzzle::SudokuPuzzle(void) : candidateListComparisonCount(0) {}
SudokuPuzzle::~SudokuPuzzle(void){}

#ifdef _DEBUG
#define INCREMENT_CANDIDATELIST_COMPARISON_COUNT incrementCandidateListComparisonCount()

void SudokuPuzzle::incrementCandidateListComparisonCount()
{
	++candidateListComparisonCount;
}

#else
#define INCREMENT_CANDIDATELIST_COMPARISON_COUNT
#endif // _DEBUG


//Solve method
void SudokuPuzzle::Solve(char filenameIn[])
{
	InitialiseGrid(filenameIn);

	LARGE_INTEGER start, end, frequency;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);

	int iterations = 0;

	propogateCandidateLists();
	
	do
	{		
		++iterations;
	} while (findHiddenSingles());
		

	QueryPerformanceCounter(&end);
	float time = (end.QuadPart - start.QuadPart) / (static_cast<float> (frequency.QuadPart));
	
	cout << "iterations " << iterations*3 << endl;
	cout << "Solve time: " << time << endl;

	#ifdef _DEBUG
		cout << "Candidate list member inspections: " << candidateListComparisonCount << endl;
	#endif // _DEBUG
	
	Output();  // Output the solved puzzle
}

//Finds naked singles
bool SudokuPuzzle::propogateCandidateLists() const
{	
	bool updated = false;

	//for each row
	for (int row = 0; row < 9; row++)
	{
		//for each column
		for (int column = 0; column < 9; column++)
		{
			//initialising preseventValue and candidateList sets
			std::unordered_set<int> presentValues;
			std::unordered_set<int> candidateList;

			//creating new cell for given column and row
			Cell* cell = gridRows[row].GetCell(column);

			//getting a cells block from row and column
			int block = ((row / 3) * 3) + column / 3;

			//creating candidateList for cells that have not been populated
			if (cell->GetValue() == 0)
			{
				for (int i = 0; i < 9; i++)
				{
					//Adding values to list of present values
					presentValues.insert((gridBlocks[block].GetCell(i))->GetValue());
					presentValues.insert((gridColumns[column].GetCell(i))->GetValue());
					presentValues.insert((gridRows[row].GetCell(i))->GetValue());
				}
				for (int i = 0; i <= 9; i++)
				{
					//adding all values not in present values to candidate list
					if (presentValues.find(i) == presentValues.end())
					{
						candidateList.insert(i);
					}
				}	
				//setting candidate list of specified cell
				cell->SetCandidateList(candidateList);
			}
		}
	}

	if (updated)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Finds hidden singles
bool SudokuPuzzle::findHiddenSingles(CellGroup *context, Context contextIdentifier)
{
	bool updated = false;

	ofstream file;
	file.open("log.txt", std::ios_base::app);

	std::unordered_set<int> candidateList;

	for (int contextIndex = 0; contextIndex < 9; contextIndex++)
	{
		for (int index = 0; index < 9; index++)
		{
			Cell* cell = context[contextIndex].GetCell(index);

			int occurances[10] = { 0 };

			#ifdef _DEBUG	
				int row = 0;
				int	column = 0;
				int block = 0;

				if (contextIdentifier == Context::row)
				{
					row = contextIndex;
					column = index;
				}
				else if (contextIdentifier == Context::column)
				{
					row = index;
					column = contextIndex;
				}
				else if (contextIdentifier == Context::block)
				{
					block = contextIndex;

					int rowOffset = (block / 3) * 3;
					int colOffset = (block % 3) * 3;
					int localRow = index / 3;
					int localCol = index % 3;

					row = rowOffset + localRow;
					column = colOffset + localCol;
				}
				cout << "row: " << row << " column: " << column << cell->GetValue() << endl;
				file << "row: " << row << " column: " << column << cell->GetValue() << endl;
			#endif // _DEBUG

			if (cell->GetValue() == 0)
			{				
				for (int i = 0; i < 9; i++)
				{
					Cell *workingCell = context[contextIndex].GetCell(i);

					candidateList = workingCell->GetCandidateList();

					INCREMENT_CANDIDATELIST_COMPARISON_COUNT;

					if (candidateList.size() == 1)
					{
						INCREMENT_CANDIDATELIST_COMPARISON_COUNT;
						workingCell->SetValue(static_cast<int>(*candidateList.begin()));
						updateCandidateListsForGivenCellsRowBlockColumn(contextIdentifier, contextIndex, i, workingCell->GetValue());
					}

					else
					{
						for (auto f : candidateList)
						{
							INCREMENT_CANDIDATELIST_COMPARISON_COUNT;
							++occurances[f];
						}
					}					
				}

				#ifdef _DEBUG					
					for (int i = 1; i <= 9; i++)
					{
						cout << "occurances: " << i << occurances[i] << endl;
						file << "occurances: " << i << occurances[i] << endl;
					}
				#endif // _DEBUG


				for (int valueIndex = 1; valueIndex < 10; valueIndex++)
				{
					if (occurances[valueIndex] == 1)
					{
						for (int i = 0; i < 9; i++)
						{
							unordered_set<int> workingCandidateList = context[contextIndex].GetCell(i)->GetCandidateList();

							INCREMENT_CANDIDATELIST_COMPARISON_COUNT;
							if (workingCandidateList.find(valueIndex) != workingCandidateList.end())
							{
								#ifdef _DEBUG	
									int row = 0;
									int	column = 0;
									int block = 0;

									if (contextIdentifier == Context::row)
									{
										row = contextIndex;
										column = i;
									}
									else if (contextIdentifier == Context::column)
									{
										row = i;
										column = contextIndex;
									}
									else if (contextIdentifier == Context::block)
									{
										block = contextIndex;

										int rowOffset = (block / 3) * 3;
										int colOffset = (block % 3) * 3;
										int localRow = i / 3;
										int localCol = i % 3;

										row = rowOffset + localRow;
										column = colOffset + localCol;
									}
									cout << "setting (row: " << row << ", column: " << column << "): " << valueIndex << endl;
									file << "setting (row: " << row << ", column: " << column << "): " << valueIndex << endl;
								#endif // _DEBUG

								context[contextIndex].GetCell(i)->SetValue(valueIndex);
								context[contextIndex].GetCell(i)->SetCandidateList(unordered_set<int>());

								updateCandidateListsForGivenCellsRowBlockColumn(contextIdentifier, contextIndex, i, valueIndex);

								updated = true;
							}
						}
					}
				}
			}
		}
	}

	if (updated)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//Hidden singles overload
bool SudokuPuzzle::findHiddenSingles()
{	
	bool updatedRow = findHiddenSingles(gridRows, Context::row);
	bool updatedCol = findHiddenSingles(gridColumns, Context::column);
	bool updatedBlock = findHiddenSingles(gridBlocks, Context::block);

	return updatedRow || updatedCol || updatedBlock;
}

//Updates the candidate lists for a given cells row, block and column
void SudokuPuzzle::updateCandidateListsForGivenCellsRowBlockColumn(Context const &context, int contextIndex, int index, int value)
{		
	int row = 0;
	int	column = 0;
	int	block = 0;

	if (context == Context::row)
	{
		row = contextIndex;
		column = index;
		block = ((row / 3) * 3) + column / 3;
		
	}
	else if (context == Context::column)
	{
		row = index;
		column = contextIndex;
		block = ((row / 3) * 3) + column / 3;
	}
	else if (context == Context::block)
	{
		block = contextIndex;

		int rowOffset = (block / 3) * 3;
		int colOffset = (block % 3) * 3;
		int localRow = index / 3;
		int localCol = index % 3;

		row = rowOffset + localRow;
		column = colOffset + localCol;
	}

	for (int i = 0; i < 9; i++)
	{
		unordered_set<int> workingRowCandidateList = gridRows[row].GetCell(i)->GetCandidateList();
		unordered_set<int> workingColumnCandidateList = gridColumns[column].GetCell(i)->GetCandidateList();
		unordered_set<int> workingBlockCandidateList = gridBlocks[block].GetCell(i)->GetCandidateList();

		if (workingRowCandidateList.find(value) != workingRowCandidateList.end())
		{
			workingRowCandidateList.erase(value);
			gridRows[row].GetCell(i)->SetCandidateList(workingRowCandidateList);
		}		

		if (workingColumnCandidateList.find(value) != workingColumnCandidateList.end())
		{
			workingColumnCandidateList.erase(value);
			gridColumns[column].GetCell(i)->SetCandidateList(workingColumnCandidateList);
		}

		if (workingBlockCandidateList.find(value) != workingBlockCandidateList.end())
		{
			workingBlockCandidateList.erase(value);
			gridBlocks[block].GetCell(i)->SetCandidateList(workingBlockCandidateList);
		}		
	}
}

//Output solved puzzle
void SudokuPuzzle::Output() const
{
	ofstream fout("sudoku_solution.txt"); // DO NOT CHANGE THE NAME OF THIS FILE
	if(fout.is_open())
	{
		//looping through all cells
		for(int y = 0; y < 9; ++y)
		{
			for(int x = 0; x < 9; ++x)
			{
				//getting value of cell
				Cell* cell = gridRows[y].GetCell(x);
				int value = cell->GetValue();
				
				//outputing solved values of solved puzzle
				fout << value << " ";
			}

			fout << endl;
		}
		fout.close();
	}
}

//Initialise Grid
void SudokuPuzzle::InitialiseGrid(char filenameIn[])
{
	ifstream fin(filenameIn);
	int value = 0;
	std::unordered_set<int> candidateList;
	bool given;

	#ifdef _DEBUG
		int initialValueCounter = 0;
	#endif // _DEBUG



	//--------gridRows and gridColumns population
	//while not end of file
	while (!fin.eof())
	{
		//for each row
		for (int row = 0; row < 9; ++row)
		{
			//for each column
			for (int column = 0; column < 9; ++column)
			{
				//reading in value
				fin >> value;
				given = false;

				if (value != 0)
				{
					#ifdef _DEBUG
						initialValueCounter++;
					#endif // _DEBUG

					given = true;
				}

				//populate row and column sets with new cell
				Cell* cell = new Cell(value, given, candidateList);
				gridRows[row].SetCell(column, cell);
				gridColumns[column].SetCell(row, cell);				
			}
		}
	}

	#ifdef _DEBUG
	cout << "Initial solved values: " << initialValueCounter << endl;
	#endif // _DEBUG


	//-------gridBlocks population----------
	int cellIndex = 0;
	int blockIndex = 0;

	//for each block row
	for (int blockY = 0; blockY <= 6; blockY += 3)
	{
		//for each block column
		for (int blockX = 0; blockX <= 6; blockX += 3)
		{			
			//for each cell row
			for (int cellY = 0; cellY < 3; cellY++)
			{
				//for each cell column
				for (int cellX = 0; cellX < 3; cellX++)
				{				
					//creating new cell from the given block and cell
					Cell* cell = gridRows[blockY +cellY].GetCell(blockX + cellX);
					//adding cell to gridBlocks
					gridBlocks[blockIndex].SetCell(cellIndex, cell);
					++cellIndex;
				}
			}	
			cellIndex = 0;	
			++blockIndex;
		}	
	}
	fin.close();	
}