#include <vector>

class inventory
{
public:
	std::vector<inventory> items;

	inventory(void);
	~inventory(void);

	inventory openInventory();

};