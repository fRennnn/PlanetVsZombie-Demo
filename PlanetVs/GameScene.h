#pragma once
#include"scene.h"
#include<iostream>
#include"SceneManager.h"

extern SceneManager scene_manager;
class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;
	
	void on_enter()
	{
		std::cout << "====Game Scene====" << std::endl;
	}
	void on_update(int delta)
	{
		std::cout << "Game is running" << std::endl;
	}
	void on_draw()
	{
		outtextxy(10, 10, _T("ÓÎÏ·»æÍ¼ÄÚÈÝ"));
	}
	void on_input(const ExMessage& msg)
	{
		if (msg.message == WM_KEYDOWN)
		{
			scene_manager.switch_to(SceneManager::SceneType::Menu);
		}
	}
	void on_exit()
	{
		std::cout << "Main Menu exit" << std::endl;
	}
private:
};