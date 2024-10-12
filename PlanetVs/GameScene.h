#pragma once
#include"scene.h"
#include<iostream>
#include"SceneManager.h"
#include"util.h"
#include"status_bar.h"
#include"platform.h"
#include"player.h"
#include"Bullet.h"

extern SceneManager scene_manager;
extern IMAGE img_sky;
extern IMAGE img_sky_mirror;
extern IMAGE img_sky1;
extern IMAGE img_sky2;
extern IMAGE img_sky3;
extern IMAGE img_sky4;

extern IMAGE img_hills;
extern IMAGE img_hills_mirror;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;
extern IMAGE Pixiv_platform;
extern IMAGE BIGPicture;
extern IMAGE* img_player_1_avatar;		//头像
extern IMAGE* img_player_2_avatar;

extern IMAGE img_1P_winner;				//胜负结算
extern IMAGE img_2P_winner;
extern IMAGE img_winner_bar;

extern Camera main_camera;				//摄像机

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

extern Player* player_1;
extern Player* player_2;
/*适配图集素材*/
class ExtensionAssert {
public:
	// x1 ... -> src x2 ... -> dst
	ExtensionAssert(int x1, int y1, int w1, int h1,
		int x2, int y2, int w2, int h2) {
		src.x = x1;
		src.y = y1;
		src.w = w1;
		src.h = h1;
		dst.x = x2;
		dst.y = y2;
		dst.w = w2;
		dst.h = h2;
	}
	~ExtensionAssert() = default;
public:
	Rect src;
	Rect dst;
};

std::vector<ExtensionAssert> BIGPicture_extensionAssert_list;
class GameScene : public Scene
{
public:
	GameScene() = default;
	~GameScene() = default;
	
