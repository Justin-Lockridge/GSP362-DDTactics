#pragma once
#include <vector>
#include "Job.h"
class CBaseCharacter
{
private: 
	std::vector<CJob> jobs;
	// base stats
public:
	CBaseCharacter(void);
	~CBaseCharacter(void);
};