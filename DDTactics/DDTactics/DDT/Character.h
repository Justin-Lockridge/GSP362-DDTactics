#pragma once

#include "MiscStructs.h"

class Character
{
private:
	Character_stats stats;
	std::vector<job_mods> Jobs;

	//TODO: create ITEM class
	int hands;
	int armors;
	int head;
	int acc;

	Battle_Node *position;	
	D3DXVECTOR3 rotation;
	

public:
	Character();
	~Character();

	//set stats/jobs
	void init();
	Character_stats returnStats();
	void levelup();

};