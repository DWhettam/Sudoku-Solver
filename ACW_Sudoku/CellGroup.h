#pragma once
#include "Cell.h"

//CellGroup class
class CellGroup
{
	public:
		//Cellgroup default constructor + destructor
		CellGroup(void);
		~CellGroup(void);	

		//cell set and get
		void SetCell(int index, Cell *cell);
		Cell* GetCell(int index) const;
	
	private:
		Cell* cells[9];
};
