/**************************************************
* File name: Engine.h
* Author: flyer<2350827470@qq.com>
* Copyright 2021. All right reserved.
* Description:
	引擎总管全局，提供服务
**************************************************/
#pragma once
#include <memory>
#include "Input.h"
#include "Dispaly.h"
#include "MsgQueue.h"
#include <vector>
#include <map>
#include <set>
#include "Object.h"
class Engine
{
	MsgQueue<InputMsg> input_queue;// for input message transit
	std::unique_ptr<Input> input_;
	std::unique_ptr<Terminal> display_;
	std::map<std::string, std::unique_ptr<Object>> objs_;// game objects' collection,can also pointe to a child class.
	std::set<std::string> to_clean_list_;//objects to clean up

	SHORT fresh_time;// millionssecond
public:
	Engine();
	Engine(const Engine&) = delete;// copy disabled
	Engine& operator=(const Engine&) = delete;
	virtual ~Engine() = default;

	/* Engine's control*/
	void add_obj(std::unique_ptr<Object> in_obj);//add game object.
	void set_fresh_time(SHORT in_time);
	void run();// start Engine
	void render();// render objects
	virtual void main_loop() {}// main logic loop

	/* Engine's service */
	//- engine related
	void debug();
	//- objects' status query service
	COORD get_obj_pos(const std::string& in_name);//get obj's postion accordding obj's name
	DrawDirect get_obj_direct(const std::string& in_name);//get obj's direct according obj's name
	void delete_obj(const std::string& in_name);//delete obj according obj's name,the remove will happen in run()'s end;
	//- input service
	std::unique_ptr<InputMsg> get_key_unblock();// get first key current frame;
	void get_keys(){} //get keys current frame;
	void get_mouse(){} //get mouse input;
	//- output service
	SMALL_RECT get_display_size() { return display_->ret_display_size(); }// get the display size;
	void set_display_size(){}
	
	/* event system */
	void deal_event();//event's splitor;

private:
	void clean_()//delete useless object.
	{
		for (auto item : to_clean_list_)
		{
			if (objs_.find(item) != objs_.end())
			{
				objs_.erase(item);
			}
		}
		to_clean_list_.clear();
	}
};

