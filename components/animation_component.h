#pragma once

#include "../ecs/Component.h"
#include "../components/image.h"
#include "../utils/checkML.h"
#include "../game/constant_variables.h"

class AnimationComponent : public Component {
protected:
	struct Animation {
	protected:
		Image* image;
		int width, height;
		int frameRow;
		int frameCount;
		int currentFrame;
		float speed;
		bool loop;
	public:
		Animation(int frame, int frameCount, bool loop = true, float animationSpeed = 1);

		void setImage(Image* img);

		void advanceFrame();

		int getCurrentFrame() { return currentFrame; }

		void render();

		inline bool operator == (Animation& other) { return frameRow == other.frameRow; }

		inline bool operator != (Animation& other) { return frameRow != other.frameRow; }

		inline float animationSpeed() { return speed; };
	};

public:
	inline AnimationComponent() {};

	inline virtual ~AnimationComponent() {};
};