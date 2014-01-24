#include "BaseCharacter.h"


CBaseCharacter::CBaseCharacter(void)
{
	CJob temp;
	temp.setJobLevel(0);
	for(int i = 0; i < 3; ++i){
		jobs.push_back(temp);
	}
	currentJob = 0;
	currentHealth = 0;
	maxHealth = 0;
	currentMana = 0;
	maxMana = 0;
	currentExperience = 0;
	currentGold = 100;
}


CBaseCharacter::~CBaseCharacter(void)
{
}
