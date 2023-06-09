#include "spaceship_station.h"
#include "../game/Game.h"
#include "../classes/shelter_scene.h"
#include "../classes/lose_scene.h"
#include "../sdlutils/SoundManager.h"

SpaceshipStation::SpaceshipStation(Manager* realMngr_, Manager* mngr_, CraftingSystem* cs, ShelterScene* shelterScene_) : Workshop(mngr_) {
	//EL MANAGER FALSO ES PARA PODER RENDERIZAR ENTIDADES POR SEPARADO SIN QUE SE HAGA DE FORMA AUTOMATICA
	craftSys = cs;
	realMngr_->addEntity(this);
	realMngr_->addRenderLayer<Interface>(this);

	renderFlag = false;
	listIndex = 0;
	mouseClick = false;
	loot = nullptr;

	falseMngr = mngr_;

	playerInv = realMngr_->getHandler<Player_hdlr>()->getComponent<InventoryController>()->inventory;
	playerTr = realMngr_->getHandler<Player_hdlr>()->getComponent<Transform>();

	renderRightWindow = false;
	rightWindowIndex = 0;


	//INICIALIZACION IMAGENES DEL FONDO, FLECHAS Y X PARA SALIR
	bg = mngr_->addEntity();
	bButton = mngr_->addEntity();

	craftButton = mngr_->addEntity();

	Vector2D bg_size = { 930, 630 };
	Vector2D bg_pos = Vector2D(sdlutils().width() / 2 - bg_size.getX() / 2, sdlutils().height() / 2 - bg_size.getY() / 2);
	Vector2D bButton_size = { 65,64 };
	Vector2D bButton_pos = Vector2D(bg_pos.getX() - bButton_size.getX() / 2, bg_pos.getY() - bButton_size.getY() / 2);

	setImg(bg, bg_pos, bg_size, "craft_bg");
	setImg(bButton, bButton_pos, bButton_size, "craft_back_button");
	bg_tr = bg->getComponent<Transform>();
	bButton_tr = bButton->getComponent<Transform>();

	Vector2D craftButton_pos = { bg_tr->getPos().getX() + bg_tr->getW() * (3.0f / 4.0f) - 132.5f, bg_tr->getPos().getY() + bg_tr->getH() - 105.0f / 1.5f };

	//BOTON DE CRAFTEO
	setImg(craftButton, craftButton_pos, Vector2D{ 265,80 }, "actionButton");
	craftButton_tr = craftButton->getComponent<Transform>();

	setWorkshopItems({ SPACESHIP_CABIN,SPACESHIP_RADAR,SPACESHIP_ROCKETS });

	rocketTop = mngr_->addEntity();
	rocketTop->addComponent<Transform>(Vector2D{ bg_pos.getX() + bg_size.getX() * 0.075f,35 }, 320, 213);
	rocketTopImg = rocketTop->addComponent<Image>(&sdlutils().images().at("rocket"), 3, 1, 0, 0, true);
	if (!realMngr_->getGame()->radar)rocketTopImg->setAlpha(125);
	rocketMid = mngr_->addEntity();
	rocketMid->addComponent<Transform>(Vector2D{ bg_pos.getX() + bg_size.getX() * 0.075f,248 }, 320, 213);
	rocketMidImg = rocketMid->addComponent<Image>(&sdlutils().images().at("rocket"), 3, 1, 1, 0, true);
	if (!realMngr_->getGame()->cabin) rocketMidImg->setAlpha(125);
	rocketBot = mngr_->addEntity();
	rocketBot->addComponent<Transform>(Vector2D{ bg_pos.getX() + bg_size.getX() * 0.075f,461 }, 320, 213);
	rocketBotImg = rocketBot->addComponent<Image>(&sdlutils().images().at("rocket"), 3, 1, 2, 0, true);
	if (!realMngr_->getGame()->rockets)rocketBotImg->setAlpha(125);

	falseMngr->addRenderLayer<Interface>(rocketTop);
	falseMngr->addRenderLayer<Interface>(rocketMid);
	falseMngr->addRenderLayer<Interface>(rocketBot);

	//referencia a ShelterScene para usar acciones
	shelterScene = shelterScene_;
}

