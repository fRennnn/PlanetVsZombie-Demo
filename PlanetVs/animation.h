#pragma once
#include"atlas.h"
#include"util.h"
#include<functional>
#include<graphics.h>
#include"camera.h"
class Animation
{
public:
	Animation() = default;
	~Animation() = default;

	void reset()
	{
		timer = 0;
		idx_frame = 0;
	}

	void set_atlas(Atlas* new_atlas)
	{
		reset();
		atlas = new_atlas;
	}

	void set_loop(bool flag)
	{
		is_loop = flag;
	}

	void set_interval(int ms)
	{
		interval = ms;
	}

	int get_idx_frame()
	{
		return idx_frame;
	}

	IMAGE* get_frame()
	{
		return atlas->get_image(idx_frame);
	}

	bool check_finished()
	{
		if (is_loop) return false;

		return (idx_frame == atlas->get_size() - 1);
	}

	void on_update(int delta)
	{
		timer += delta;
		if (timer >= interval)
		{
			timer = 0;
			++idx_frame;
			if (idx_frame >= atlas->get_size())
			{
				idx_frame = is_loop ?  0 : atlas->get_size() - 1;
				if (!is_loop && callback)
					callback();
			}
		}
	}

	void on_draw(const Camera& camera,int x, int y) const
	{
		putimage_alpha(camera,x, y, atlas->get_image(idx_frame));
	}

	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}
private:
	Atlas* atlas = nullptr;
	int timer = 0;			//计时器 
	int interval = 0;			//帧间隔
	int idx_frame = 0;		//帧索引
	bool is_loop = true; //是否循环
	std::function<void()> callback;
};