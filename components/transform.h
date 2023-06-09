// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include "../utils/checkML.h"

#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../utils/Collisions.h"

class Transform : public Component {
private:
	Vector2D pos_;
	float width_;
	float height_;
	float rotation_;

public:
	Transform();

	Transform(Vector2D pos, float width = 0, float height = 0,
		float rotation = 0);

	virtual ~Transform();

	virtual void init() override {};

	inline Vector2D& getPos() { return pos_; }

	Vector2D getCenterPos();

	void setCenterPos(Vector2D pos);

	inline float getH() const { return height_; }

	inline void setH(float height) { height_ = height; }

	inline float getW() const { return width_; }

	inline void setW(float width) { width_ = width; }

	inline float getRot() const {return rotation_;}

	inline Point2D getSize() const { return Point2D(width_, height_); }

	inline void setSize(float width, float height) { width_ = width; height_ = height; }

	inline void setRot(float rot) { rotation_ = rot; }

	inline void setPos(Vector2D pos) { pos_ = pos; }
};

