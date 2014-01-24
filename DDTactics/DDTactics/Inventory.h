#include <vector>

#define MAX_ITEMS 100;

enum ITEMTYPE
{
	HEAD,
	CHEST,
	WEAPON,
	POTIONS,
	ETHER,
};

class inventory
{
public:
	struct Items
	{
		int buyPrice, sellPrice;
		int type;
		int armor;
		int damage;
		int addHealth;
		int addMana;
	};

	std::vector<Items> myItems[100]; //should be MAX_ITEMS but getting error

	inventory(void);
	~inventory(void);

	inventory openInventory();
	
	void add(Items);
	void use(Items);
};
