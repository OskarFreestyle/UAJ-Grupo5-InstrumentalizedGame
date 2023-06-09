#pragma once

#include "../ecs/Entity.h"
#include "../ecs/Manager.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

#include "../utils/Collisions.h"
#include "../utils/Vector2D.h"
#include "../utils/checkML.h"

#include "../components/image.h"
#include "../components/transform.h"
#include "../components/inventory_controller.h"

#include "../classes/crafting_system.h"

#include <vector>

struct Slot {
	int index;
	Entity* slot;
};

class ShelterScene;

class Workshop : public Entity
{
protected:
	Entity* bg;
	Transform* bg_tr;
	Entity* bButton;
	Transform* bButton_tr;
	Entity* arrowUp;
	Transform* arrowUp_tr;
	Entity* arrowDown;
	Transform* arrowDown_tr;
	Entity* craftButton;
	Transform* craftButton_tr;

	Entity* arrowLeft;
	Transform* arrowLeft_tr;
	Entity* arrowRight;
	Transform* arrowRight_tr;

	WeaponBehaviour* weapon;
	Transform* weaponTr;
	Image* weaponImg;

	Manager* falseMngr;

	vector<Texture*> leftRenderTexts;
	vector<Entity*> leftRenderImgs;

	vector<Texture*> rightRenderTexts;
	vector<Entity*> rightRenderImgs;

	std::vector<Slot>craftList;
	std::vector<Transform*>craftList_tr;

	int listIndex;
	bool renderFlag;

	bool renderRightWindow;
	int	 rightWindowIndex;

	bool mouseClick;
	CraftingSystem* craftSys;
	std::vector<ITEMS> workshopItems;
	Inventory* playerInv;
	Transform* playerTr;

	Loot* loot;
	ShelterScene* shelterScene;

public:
	Workshop(Manager* mngr_) : Entity(mngr_) { };
	Workshop(Manager* realMngr_, Manager* mngr_, CraftingSystem* cs, ShelterScene* shelterScene_);
	~Workshop() {
		for (int i = 0; i < leftRenderTexts.size(); ++i) {
			delete  leftRenderTexts[i];
		}
		leftRenderTexts.clear();

		for (int i = 0; i < rightRenderTexts.size(); ++i) {
			delete  rightRenderTexts[i];
		}
		rightRenderTexts.clear();

		//delete falseMngr;
	}

	void closeCraft();

	virtual void setLeftRender();
	virtual void setRightRender();

	void renderWeaponUpgrade(float offsetX, float offsetY);
	virtual void setWorkshopItems(vector<ITEMS>&& items);
	void setImg(Entity* entity, Vector2D pos, Vector2D size, std::string name);
	void renderImg(float posX, float posY, int row, int col, int sizeX = 64, int sizeY = 64);
	virtual void render();
	virtual void rightWindowRender();
	virtual void update();
	void setRenderFlag(bool set);

	void setLoot(Loot* l) { loot = l; }
};