//INICIALIZACION SLOTS DE LA LISTA 
void SpaceshipStation::setWorkshopItems(vector<ITEMS>&& items) {
	workshopItems = move(items); //vector que contiene los items de la nave

	//Para cada item de la nave
	for (int i = 0; i < workshopItems.size() && i < 4; ++i) {
		auto itemInfo = craftSys->getItemInfo(workshopItems[i]);
		int imgRow = itemInfo->row();
		int imgCol = itemInfo->col();
		std::string itemName = itemInfo->strName();

		//Creamos un slot con su respectiva imagen y lo a�adimos a lalista de items
		Slot aux = { 0,getMngr()->addEntity() };
		aux.slot->addComponent<Transform>(Vector2D(), 64, 64, 0);
		aux.slot->addComponent<Image>(&sdlutils().images().at("items"), 8, 3, imgRow, imgCol, true);
		craftList.push_back(aux);
		falseMngr->addRenderLayer<Interface>(aux.slot);

		delete itemInfo;
	}

	//Guardamos los transforms de los slots en un vector
	float offsetX = bg_tr->getPos().getX() + bg_tr->getW() * 0.25f - 32;
	float offsetY = bg_tr->getPos().getY() + bg_tr->getH() * 0.2f - 32;
	for (int i = 0; i < workshopItems.size() && i < 4; ++i) {
		craftList[i].slot->getComponent<Transform>()->setPos(Vector2D{ offsetX,offsetY });
		craftList_tr.push_back(craftList[i].slot->getComponent<Transform>());
		offsetY += 180;
	}
}

//Preparacion de imagenes y textos de la zona derecha
void SpaceshipStation::setRightRender() {

	//limpio los vectores de texturas e imagenes
	for (int i = 0; i < rightRenderTexts.size(); ++i) {
		delete  rightRenderTexts[i];
	}
	rightRenderTexts.clear();

	for (int i = 0; i < rightRenderImgs.size(); ++i) {
		rightRenderImgs[i]->setDead(true);
		rightRenderImgs[i]->setActive(false);
	}
	falseMngr->refresh();
	rightRenderImgs.clear();

	//Guardo el nombre del item y otro texto
	auto rightWindowItemInfo = craftSys->getItemInfo(workshopItems[rightWindowIndex]);
	std::string itemName = rightWindowItemInfo->strName();
	rightRenderTexts.push_back(new Texture(sdlutils().renderer(), itemName, sdlutils().fonts().at("Orbitron32"), build_sdlcolor(0xffffffff)));
	rightRenderTexts.push_back(new Texture(sdlutils().renderer(), "Needed items: ", sdlutils().fonts().at("Orbitron32"), build_sdlcolor(0xffffffff)));

	float offsetX = bg_tr->getPos().getX() + bg_tr->getW() * (3.0f / 4.0f);
	float offsetY = rightRenderTexts[0]->height() + 25;
	int imgRow = rightWindowItemInfo->row();
	int imgCol = rightWindowItemInfo->col();
	delete rightWindowItemInfo;

	//Guardo imagen de item seleccionado
	Entity* selectedItemImg = falseMngr->addEntity();
	selectedItemImg->addComponent<Transform>(Vector2D{ offsetX - 32,offsetY + 80 }, 64, 64, 0);
	Component* img = selectedItemImg->addComponent<Image>(&sdlutils().images().at("items"), 8, 3, imgRow, imgCol, true);
	rightRenderImgs.push_back(selectedItemImg);

	offsetY = 312;
	offsetX = bg_tr->getPos().getX() + bg_tr->getW() / 2 + 30;


	//Para cada item que se necesita para craftear el item elegido
	vector<ItemInfo*> itemsNeeded = craftSys->getCrafts()->find(workshopItems[rightWindowIndex])->second;
	for (int i = 0; i < itemsNeeded.size(); ++i) {
		int quantityOfItem = 0;

		//Cuento cuanta cantidad tengo en el inventario del item a buscar
		list<Item*> items = playerInv->getItems();
		for (auto it = items.begin(); it != items.end(); ++it)
		{
			if ((*it)->getItemInfo()->name() == itemsNeeded[i]->name())
				quantityOfItem++;
		}

		//Cargo el texto  "itemsquetengo/itemsnecesarios"
		rightRenderTexts.push_back(new Texture(sdlutils().renderer(), to_string(quantityOfItem) + "/" + to_string(itemsNeeded[i]->getAmount()), sdlutils().fonts().at("OrbitronRegular"), build_sdlcolor(0xffffffff)));

		//Cargo el nombre del item que necesito
		rightRenderTexts.push_back(new Texture(sdlutils().renderer(), itemsNeeded[i]->strName(), sdlutils().fonts().at("OrbitronRegular"), build_sdlcolor(0xffffffff)));

		//Cargo la imagen del item necesario
		Entity* necessaryItemImage = falseMngr->addEntity();
		necessaryItemImage->addComponent<Transform>(Vector2D{ offsetX,offsetY }, 48, 48, 0);
		Component* img = necessaryItemImage->addComponent<Image>(&sdlutils().images().at("items"), 8, 3, itemsNeeded[i]->row(), itemsNeeded[i]->col(), true);
		rightRenderImgs.push_back(necessaryItemImage);

		offsetY += 48 + 20;
	}
	//Cargo el texto para el bot�n de craftear
	rightRenderTexts.push_back(new Texture(sdlutils().renderer(), "CRAFT", sdlutils().fonts().at("OrbitronRegular"), build_sdlcolor(0xffffffff)));
}

