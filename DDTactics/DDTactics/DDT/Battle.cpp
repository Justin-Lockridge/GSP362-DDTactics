#include "Battle.h"


Battle::Battle(void)
{
	m_Units.clear();
	m_areaHighlight.clear();
	m_buttons.clear();
	m_actButtons1.clear();
	m_actButtonsItems.clear();
	m_actButtonsSkills.clear();

	m_3Dcursor = NULL;
	m_activeChar = NULL;
	m_charSelected = NULL;

	m_charState = 0;
	m_actState = 0;
	m_turnIndex = 0;
	m_action = 0;
	m_count = 0;
	moved = false;
	acted = false;
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
	m_charState = 0;
	m_actState = 0;
	m_action = 0;
	m_turnIndex = 0;
	m_count = 0;
	moved = false;
	acted = false;
	
	m_charSelected = NULL;

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
	RData actMenu[] = 
	{
		{0,0,25,60,550,170},//Attack
		{0,0,25,60,550,215},//Skill
		{0,0,25,70,550,255},//Item
	};
	RData itemMenu[] = 
	{
		{0,0,25,60,650,160},//potion
		{0,0,25,60,650,205},//hi-potion
		{0,0,25,70,650,245},//ether
		{0,0,25,90,650,285},//hi-ether
		{0,0,50,90,650,335},//phonix down
	};
	RData skillMenu[] = 
	{
		{0,0,25,60,630,195},//thunder
		{0,0,25,70,630,235},//cure
	};
	m_buttons.clear();
	m_actButtons1.clear();
	m_actButtonsItems.clear();
	m_actButtonsSkills.clear();
	RECT rect;
	Button tempB;
	for(int i = 0; i < 4 ;i++){
		rect.left =menu[i].l; rect.right = menu[i].r;
		rect.top =menu[i].t; rect.bottom = menu[i].b;
		tempB.setRect(rect);
		tempB.setPos(menu[i].x,menu[i].y);
		tempB.setHighlight(false);
		m_buttons.push_back(tempB);
	}

	for(int i = 0; i < 3 ;i++){
		rect.left =actMenu[i].l; rect.right = actMenu[i].r;
		rect.top =actMenu[i].t; rect.bottom = actMenu[i].b;
		tempB.setRect(rect);
		tempB.setPos(actMenu[i].x,actMenu[i].y);
		tempB.setHighlight(false);
		m_actButtons1.push_back(tempB);
	}

	for(int i = 0; i < 5 ;i++){
		rect.left =itemMenu[i].l; rect.right = itemMenu[i].r;
		rect.top =itemMenu[i].t; rect.bottom = itemMenu[i].b;
		tempB.setRect(rect);
		tempB.setPos(itemMenu[i].x,itemMenu[i].y);
		tempB.setHighlight(false);
		m_actButtonsItems.push_back(tempB);
	}

	for(int i = 0; i < 2 ;i++){
		rect.left =skillMenu[i].l; rect.right = skillMenu[i].r;
		rect.top =skillMenu[i].t; rect.bottom = skillMenu[i].b;
		tempB.setRect(rect);
		tempB.setPos(skillMenu[i].x,skillMenu[i].y);
		tempB.setHighlight(false);
		m_actButtonsSkills.push_back(tempB);
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
	if(m_activeChar->getCharacterStats().health <= 0){
		m_activeChar->setRotation(D3DXVECTOR3(90,90,90));
		m_turnIndex++;
	}
	if(m_turnIndex >= m_Units.size()){
		m_turnIndex = 0;
		m_activeChar = &m_Units[m_turnIndex];
	}
	GManager3->m_lookAt	= m_3Dcursor->position;	

	D3DXMatrixLookAtLH(
		&GManager3->m_viewMat,
		&GManager3->m_eyePos,
		&GManager3->m_lookAt,
		&GManager3->m_upVec);

	device->SetTransform(D3DTS_VIEW, &GManager3->m_viewMat);

	if(!m_activeChar->isAnEnemy()){
		if(moved && acted)
			m_charState = CHAR_STATE::WAIT;
		switch(m_charState)
		{
		case CHAR_STATE::START:
			m_charSelected = NULL;
			if( IManager->get_mouseX() || IManager->get_mouseY() ){
				for(auto &buttons: m_buttons){
					if(buttons.isOn(cursor->cursorPos.x, cursor->cursorPos.y, 2))
						buttons.setHighlight(true);
					else
						buttons.setHighlight(false);
				}
			}
			if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON)){
				if(!IManager->check_button_down(DIK_9)){
					IManager->set_button(DIK_9, true);
					int selected = 99;

					for(int i = 0; i < m_buttons.size(); i++){
						if(m_buttons[i].isHighlighted())
							selected = i;
					}

					switch(selected)
					{
					case 0:	// move
						if(!moved){
							m_charState = CHAR_STATE::MOVE;
							highlightMap(4);
							m_3Dcursor = m_activeChar->getPosition()->edges[0];
						}
						break;
					case 1:	// act
						if(!acted){
							m_charState = CHAR_STATE::ACT;
							m_actState = ACT_STATE::INITIAL;
						}
						break;
					case 2:	// wait
						m_charState = CHAR_STATE::WAIT;
						break;
					case 3:	// check status
						m_charState = CHAR_STATE::CHECK_STATUS;
						break;
					default:
						break;
					}
				}
			}else IManager->set_button(DIK_9, false);
			break;
		case CHAR_STATE::MOVE:
			Move(cursor,IManager,SManager,player,game_state,dt,GManager3,device);
			break;
		case CHAR_STATE::ACT:
			Act(cursor,IManager,SManager,player,game_state,dt,GManager3,device);
			//m_charState = CHAR_STATE::START;
			break;
		case CHAR_STATE::WAIT:
			Wait(cursor,IManager,SManager,player,game_state,dt,GManager3,device);
			break;
		case CHAR_STATE::CHECK_STATUS:
			m_charState = CHAR_STATE::START;
			break;
		}
	}
	else{
		m_count+=dt;
		if(m_count > 1.0f){
			m_count = 0;
			m_turnIndex++;
			if(m_turnIndex >= m_Units.size())
				m_turnIndex = 0;
			m_3Dcursor = m_Units[m_turnIndex].getPosition();
		}
	}

}
void Battle::Render3D(GraphicsManager2D *GManager2, ID3DXSprite *spriteObj, GraphicsManager3D * GManager3, 
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

		switch(unit.getCurrentJob())
		{
		case JOB::WARRIOR:
			GManager3->DrawCharacter(D3DXVECTOR3(1.7f,1.7f,1.7f),
				unit.getPosition()->position + D3DXVECTOR3(1,2,0),unit.getRotation()+D3DXVECTOR3(0,0,70),
				MODEL_SWORD, D3DXCOLOR(0.0f,0.0f,0.0f,0.0f));
			break;
		case JOB::GREYMAGE:
			GManager3->DrawCharacter(D3DXVECTOR3(3.7f,3.7f,3.7f),
				unit.getPosition()->position + D3DXVECTOR3(1,2,0),unit.getRotation()+D3DXVECTOR3(0,0,70),
				MODEL_STAFF, D3DXCOLOR(0.0f,0.0f,0.0f,0.0f));
			break;
		case JOB::ARCHER:
			break;
		}
	}
	if(notRendered)
		GManager3->DrawCharacter(D3DXVECTOR3(1.5f,1.5f,1.5f),m_3Dcursor->position,D3DXVECTOR3(0,0,0),MODEL_3DCURSOR, D3DXCOLOR(1.0f,1.0f,0.0f,1.0f));

	if(!m_areaHighlight.empty())
		for(auto &node:m_areaHighlight)
			GManager3->DrawPlane(device,node->position);
}
void Battle::Render2D(GraphicsManager2D *GManager2, ID3DXSprite *spriteObj, GraphicsManager3D * GManager3, 
					  float dt,IDirect3DDevice9 *device)
{
	GManager2->Draw2DObject(D3DXVECTOR3(0.50f, 0.70f, 1.0f),D3DXVECTOR3(200.0f, 500.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
		spriteObj,GRAPHICS_BSTATS_WINDOW,D3DCOLOR_ARGB(255,255,255,255));

	if(m_activeChar){
		switch(m_activeChar->getCurrentJob())
		{
		case JOB::ARCHER:
			GManager2->Draw2DObject(D3DXVECTOR3(0.16f, 0.16f, 1.0f),D3DXVECTOR3(80.0f, 494.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
				spriteObj,GRAPHICS_ARCHER_ICON,D3DCOLOR_ARGB(255,255,255,255));
			break;
		case JOB::GREYMAGE:
			GManager2->Draw2DObject(D3DXVECTOR3(0.16f, 0.16f, 1.0f),D3DXVECTOR3(80.0f, 494.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
				spriteObj,GRAPHICS_MAGE_ICON,D3DCOLOR_ARGB(255,255,255,255));
			break;
		case JOB::WARRIOR:
			GManager2->Draw2DObject(D3DXVECTOR3(0.16f, 0.16f, 1.0f),D3DXVECTOR3(80.0f, 494.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
				spriteObj,GRAPHICS_WAR_ICON,D3DCOLOR_ARGB(255,255,255,255));
			break;
		}
		if(!m_activeChar->isAnEnemy() && m_charState != CHAR_STATE::WAIT){
			GManager2->Draw2DObject(D3DXVECTOR3(0.70f, 0.90f, 1.0f),D3DXVECTOR3(650.0f, 200.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
				spriteObj,GRAPHICS_BMENU,D3DCOLOR_ARGB(255,255,255,255));
			for(auto &buttons: m_buttons){
				if(buttons.isHighlighted()){
					GManager2->Draw2DObject(D3DXVECTOR3(0.5f, 0.5f, 0.5f),D3DXVECTOR3(buttons.getPos().x-40, buttons.getPos().y-10, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
						spriteObj,GRAPHICS_POINTER,D3DCOLOR_ARGB(255,255,255,255));
				}
			}
		}
		if(m_charState == CHAR_STATE::WAIT)
			GManager2->Draw2DObject(D3DXVECTOR3(0.5f, 0.90f, 1.0f),D3DXVECTOR3(550.0f, 100.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
			spriteObj,GRAPHICS_WAIT_WINDOW,D3DCOLOR_ARGB(255,255,255,255));
	}
	if(m_charState == CHAR_STATE::ACT){
		switch(m_actState)
		{
		case ACT_STATE::INITIAL:
			GManager2->Draw2DObject(D3DXVECTOR3(1.0f, 1.0f, 1.0f),D3DXVECTOR3(550.0f, 200.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
				spriteObj,GRAPHICS_ACT_WINDOW,D3DCOLOR_ARGB(255,255,255,255));
			for(auto &buttons: m_actButtons1){
				if(buttons.isHighlighted()){
					GManager2->Draw2DObject(D3DXVECTOR3(0.5f, 0.5f, 0.5f),D3DXVECTOR3(buttons.getPos().x-50, buttons.getPos().y-10, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
						spriteObj,GRAPHICS_POINTER,D3DCOLOR_ARGB(255,255,255,255));
				}
			}
			break;
		case ACT_STATE::SKILL:
			GManager2->Draw2DObject(D3DXVECTOR3(1.0f, 1.0f, 1.0f),D3DXVECTOR3(550.0f, 200.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
				spriteObj,GRAPHICS_ACT_WINDOW,D3DCOLOR_ARGB(255,255,255,255));
			for(auto &buttons: m_actButtons1){
				if(buttons.isHighlighted()){
					GManager2->Draw2DObject(D3DXVECTOR3(0.5f, 0.5f, 0.5f),D3DXVECTOR3(buttons.getPos().x-50, buttons.getPos().y-10, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
						spriteObj,GRAPHICS_POINTER,D3DCOLOR_ARGB(255,255,255,255));
				}
			}
			GManager2->Draw2DObject(D3DXVECTOR3(1.0f, 1.0f, 1.0f),D3DXVECTOR3(650.0f, 200.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
				spriteObj,GRAPHICS_MAGE_SKILLS,D3DCOLOR_ARGB(255,255,255,255));
			for(auto &buttons: m_actButtonsSkills){
				if(buttons.isHighlighted()){
					GManager2->Draw2DObject(D3DXVECTOR3(0.5f, 0.5f, 0.5f),D3DXVECTOR3(buttons.getPos().x-50, buttons.getPos().y-10, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
						spriteObj,GRAPHICS_POINTER,D3DCOLOR_ARGB(255,255,255,255));
				}
			}
			break;
		case ACT_STATE::ITEMS:
			GManager2->Draw2DObject(D3DXVECTOR3(1.0f, 1.0f, 1.0f),D3DXVECTOR3(550.0f, 200.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
				spriteObj,GRAPHICS_ACT_WINDOW,D3DCOLOR_ARGB(255,255,255,255));
			for(auto &buttons: m_actButtons1){
				if(buttons.isHighlighted()){
					GManager2->Draw2DObject(D3DXVECTOR3(0.5f, 0.5f, 0.5f),D3DXVECTOR3(buttons.getPos().x-50, buttons.getPos().y-10, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
						spriteObj,GRAPHICS_POINTER,D3DCOLOR_ARGB(255,255,255,255));
				}
			}
			GManager2->Draw2DObject(D3DXVECTOR3(1.0f, 1.0f, 1.0f),D3DXVECTOR3(650.0f, 200.0f, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
				spriteObj,GRAPHICS_ACT_ITEMS,D3DCOLOR_ARGB(255,255,255,255));
			for(auto &buttons: m_actButtonsItems){
				if(buttons.isHighlighted()){
					GManager2->Draw2DObject(D3DXVECTOR3(0.5f, 0.5f, 0.5f),D3DXVECTOR3(buttons.getPos().x-60, buttons.getPos().y-10, 0.0f),D3DXVECTOR3(0.0f,0.0f,0.0f),
						spriteObj,GRAPHICS_POINTER,D3DCOLOR_ARGB(255,255,255,255));
				}
			}
			break;
		default:
			break;
		}
	}
}

void Battle::RenderText(ID3DXFont *font)
{
	RECT rect;
	rect.left = 200;
	rect.top = 200;
	wchar_t tbuffer[64];
	if (m_charState == CHAR_STATE::ACT){
		if(m_dmg){
			swprintf_s(tbuffer, 64,L"%d",m_dmg);
			font->DrawText(0, tbuffer, -1, &rect, 
				DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 0));
		}
	}
	if(m_activeChar){

		rect.left = 140;
		rect.top = 470;
		swprintf_s(tbuffer, 64,L"HP %d",m_activeChar->getCharacterStats().health);
		font->DrawText(0, tbuffer, -1, &rect, 
			DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.left = 240;
		swprintf_s(tbuffer, 64,L"/ %d",m_activeChar->getCharacterStats().max_health);
		font->DrawText(0, tbuffer, -1, &rect, 
			DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

		rect.left = 140;
		rect.top = 500;

		swprintf_s(tbuffer, 64,L"MP %d",m_activeChar->getCharacterStats().mana);
		font->DrawText(0, tbuffer, -1, &rect, 
			DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.left = 240;
		swprintf_s(tbuffer, 64,L"/ %d",m_activeChar->getCharacterStats().max_mana);
		font->DrawText(0, tbuffer, -1, &rect, 
			DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

		rect.left = 360;
		rect.top = 470;

		swprintf_s(tbuffer, 64,L"(Name Here)");
		font->DrawText(0, tbuffer, -1, &rect, 
			DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
		rect.top = 500;
		switch(m_activeChar->getCurrentJob())
		{
		case JOB::ARCHER:
			swprintf_s(tbuffer, 64,L"Archer");
			break;
		case JOB::GREYMAGE:
			swprintf_s(tbuffer, 64,L"Mage");
			break;
		case JOB::WARRIOR:
			swprintf_s(tbuffer, 64,L"Warrior");
			break;
		}
		font->DrawText(0, tbuffer, -1, &rect, 
			DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));
	}
}

void Battle::Act(Cursor * cursor, InputManager *IManager, SoundManager *SManager, 
				 Player *player, int &game_state,  float dt,GraphicsManager3D * GManager3,
				 IDirect3DDevice9 *device)
{
	switch(m_actState)
	{
	case ACT_STATE::INITIAL:
		m_count = 0;
		if(IManager->check_mouse_button(RIGHT_MOUSE_BUTTON)){
			if(!IManager->check_button_down(DIK_0)){
				IManager->set_button(DIK_0, true);
				m_charState = CHAR_STATE::START;
			}
		}else IManager->set_button(DIK_0,false);

		if( IManager->get_mouseX() || IManager->get_mouseY() ){
			for(auto &buttons: m_actButtons1){
				if(buttons.isOn(cursor->cursorPos.x, cursor->cursorPos.y, 2))
					buttons.setHighlight(true);
				else
					buttons.setHighlight(false);
			}
		}
		if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON)){
			if(!IManager->check_button_down(DIK_9)){
				IManager->set_button(DIK_9, true);
				int selected = 99;
				for(int i = 0; i < m_actButtons1.size(); i++){
					if(m_actButtons1[i].isHighlighted())
						selected = i;
				}
				switch(selected)
				{
				case 0:	// Attack
					if(m_activeChar->getCurrentJob() == JOB::ARCHER){
						m_action = ACTION::ARROW;
						highlightMap(4);
						m_actState = ACT_STATE::SELECTION;
					}
					else{
						m_action = ACTION::BASIC;
						highlightMap(1);
						m_actState = ACT_STATE::SELECTION;
					}
					//m_action = 
					break;
				case 1:	// Skill
					if(m_activeChar->getCurrentJob() == JOB::GREYMAGE)
						m_actState = ACT_STATE::SKILL;
					break;
				case 2:	// Items
					m_actState = ACT_STATE::ITEMS;
					break;
				default:
					break;
				}
			}
		}else IManager->set_button(DIK_9, false);
		break;
	case ACT_STATE::SKILL:

		if(IManager->check_mouse_button(RIGHT_MOUSE_BUTTON)){
			if(!IManager->check_button_down(DIK_0)){
				IManager->set_button(DIK_0, true);
				m_actState = ACT_STATE::INITIAL;
			}
		}else IManager->set_button(DIK_0,false);

		if( IManager->get_mouseX() || IManager->get_mouseY() ){
			for(auto &buttons: m_actButtonsSkills){
				if(buttons.isOn(cursor->cursorPos.x, cursor->cursorPos.y, 2))
					buttons.setHighlight(true);
				else
					buttons.setHighlight(false);
			}
		}
		if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON)){
			if(!IManager->check_button_down(DIK_9)){
				IManager->set_button(DIK_9, true);
				int selected = 99;
				for(int i = 0; i < m_actButtonsSkills.size(); i++){
					if(m_actButtonsSkills[i].isHighlighted())
						selected = i;
				}
				switch(selected)
				{
				case 0:	// Thunder
					//m_actState = ACT_STATE::END;
					m_action = ACTION::THUNDER;
					highlightMap(4);
					m_actState = ACT_STATE::SELECTION;
					break;
				case 1:	// Cure
					//m_actState = ACT_STATE::END;
					m_action = ACTION::CURE;
					highlightMap(4);
					m_actState = ACT_STATE::SELECTION;
					break;
				default:
					break;
				}
			}
		}else IManager->set_button(DIK_9, false);
		break;
	case ACT_STATE::ITEMS:

		if(IManager->check_mouse_button(RIGHT_MOUSE_BUTTON)){
			if(!IManager->check_button_down(DIK_0)){
				IManager->set_button(DIK_0, true);
				m_actState = ACT_STATE::INITIAL;
			}
		}else IManager->set_button(DIK_0,false);

		if( IManager->get_mouseX() || IManager->get_mouseY() ){
			for(auto &buttons: m_actButtonsItems){
				if(buttons.isOn(cursor->cursorPos.x, cursor->cursorPos.y, 2))
					buttons.setHighlight(true);
				else
					buttons.setHighlight(false);
			}
		}
		if(IManager->check_mouse_button(LEFT_MOUSE_BUTTON)){
			if(!IManager->check_button_down(DIK_9)){
				IManager->set_button(DIK_9, true);
				int selected = 99;
				for(int i = 0; i < m_actButtonsItems.size(); i++){
					if(m_actButtonsItems[i].isHighlighted())
						selected = i;
				}
				switch(selected)
				{
				case 0://POTION
					m_actState = ACT_STATE::SELECTION;
					m_action = ACTION::ACTION_POTION;
					highlightMap(1);
					break;
				case 1://HI-POTION
					m_actState = ACT_STATE::SELECTION;
					m_action = ACTION::ACTION_HI_POTION;
					highlightMap(1);
					break;
				case 2://ETHER
					m_actState = ACT_STATE::SELECTION;
					m_action = ACTION::ACTION_ETHER;
					highlightMap(1);
					break;
				case 3://HI-ETHER
					m_actState = ACT_STATE::SELECTION;
					m_action = ACTION::ACTION_HI_ETHER;
					highlightMap(1);
					break;
				case 4://PHONIX DOWN
					m_actState = ACT_STATE::SELECTION;
					m_action = ACTION::ACTION_PHEONIX_DOWN;
					highlightMap(1);
					break;
				default:
					break;
				}
			}
		}else IManager->set_button(DIK_9, false);
		break;
	case ACT_STATE::SELECTION:
		// use attack/skill/item 
		if(IManager->check_mouse_button(RIGHT_MOUSE_BUTTON)){
			if(!IManager->check_button_down(DIK_0)){
				IManager->set_button(DIK_0, true);
				m_areaHighlight.clear();
				m_action = 0;
				m_actState = ACT_STATE::INITIAL;
			}
		}else IManager->set_button(DIK_0,false);
		if(IManager->push_button(DIK_W)){
			if(!IManager->check_button_down(DIK_W)){
				IManager->set_button(DIK_W,true); 
				//index++;
				//if edge is north +z
				for(auto &edge:m_3Dcursor->edges)
				{
					if(edge->position.z > m_3Dcursor->position.z)
						m_3Dcursor = edge;
				}
			}
		}else IManager->set_button(DIK_W,false); 

		if(IManager->push_button(DIK_A)){
			if(!IManager->check_button_down(DIK_A)){
				IManager->set_button(DIK_A,true); 
				//if edge is west -x
				for(auto &edge:m_3Dcursor->edges)
				{
					if(edge->position.x < m_3Dcursor->position.x)
						m_3Dcursor = edge;
				}
			}
		}else IManager->set_button(DIK_A,false); 

		if(IManager->push_button(DIK_S)){
			if(!IManager->check_button_down(DIK_S)){
				IManager->set_button(DIK_S,true); 
				//if edge is south -z
				for(auto &edge:m_3Dcursor->edges)
				{
					if(edge->position.z < m_3Dcursor->position.z)
						m_3Dcursor = edge;
				}
			}
		}else IManager->set_button(DIK_S,false); 

		if(IManager->push_button(DIK_D)){
			if(!IManager->check_button_down(DIK_D)){
				IManager->set_button(DIK_D,true); 
				//if edge is east +x
				for(auto &edge:m_3Dcursor->edges)
				{
					if(edge->position.x > m_3Dcursor->position.x)
						m_3Dcursor = edge;
				}
			}
		}else IManager->set_button(DIK_D,false); 

		if(IManager->push_button(DIK_F)){
			if(!IManager->check_button_down(DIK_F)){
				IManager->set_button(DIK_F,true); 

				if(checkHighlight(m_3Dcursor) || m_3Dcursor == m_activeChar->getPosition()){
					// TODO select unit to deal dmg or heal;
					for(auto &unit: m_Units){
						if(m_3Dcursor == unit.getPosition()){
							m_charSelected = &unit;
							m_actState = ACT_STATE::END;
						}
					}
				}
			}
		}else IManager->set_button(DIK_F,false); 

		break;
	case ACT_STATE::END:

		if(m_charSelected){
			switch(m_action)
			{
			case ACTION::BASIC:
			case ACTION::ARROW:
				m_dmg = (m_charSelected->getCharacterStats().max_health/3)+1;
				m_charSelected->adjustHealth(-m_dmg);
				m_charSelected = NULL;
				break;

			case ACTION::THUNDER:
				m_dmg = (m_charSelected->getCharacterStats().max_health/2)+1;
				m_charSelected->adjustHealth(-m_dmg);
				m_charSelected = NULL;
				break;

			case ACTION::CURE:
			case ACTION::ACTION_POTION:
				m_dmg = (m_charSelected->getCharacterStats().max_health/3);
				m_charSelected->adjustHealth(m_dmg);
				m_charSelected = NULL;
				break;

			case ACTION::ACTION_HI_POTION:
				m_dmg = (m_charSelected->getCharacterStats().max_health/2);
				m_charSelected->adjustHealth(m_dmg);
				m_charSelected = NULL;
				break;

			case ACTION::ACTION_ETHER:
				m_dmg = (m_charSelected->getCharacterStats().max_mana/3);
				m_charSelected->adjustMana(m_dmg);
				m_charSelected = NULL;
				break;
			case ACTION::ACTION_HI_ETHER:
				m_dmg = (m_charSelected->getCharacterStats().max_mana/2);
				m_charSelected->adjustMana(m_dmg);
				m_charSelected = NULL;
				break;

			case ACTION::ACTION_PHEONIX_DOWN:
				m_dmg = (m_charSelected->getCharacterStats().max_health/2);
				m_charSelected->adjustHealth(m_dmg);
				m_charSelected = NULL;
				break;
			}
		}
		m_count += dt;
		if(m_count >= 2.0f){
		acted = true;
		m_dmg = 0;
		m_actState = ACT_STATE::INITIAL;
		m_charState = CHAR_STATE::START;
		m_areaHighlight.clear();
		}

		break;
	}
}

void Battle::Move(Cursor * cursor, InputManager *IManager, SoundManager *SManager, 
				  Player *player, int &game_state,  float dt,GraphicsManager3D * GManager3,
				  IDirect3DDevice9 *device)
{
	if(IManager->push_button(DIK_W)){
		if(!IManager->check_button_down(DIK_W)){
			IManager->set_button(DIK_W,true); 
			//index++;
			//if edge is north +z
			for(auto &edge:m_3Dcursor->edges)
			{
				if(edge->position.z > m_3Dcursor->position.z)
					m_3Dcursor = edge;
			}
		}
	}else IManager->set_button(DIK_W,false); 

	if(IManager->push_button(DIK_A)){
		if(!IManager->check_button_down(DIK_A)){
			IManager->set_button(DIK_A,true); 
			//if edge is west -x
			for(auto &edge:m_3Dcursor->edges)
			{
				if(edge->position.x < m_3Dcursor->position.x)
					m_3Dcursor = edge;
			}
		}
	}else IManager->set_button(DIK_A,false); 

	if(IManager->push_button(DIK_S)){
		if(!IManager->check_button_down(DIK_S)){
			IManager->set_button(DIK_S,true); 
			//if edge is south -z
			for(auto &edge:m_3Dcursor->edges)
			{
				if(edge->position.z < m_3Dcursor->position.z)
					m_3Dcursor = edge;
			}
		}
	}else IManager->set_button(DIK_S,false); 

	if(IManager->push_button(DIK_D)){
		if(!IManager->check_button_down(DIK_D)){
			IManager->set_button(DIK_D,true); 
			//if edge is east +x
			for(auto &edge:m_3Dcursor->edges)
			{
				if(edge->position.x > m_3Dcursor->position.x)
					m_3Dcursor = edge;
			}
		}
	}else IManager->set_button(DIK_D,false); 

	if(IManager->push_button(DIK_F)){
		if(!IManager->check_button_down(DIK_F)){
			IManager->set_button(DIK_F,true); 
			if(checkHighlight(m_3Dcursor)){
				m_activeChar->setPosition(*m_3Dcursor);
				moved = true;
				m_charState = START;
				m_areaHighlight.clear();
			}
		}
	}else IManager->set_button(DIK_F,false); 
}
void Battle::Wait(Cursor * cursor, InputManager *IManager, SoundManager *SManager, 
				  Player *player, int &game_state,  float dt,GraphicsManager3D * GManager3,
				  IDirect3DDevice9 *device)
{
	m_3Dcursor = m_activeChar->getPosition();
	if(IManager->check_mouse_button(RIGHT_MOUSE_BUTTON)){
		if(!IManager->check_button_down(DIK_0)){
			IManager->set_button(DIK_0, true);
			m_charState = CHAR_STATE::START;
		}
	}else IManager->set_button(DIK_0,false);

	// Turn your character in some direction
	if(IManager->push_button(DIK_W)){
		if(!IManager->check_button_down(DIK_W)){
			IManager->set_button(DIK_W,true); 
			//if edge is north +z
			m_activeChar->setRotation(D3DXVECTOR3(0.0f,180.0f,0.0f));
		}
	}else IManager->set_button(DIK_W,false); 

	if(IManager->push_button(DIK_A)){
		if(!IManager->check_button_down(DIK_A)){
			IManager->set_button(DIK_A,true); 
			//if edge is west -x
			m_activeChar->setRotation(D3DXVECTOR3(0.0f,90.0f,0.0f));
		}
	}else IManager->set_button(DIK_A,false); 

	if(IManager->push_button(DIK_S)){
		if(!IManager->check_button_down(DIK_S)){
			IManager->set_button(DIK_S,true); 
			//if edge is south -z
			m_activeChar->setRotation(D3DXVECTOR3(0.0f,0.0f,0.0f));
		}
	}else IManager->set_button(DIK_S,false); 

	if(IManager->push_button(DIK_D)){
		if(!IManager->check_button_down(DIK_D)){
			IManager->set_button(DIK_D,true); 
			//if edge is east +x
			m_activeChar->setRotation(D3DXVECTOR3(0.0f,270.0f,0.0f));
		}
	}else IManager->set_button(DIK_D,false); 

	if(IManager->push_button(DIK_F)){
		if(!IManager->check_button_down(DIK_F)){
			IManager->set_button(DIK_F,true); 
			m_charState = START;
			moved = false;
			acted = false;
			m_turnIndex++;
			if(m_turnIndex >= m_Units.size())
				m_turnIndex = 0;
			m_3Dcursor = m_Units[m_turnIndex].getPosition();
		}

	}else IManager->set_button(DIK_F,false); 
	// then switch active character

}
void Battle::highlightMap(int dist)
{
	bool clear = true;
	m_areaHighlight.clear();
	m_activeChar->getPosition()->visited = true;
	dist -=1;
	Battle_Node*temp = m_activeChar->getPosition();
	std::queue<Battle_Node*> Q;
	Q.push(temp);

	Battle_Node*next = temp;
	while(!Q.empty()){
		clear = true;
		temp = Q.front();
		Q.pop();
		for( auto &edge: temp->edges){
			if(!edge->visited){
				for(auto & unit : m_Units )
					if(edge == unit.getPosition())
						clear = false;
				if(clear || m_charState == CHAR_STATE::ACT)
					m_areaHighlight.push_back(edge);
				else
					clear = true;
				if(dist>0)
					Q.push(edge);
				edge->visited = true;
			}
		}
		if(temp == next){
			dist--;
			if(!Q.empty())
				next = Q.back();
		}
	}
	for(auto &node:m_Map.m_graph)
		node.visited = false;
}

bool Battle::checkHighlight(Battle_Node* a_node){
	for(auto &node:m_areaHighlight)
		if ( a_node == node)
			return true;
	return false;
};

