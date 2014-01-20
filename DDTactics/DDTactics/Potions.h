#include <vector>

class potions
{
public:
	int healthpoints;
	int manapoints;
	
	potions(void);
	~potions(void);

	int useHealthPotion();
	int useManaPotion();
};