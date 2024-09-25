#pragma once
#include"camera.h"
#include<graphics.h>
#include"vector2.h"
#include"animation.h"
#include"player_id.h"
#include"platform.h"
#include"Bullet.h"
#include"particle.h"
extern IMAGE img_1P_cursor;
extern IMAGE img_2P_cursor;

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;
extern Atlas atlas_run_effect;
extern Atlas atlas_jump_effect;
extern Atlas atlas_land_effect;
extern bool is_debug;
extern Camera main_camera;
class Player {
public:
	Player(bool facing_right = true):is_facing_right(facing_right) {
		current_animation = facing_right 
			? &animation_idle_right
			:  &animation_idle_left;

		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]() {
			can_attack = true;
			});

		timer_invulnerable.set_wait_time(750);
		timer_invulnerable.set_one_shot(true);
		timer_invulnerable.set_callback([&]() {
			is_invulnerable = false;
			});

		timer_invulnerable_blink.set_wait_time(75);
		timer_invulnerable_blink.set_callback([&]() {
			//std::cout << "Tick() from timer_invulnerable_blink" << std::endl;
			is_showing_sketch_frame = !is_showing_sketch_frame;
			});

		timer_getshotted.set_wait_time(250);
		timer_getshotted.set_callback([&]() {
			//std::cout << "Tick() from timer_getshotted" << std::endl;
			is_getShotted = false;
			velocity.x = 0;
			});

		timer_run_effect_generation.set_wait_time(75);
		timer_run_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 45);
			});

		timer_die_effect_generation.set_wait_time(35);
		timer_die_effect_generation.set_callback([&]()
			{
				Vector2 particle_position;
				IMAGE* frame = atlas_run_effect.get_image(0);
				particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
				particle_position.y = position.y + size.y - frame->getheight();
				particle_list.emplace_back(particle_position, &atlas_run_effect, 150);
			});

		animation_jump_effect.set_atlas(&atlas_jump_effect);
		animation_jump_effect.set_interval(50);
		animation_jump_effect.set_loop(false);
		animation_jump_effect.set_callback([&]() {
			is_jump_effect_visible = false;
			});

		animation_land_effect.set_atlas(&atlas_land_effect);
		animation_land_effect.set_interval(50);
		animation_land_effect.set_loop(false);
		animation_land_effect.set_callback([&]() {
			is_land_effect_visible = false;
			});

		timer_cursor_visibility.set_wait_time(2000);
		timer_cursor_visibility.set_one_shot(true);
		timer_cursor_visibility.set_callback([&]() {
			is_cursor_visible = false;
			});

		
	};
	~Player() = default;
	virtual void on_run(float distance) {
		if (is_attacking_ex)
			return;

		position.x += distance;
		timer_run_effect_generation.resume();
	}

	virtual void on_jump(){
		//std::cout << "Jumping......------------------------=================\n";
		if (!can_double_jump || is_attacking_ex)
			return;

		if (jump_count < 2) {
			//std::cout << jump_count<<'\n';
			velocity.y = jump_velocity;
			is_jump_effect_visible = true;
			animation_jump_effect.reset();

			IMAGE* effect_frame = animation_jump_effect.get_frame();
			position_jump_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
			position_jump_effect.y = position.y + size.y - effect_frame->getheight();
			++jump_count;
			if (jump_count == 2)
				can_double_jump = false;
		}
		
	}

	virtual void on_land() {
		is_land_effect_visible = true;
		animation_land_effect.reset();

		IMAGE* effect_frame = animation_land_effect.get_frame();
		position_land_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
		position_land_effect.y = position.y + size.y - effect_frame->getheight();
		jump_count = 0;
		can_double_jump = true;
	}

	virtual void on_update(int delta) {
		//std::cout << "Player " << (int)id + 1 << " Updateing...\n";
		int direction = is_right_key_down - is_left_key_down;

		if (direction != 0) {
			if(!is_attacking_ex)
				is_facing_right = direction > 0;
			current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
			float distance = direction * run_velocity * delta;
			on_run(distance);
		}
		else {
			current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
			timer_run_effect_generation.pause();
		}
		if (is_attacking_ex)
			current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;

		if (hp <= 0) {
			current_animation = last_hurt_direction.x < 0 ? &animation_die_left : &animation_die_right;
			//std::cout << "AnimationSize : "<<current_animation->get_size() << std::endl;
		}

		current_animation->on_update(delta);
		animation_jump_effect.on_update(delta);
		animation_land_effect.on_update(delta);

		timer_attack_cd.on_update(delta);
		timer_invulnerable.on_update(delta);
		timer_getshotted.on_update(delta);
		timer_invulnerable_blink.on_update(delta);
		timer_run_effect_generation.on_update(delta);
		timer_cursor_visibility.on_update(delta);

		if (hp <= 0) {
			timer_die_effect_generation.on_update(delta);
			timer_getshotted.pause();
		}
			

		particle_list.erase(std::remove_if(
			particle_list.begin(), particle_list.end(),
			[](const Particle& particle) {
				return !particle.check_valid();
			}),
			particle_list.end());

		/*粒子系统*/
		for (Particle& particle : particle_list)
			particle.on_update(delta);
		if (is_showing_sketch_frame)
			sketch_image(current_animation->get_frame(), &img_sketch);

		move_and_collide(delta);
	}

	virtual void on_draw(const Camera& camera) {
		if (is_jump_effect_visible)
			animation_jump_effect.on_draw(camera, (int)position_jump_effect.x, (int)position_jump_effect.y);
		if (is_land_effect_visible)
			animation_land_effect.on_draw(camera, (int)position_land_effect.x, (int)position_land_effect.y);
		for (const Particle& p : particle_list)
			p.on_draw(camera);
		//std::cout << (int)id + 1<<" : " << hp<<" "<< (int)is_invulnerable << (int)is_showing_sketch_frame << std::endl;
		if (hp > 0 && is_invulnerable && is_showing_sketch_frame) {
			putimage_alpha(camera, (int)position.x, (int)position.y, &img_sketch);
		}	
		else
			current_animation->on_draw(camera, (int)position.x, (int)position.y);
		
		if (is_cursor_visible) {
			switch (id)
			{
			case PlayerID::P1: 
				putimage_alpha(camera, (int)(position.x + (size.x - img_1P_cursor.getwidth()) / 2),
					(int)(position.y - img_1P_cursor.getheight()) , &img_1P_cursor);
				break;
			case PlayerID::P2:
				putimage_alpha(camera, (int)(position.x + (size.x - img_2P_cursor.getwidth()) / 2),
					(int)(position.y - img_2P_cursor.getheight()), &img_2P_cursor);
				break;
			default:
				break;
			}
		}
		if (is_debug) {
			setlinecolor(RGB( 0, 125, 255));
			const Vector2& pos_camera = camera.get_position();
			rectangle((int)(position.x - pos_camera.x), (int)(position.y - pos_camera.y),
				(int)(position.x + size.x - pos_camera.x), (int)(position.y + size.y - pos_camera.y));
			//circle((int)(position.x - pos_camera.x), (int)(position.y - pos_camera.y), 1);
			line((int)(position.x - pos_camera.x), (int)position.y + size.y - 10, (int)(position.x + size.x - pos_camera.x), (int)position.y + size.y - 10);
			line((int)(position.x - pos_camera.x), (int)position.y +  10, (int)(position.x + size.x - pos_camera.x), (int)position.y + 10);
		}
	}

 	virtual void on_input(const ExMessage& msg) {
		switch (msg.message)
		{
			case WM_KEYDOWN: 
			{
				switch (id)
				{
					case PlayerID::P1: {
							switch (msg.vkcode) 
							{
							case 0x41:// 'A'
								is_left_key_down = true;
								break;
							case 0x44:// ' D '
								is_right_key_down = true;
								break;
							case 0x57:// 'W'
								{
									on_jump();
									break;
								}
							case 0x46://	'F'
								if (can_attack) {
									on_attack();
									can_attack = false;
									//std::cout << "P1 { "<<this->position.x << " , " << this->position.y << " }\n";
									timer_attack_cd.restart();
								}
								break;
							case 0x47://'G'
								if (mp >= 100) {
									on_attack_ex();
									//std::cout << "Shoot!" << std::endl;
									mp = 0;
								}
								break;
							}
							break;
					}
						break;
					case PlayerID::P2: {
						switch (msg.vkcode)
						{
						case VK_LEFT: // ' <- '
							is_left_key_down = true;
							break;
						case VK_RIGHT: // ' -> '
							is_right_key_down = true;
							break;
						case VK_UP:
						{
							on_jump();
							break;
						}
						case VK_OEM_PERIOD: // ' . '
							if (can_attack) {
								on_attack();
								can_attack = false;
								//std::cout << "P2 { " << this->position.x <<" , " << this->position.y << " }\n";
								timer_attack_cd.restart();
							}
							break;
						case VK_OEM_2: // ' / '
							if (mp >=  100) {
								on_attack_ex();
								mp = 0;
							}
							break;
						}
						break;
					}
						break;
					}
			}
			break;

			case WM_KEYUP: 
			{
				switch (id)
				{
				case PlayerID::P1: {
					switch (msg.vkcode)
					{
						case 0x41:// 'A'
								is_left_key_down = false;
								break;
						case 0x44:// ' D '
								is_right_key_down = false;
								break;
					}
					break;
				}
					break;
				case PlayerID::P2: {
					switch (msg.vkcode)
					{
						case VK_LEFT: // ' <- '
								is_left_key_down = false;
								break;
						case VK_RIGHT: // ' -> '
								is_right_key_down = false;
								break;
					}
					break;
				}
					break;
				}
			}
			break;
		}
	}

	void set_id(PlayerID id) {
		this->id = id;
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

	void make_invulnerable() {
		is_invulnerable = true;
		timer_invulnerable.restart();
	}
	
	inline void make_getShotted() {
		is_getShotted = true;
		timer_getshotted.restart();
	}
	virtual void on_attack(){}
	virtual void on_attack_ex(){}

	void move_and_collide(int delta) {
		float last_velocity_y = velocity.y;
		velocity.y += gravity * delta;
		position += velocity * (float)delta;

		if (hp <= 0)
			return;
		/*
			2024 9/9留
			我说怎么一按跳跃键就无敌，原来velocit y.y > 0本来就是false^^ 下面代码也不用走了
		*/
		
		for (const Platform& platform : platform_list)//平台碰撞逻辑 从下到上可以穿过?
		{
			
			if (velocity.y > 0) //WHen you fall
			{
				const Platform::CollisionShape& shape = platform.shape;
				bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left))
					<= size.x + (shape.right - shape.left);
				bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);
				if (is_collide_x && is_collide_y) 
				{
					float delta_pos_y = velocity.y * delta;
					float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;
					if (last_tick_foot_pos_y <= shape.y)
					{
						position.y = shape.y - size.y;
						velocity.y = 0;
						//判断落地条件
						if (last_velocity_y != 0){
							on_land();
						}
					}
				}
			}
			if (platform.is_wall) {
				for (const auto& it : platform.wall_lists) {
					float delta_pos_y = velocity.y * delta;
					float last_tick_foot_pos_y = position.y + 10 - delta_pos_y;
					bool is_collide_x = (max(position.x + size.x, it.right) - min(position.x, it.left))
						<= size.x + std::abs(it.right - it.left);
					bool is_under_wall = last_tick_foot_pos_y > it.height;
					bool is_hit_wall = position.y + 10 < it.height && last_tick_foot_pos_y > it.height;
					//not under the wall
					if (is_collide_x && !is_under_wall && !is_hit_wall) {
						const Platform::CollisionShape& shape = platform.shape;
						bool is_collide_yL_DOWN = (position.y + size.y - 10 > shape.y) && (position.y + size.y - 10 < it.height);
						bool is_collide_yL_UP = (position.y + 10 > shape.y) && (position.y + 10 < it.height);

						if ((is_collide_yL_DOWN || is_collide_yL_UP) && is_collide_x) {
							bool leftOrRight = (position.x + size.x >= it.left && position.x <= it.left) ? true : false;
							position.x = leftOrRight ? it.left - size.x : it.right;
						}
					}else if(is_collide_x && is_under_wall){
						if (is_hit_wall) {
							position.y = it.height;
							velocity.y = 0;
						}
					}
				}
			}
		}
		
		/*这个别放if (velocity.y > 0)里面,不然就是"为什么我跳跃有无敌效果啊^^?"了*/
		check_bullet_collide();
	}

	inline void check_bullet_collide(){
			/*检测非无敌情况*/
			if (!is_invulnerable) {
				//std::cout << "---------------------------------------\n";
				for (Bullet* bullet : bullet_list) {
					if (!bullet->get_valid() || bullet->get_collide_target() != id) {
						continue;
					}

					if (bullet->check_collision(position, size)) {//检测子弹碰撞后的处理
						//std::cout << "GetShotted" <<'\n';
						//std::cout << "This bullet is valid----------------------------------\n";
						make_invulnerable();

						bullet->on_collide();
						bullet->set_valid(false);
						hp -= bullet->get_damage();
						make_getShotted();
						if (is_getShotted)// 受击的话就设置反方向的速度
						{
							//std::cout << "Start X velocity" << std::endl;
							normal_hurted_direction = bullet->get_position() - position;
							velocity.x = normal_hurted_direction.x <= 0 ? bullet->get_shotted().x : -bullet->get_shotted().x;
							velocity.y = bullet->get_shotted().y;
						}

						last_hurt_direction = bullet->get_position() - position;
						if (hp <= 0) {
							velocity.x = last_hurt_direction.x < 0 ? 1.20f : -1.20f;	//飞起来!
							velocity.y = -1.0f;
						}
						//std::cout << "Player: " << (int)this->id + 1 << " hp -- " << std::endl;
					}
				}
				//std::cout << "---------------------------------------\n";
			}
	}
	int get_hp() const {
		return hp;
	}

	int get_mp() const {
		return mp;
	}

	void set_hp(int val) {
		hp = val;
	}

