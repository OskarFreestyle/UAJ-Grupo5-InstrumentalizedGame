#include "shelter_hud.h"
#include "../ecs/Manager.h"
#include "../classes/player.h"
#include "../components/hunger_component.h"
#include "../components/tiredness_component.h"
#include "../sdlutils/SDLUtils.h"
#include "../game/constant_variables.h"
#include "../classes/shelter_scene.h"
#include "../components/transform.h"
#include "../components/text_with_background.h"


ShelterHud::ShelterHud(Manager* mngr, ShelterScene* shelter_) : Entity(mngr) {
	shelter = shelter_;
	mngr->addEntity(this);
	mngr->addRenderLayer<Interface>(this);

	player = static_cast<Player*>(mngr->getHandler<Player_hdlr>());
	hunger = player->getComponent<HungerComponent>();
	tiredness = player->getComponent<TirednessComponent>();

	hungerState = tirednessState = &sdlutils().images().at("shelterStatusEffects");
	clock = &sdlutils().images().at("clock");

	Entity* tooltip = mngr->addEntity();
	tooltipTr = tooltip->addComponent<Transform>(Vector2D(), 400, 10);
	tooltipText = tooltip->addComponent<TextWithBackground>("Inventario", sdlutils().fonts().at("Orbitron32"), build_sdlcolor(0xffffffff), &sdlutils().images().at("tooltipBox"));
	tooltip->setActive(false);

	assert(hunger != nullptr && tiredness != nullptr);


	woundDescriptions = std::vector<std::string>{
		{"You are not hungry, damage resistance is NORMAL"},
		{"You are hungry, damage resistance is LOW"},
		{"You are starving, damage resistance is VERY LOW"},
		{"You are not tired, speed is NORMAL"},
		{"You are tired, speed is LOW"},
		{"You are exhausted, speed is VERY LOW"},
	};
}

void ShelterHud::render() {
	//Hunger
	SDL_Rect dest = { consts::STATUS_EFFECTS_SIZEX / 2, 30, consts::STATUS_EFFECTS_SIZEX, consts::STATUS_EFFECTS_SIZEY };
	SDL_Rect src = { (32 * (int)hunger->getHungerLevel()), 0, 32, 32 };
	hungerState->render(src, dest);

	//Tiredness
	SDL_Rect dest2 = dest;
	dest2.x += consts::STATUS_EFFECTS_SIZEX; src.y = 32; src.x = 32 * (int)tiredness->getTirednessLevel();
	tirednessState->render(src, dest2);

	//Acciones
	int acciones = shelter->getActions(); //Numero de acciones para probar
	SDL_Rect accDest = { consts::WINDOW_WIDTH - clock->width() / 2 * acciones, dest.y, dest.w, dest.h };
	SDL_Rect accSrc = { 0,0, 64, 64 };

	for (int i = 0; i < acciones; i++) {
		clock->render(accSrc, accDest);
		accDest.x += accSrc.w;
	}
	//Tooltips
	manageToolTips();
	drawTooltip(0, dest);
	drawTooltip(1, dest2);
}

void ShelterHud::drawTooltip(int indice, SDL_Rect dest) {
	Vector2D mouse(ih().getMousePos().first, ih().getMousePos().second);
	if (mouse.getX() > dest.x && mouse.getX() < dest.x + dest.w &&
		mouse.getY() > dest.y && mouse.getY() < dest.y + dest.h) {

		tooltipTextures[indice].t->setPos(mouse);

		if (!indice) tooltipTextures[indice].text->changeText(woundDescriptions[(int)hunger->getHungerLevel()]);
		else {
			int index = (int)tiredness->getTirednessLevel() + 3;
			tooltipTextures[indice].text->changeText(woundDescriptions[index]);
		}

		tooltipTextures[indice].text->render();
	}
}

void ShelterHud::manageToolTips()
{
	//Tooltips
	for (int i = 0; i < tooltipTextures.size(); i++) {
		tooltipTextures[i].t->getEntity()->setDead(true);
	}
	tooltipTextures.clear();

	tooltipTextures.reserve(2);
	for (int i = 0; i < 2; i++) {
		Entity* ent = mngr_->addEntity();
		Transform* t = ent->addComponent<Transform>(Vector2D(), 400, 10);
		TextWithBackground* text = ent->addComponent<TextWithBackground>(" ",
			sdlutils().fonts().at("Orbitron24"), build_sdlcolor(0xffffffff), &sdlutils().images().at("tooltipBox"));
		tooltipTextures.push_back({ t, text });
		ent->setActive(false);
	}
}