void SpaceshipStation::update() {
	falseMngr->refresh();

	if (renderFlag) {
		Vector2D mousePos(ih().getMousePos().first, ih().getMousePos().second);

		//Cerramos el menu si pulsamos la E
		if (ih().isKeyDown(SDL_SCANCODE_E)) {
			renderFlag = false;
			renderRightWindow = false;
			playerTr->getEntity()->setActive(true);
			ih().clearState();
		}

		//Al hacer click
		if (ih().getMouseButtonState(InputHandler::LEFT) && !mouseClick) {
			mouseClick = true;

			//En el bot�n de regreso cerramos el menu
			if (Collisions::collides(mousePos, 1, 1, bButton_tr->getPos(), bButton_tr->getW(), bButton_tr->getH())) {
				renderFlag = false;
				playerTr->getEntity()->setActive(true);
				renderRightWindow = false;
			}

			//Si haces click en alguno de los slots se abre el men� derecho
			for (int i = 0; i < workshopItems.size() && i < 4; ++i) {
				if ((workshopItems[i] == SPACESHIP_CABIN && !mngr_->getGame()->cabin) ||
					(workshopItems[i] == SPACESHIP_ROCKETS && !mngr_->getGame()->rockets)
					|| (workshopItems[i] == SPACESHIP_RADAR && !mngr_->getGame()->radar))
					if (Collisions::collides(mousePos, 1, 1, craftList_tr[i]->getPos(), craftList_tr[i]->getW(), craftList_tr[i]->getH())) {
						renderRightWindow = true;
						rightWindowIndex = craftList[i].index; 	//Right window index es para guardar el indice del item clickado y asi renderizarlo a la derecha
						setRightRender(); // Cargamos imagenes y textos de la zona derecha
					}
			}

			//Si la zona derecha est� abierta
			if (renderRightWindow) {
				//Si hacemos click en craftear
				if (Collisions::collides(mousePos, 1, 1, craftButton_tr->getPos(), craftButton_tr->getW(), craftButton_tr->getH())) {
					//si hay acciones
					if (shelterScene->getActions() > 0) {
						//Comprobamos si es crafteable
						bool isCraftable = craftSys->CraftItem(workshopItems[rightWindowIndex], craftButton_tr->getPos().getX() * 3 / 2, consts::WINDOW_HEIGHT / 3, this, false);
						if (isCraftable) {

							//Comprobamos cual de las 3 parte de la nave es, ponemos su correspondiente booleano a true e iluminamos esa zona de la nave
							if (workshopItems[rightWindowIndex] == SPACESHIP_CABIN) {
								mngr_->getGame()->cabin = true;
								rocketTopImg->setAlpha(255);
							}
							else if (workshopItems[rightWindowIndex] == SPACESHIP_RADAR) {
								mngr_->getGame()->radar = true;
								rocketMidImg->setAlpha(255);
							}
							else if (workshopItems[rightWindowIndex] == SPACESHIP_ROCKETS) {
								mngr_->getGame()->rockets = true;
								rocketBotImg->setAlpha(255);
							}

							//gastar accion
							shelterScene->useActions(1);

							renderFlag = true;
							renderRightWindow = true;

							//Cargamos de nuevo las imagenes de la izquierda y derecha
							setLeftRender();
							setRightRender();

							//Si ya esta construida la nave lo marcamos con el booleano endGame
							if (mngr_->getGame()->rockets && mngr_->getGame()->radar && mngr_->getGame()->cabin)
								endGame = true;
						}
					}
					else soundManager().playSFX("error");
				}
			}
		}
		else if (!ih().getMouseButtonState(InputHandler::LEFT)) { mouseClick = false; }
	}
}

