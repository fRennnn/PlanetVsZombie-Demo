#pragma once
#include "vector2.h"
#include"timer.h"
#include<iostream>
class Camera {
public:
	enum Direction {
		Front=0,
		Back
	};
	Camera(){	
		timer_shake.set_one_shot(true);
		timer_shake.set_callback([&]()
			{
				is_shaking = false;
				reset();
			});
		timer_map_rolling.set_one_shot(true);
		timer_map_rolling.set_wait_time(100);
		timer_map_rolling.set_callback([&]()
			{
				map_roll = false;
				stop();
			});
	};
	~Camera() = default;
	
	virtual const Vector2& get_position() const {
		return position;
	}

	void reset() {
		position = { 0,0 };
		//do_nothing
	}

	void stop() {
		map_roll = false;
	}

	void on_update(int delta) {
		timer_shake.on_update(delta);
		
		//position += {0, 0.35f};
		
		if (is_shaking) { 
			position.x = (-50 + rand() % 100) / 50.0f * shaking_strength; //[-1.0 , 1.0] <-- 抖动强度
			position.y = (-50 + rand() % 100) / 50.0f * shaking_strength;
		}
		if (map_roll) {
			timer_map_rolling.on_update(delta);
			position.x = lerp(position.x, position.x + camera_move_distance * direction, delta);
		}
		
		//std::cout << "Main camera's position : { " << position.x << " , " << position.y << " }\n";
	}
	

	void shake(float strength, int duration) {
		is_shaking = true;
		shaking_strength = strength;

		timer_shake.set_wait_time(duration);
		timer_shake.restart();
	}

	void can_move() {
		timer_map_rolling.restart();
		map_roll = true;
		if (now_direction == Direction::Front) {
			direction = 1.0f;
		}
		else {
			direction = -1.0f;
		}
	}

	void set_position(float x,float y){
		position.x = x;
		position.y = y;
	}

	//True -> Front False -> Back
	void set_direction(bool Front) {
		now_direction = Front ? Direction::Front : Direction::Back;
	}

	float lerp(float src, float dis, float delta) {
		return src + (dis - src) * delta;
	}

	bool two_inside(const Vector2& left, const Vector2& right) {
		return (int)abs_x(left, right) < camera_width;//1036是相机尺寸?
	}

	bool can_moveforward(const Vector2& left, const Vector2& right) {
		/*std::cout << "left.x = " << left.x << " cam + cam_wid = " << position.x + camera_width << '\n';
		std::cout << "right.x = " << right.x << " cam + cam_wid = " << position.x + camera_width << '\n';*/
		return left.x >= position.x + camera_width || right.x >= position.x + camera_width;
	}

	bool can_moveback(const Vector2& left, const Vector2& right) {
		/*std::cout << "left.x = " << left.x << " cam.x = " << position.x + 10 << '\n';
		std::cout << "right.x = " << right.x << " cam.x = " << position.x + 10 << '\n';*/
		return left.x <  position.x  || right.x < position.x ;
	}
private: 
	Vector2 position;

	//Shake
	Timer timer_shake;
	Timer timer_map_rolling;
	bool is_shaking = false;
	float shaking_strength = 0;
	
	//Move the Camera
	bool map_roll = false;
	float camera_move_distance = 0.55f;
	float direction = 1.0f;
	Direction now_direction;
	const int camera_width = 1250;
};