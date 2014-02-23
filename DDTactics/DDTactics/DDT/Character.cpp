#include "Character.h"

Character::Character()
{
	position = 0;
	stats.attack = 0;
	stats.defense = 0;
	stats.evasion = 0;
	stats.health = 0;
	stats.hit = 0;
	stats.level = 1;
	stats.magic = 0;
	stats.mana = 0;
	stats.max_health = 0;
	stats.max_mana = 0;
	stats.resist = 0;
	stats.speed = 0;
	stats.xp = 0;

	enemy = false;
	position = NULL;
	rotation = D3DXVECTOR3(0,0,0);

	hands = 0;
	armors = 0;
	head = 0;
	acc = 0;

	for(int i = 0; i < MAXJOBS; ++i){
		jobLevels[i] = 0;
	};

	currentJobType = 0;
}


Character::~Character()
{}


void Character::init()
{
	stats.level = 1;
	stats.xp = 0;

	stats.max_health = rand() % 11 + 25;
	stats.health = stats.max_health;
	stats.max_mana = rand() % 11 + 25;
	stats.mana = stats.max_mana;

	stats.attack = rand() % 5 + 5;
	stats.magic = rand() % 5 + 5;
	
	stats.defense = rand() % 5 + 5;
	stats.resist = rand() % 5 + 5;	
	
	stats.hit = rand() % 5 + 5;
	stats.evasion = rand() % 5 + 5;	
	
	stats.speed = rand() % 5 + 5;


}

Character_stats Character::getCharacterStats()
{return stats;}

int Character::getCurrentJob()
{return currentJobType;}

void Character::setCurrentJob(int job)
{ currentJobType = job;}

int Character::getJobLevel(int iter)
{return jobLevels[iter];}

void Character::setJobLevel(int iter, int level)
{jobLevels[iter] = level;}

void Character::gainALevel(job_mods mod)
{

}

void Character::setIfEnemy(bool is)
{enemy = is;}

bool Character::isAnEnemy()
{return enemy;}

void Character::setPosition(Battle_Node &a_node)
{position = &a_node;}

Battle_Node* Character::getPosition()
{return position;}