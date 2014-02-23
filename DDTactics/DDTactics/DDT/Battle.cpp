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
		temp.setRotation(D3DXVECTOR3(0.0f,-90.0f,0.0f));
		enemies.push_back(temp);
	}
	for(int i = 0; i< enemies.size();i++){
		sortBySpeed.push(enemies[i]);
	}
	// May have problems adjusting characters in player
	std::vector<Character> * tempArmy = player->returnArmy();
	if(!tempArmy->empty()){
		for(int i = 0; i < tempArmy->size(); i++){
			player->getCharacter(i)->setPosition(m_Map.m_graph[pn+=10]);
			player->getCharacter(i)->setRotation(D3DXVECTOR3(0.0f,90.0f,0.0f));
			sortBySpeed.push(*player->getCharacter(i));
		}
	}
	// fill units vector
	while(!sortBySpeed.empty()){
		m_Units.push_back(sortBySpeed.top());
		sortBySpeed.pop();
	}
	// set active character and 3D cursor
	m_3Dcursor = m_Units.front().getPosition(); 
	// set buttons
	RData menu[] = 
	{
		{0,0,25,60,650,170},//Move
		{0,0,25,60,650,215},//ACT
		{0,0,25,70,650,255},//Wait
		{0,0,25,90,650,305},//status
	};
	m_buttons.clear();
	RECT rect;
	Button tempB;
	for(int i = 0; i <= sizeof(menu)/sizeof(menu[0]) ;i++){
		rect.left =menu[i].l; rect.right = menu[i].r;
		rect.top =menu[i].t; rect.bottom = menu[i].b;
		tempB.setRect(rect);
		tempB.setPos(menu[i].x,menu[i].y);
		tempB.setHighlight(false);
		m_buttons.push_back(tempB);
	}
}
void Battle::Shutdown()
{
}

void Battle::Update(Cursor * cursor, InputManager *IManager, SoundManager *SManager, 
				Player *player, int &game_state,  float dt,GraphicsManager3D * GManager3,
				IDirect3DDevice9 *device)
{
	m_activeChar = &m_Units[m_turnIndex];

	GManager3->m_lookAt	= m_3Dcursor->position;	

	D3DXMatrixLookAtLH(
		&GManager3->m_viewMat,
		&GManager3->m_eyePos,
		&GManager3->m_lookAt,
		&GManager3->m_upVec);

	device->SetTransform(D3DTS_VIEW, &GManager3->m_viewMat);

	if( IManager->get_mouseX() || IManager->get_mouseY() ){
		for(auto &buttons: m_buttons){
			if(buttons.isOn(cursor->cursorPos.x, cursor->cursorPos.y, 2))
				buttons.setHighlight(true);
			else
				buttons.setHighlight(false);
		}
	}
}
void Battle::Render(GraphicsManager2D *GManager2, ID3DXSprite *spriteObj, GraphicsManager3D * GManager3, 
					float dt,IDirect3DDevice9 *device)
{
	GManager3->DrawMap(D3DXVECTOR3(5,5,5), D3DXVECTOR3(0,0,0),
						D3DXVECTOR3(0,0,0), MAP_DEFAULT);
	bool notRendered = true;
	for(auto &unit:m_Units){	
		if(!unit.isAnEnemy())
			GManager3->DrawCharacter(D3DXVECTOR3(0.7f,0.7f,0.7f),unit.getPosition()->position,unit.getRotation(),MODEL_DEFAULT, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
		else
			GManager3->DrawCharacter(D3DXVECTOR3(0.7f,0.7f,0.7f),unit.getPosition()->position,unit.getRotation(),MODEL_DEFAULT, D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));
		if(unit.getPosition()->position == m_3Dcursor->position){
			GManager3->DrawCharacter(D3DXVECTOR3(1.5f,1.5f,1.5f),m_3Dcursor->position+D3DXVECTOR3(0,3,0),D3DXVECTOR3(0,0,0),MODEL_3DCURSOR, D3DXCOLOR(1.0f,1.0f,0.0f,1.0f));
			notRendered = false;
		}
	}
	if(notRendered)
		GManager3->DrawCharacter(D3DXVECTOR3(1.5f,1.5f,1.5f),m_3Dcursor->position,D3DXVECTOR3(0,0,0),MODEL_3DCURSOR, D3DXCOLOR(1.0f,1.0f,0.0f,1.0f));
	
	if(!m_areaHighlight.empty())
		for(auto &node:m_areaHighlight)
			GManager3->DrawPlane(device,node.position);
	
	GManager2->Draw2DObject(D3DXVECTOR3(0.70f, 0.90f, 1.0f),D3DXVECTOR3(650.0f, 200.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
		spriteObj,GRAPHICS_BMENU,D3DCOLOR_ARGB(255,255,255,255));

	GManager2->Draw2DObject(D3DXVECTOR3(0.50f, 0.70f, 1.0f),D3DXVECTOR3(200.0f, 500.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
		spriteObj,GRAPHICS_BSTATS_WINDOW,D3DCOLOR_ARGB(255,255,255,255));

	for(auto &buttons: m_buttons){
		if(buttons.isHighlighted()){
			GManager2->Draw2DObject(D3DXVECTOR3(0.5f, 0.5f, 0.5f),D3DXVECTOR3(buttons.getPos().x-40, buttons.getPos().y-10, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
				spriteObj,GRAPHICS_POINTER,D3DCOLOR_ARGB(255,255,255,255));
		}
	}
}