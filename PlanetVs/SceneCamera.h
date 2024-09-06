#pragma once
#include"camera.h"
class SceneCamera : public Camera{
public:
	SceneCamera() = default;
	~SceneCamera() = default;
	
	float lerp(float x, float y, float delta) {
		return x + (y - x) * delta;
	}

	void on_update(int delta) {
		if (is_moved) {
			//do_nothing
		}
		else {
			//test
		}
		//std::cout << "Main camera's position : { " << position.x << " , " << position.y << " }\n";
	}

	virtual Vector2& get_position(){
		return position;
	}

private:
	bool is_moved = false;
	Vector2 position;
	Timer timer_shake;
	bool is_shaking = false;
	float shaking_strength = 0;
};