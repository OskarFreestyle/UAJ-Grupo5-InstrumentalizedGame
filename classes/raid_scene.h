#pragma once
#include "game_scene.h"
#include "enemy.h"
#include "player.h"
#include "../utils/checkML.h"
#include <string>
class Countdown;
class Player;
class Texture;

class RaidScene : public GameScene
{
private:
	string path_;
	Countdown* timer;
	Player* player;
	bool raidTimeEnded;
	void createParallaxBackground(int numOfRep) override;
public:
	RaidScene(string path, string name, Game* game);
	~RaidScene() { delete timer; }
	void init() override;
	void update() override;
};

