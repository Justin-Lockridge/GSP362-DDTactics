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

int Player::getMoney()
{
	return money;
}

Inventory Player::getInventory()
{
	Inventory temp;
	return temp;
	//return inventory;
}