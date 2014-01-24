#pragma once
#include <string>
#include <vector>
#include "Job.h"

class CBaseCharacter
{
private: 
	std::vector<CJob> jobs;
	// base stats
	int currentJob, 
		currentHealth, 
		maxHealth, 
		currentMana, 
		maxMana, 
		currentExperience,
		currentGold;
public:
	//	getters
	int   getJobLevel(int j){return jobs[j].getJobLevel();}
	int   getCurrentJob(){return currentJob;}
	int   getCurrentHealth(){return currentHealth;}
	int   getMaxHealth(){return maxHealth;}
	int   getCurrentMana(){return currentMana;}
	int   getMaxMana(){return maxMana;}
	int   getCurrentExperience(){return currentExperience;}
	int   getCurrentGold(){return currentGold;}
	//  setters
	void  setCurrentJob(int j){currentJob = j;}
	void  setCurrentHealth(int health){currentHealth = health;}
	void  setMaxHealth(int health){maxHealth = health;}
	void  setCurrentMana(int mana){currentMana = mana;}
	void  setMaxMana(int mana){maxMana = mana;}
	void  setCurrentExperience(int experience){currentExperience = experience;}
	void  setCurrentGold(int g){currentGold += g;}

	//  INFO:  For testing
	void  setJobLevel(int job, int level){jobs[job].setJobLevel(level);}

	CBaseCharacter(void);
	~CBaseCharacter(void);

	//////////////////////////////////////////////////////////////////
	//Name:			init											//
	//Parameters:	none											//
	//Return:		void											//
	//Description:	initialize character stats 
	void init();
};