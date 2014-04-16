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

	inventory[DAGGER].itemName = L"Dagger";
	inventory[BROADSWORD].itemName = L"Broad Sword";
	inventory[ROD].itemName = L"Rod";
	inventory[OAKSTAFF].itemName = L"Oak Staff";
	inventory[LEATHERHAT].itemName = L"Leather Hat";
	inventory[CLOTHARMOR].itemName = L"Cloth Armor";
	inventory[PHOENIXDOWN].itemName = L"Phoenix Down";
	inventory[ETHER].itemName = L"Ether";
	inventory[HIETHER].itemName = L"Hi-Ether";
	inventory[POTION].itemName = L"Potion";
	inventory[HIPOTION].itemName = L"Hi-Potion";


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

void Player::setCharacterJobLevel(int iter, int job, int level)
{
	army[iter].setJobLevel(job, level);
}

void Player::setCharacterLevel(int index, int level){
	army[index].setCharacterLevelOnly(level);
};

void Player::resetStats(int iter, job_mods jobMod)
{
	army[iter].resetStats(jobMod);
};

void Player::setCharacterJobExperience(int iter, int job, int experience){
	army[iter].setJobExperience(job, experience);
};

void Player::fillInventoryAfterLoading(int item, int amount){
	inventory[item].itemCount = amount;
};

void Player::setMoney(int amount){
	money = amount;
};