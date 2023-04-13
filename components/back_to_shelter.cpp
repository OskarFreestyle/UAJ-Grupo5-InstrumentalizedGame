#include "back_to_shelter.h"
#include "..//sdlutils/InputHandler.h"
#include "../classes/game_scene.h"
#include "../classes/shelter_scene.h"
#include "../classes/player.h"
#include "../classes/physiognomy.h"
#include "../classes/crafting_system.h"
#include "Tracker.h"

BackToShelter::BackToShelter(GameScene* scene) : InteractableElement("Go back to SHELTER"), scene_(scene) {}

void BackToShelter::init() {
	InteractableElement::init();
	player_ = entity_->getMngr()->getHandler<Player_hdlr>();
}

void BackToShelter::Interact() {
	entity_->getMngr()->getGame()->setShouldRenderFPS(true);
	Player* p = static_cast<Player*>(player_);
	p->getPhysiognomy()->removeAllStates();

	Manager* mngr = p->getMngr();
	CraftingSystem* craftingSystem = mngr->getHandler<Player_hdlr>()->getComponent<InventoryController>()->getCraftingSystem();

	int spaceship_crafteable_count = 0;

	if (craftingSystem->isItemCrafteable(SPACESHIP_CABIN) && !mngr->getGame()->cabin) spaceship_crafteable_count++;
	if (craftingSystem->isItemCrafteable(SPACESHIP_RADAR) && !mngr->getGame()->radar) spaceship_crafteable_count++;
	if (craftingSystem->isItemCrafteable(SPACESHIP_ROCKETS) && !mngr->getGame()->rockets) spaceship_crafteable_count++;

	int crafted_count = 0;

	if (mngr->getGame()->rockets) crafted_count++;
	if (mngr->getGame()->cabin) crafted_count++;
	if (mngr->getGame()->radar) crafted_count++;

	// LEAVE_BASE_EVENT
	int currentDay = mngr->getGame()->numDays;
	auto e = Tracker::Instance()->createShipItemCraftedEvent();

	e->setCraftables(spaceship_crafteable_count)->setCrafted(crafted_count)->setDay(currentDay);
	Tracker::Instance()->trackEvent(e);

	entity_->getMngr()->ChangeScene(new ShelterScene(scene_->getGame()), SceneManager::SceneMode::OVERRIDE);

	//Evento
	
}

void BackToShelter::update() {
	//if (ih().isKeyDown(SDLK_e) && CheckCollision(player_->getComponent<Transform>())) Interact();
}