protected:
	const float run_velocity = 0.55f;
	const float gravity = 1.6e-3f;
	const float jump_velocity = -0.7f;

	


protected:
	Animation animation_idle_left;
	Animation animation_idle_right;
	Animation animation_run_left;
	Animation animation_run_right;
	Animation animation_jump_effect;
	Animation animation_land_effect;
	Animation animation_die_left;
	Animation animation_die_right;
	Animation animation_attack_ex_left;
	Animation animation_attack_ex_right;
	Animation* current_animation = nullptr;

	bool is_jump_effect_visible = false;			//跳跃动画是否可见
	bool is_land_effect_visible = false;

	bool is_left_key_down = false;					//判断按键按下情况
	bool is_right_key_down = false;
	bool is_facing_right = true;
	int jump_count = 0;								//跳跃相关
	bool can_double_jump = true;

	PlayerID id = PlayerID::P1;
	int mp = 0;
	int hp = 100;
	Vector2 position_jump_effect;					//跳跃动画播放位置
	Vector2 position_land_effect;

	Vector2 position;
	Vector2 velocity;
	Vector2 size;
	

	

	int attack_cd = 500;									//人物功能
	bool can_attack = true;
	Timer timer_attack_cd;//定时器

	IMAGE img_sketch;
	bool is_attacking_ex = false;
	bool is_invulnerable = false;						// 角色是否处于无敌状态
	bool is_showing_sketch_frame = false;		// 当前帧是否应该显示剪影
	Timer timer_invulnerable;							//	无敌定时器
	Timer timer_invulnerable_blink;

	std::vector<Particle> particle_list;				//粒子对象列表
	Timer timer_run_effect_generation;			//跑动特效粒子发射定时器
	Timer timer_die_effect_generation;			//死亡特效粒子发射定时器
	
	bool is_cursor_visible = true;					//光标可见性
	Timer timer_cursor_visibility;

	Vector2 last_hurt_direction;						//死之前的受击方向
	Vector2 normal_hurted_direction;			//受击方向
	bool is_getShotted = false;						//是否受击
	Timer timer_getshotted;
};