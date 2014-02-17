#include "Player.h"

Player::Player()
{}

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
	return inventory;
}


