#pragma once
#include"util.h"
#include"camera.h"
#include"timer.h"

extern bool is_debug;

class CollisionWall {
public:
	/*
	abs(right - left) >= 20!!!
	*/
	CollisionWall(float _height, float _left, float _right) :height(_height), left(_left), right(_right) {
		
	}
	~CollisionWall() = default;
	float height;
	float left,right;
};

class Platform {
public:
	Platform() = default;
	~Platform() = default;
	friend class Wall;
	void on_draw(const Camera& camera) const {
		putimage_alpha(camera, render_position.x, render_position.y, img); 

		if (is_debug) {
			setlinecolor(RGB(255, 0, 0));
			line(camera, (int)shape.left, (int)shape.y, (int)shape.right, (int)shape.y);
			if (is_wall) {
				for (const auto& it : wall_lists) {
					line(camera, (int)it.left, (int)shape.y, (int)it.left, (int)it.height);
					line(camera, (int)it.right, (int)shape.y, (int)it.right, (int)it.height);
					line(camera, (int)it.right, (int)it.height, (int)it.left, (int)it.height);
				}
			}
		}
	}
public:
	struct CollisionShape
	{
		float y; 
		float left, right;//x1 , x2
	}; 
public:
	CollisionShape shape;
	IMAGE* img = nullptr;
	POINT render_position{ 0 }; //Êý¾ÝÂß¼­ äÖÈ¾Âß¼­Ïà¸ôÀë
	std::vector<CollisionWall> wall_lists;
	bool is_wall = false;
};


