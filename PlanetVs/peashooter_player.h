#pragma once
#include"player.h"
#include<iostream>

extern Atlas altas_peashooter_idle_left;
extern Atlas altas_peashooter_idle_right;
extern Atlas altas_peashooter_run_left;
extern Atlas altas_peashooter_run_right;
class PeashooterPlayer : public Player {
public:
	PeashooterPlayer() {
		animation_idle_left.set_atlas(&altas_peashooter_idle_left);
		animation_idle_right.set_atlas(&altas_peashooter_idle_right);
		animation_run_left.set_atlas(&altas_peashooter_run_left);
		animation_run_right.set_atlas(&altas_peashooter_run_right);

		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);

		size.x = 96, size.y = 96;
	}
	~PeashooterPlayer() = default;

	void on_draw(const Camera& camera) {
		current_animation->on_draw(camera, (int)position.x, (int)position.y);
	}

	/*void on_input(const ExMessage& msg) {
		Player::on_input(msg);
	}*/

	void on_update(int delta) {

		Player::on_update(delta);
		std::cout << "�㶹���ָ�����..." << std::endl;
	}
private:

};