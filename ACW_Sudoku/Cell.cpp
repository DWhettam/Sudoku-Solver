#include "Cell.h"

//cell constructor
Cell::Cell(int value, bool given, std::unordered_set<int> const &candidateList) :
	m_candidateList(candidateList), m_value(value), m_given(given){}

//Default constructor + destructor
Cell::Cell() : m_candidateList(std ::unordered_set<int>()), m_value(0), m_given(false){}
Cell::~Cell() {}

//value get and set
void Cell::SetValue(int inValue)
{
	m_value = inValue;
}
int Cell::GetValue() const
{
	return m_value;
}

//given get and set
void Cell::SetGiven(bool inGiven)
{
	m_given = inGiven;
}
bool Cell::GetGiven() const
{
	return m_given;
}

//candidatelist get and set
void Cell::SetCandidateList(std::unordered_set<int> const &inCandidateList)
{	
	m_candidateList = inCandidateList;
}
const std::unordered_set<int> &Cell::GetCandidateList() const
{
	return m_candidateList;
}