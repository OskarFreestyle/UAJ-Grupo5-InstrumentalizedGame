#pragma once
#include "interactable_element.h"

class GameScene;
class BackToShelter : public InteractableElement
{
private:
	Entity* player_;
	GameScene* scene_;

public: 
	BackToShelter(GameScene* scene);
	void init() override;
	void Interact() override;
	void update() override;
};

