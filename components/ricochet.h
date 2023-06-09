// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <cassert>

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include "transform.h"
#include "../utils/checkML.h"
#include "../components/rigid_body.h"


class Ricochet : public Component {
private:
	Transform* tr_;
	Transform* playerTr;
	RigidBody* rb;
	int n; //numero rebotes
	int tier;
	int explosion = 100; // Da�o de la explosion
	void OnCollision(Entity* collider) override;
	void hitEnemies(Point2D raycastOrigin);

public:
	Ricochet(Transform* player, int nrebotes, int typeOfWeapon);

	virtual ~Ricochet();

	void init() override;

	void createExplosion();



};
