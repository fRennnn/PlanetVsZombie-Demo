#pragma once
#include"scene.h"

extern Scene* menu_scene;
extern Scene* game_scene;
extern Scene* selector_scene;

class SceneManager
{
public:
	enum class SceneType
	{
		Menu,
		Game,
		Selector
	};
public:
	SceneManager() = default;
	~SceneManager() = default;

	void set_curren_scene(Scene * scene)
	{
		curren_scene = scene;
		curren_scene->on_enter();
	}
	 
	void switch_to(SceneType type)
	{
		curren_scene->on_exit();
		switch (type)
		{
		case SceneType::Menu:
			curren_scene = menu_scene;
			break;
		case SceneType::Game:
			curren_scene = game_scene;
			break;
		case SceneType::Selector:
			curren_scene = selector_scene;
			break;
		default:
			break;
			curren_scene->on_enter();
		}
	}

	void on_update(int delta)
	{
		curren_scene->on_update(delta);
	}

	void on_draw()
	{
		curren_scene->on_draw();
	}

	void on_input(const ExMessage& msg)
	{
		curren_scene->on_input(msg);
	}
private:
	Scene* curren_scene = nullptr;
};