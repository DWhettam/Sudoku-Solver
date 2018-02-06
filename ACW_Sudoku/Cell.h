#pragma once
#include <unordered_set>

//Cell class
class Cell
{
public:
	//default constructor + destructor
	Cell(void);
	~Cell(void);

	//constructor
	Cell(int value, bool given, std::unordered_set<int> const &candidateList);	

	//value get and set
	void SetValue(int inValue);
	int GetValue() const;

	//given get and set
	void SetGiven(bool inGiven);
	bool GetGiven() const;

	//candidateList get and set
	std::unordered_set<int> const &GetCandidateList() const;
	void SetCandidateList(std::unordered_set<int> const &inCandidateList);
	

private:
	std::unordered_set<int> m_candidateList;
	int m_value;
	bool m_given;
};
