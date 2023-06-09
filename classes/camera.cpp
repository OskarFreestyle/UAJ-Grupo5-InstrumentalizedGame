#include "camera.h"
#include <iostream>
#include "../game/constant_variables.h"

Camera* Camera::mainCamera = nullptr;

Camera::Camera(Vector2D&& initial, int cam_w, int cam_h) {
	pos = std::move(initial);
	width = cam_w;
	height = cam_h;

	winWidth = consts::WINDOW_WIDTH;
	winHeight = consts::WINDOW_HEIGHT;

	xmin = ymin = xmax = ymax = 0;

	ogScale = scale = float(winWidth) / width;
	//setScale(scale * 1.2f);
}

void Camera::setScale(float value) {
	int pwidth = width;
	int pheight = height;
	float pScale = scale;

	scale = value;
	width = winWidth / scale;
	height = winHeight / scale;

	xmin = ogxmin * scale;
	xmax = ogxmax * scale;
	ymin = ogymin * scale;
	ymax = ogymax * scale;

	pos = pos + Vector2D(pwidth / 2 - width / 2, pheight / 2 - height /2);
}

float Camera::getScale() {
	return scale;
}

void Camera::restoreScale() {
	setScale(ogScale);
}

void Camera::setMain(Camera* cam) {
	if (mainCamera != nullptr)
		delete mainCamera;
	mainCamera = cam;
}

void Camera::Move(Vector2D& newPos) {
	pos = newPos;
}

void Camera::MoveToPoint(Vector2D& newPos) {
	Vector2D newcamerapos = newPos - Vector2D(winWidth / 2, winHeight / 2) + Vector2D(winWidth - width, winHeight - height) / 2;
	pos = newcamerapos;
}

void Camera::Lerp(const Vector2D& newPos, float i) {
	Vector2D newcamerapos = newPos - Vector2D(winWidth / 2, winHeight / 2) + Vector2D(winWidth - width, winHeight - height) / 2;
	if ((pos - newcamerapos).magnitude() > 2)
		pos = Vector2D::Lerp(pos, newcamerapos, i * consts::DELTA_TIME);
}
void Camera::LerpWithBounds(const Vector2D& newPos, float i) {
	Lerp(newPos, i);

	Vector2D& p = pos;
	if (p.getX() < xmin) p.setX(xmin);
	else if (p.getX() > xmax - winWidth) p.setX(xmax - winWidth);

	if (p.getY() < ymin ) p.setY(ymin );
	else if (p.getY() + height > ymax ) p.setY(ymax - height);

}

void Camera::setBounds(float a, float b, float c, float d) {
	ogxmin = a;
	ogymin = b;
	ogxmax = c;
	ogymax = d;

	xmin = ogxmin * scale;
	xmax = ogxmax * scale;
	ymin = ogymin * scale;
	ymax = ogymax * scale;
}

void Camera::MoveDir(Vector2D dir) {
	pos = pos + dir;
}

void Camera::FollowPlayer(Vector2D& playerPos) {
	pos = playerPos + Vector2D(-width / 2, -height / 4 * 3);
}


Vector2D Camera::renderRect(Vector2D& imagePos, int w, int h, bool& shouldRender) {
	if (!(
		imagePos.getX()+ w > pos.getX() &&
		imagePos.getX()< pos.getX() + width &&
		imagePos.getY()+ h> pos.getY() &&
		imagePos.getY() < pos.getY()+ height ))
	{
		shouldRender = false;
		return Vector2D();
	}

	return Vector2D(imagePos.getX() - pos.getX(), imagePos.getY() - pos.getY());
}

Vector2D Camera::renderRect(Vector2D& imagePos) {
	return Vector2D(imagePos.getX() - pos.getX(), imagePos.getY() - pos.getY());
}


Point2D Camera::PointToWorldSpace(Point2D point) {
	return point / scale + pos;
}

Point2D Camera::WorldToPointSpace(Point2D point) {
	return point / scale - pos;
}

Point2D Camera::getCameraPosition()
{
	return pos;
}

Point2D Camera::getCameraCenterPosition()
{
	return pos + Vector2D(winWidth / 2, winHeight / 2) - Vector2D(winWidth - width, winHeight - height) / 2;
}

bool Camera::isVisible(Point2D point) {
	return
		point.getX() >= pos.getX() &&
		point.getX() < pos.getX() + width &&
		point.getY() >= pos.getY() &&
		point.getY() < pos.getY() + height;
}