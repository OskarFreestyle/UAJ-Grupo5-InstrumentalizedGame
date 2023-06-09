#include "charge.h"
#include "../classes/enemy.h"


void Charge::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	hitEnemies();
}

void Charge::update() {
	if (tr_->getH() < consts::CHARGEWEAPON_HEIGHT) {
		tr_->setH(tr_->getH() + 1);
		tr_->getPos().setX(tr_->getPos().getX() - sin(-angle) * 0.5f);
		tr_->getPos().setY(tr_->getPos().getY() - cos(-angle) * 0.5f);
	}
	else entity_->setDead(true);
}

void Charge::hitEnemies()
{
	std::list<Entity*> collided = raycast->allCollisionsInLine<Enemy_grp>(entity_->getMngr());
	for (Entity* hit_entity : collided) {
		static_cast<Enemy*>(hit_entity)->receiveDamage(3);
	}
}
