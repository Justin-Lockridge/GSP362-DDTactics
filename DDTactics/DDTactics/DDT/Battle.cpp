#include "Battle.h"


Battle::Battle(void)
{
	m_Units.clear();
	m_areaHighlight.clear();
	m_buttons.clear();

	m_3Dcursor = NULL;
	m_activeChar = NULL;

	m_charState = 0;
	m_actState = 0;
	m_turnIndex = 0;
}


Battle::~Battle(void)
{
}

Battle* Battle::instance()
{
	static Battle battle;
	return &battle;
}

void Battle::Init(Player* player)
{
	// generate map
	m_Map.init();
	// generate enemies
	std::priority_queue<Character*,std::vector<Character>,Character> sortBySpeed;
	Character temp;
	std::vector<Character> enemies;
	int en = 0 ,pn = 49;
	for(int i = 0; i < 3/*num_enemies*/; i++){
		temp.init();
		int job = rand() % 3;
		temp.setJobLevel(job,1);
		temp.setCurrentJob(job);
		temp.setIfEnemy(true);
		temp.setPosition(m_Map.m_graph[en+=10]);
		enemies.push_back(temp);
	}
	for(int i = 0; i< enemies.size();i++){
		sortBySpeed.push(enemies[i]);
	}
	// May have problems adjusting characters in player
	std::vector<Character> * tempArmy = player->returnArmy();
	if(!tempArmy->empty()){
		for(int i = 0; i < tempArmy->size(); i++){
			player->getCharacter(i).setPosition(m_Map.m_graph[pn+=10]);
			sortBySpeed.push(player->getCharacter(i));
		}
	}
	// fill units vector
	while(!sortBySpeed.empty()){
		m_Units.push_back(sortBySpeed.top());
		sortBySpeed.pop();
	}
	// set active character and 3D cursor
	m_3Dcursor = m_Units.front().getPosition(); 
}
void Battle::Shutdown()
{
}

void Battle::Update(D3DXVECTOR2 &cursorPos, InputManager *IManager, SoundManager *SManager, 
				Player *player, int &game_state,  float dt)
{

}
void Battle::Render(GraphicsManager2D *GManager2, ID3DXSprite *spriteObj, GraphicsManager3D * GManager3, float dt)
{

}