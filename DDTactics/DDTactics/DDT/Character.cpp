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
		jobLevels[i] = 2;
	};

	currentJobType = 0;

	previous = 0;
	next = 0;

	name = L"(Name Here)";

}


Character::~Character()
{}


void Character::init()
{
	stats.level = mod_stats.level = 1;
	stats.xp = mod_stats.xp = 0;

	stats.max_health = mod_stats.max_health = rand() % 11 + 25;
	stats.health = mod_stats.health = stats.max_health;
	stats.max_mana = mod_stats.max_mana = rand() % 11 + 25;
	stats.mana = mod_stats.mana = stats.max_mana;

	stats.attack = mod_stats.attack = rand() % 5 + 5;
	stats.magic = mod_stats.magic = rand() % 5 + 5;
	
	stats.defense = mod_stats.defense = rand() % 5 + 5;
	stats.resist = mod_stats.resist = rand() % 5 + 5;	
	
	stats.hit = mod_stats.hit = rand() % 5 + 5;
	stats.evasion = mod_stats.evasion = rand() % 5 + 5;	
	
	stats.speed = mod_stats.speed = rand() % 5 + 5;

	

}

Character_stats Character::getCharacterStats()
{return stats;}

Character_stats Character::getModStats()
{return mod_stats;}

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

void Character::setRotation(D3DXVECTOR3 a_vector)
{rotation = a_vector;}

D3DXVECTOR3 Character::getRotation()
{return rotation;}

void Character::adjustHealth(int a_amt)
{
	stats.health += a_amt;
	if(stats.health < 0)
		stats.health = 0;
	if(stats.health > stats.max_health)
		stats.health = stats.max_health;
}
void Character::adjustMana(int a_amt)
{
	stats.mana += a_amt;
	if(stats.mana < 0)
		stats.mana = 0;
	if(stats.mana > stats.max_mana)
		stats.mana = stats.max_mana;
}

void Character::resetStats(job_mods jobMod){
	mod_stats.attack = stats.attack + (this->getJobLevel(this->getCurrentJob()) - 1)  * jobMod.mod_attack;
	mod_stats.defense = stats.defense + (this->getJobLevel(this->getCurrentJob()) - 1) * jobMod.mod_defense;
	mod_stats.evasion = stats.evasion + (this->getJobLevel(this->getCurrentJob()) - 1) * jobMod.mod_evasion;
	mod_stats.health = stats.health + (this->getJobLevel(this->getCurrentJob()) - 1) * jobMod.mod_health;
	mod_stats.hit = stats.hit + (this->getJobLevel(this->getCurrentJob()) - 1) * jobMod.mod_hit;
	mod_stats.magic = stats.magic + (this->getJobLevel(this->getCurrentJob()) - 1) * jobMod.mod_magic;
	mod_stats.mana = stats.mana +  (this->getJobLevel(this->getCurrentJob()) - 1) * jobMod.mod_mana;
	mod_stats.max_health = stats.max_health + (this->getJobLevel(this->getCurrentJob()) - 1) * jobMod.mod_health;
	mod_stats.max_mana = stats.max_mana + (this->getJobLevel(this->getCurrentJob()) - 1) * jobMod.mod_mana;
	mod_stats.resist = stats.resist + (this->getJobLevel(this->getCurrentJob()) - 1) * jobMod.mod_resistance;
	mod_stats.speed = stats.speed + (this->getJobLevel(this->getCurrentJob()) - 1) * jobMod.mod_speed;
}

void Character::setNext(Character *character)
{
	next = character;
}

Character* Character::getNext()
{
	return next;
}

void Character::setPrevious(Character *character)
{
	previous = character;
}

Character* Character::getPrevious()
{
	return previous;
}

void Character::setName(LPCWSTR name)
{
	this->name = name;
}

LPCWSTR Character::getName()
{
	return name;
}