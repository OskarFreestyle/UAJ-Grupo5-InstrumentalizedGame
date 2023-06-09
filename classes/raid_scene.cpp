#include "raid_scene.h"
#include "../game/Game.h"
#include "hud.h"
#include "../classes/pause_scene.h"
#include "../classes/camera.h"
#include "../sdlutils/InputHandler.h"
#include "../components/transition_component.h";
#include "../components/text_with_background.h"
#include "../components/loot.h"
#include "../classes/physiognomy.h"
#include "../classes/countdown.h"
#include "../components/parallax_component.h"
#include "../components/initial_camera_zoom.h"
#include "../components/camera_movement.h"
#include "../components/hunger_component.h"
#include "Tracker.h"

void RaidScene::init() {
	loadMap(path_);

	mngr_->getHandler<Player_hdlr>()->setActive(true);
	timer = new Countdown(240);

	player = static_cast<Player*>(mngr_->getHandler<Player_hdlr>());
	raidTimeEnded = false;

	auto weapon = player->getWeapon();

	if (!weapon->isActive())
		weapon->setActive(true);

	hud* h = new hud(mngr_, new Transform(Vector2D(100, 100), 64, 64, 0), player, timer);

	createParallaxBackground(5);

	auto cameraZoom = mngr_->addEntity();
	cameraZoom->addComponent<InitialCameraZoom>(1.2, 2, [this]() {
		mngr_->getHandler<Player_hdlr>()->getComponent<CameraMovement>()->enabled = true;
		});

	createTransition();

	int currentDay = mngr_->getGame()->numDays;
	auto e = Tracker::Instance()->createRaidSelectedEvent();

	int hungerLevel = 0;

	std::list<int> itemsIndexInEnum;

	if (currentDay > 1) {
		// RAID SELECT EVENT
		HungerComponent* hunger = mngr_->getHandler<Player_hdlr>()->getComponent<HungerComponent>();
		hungerLevel = (int)hunger->getHungerLevel();
		Inventory* playerInventory = mngr_->getHandler<Player_hdlr>()->getComponent<InventoryController>()->inventory;
		std::list<Item*>& items = playerInventory->getItems();

		for (auto it = items.begin(); it != items.end(); it++) {
			itemsIndexInEnum.push_back((int)(*it)->getItemInfo()->name());
		}
	}

	e->setDay(currentDay)->setHunger(hungerLevel)->setItems(itemsIndexInEnum)->setLocation((int)mngr_->getGame()->currentScene);
	Tracker::Instance()->trackEvent(e);
}

void RaidScene::update() {
	GameScene::update();


	if (!raidTimeEnded) {
		if (!timer->keepPlaying()) {
			player->getPhysiognomy()->addHypothermiaState();
			raidTimeEnded = true;
		}
	}
	
	if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_ESCAPE)) {
		mngr_->getGame()->setShouldRenderFPS(false);
		mngr_->ChangeScene(new PauseScene(mngr_->getGame()), SceneManager::SceneMode::ADDITIVE);
	}
}

void RaidScene::createParallaxBackground(int numOfRep) {
	createParallaxLayer(0, &sdlutils().images().at("sky"), numOfRep);
	createParallaxLayer(0.2, &sdlutils().images().at("houses4"), numOfRep);
	createParallaxLayer(0.3, &sdlutils().images().at("houses3"), numOfRep);
	createParallaxLayer(0.4, &sdlutils().images().at("houses2"), numOfRep);
	createParallaxLayer(0.5, &sdlutils().images().at("houses1"), numOfRep);
	createParallaxLayer(0.6, &sdlutils().images().at("wall"), numOfRep);
	createParallaxLayer(0.7, &sdlutils().images().at("road"), numOfRep);
}

RaidScene::RaidScene(string path, string name, Game* game): GameScene(game, name)
{
	path_ = path;
}
