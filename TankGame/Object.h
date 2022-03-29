/**************************************************
* File name: Object.h
* Author: flyer<2350827470@qq.com>
* Copyright 2021. All right reserved.
* Description:
	引擎的游戏对象
**************************************************/
#pragma once
#include <iostream>
#include <Windows.h>
#include <vector>
#include "Dispaly.h"
enum DrawDirect { Up, Right, Down, Left };
class Engine;
class KeyEvent;
class Object
{
	typedef std::shared_ptr<std::vector<COORD>>	 rpos_ptr_type;
	typedef std::shared_ptr<std::vector<TermChar>>content_type ;

	Engine* engine_ptr;

	bool is_pre_run;// for prerun()
	std::string name;// 为了辨识drawable
	bool is_cleaning;// if in clean period
	bool is_managed;// 是否已经被显示接管
	bool is_static;// 静态物体不需要重复渲染
	bool visualable;//可见性
	DrawDirect direct;//表示图形的朝向
	DrawDirect old_direct; //老朝向
	COORD origin_point;//原点坐标
	COORD old_point;
	bool is_point_repeat;//repeat to overwrite old_point in a frame?
	bool is_direct_repeat;//repeat to overwrite old_direct in a frame?
	rpos_ptr_type relative_pos;// share_ptr for share.
	content_type content;//share_ptr
public:
	Object(const std::string& in_name);
	virtual ~Object() = default;
	Object(const Object&) = delete;
	Object& operator= (const Object&) = delete;

	virtual void pre_run();
	virtual void run();
	void set_engine(Engine* in_engine_ptr);
	Engine* get_engine();//use engine's service through this pointer.
	void set_draw(const std::string& filename);//construct new game obj from .draw file.
	std::string ret_name();
	void delete_self();//delet itself.(convenient way to call engine->delet_objs)

	/*game objects' actions.*/
	void turn_left();
	void turn_right();
	void turn_up();
	void turn_down();
	void move_left(SHORT step);
	void move_right(SHORT step);
	void move_up(SHORT step);
	void move_down(SHORT step);
	void move(COORD in_coord);//绝对移动
	void set_visualable(); // 让图形可见
	void set_unvisualable();// 让图形不可见
	void set_static(bool flag);
	void set_no_point_repeat();//temprory solution.
	void set_no_direct_repeat();//same
	void set_cleaning();// set the state as cleaning,for engine to clean up 
	void set_pre_run() { is_pre_run = true; }

	/*get information about the object*/
	bool ret_visualable();
	rpos_ptr_type ret_relative_pos() { return relative_pos; }
	content_type ret_content() { return content; }
	COORD ret_origin_point() { return origin_point; }
	COORD ret_old_point() { return old_point; }
	DrawDirect ret_direct() { return direct; }
	DrawDirect ret_old_direct() { return old_direct; }
	bool ret_static() { return is_static; }
	bool ret_is_cleaning() { return is_cleaning; }
	bool ret_is_pre_run() { return is_pre_run; }


	/*event*/
	virtual void key_event(KeyEvent* ev) {}// key's event
};

