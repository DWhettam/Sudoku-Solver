#include "CellGroup.h"

//CellGroup default constructor + destructor
CellGroup::CellGroup():cells{ nullptr } {}
CellGroup::~CellGroup() {}

//Cell set and get
void CellGroup::SetCell(int index, Cell *cell)
{
	cells[index] = cell;
}
Cell* CellGroup::GetCell(int index) const
{
	return cells[index];
}
