#pragma once

#include "GameEnums.h"
#include "MiscStructs.h"

class Job
{
private:
	
	bool job_unlocked;
	job_mods	mods;
	job_mods	mod_growth;

public:
	Job();
	~Job();


};