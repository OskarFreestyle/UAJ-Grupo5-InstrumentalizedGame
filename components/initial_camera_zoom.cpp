#include "initial_camera_zoom.h"
#include "../classes/camera.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../game/constant_variables.h"
#include "camera_movement.h"
InitialCameraZoom::InitialCameraZoom(float zoom, float time, std::function<void()> f):
	 totalTime(time), function(f)
{
	t = 0;
	initialZoom = Camera::mainCamera->getScale();
	targetZoom = initialZoom * zoom;
}

void InitialCameraZoom::init() {
	entity_->getMngr()->getHandler<Player_hdlr>()->getComponent<CameraMovement>()->enabled = false;
}

void InitialCameraZoom::update()
{
	t += consts::DELTA_TIME;

	if (t > totalTime) {
		Camera::mainCamera->setScale(targetZoom);

		function();

		entity_->setDead(true);
		return;
	}

	float i = t / totalTime;

	float dist = 1 - i;

	dist = dist * dist;

	i = 1 - dist;

	float scale = Vector2D::Lerp(initialZoom, targetZoom, i);
	Camera::mainCamera->setScale(scale);
}
