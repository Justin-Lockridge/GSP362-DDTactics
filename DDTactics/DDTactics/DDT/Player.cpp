#include "Player.h"

Player::Player()
{
	Character temp;
	temp.init();
	temp.setCurrentJob(JOB::WARRIOR);
	army.push_back(temp);
	temp.init();
	temp.setCurrentJob(JOB::GREYMAGE);
	army.push_back(temp);
	temp.init();
	temp.setCurrentJob(JOB::ARCHER);
	army.push_back(temp);

	Items temporary;
	for(int i = 0; i < MAX_ITEMS; ++i)
	{
		inventory.push_back(temporary);
		inventory[i].itemCount = 0;
	}
	money = 1000;
}

Player::~Player()
{}

Player* Player::instance()
{
	static Player player;
	return &player;
}

std::vector<Character>* Player::returnArmy()
{
	return &army;
}

std::vector<Items>* Player::returnInv()
{
	return &inventory;
}

int Player::getMoney()
{
	return money;
}

Inventory Player::getInventory()
{
	Inventory temp;
	return temp;
}

void Player::setCharacterLevel(int iter, int job, int level)
{
	army[iter].setJobLevel(job, level);
}

void Player::resetStats(int iter, job_mods jobMod)
{
	army[iter].resetStats(jobMod);
};