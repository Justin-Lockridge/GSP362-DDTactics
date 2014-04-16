#pragma once

#include "MiscStructs.h"

#define MAXJOBS 3

class Character
{
private:
	Character_stats stats;
	Character_stats mod_stats;
	int jobLevels[MAXJOBS];
	int currentJobType;
	//TODO: create ITEM class
	int hands;
	int armors;
	int head;
	int acc;

	/////////////////////////////////////////////
	//  INFO:  Array for job experience
	int jobExperience[MAXJOBS];

	Battle_Node *position;	
	D3DXVECTOR3 rotation;
	
	Character* previous;
	Character* next;

	LPCWSTR name;
	

	bool enemy;
public:
	Character();
	~Character();

	Character_stats getCharacterStats();
	Character_stats getModStats();

	int getCurrentJob();
	void setCurrentJob(int job);

	int getJobLevel(int iter);
	void setJobLevel(int iter, int level);

	void gainALevel(job_mods mod);

	void setIfEnemy(bool is);
	bool isAnEnemy();

	void setPosition(Battle_Node &a_node);
	Battle_Node* getPosition();

	void setRotation(D3DXVECTOR3 a_vector);
	D3DXVECTOR3 getRotation();

	
	void setNext(Character *character);
	Character* getNext();

	void setPrevious(Character *character);
	Character* getPrevious();

	void setName(LPCWSTR name);
	LPCWSTR getName();

	void adjustHealth(int a_amt);
	void adjustMana(int a_amt);

	//set stats/jobs
	void init();
	void levelup();

	bool operator()(Character l, Character r){ return l.getCharacterStats().speed > r.getCharacterStats().speed;}

	void resetStats(job_mods jobMod);
	void setJobExperience(int job, int experience);
	int  getJobExperience(int job);
	void setCharacterLevelOnly(int level);
};