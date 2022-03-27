#include <iostream>
#include "Engine.h"
#include "Object.h"
#include <sstream>

//test bullet class 
class bullet :public Object {
	int direct; // 1,2,3,4,-1:up,right,down,left,stop
	int v; // velocity;
public:
	bullet(const std::string& in_name) :
		Object(in_name)
		,direct(-1)
		,v(0) 
	{
		//std::cout << "produce bullet!!!";
		set_draw("bullet.draw"); // set bullet as its draw.
	}
	void pre_run() override {
		//std::cout << "set visualale";
		set_visualable(); // is visualable.
	}
	void run() override {
		switch (direct)
		{
		case 1:
			move_up(v);
			break;
		case 2:
			move_right(v);
			break;
		case 3:
			move_down(v);
			break;
		case 4:
			move_left(v);
			break;
		case -1:
			break;
		default:
			//std::cout << "bullet direct is error.";
			break;
		}
		
		//need to delet it self here.
		auto display_size = get_engine()->get_display_size();
		auto self_pos = ret_origin_point();
		if (self_pos.X <= 0 || self_pos.X >= display_size.Right || self_pos.Y <= 0 || self_pos.Y >= display_size.Bottom)
		{
			//std::cout << "delete:" << ret_name();
			delete_self(); // delete ,because the bullet is no more needed.
		}

	}
	void set_dirct(int d) {
		direct = d;
	}
	void set_v(int in_v) {
		v = in_v;
	}

};

//test tank class
class mytank:public Object {
	bool right_moved;
	bool left_moved;
	bool up_moved;
	bool down_moved;
	int bullet_id;
public:
	mytank(const std::string& in_name):Object(in_name),bullet_id(3){}
	void pre_run()override {
		set_visualable();
	}
	void run() override {
		//move_right(1);
		//::printf("hello world!!!");
		auto ptr = get_engine();
		auto key_r = ptr->get_key_unblock();
		if (key_r) {
			if (key_r->ret_key_code() == VK_RIGHT && key_r->is_down())
			{
				right_moved = true;
			}
			if (key_r->ret_key_code() == VK_RIGHT && !key_r->is_down())
			{
				right_moved = false;
			}
			if (key_r->ret_key_code() == VK_LEFT && key_r->is_down())
			{
				left_moved = true;
			}
			if (key_r->ret_key_code() == VK_LEFT && !key_r->is_down())
			{
				left_moved = false;
			}
			if (key_r->ret_key_code() == VK_UP && key_r->is_down())
			{
				up_moved = true;
			}
			if (key_r->ret_key_code() == VK_UP && !key_r->is_down())
			{
				up_moved = false;
			}
			if (key_r->ret_key_code() == VK_DOWN && key_r->is_down())
			{
				down_moved = true;
			}
			if (key_r->ret_key_code() == VK_DOWN && !key_r->is_down())
			{
				down_moved = false;
			}
			if (key_r->ret_key_code() == VK_SPACE && !key_r->is_down())
			{
				//ptr->delete_obj("2");
				//test ,craete bullets when the space is pressed.
				std::stringstream ss;
				ss.str("");
				ss << bullet_id;
				auto bullet_name = ss.str();
				//std::cout << "name = " << bullet_id;
				auto self_pos = ptr->get_obj_pos("1");
				auto tmp_bullet = std::unique_ptr<bullet>(new bullet(bullet_name));
				tmp_bullet->set_v(2);// set velocity
				// set bullet direct
				if (this->ret_direct() == DrawDirect::Up)
					tmp_bullet->set_dirct(1);
				else if (this->ret_direct() == DrawDirect::Right)
					tmp_bullet->set_dirct(2);
				else if (this->ret_direct() == DrawDirect::Down)
					tmp_bullet->set_dirct(3);
				else
					tmp_bullet->set_dirct(4);
				tmp_bullet->move(self_pos);// set bullet init position
				ptr->add_obj(std::move(tmp_bullet)); //add bullet.
				bullet_id++;
				
			}
		}
		if (right_moved == true)
		{
			turn_right();
			move_right(1);
		}
		if (left_moved == true)
		{
			turn_left();
			move_left(1);
		}
		if (up_moved == true)
		{
			turn_up();
			move_up(1);
		}
		if (down_moved == true)
		{
			turn_down();
			move_down(1);
		}
		//ptr->debug();
	}
};
//enemy just walk in a round.
class enemy_tank:public Object {
	int right_steps;
	int is_right;
	int down_steps;
	int is_down;
public:
	enemy_tank(const std::string& in_name):
		Object(in_name),
		right_steps(0),
		is_right(1),
		down_steps(0),
		is_down(0)
	{
	}
	void run() override {
		if (is_right == 1 && is_down == 0) {
			if (right_steps < 30)
			{
				turn_right();
				move_right(1);
				right_steps++;
			}
			else
			{
				is_right = 0;
				is_down = 1;
			}
		}
		else if (is_right == 0 && is_down == 1) {
			if (down_steps < 30)
			{
				turn_down();
				move_down(1);
				down_steps++;
			}
			else
			{
				is_right = -1;
				is_down = 0;
			}
		}
		else if (is_right == -1 && is_down == 0) {
			if (right_steps > 0)
			{
				turn_left();
				move_left(1);
				right_steps--;
			}
			else
			{
				is_right = 0;
				is_down = -1;
			}
		}
		else if (is_right == 0 && is_down == -1) {
			if (down_steps > 0)
			{
				turn_up();
				move_up(1);
				down_steps--;
			}
			else
			{
				is_right = 1;
				is_down = 0;
			}
		}
	}
};
int main()
{
	Engine e;
	e.set_fresh_time(25);
	auto tank = std::unique_ptr<Object>(new mytank("1"));
	tank->set_draw("tank.draw");
	auto enemy = std::unique_ptr<Object>(new enemy_tank("2"));
	enemy->set_visualable();
	enemy->set_draw("tank.draw");
	e.add_obj(std::move(tank));
	e.add_obj(std::move(enemy));
	e.run();
}

