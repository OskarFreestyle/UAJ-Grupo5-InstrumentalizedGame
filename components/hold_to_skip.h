#pragma once

#include "../ecs/Component.h"
#include "../components/transform.h"
#include "../components/image.h"
#include <functional>
class HoldToSkip: public Component
{
public:
	HoldToSkip(float maxTime, std::function<void()> f, bool isTutorial = false);
	void init() override;
	void update() override;
	void render() override;
private:
	float t;
	float maxT;
	std::function<void()> function;

	bool isTutorial;
	Transform* transform;
	Image* image;

	SDL_KeyCode skipButton;
};

