#pragma once

#include "../ecs/Manager.h"
#include "../ecs/Component.h"
#include "../ecs/Entity.h"

#include "../utils/Collisions.h"
#include "../utils/checkML.h"

#include "transform.h"
#include "../game/constant_variables.h"

#include <list>

class RigidBody;

class BoxCollider : public Component
{
	friend RigidBody;
private:
	Transform* tr_;

	bool trigger;
	int collisionLayer;
	std::list<BoxCollider*>::iterator itCollisions;
public:
	inline BoxCollider(bool isTrigger = false, int collisionLayer = 0) : tr_(nullptr),
		trigger(isTrigger), collisionLayer(collisionLayer) {
		//if collision layer is equal to 0, then it will collide with all other layers
		assert(collisionLayer >= 0 && collisionLayer < consts::COLLISION_LAYERS);
	}
	

	inline ~BoxCollider() {};

	virtual void init() override;

	inline bool isTrigger() { return trigger; };

	inline std::list<BoxCollider*>::iterator getCollisionIterator() { return itCollisions; };

	bool collision(Transform* ent, RigidBody* rb_);

	inline Transform* getTransform() { return tr_; };
};

