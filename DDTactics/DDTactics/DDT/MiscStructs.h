#pragma once


#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <vector>


struct Overworld_node
{
	D3DXVECTOR2 node_position;
	Overworld_node *up;
	Overworld_node *down;
	Overworld_node *left;
	Overworld_node *right;

	static const int width = 80;
	static const int height = 50;

	int area_num;
	int area_type;
	bool story_battle;	
	D3DCOLOR node_color;

	Overworld_node()
	{
		up = down = left = right = 0;	
	}
		
};

struct Area_location
{
	float x, y;
};

struct RData{
	int t, l, b, r;
	float x, y;
	bool highlight;
};

struct simpleRData
{
	int t, b, l, r;
};

struct job_mods
{
	unsigned int job_level;
	unsigned int job_xp;

	float mod_health;
	float mod_mana;
	float mod_attack;
	float mod_magic;
	float mod_defense;
	float mod_resistance;
	float mod_speed;
	float mod_evasion;
	float mod_hit;
	
};

struct Character_stats
{
	int xp;
	int level;
	
	int health;			
	int max_health;		//18-25

	int mana;			//15-20
	int max_mana;

	int attack;			
	int magic;
	int defense;
	int resist;
	int speed;
	int evasion;
	int hit;
};

struct Battle_Node
{
	D3DXVECTOR3 position;
	std::vector<Battle_Node*> edges;
};