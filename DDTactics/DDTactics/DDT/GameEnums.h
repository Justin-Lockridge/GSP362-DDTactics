#pragma once

enum GAME_STATE
{
	MENU, LOAD, OPTIONS, CREDITS, BATTLE, 
	ACTUAL_BATTLE, MAP, TAVERN, OVERWORLD, 
	QUIT, TOWN, STATUS, SAVE, INTRO,
	WIN,LOSS
};

enum GRAPHICS_SELECTION
{
	GRAPHICS_MENU, 
	GRAPHICS_MENU_BUTTONS, 
	GRAPHICS_CURSOR,
	GRAPHICS_OVERWORLD,
	GRAPHICS_OVERWORLD_CHARACTER,
	GRAPHICS_OVERWORLD_MENU_BACKGROUND,
	GRAPHICS_OVERWORLD_MENU_TEXT,
	GRAPHICS_OVERWORLD_TOWN,
	GRAPHICS_OVERWORLD_STORY,
	GRAPHICS_OVERWORLD_BATTLE,
	GRAPHICS_STATUS_BACKGROUND,
	GRAPHICS_STATUS_BORDER,
	GRAPHICS_STATUS_BUTTONS,
	GRAPHICS_BATTLE_SPLASH,
	GRAPHICS_SHOP_KEEP,
	GRAPHICS_SHOP_WEAPONS,
	GRAPHICS_SHOP_HELMS,
	GRAPHICS_SHOP_CHESTS,
	GRAPHICS_SHOP_ACCESORY,
	GRAPHICS_SHOP_POTIONS,
	GRAPHICS_SAVE_BACKGROUND,
	GRAPHICS_SAVE_BUTTONS,
	GRAPHICS_INTRO,
	GRAPHICS_BUTTON,

	GRAPHICS_BSTATS_WINDOW,
	GRAPHICS_BLANK_WINDOW,
	GRAPHICS_BMENU,
	GRAPHICS_POINTER,

	GRAPHICS_ACT_WINDOW,
	GRAPHICS_MAGE_SKILLS,
	GRAPHICS_ACT_ITEMS,

	GRAPHICS_WAIT_WINDOW,

	GRAPHICS_ARCHER_ICON,
	GRAPHICS_MAGE_ICON,
	GRAPHICS_WAR_ICON,
	
	GRAPHICS_MONGOOSE,
	
	GRAPHICS_ITEM_BACKGROUND,
	GRAPHICS_ITEM_BUTTONS,
	GRAPHICS_ITEM_ARROW,

	GRAPHICS_MENU_WAR_FULL,
	GRAPHICS_MENU_ARCHER_FULL,
	GRAPHICS_MENU_MAGE_FULL,

	GRAPHICS_MENU_JOB_SELECTION,


};

enum CHAR_MODEL_SELECTION
{
	MODEL_DEFAULT,
	MODEL_3DCURSOR,
	MODEL_SWORD,
	MODEL_STAFF,
	//MODEL_BOW,
};

enum MAP_SELECTION
{
	MAP_DEFAULT,
	MAP_SKYBOX,
};

enum SONG_SELECTION
{
	SONG_MENU,
	SONG_OVERWORLD,
	SONG_TOWN,
	SONG_BATTLE,
	SONG_INTRO,
	SOUND_MONGOOSE,
	
};

enum SOUND_EFFECT_SELECTION
{
	//Delete placeholder if you have created a new sound effect and replace with your own enum
	SOUND_EFFECT_PLACEHOLDER,
};

enum OVERWORLD_STATES
{
	OVERWORLD_SELECTION,
	OVERWORLD_TRANSITION,
	OVERWORLD_TOWN,
	OVERWORLD_MENU,
	OVERWORLD_OPTION,
	OVERWORLD_BATTLE,
	OVERWORLD_SAVE,
	OVERWORLD_LOAD,
	OVERWORLD_CHECK,

};

enum TOWN_STATES
{
	TOWN_SELECT,
	TOWN_SHOP,
	TOWN_RECRUIT,
	TOWN_EXIT
};

enum SHOP_STATES
{
	SHOP_SELECTION,
	SHOP_WEAPONS,
	SHOP_HEAD,
	SHOP_CHEST,
	SHOP_ACCESORY,
	SHOP_POTIONS,
};

enum NODE_TYPES
{
	TYPE_TOWN,
	TYPE_STORY,
	TYPE_RANDOM_BATTLE
};

//enum ITEMTYPE
//{
//	HEAD,
//	CHEST,
//	WEAPON,
//	POTION,
//	HIPOTION,
//	ETHER,
//	HIETHER,
//	PHOENIXDOWN,
//};

enum JOB
{
	WARRIOR,
	ARCHER,
	GREYMAGE
};

enum STATUSMENU
{
	STATUS_MAIN,
	STATUS_ITEM,
	STATUS_ABILITY,
	STATUS_MAGIC,
	STATUS_JOB,
	STATUS_STATUS,
	STATUS_ORDER,
	STATUS_BACK,
	STATUS_ITEM_USE,
	STATUS_ITEM_ORGANIZE,
	STATUS_ABILITY_USE,
	STATUS_MAGIC_USE,




};

enum STATUSMENUITEM
{
	STATUSMENUITEM_USE,
	STATUSMENUITEM_ORGANIZE,
	STATUSMENUITEM_BACK,
};

enum ITEM_NAMES
{
	DAGGER,
	BROADSWORD,
	ROD,
	OAKSTAFF,
	LEATHERHAT,
	CLOTHARMOR,
	PHOENIXDOWN,
	ETHER,
	HIETHER,
	POTION,
	HIPOTION,

};