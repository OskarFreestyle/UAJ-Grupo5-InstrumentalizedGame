#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Component.h"

#include "../utils/checkML.h"

#include "../game/constant_variables.h"

#include "../components/transform.h"

#include "../classes/item.h"
#include "../classes/inventory_dropdown.h"

#include <list>
#include <string>
#include <functional>

class TextWithBackground;
class Game;
class WeaponBehaviour;
class DropDownRender;
class Inventory : public Component
{
	friend class Item;
public:
	static bool firstInitialization;
	Inventory(int width, int height, WeaponBehaviour* playerWeapon);
	Inventory(int width, int height);
	Inventory(int width, int height, Inventory* player);
	~Inventory();

	void init() override;
	void update() override;
	void render() override;

	bool forceDelete = false;

	inline std::list<Item*>& getItems() { return storedItems; };

	void storeItem(Item* item);
	void moveItem(Item* item, int x, int y);
	void removeItem(Item* item);
	void storeDefaultItems();

	static void setItemDimensions(Transform* transform, int width, int height);
	void adjustPanelSize();

	inline void setOther(Inventory* o) { other = o; };
	void defaultPosition();

	void moveInventory(Point2D pos);

	void onEnable() override;
	void onDisable() override;

	bool isPlayer = false;

private:
	Point2D originalPos;
	const float timeToHold = 0.08f; //seconds
	float timer = 0;

	Inventory* other;
	Player* player;
	WeaponBehaviour* playerWeapon;

	Vector2D itemPosition(int x, int y);
	Vector2D itemPosition(int x, int y, Transform* transform);

	static int itemWidth, itemHeight;
	int width, height;
	Transform* transform;

	std::list<Item*> storedItems;

	std::vector<std::vector<Item*>> grid;

	Item* findItemInSlot(int x, int y);

	bool availableSpace(int x, int y, Item* item);
	bool isAvailableInTheScene(int itemName);
	bool isUsable(int itemName);

	bool justPressed;
	Item* selectedItem;
	Item* selectedItem_;
	Item* itemClickedInDropdown;
	Item* lastItemHovered;

	bool insideSquare(int mouseX, int mouseY, Transform* rect);
	bool insideSquare(int mouseX, int mouseY);
	bool insideSquare(int mouseX, int mouseY, Vector2D pos, Vector2D dimensions);

	bool dropDownActive;

	InventoryDropdown* dropDown;
	DropDownRender* dropDownRender;
	Entity* toolTips;
	Transform* toolTipsTr;
	TextWithBackground* toolTipsText;
	bool showToolTip;
};

