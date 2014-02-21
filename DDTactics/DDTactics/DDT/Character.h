#pragma once

#include "MiscStructs.h"

#define MAXJOBS 3

class Character
{
private:
	Character_stats stats;
	int jobLevels[MAXJOBS];
	int currentJobType;
	//TODO: create ITEM class
	int hands;
	int armors;
	int head;
	int acc;

	Battle_Node *position;	
	D3DXVECTOR3 rotation;
	
	bool enemy;
public:
	Character();
	~Character();

	Character_stats getCharacterStats(){return stats;}
	int getCurrentJob(){return currentJobType;}
	void setCurrentJob(int job){ currentJobType = job;}
	int getJobLevel(int iter){return jobLevels[iter];}
	void setJobLevel(int iter, int level){jobLevels[iter] = level;}
	void gainALevel(job_mods mod);
	//set stats/jobs
	void init();
	void levelup();

};