	void on_enter()
	{
		is_gameover = false;
		is_slide_out_started = false;

		pos_img_winner_bar.x = -img_winner_bar.getwidth();
		pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
		pos_x_img_winner_bar_dst = (getwidth() - img_winner_bar.getwidth()) / 2;

		pos_img_winner_text.x = pos_img_winner_bar.x;
		pos_img_winner_text.y = (getheight() - img_1P_winner.getheight()) / 2;
		pos_x_img_winner_text_dst = (getwidth() - img_1P_winner.getwidth()) / 2;

		timer_winner_slide_in.restart();
		timer_winner_slide_in.set_wait_time(4000);
		timer_winner_slide_in.set_one_shot(true);
		timer_winner_slide_in.set_callback([&]() {
			is_slide_out_started = true;
			});

		timer_winner_slide_out.restart();
		timer_winner_slide_out.set_wait_time(4000);
		timer_winner_slide_out.set_one_shot(true);
		timer_winner_slide_out.set_callback([&]() {
			scene_manager.switch_to(SceneManager::SceneType::Menu);
			});

		status_bar_1P.set_avatar(img_player_1_avatar);
		status_bar_2P.set_avatar(img_player_2_avatar);

		/*设置元素坐标*/

		status_bar_1P.set_position(235, 625);
		status_bar_2P.set_position(675, 625);
		std::cout << "In the GameScene..." << std::endl;
		player_1->set_position(200, 50);
		player_2->set_position(975, 50);
		pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

		pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;

		/*平台数据初始化*/
		platform_list.resize(6);	

		Platform& large_platform = platform_list[0];
		large_platform.img = &img_platform_large;
		large_platform.render_position.x = 122;
		large_platform.render_position.y = 455;
		large_platform.shape.left = (float)large_platform.render_position.x + 30;
		large_platform.shape.right = (float)large_platform.render_position.x + img_platform_large.getwidth() - 30;
		large_platform.shape.y = (float)large_platform.render_position.y + 60;
		large_platform.wall_lists.emplace_back((float)large_platform.shape.y + 250, large_platform.shape.left, large_platform.shape.right);
		large_platform.is_wall = true;

		Platform& small_platform_1 = platform_list[1];
		small_platform_1.img = &img_platform_small;
		small_platform_1.render_position.x = 175;
		small_platform_1.render_position.y = 360;
		small_platform_1.shape.left = (float)small_platform_1.render_position.x + 40;
		small_platform_1.shape.right = (float)small_platform_1.render_position.x + img_platform_small.getwidth() - 40;
		small_platform_1.shape.y = (float)small_platform_1.render_position.y + img_platform_small.getheight() / 2;
		

		Platform& small_platform_2 = platform_list[2];
		small_platform_2.img = &img_platform_small;
		small_platform_2.render_position.x = 855;
		small_platform_2.render_position.y = 360;
		small_platform_2.shape.left = (float)small_platform_2.render_position.x + 40;
		small_platform_2.shape.right = (float)small_platform_2.render_position.x + img_platform_small.getwidth() - 40;
		small_platform_2.shape.y = (float)small_platform_2.render_position.y + img_platform_small.getheight() / 2;
		

		Platform& small_platform_3 = platform_list[3];
		small_platform_3.img = &img_platform_small;
		small_platform_3.render_position.x = 515;
		small_platform_3.render_position.y = 225;
		small_platform_3.shape.left = (float)small_platform_3.render_position.x + 40;
		small_platform_3.shape.right = (float)small_platform_3.render_position.x + img_platform_small.getwidth() - 40;
		small_platform_3.shape.y = (float)small_platform_3.render_position.y + img_platform_small.getheight() / 2;
		small_platform_3.wall_lists.emplace_back((float)small_platform_3.shape.y + 150, small_platform_3.shape.left, small_platform_3.shape.right);
		small_platform_3.is_wall = true;
 
		Platform& small_platform_4 = platform_list[4];
		small_platform_4.img = &img_platform_small;
		small_platform_4.render_position.x = 1255;
		small_platform_4.render_position.y = 225;
		small_platform_4.shape.left = (float)small_platform_4.render_position.x + 40;
		small_platform_4.shape.right = (float)small_platform_4.render_position.x + img_platform_small.getwidth() - 40;
		small_platform_4.shape.y = (float)small_platform_4.render_position.y + img_platform_small.getheight() / 2;

		Platform& small_platform_5 = platform_list[5];
		small_platform_5.img = &Pixiv_platform;
		
		//  x = 79, y = 62, w = 84, h = 18; 
		small_platform_5.render_position.x = -10;
		small_platform_5.render_position.y = 225;
		small_platform_5.shape.left = (float)small_platform_5.render_position.x + 40;
		small_platform_5.shape.right = (float)small_platform_5.render_position.x + img_platform_small.getwidth() - 40;
		small_platform_5.shape.y = (float)small_platform_5.render_position.y + img_platform_small.getheight() / 2;
		BIGPicture_extensionAssert_list.emplace_back(79, 62, 84, 18,
			30, 235, (84 << 1) + 10, 18 << 1);
		mciSendString(_T("play ddtGame repeat from 0"), NULL, 0, NULL);
	}
	void on_update(int delta)
	{
		//std::cout << "Game over : " << (int)is_gameover << std::endl;
		player_1->on_update(delta);
		player_2->on_update(delta);

		main_camera.on_update(delta);

		//删除子弹
		//std::remove(first , last , op) if op return true  then delete this element
		bullet_list.erase(std::remove_if(
			bullet_list.begin(), bullet_list.end(),
			[](const Bullet* bullet) {
				bool deletable = bullet->check_can_remove();
				if (deletable) { 
					//std::cout << "Delete Bullet" << std::endl;
					delete bullet;
				}
				return deletable;
			}),
			bullet_list.end());
		for (Bullet* bullet : bullet_list) {
			bullet->on_update(delta);
		 } 

		/*摄像机运动*/
		const Vector2& position_player_1 = player_1->get_position();
		const Vector2& position_player_2 = player_2->get_position();
		bool two_inside = main_camera.two_inside(position_player_1, position_player_2);
		bool can_moveback = main_camera.can_moveback(position_player_1, position_player_2);
		bool can_moveforward = main_camera.can_moveforward(position_player_1, position_player_2);
		if (two_inside && (can_moveback ^ can_moveforward)) {
			main_camera.can_move();
			if (can_moveforward) {
				main_camera.set_direction(1);
				//std::cout << "State can_moveforward " << can_moveforward << '\n';
			}
			else {
				main_camera.set_direction(0);
				//std::cout << "State can_moveback " << can_moveback << '\n';
			}
		}

		if (position_player_1.y >= getheight())
			player_1->set_hp(0);
		if (position_player_2.y >= getheight())
			player_2->set_hp(0);
		if (player_1->get_hp() <= 0 || player_2->get_hp() <= 0 ) {
			if (!is_gameover) {
				mciSendString(_T("stop ddtGame"), NULL, 0, NULL);
				mciSendString(_T("play ddtWin from 0"), NULL, 0, NULL);
			}

			is_gameover = true;
		}
		status_bar_1P.set_hp(player_1->get_hp());
		status_bar_1P.set_mp(player_1->get_mp());
		status_bar_2P.set_hp(player_2->get_hp());
		status_bar_2P.set_mp(player_2->get_mp());

		if (is_gameover) {

			pos_img_winner_bar.x += (int)(speed_winner_bar * delta);
			pos_img_winner_text.x += (int)(speed_winner_text * delta);

			if (!is_slide_out_started) {
				timer_winner_slide_in.on_update(delta);
				if (pos_img_winner_bar.x > pos_x_img_winner_bar_dst)
					pos_img_winner_bar.x = pos_x_img_winner_bar_dst;
				if (pos_img_winner_text.x > pos_x_img_winner_text_dst)
					pos_img_winner_text.x = pos_x_img_winner_text_dst;
			}
			else {
				timer_winner_slide_out.on_update(delta);
			}
		}
	}
	void on_draw(const Camera& main_camera)
	{ 
		const int N = main_camera.get_position().x / img_sky1.getwidth() > 0
			? main_camera.get_position().x / img_sky1.getwidth()
			: main_camera.get_position().x / img_sky1.getwidth() - 1;
		int X = main_camera.get_position().x + N * img_sky1.getwidth();
		/*卷轴效果*/
		/*
		N = camera.x / window_width 区分在第N+1个窗口处
		X = camera.x + N * window_width
		if( N > 0)
				N* img_sky1.getwidth() < X < (N+1)* img_sky1.getwidth()
		else if(N <=0 ) 
				(N-1)* img_sky1.getwidth() < X < N* img_sky1.getwidth()
		*/
		putimage_alpha(main_camera, pos_img_sky.x + (N) * img_sky1.getwidth(), pos_img_sky.y, &img_sky1);
		putimage_alpha(main_camera, pos_img_sky.x + (N + 1) * img_sky1.getwidth(), pos_img_sky.y, &img_sky1);
		
		
		putimage_alpha(main_camera, pos_img_sky.x, pos_img_sky.y, &img_sky2);
		putimage_alpha(main_camera, pos_img_sky.x, pos_img_sky.y, &img_sky3);
		putimage_alpha(main_camera, pos_img_sky.x, pos_img_sky.y, &img_sky4);
		
		/*putimage_alpha(main_camera,pos_img_hills.x, pos_img_hills.y, &img_hills);
		putimage_alpha(main_camera, pos_img_hills.x + img_hills.getwidth(), pos_img_hills.y, &img_hills_mirror);

		putimage_alpha(main_camera, pos_img_hills.x - img_hills.getwidth(), pos_img_hills.y, &img_hills_mirror);*/

		putimage_alpha(main_camera, pos_img_platform.x, pos_img_platform.y, &img_platform_large);//1036 width

		for (const auto& it : BIGPicture_extensionAssert_list)
			putimage_ex(main_camera, &BIGPicture, &it.dst, &it.src);

		settextcolor(RGB(255, 0, 0));
		for (const Platform& platform : platform_list)
			platform.on_draw(main_camera);

		if (is_debug) {
			settextcolor(RGB(255, 0, 0));
			outtextxy(15, 15,_T("已开启调试模式^^,按'Q' 关闭"));
		}

		player_1->on_draw(main_camera);
		player_2->on_draw(main_camera);
		for (const Bullet* bullet : bullet_list) {
			bullet->on_draw(main_camera);
		}
		if (is_gameover) {
			putimage_alpha(pos_img_winner_bar.x, pos_img_winner_bar.y, &img_winner_bar);
			putimage_alpha(pos_img_winner_text.x, pos_img_winner_text.y,
				player_1->get_hp() > 0 ? &img_1P_winner : &img_2P_winner);
		}
		else {
			status_bar_1P.on_draw();
			status_bar_2P.on_draw();
		}
	}
	void on_input(const ExMessage& msg)
	{
		player_1->on_input(msg);
		player_2->on_input(msg);

		switch(msg.message){
				case WM_KEYUP:
					if (msg.vkcode == 0x51)
						is_debug = !is_debug;
					break;	
				default:
					break;
		}
	}
	void on_exit()
	{
		delete player_1; player_1 = nullptr;
		delete player_2; player_2 = nullptr;

		is_debug = false;
		bullet_list.clear();
		main_camera.reset();
	}
private:
	POINT pos_img_sky { 0 };
	POINT pos_img_hills{ 0 };
	POINT pos_img_platform{ 125,550 };

	StatusBar status_bar_1P;
	StatusBar status_bar_2P;

	bool is_gameover = false;

	POINT pos_img_winner_bar = { 0 };
	POINT pos_img_winner_text = { 0 };
	int pos_x_img_winner_bar_dst = 0;
	int pos_x_img_winner_text_dst = 0;
	Timer timer_winner_slide_in;
	Timer timer_winner_slide_out;
	bool is_slide_out_started = false;
	int speed_winner_bar = 2;					//划出速度
	int speed_winner_text = 2;
};