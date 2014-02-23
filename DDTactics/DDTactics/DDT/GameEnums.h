#pragma once

enum GAME_STATE
{
	MENU, LOAD, OPTIONS, CREDITS, BATTLE, 
	ACTUAL_BATTLE, MAP, TAVERN, OVERWORLD, 
	QUIT, TOWN, STATUS, SAVE, INTRO,
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
//	GRAPHICS_SHOP_MENU,
//	GRAPHICS_SHOP_WEAPON_BUTTON,
//	GRAPHICS_SHOP_HELM_BUTTON,				// again removed buttons cause i think its better without em
//	GRAPHICS_SHOP_CHEST_BUTTON,
//	GRAPHICS_SHOP_ACCESORY_BUTTON,
//	GRAPHICS_SHOP_POTION_BUTTON,
	GRAPHICS_SAVE_BACKGROUND,
	GRAPHICS_SAVE_BUTTONS,
	GRAPHICS_INTRO,
	GRAPHICS_BUTTON,

	GRAPHICS_BSTATS_WINDOW,
	GRAPHICS_BLANK_WINDOW,
	GRAPHICS_BMENU,
	GRAPHICS_POINTER,
};

enum CHAR_MODEL_SELECTION
{
	MODEL_DEFAULT,
	MODEL_3DCURSOR,
};

enum MAP_SELECTION
{
	MAP_DEFAULT,
};

enum SONG_SELECTION
{
	SONG_MENU,
	SONG_OVERWORLD,
	SONG_TOWN,
	SONG_BATTLE,
	SONG_INTRO,
	
};

enum SOUND_EFFECT_SELECTION
{
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

enum ITEMTYPE
{
	HEAD,
	CHEST,
	WEAPON,
	POTION,
	HIPOTION,
	ETHER,
	HIETHER,
	PHOENIXDOWN,
};

enum JOB
{
	WARRIOR,
	ARCHER,
	GREYMAGE
};