#include "Character.h"

Character::Character()
{
	position = 0;
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