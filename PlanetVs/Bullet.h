#pragma once

#include"vector2.h"
#include"camera.h"
#include"player_id.h"
#include<graphics.h>
#include<functional>
#include<iostream>
extern bool is_debug;
extern Camera main_camera;
class Bullet {
public:
	Bullet() = default;
	~Bullet() = default;

	void set_damage(int val) {
		damage = val;
	}

	int get_damage() {
		return damage;
	}

	void set_position(float x, float y) {
		position.x = x, position.y = y;
	}

	const Vector2& get_position() const {
		return position;
	}

	const Vector2& get_size() const {
		return size;
	}

	void set_velocity(float x, float y) {
		velocity.x = x, velocity.y = y;
	}

	void set_collide_target(PlayerID target) {
		target_id = target;
	}

	PlayerID get_collide_target() const {
		return target_id;
	}

	void set_callback(std::function<void()> callback) {
		this->callback = callback;
	 }

	void set_valid(bool flag) {
		valid = flag;
	}

	bool get_valid() const {
		return valid;
	}

	bool check_can_remove() const {
		return can_remove;
	}

	virtual void on_collide() {
		if (callback) {
			callback();
		}
	}

	virtual bool check_collision(const Vector2& position, const Vector2& size) {
		//std::cout << "\nIn check_collision()\|\|\|\|/" << '\n';
		//if (this->position.x + this->size.x / 2 >= position.x
		//	&& this->position.x + this->size.x / 2 <= position.x + size.x
		//	&& this->position.y + this->size.y / 2 >= position.y
		//	&& this->position.y + this->size.y / 2 <= position.y + size.y) {
		//	//std::cout<<"This work...\n";
		//}
		return this->position.x + this->size.x / 2 >= position.x
			&& this->position.x + this->size.x / 2 <= position.x + size.x
			&& this->position.y + this->size.y / 2 >= position.y
			&& this->position.y + this->size.y / 2 <= position.y + size.y;
	}

	virtual void on_update(int delta){}
	virtual void on_draw(const Camera& camera)const {
		if (is_debug) {
			setfillcolor(RGB(255, 255, 255));
			setlinecolor(RGB(255, 255, 255));
			const Vector2& pos_camera = camera.get_position();

			rectangle((int)(position.x - pos_camera.x), (int)(position.y - pos_camera.y),
				(int)(position.x + size.x - pos_camera.x), (int)(position.y + size.y - pos_camera.y));
			solidcircle((int)(position.x + size.x / 2 - pos_camera.x), (int)(position.y + size.y / 2 - pos_camera.y), 5);
		}
	}
	virtual const Vector2& get_shotted() = 0;
protected:
	Vector2 size;
	Vector2 position;
	Vector2 velocity;
	Vector2 bulletPower;
	int damage = 10;
	bool valid = true;
	bool can_remove = false;
	std::function<void()> callback;
	PlayerID target_id = PlayerID::P1;

protected:
	/*����ӵ��Ƿ����������Χ��*/
	bool check_if_exceeds_screen() {
		return (position.x + size.x  <= main_camera.get_position().x || position.x >= getwidth() + main_camera.get_position().x
			|| position.y + size.y + main_camera.get_position().y <= 0 || position.y >= getheight() + main_camera.get_position().y);
	}
};
