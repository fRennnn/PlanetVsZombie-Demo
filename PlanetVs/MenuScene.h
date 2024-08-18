#pragma once
#include"scene.h"
#include<iostream>
#include"SceneManager.h"
#include"animation.h"
#include"atlas.h"
#include"camera.h"
#include"timer.h"
extern Atlas altas_peashooter_run_right;
extern IMAGE img_menu_bg;
extern SceneManager scene_manager;

class MenuScene : public Scene
{
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter()
	{
		mciSendString(_T("play ddtMenu repeat from 0"), NULL, 0, NULL);
		std::cout << "In MenuScene..." << std::endl;
	} 
	void on_update(int delta) 
	{
		
	}
	void on_draw(const Camera& camera)
	{
		putimage(0, 0, &img_menu_bg); 
	}
	void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN) {
			mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
			scene_manager.switch_to(SceneManager::SceneType::Selector);
		}
	}
	void on_exit() 
	{ 
		mciSendString(_T("stop ddtMenu"), NULL, 0, NULL);
	}
private:
	Animation animation_peashooter_run_right;
	Camera camera;
	Timer timer;
};