#pragma once
#include"scene.h"
#include<iostream>
#include"SceneManager.h"
#include"animation.h"
#include"atlas.h"
#include"camera.h"
#include"timer.h"
extern Atlas altas_peashooter_run_right;

extern SceneManager scene_manager;

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter()
	{
		/*std::cout << "====Menu Scene====" << std::endl;*/
		animation_peashooter_run_right.set_atlas(&altas_peashooter_run_right);
		animation_peashooter_run_right.set_interval(75);
		animation_peashooter_run_right.set_loop(true);

		timer.set_wait_time(1000);
		timer.set_one_shot(false);
		timer.set_callback([]()
			{
				std::cout << "Shot!" << std::endl;
			});
	} 
	void on_update(int delta) 
	{
		/*std::cout << "Main Menu is running" << std::endl;*/
		timer.on_update(delta);
		camera.on_update(delta);
		animation_peashooter_run_right.on_update(delta);
	}
	void on_draw() 
	{
		/*outtextxy(10, 10, _T("Ö÷²Ëµ¥»æÍ¼ÄÚÈÝ"));*/
		const Vector2& pos_camera = camera.get_position();
		animation_peashooter_run_right.on_draw(
			(int)(100 - pos_camera.x), (int)(100 - pos_camera.y)
		);
	}
	void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			camera.shake(10, 350);
			std::cout << "Shake!!!" << std::endl;
		}
	}
	void on_exit() 
	{ 
		std::cout << "Main Menu exit" << std::endl;
	}
private:
	Animation animation_peashooter_run_right;
	Camera camera;
	Timer timer;
};