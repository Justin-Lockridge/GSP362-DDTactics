#pragma once
#include <string>
#include <vector>
#include "Job.h"



class CBaseCharacter
{
private: 
	//Private to prevent outside creation
	struct Attributes
	{
		unsigned int level;
		unsigned int xp;
		unsigned int max_health;		
		unsigned int current_health;   
		unsigned int max_mana;
		unsigned int current_mana;
		unsigned int attack;
		unsigned int magic;
		unsigned int defense;
		unsigned int resistance;
		unsigned int speed;
		unsigned int evasion;
		unsigned int hit;
	};
	
	
	//std::vector<CJob> jobs;
	// base stats

	std::string char_name;
	Attributes stats;
	std::vector<CJob> char_jobs;
	


public:
	CBaseCharacter(void);
	~CBaseCharacter(void);

	//////////////////////////////////////////////////////////////////
	//Name:			init											//
	//Parameters:	none											//
	//Return:		void											//
	//Description:	initialize character stats 
	void init();
};