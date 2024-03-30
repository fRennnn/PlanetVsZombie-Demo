#pragma once
#include"scene.h"
#include<iostream>
#include"SceneManager.h"
#include"animation.h"
#include"atlas.h"

extern Atlas atlas_peashooter_run_right;
extern SceneManager scene_manager;

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter()
	{
		/*std::cout << "====Menu Scene====" << std::endl;*/
		animation_peashooter_run_right.set_atlas(&atlas_peashooter_run_right);
		animation_peashooter_run_right.set_interval(75);
		animation_peashooter_run_right.set_loop(true);
	}
	void on_update(int delta) 
	{
		/*std::cout << "Main Menu is running" << std::endl;*/
		animation_peashooter_run_right.on_update(delta);
	}
	void on_draw() 
	{
		/*outtextxy(10, 10, _T("Ö÷²Ëµ¥»æÍ¼ÄÚÈÝ"));*/
		animation_peashooter_run_right.on_draw(100, 100);
	}
	void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			scene_manager.switch_to(SceneManager::SceneType::Game);
		}
	}
	void on_exit() 
	{ 
		std::cout << "Main Menu exit" << std::endl;
	}
private:
	Animation animation_peashooter_run_right;
};