void SpaceshipStation::render() {
	falseMngr->refresh();


	if (renderFlag) {

		//render del fondo, boton de volver y las 3 piezas de la nave
		bg->render();
		bButton->render();
		rocketTop->render();
		rocketMid->render();
		rocketBot->render();

		//Para cada item de la lista
		for (int i = 0; i < workshopItems.size() && i < 4; ++i) {
			//Comprobamos si el booleano correspondiente esta activado para saber si renderizarlo o no
			if ((workshopItems[i] == SPACESHIP_CABIN && !mngr_->getGame()->cabin) ||
				(workshopItems[i] == SPACESHIP_ROCKETS && !mngr_->getGame()->rockets)
				|| (workshopItems[i] == SPACESHIP_RADAR && !mngr_->getGame()->radar)) {

				//Renderizamos la imagen y establecemos su indice
				craftList[i].index = listIndex + i;
				craftList[i].slot->render();

				rightWindowRender();
			}
		}
	}

}

void SpaceshipStation::rightWindowRender() {
	if (renderRightWindow) {

		float offsetX = bg_tr->getPos().getX() + bg_tr->getW() * (3.0f / 4.0f);
		float offsetY = bg_tr->getPos().getY() + 35;

		auto rightItemInfo = craftSys->getItemInfo(workshopItems[rightWindowIndex]);
		std::string itemName = rightItemInfo->strName();

		SDL_Rect dest{ offsetX - rightRenderTexts[0]->width() / 2  , offsetY ,rightRenderTexts[0]->width(),rightRenderTexts[0]->height() };

		rightRenderTexts[0]->render(dest, 0); //Renderizamos el nombre del item que siempre es la posicion 0

		offsetY += rightRenderTexts[0]->height() + 25;

		int imgRow = rightItemInfo->row();
		int imgCol = rightItemInfo->col();


		delete rightItemInfo;

		rightRenderImgs[0]->render();//renderizamos la imagen del item que siempre es la posicion 0
		offsetY += 90;

		dest = { (int)(offsetX - rightRenderTexts[1]->width() / 2)  , (int)offsetY ,rightRenderTexts[1]->width(),rightRenderTexts[1]->height() };

		rightRenderTexts[1]->render(dest, 0); //Renderizamos el texto "Items needed" que siempre es la posici�n 1

		offsetY += rightRenderTexts[1]->height() + 35;
		offsetX = bg_tr->getPos().getX() + bg_tr->getW() / 2 + 30;

		//Para cada item necesario
		vector<ItemInfo*> itemsNeeded = craftSys->getCrafts()->find(workshopItems[rightWindowIndex])->second;
		for (int i = 0; i < itemsNeeded.size(); ++i) {
			rightRenderImgs[1 + i]->render(); //Renderizamos su imagen

			dest = { (int)(offsetX + 30) , (int)offsetY + 48 - rightRenderTexts[2 * (i + 1)]->height() / 2 ,	rightRenderTexts[2 * (i + 1)]->width(),	rightRenderTexts[2 * (i + 1)]->height() };
			
			rightRenderTexts[2 * (i + 1)]->render(dest, 0); // Renderizamos el texto "itemsquetengo/itemsnecesarios"

			dest = { (int)(offsetX + 80) , (int)offsetY + 24 - rightRenderTexts[2 * (i + 1) + 1]->height() / 2 ,	rightRenderTexts[2 * (i + 1) + 1]->width(),	rightRenderTexts[2 * (i + 1) + 1]->height() };
			
			rightRenderTexts[2 * (i + 1) + 1]->render(dest, 0); //Renderizamos el nombre del item

			offsetY += 48 + 20;
		}

		dest = { (int)(craftButton_tr->getPos().getX() + craftButton_tr->getW() / 1.5f - rightRenderTexts[rightRenderTexts.size() - 1]->width() / 2),
			(int)(craftButton_tr->getPos().getY() + craftButton_tr->getH() / 2 - rightRenderTexts[rightRenderTexts.size() - 1]->height() / 2),
			rightRenderTexts[rightRenderTexts.size() - 1]->width(),	rightRenderTexts[rightRenderTexts.size() - 1]->height() };

		craftButton->render(); //Renderizamos el texto "craftear"
		rightRenderTexts[rightRenderTexts.size() - 1]->render(dest, 0);

	}
}