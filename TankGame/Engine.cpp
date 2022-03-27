#include "Engine.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <utility>
#include <time.h>
Engine::Engine() :
	input_queue(),
	input_(new Input(input_queue)),
	display_(new Terminal()),
	objs_(),
	fresh_time(100)
{
	input_->start();
}

void Engine::add_obj(std::unique_ptr<Object> in_obj) {
	in_obj->set_engine(this);
	//objs.push_back(std::move(in_obj));
	objs_[in_obj->ret_name()] = std::move(in_obj);
	//::printf("add obj.\n");
	//::printf("num = %d\n", objs.size());
}

//add game object.

void Engine::set_fresh_time(SHORT in_time) { fresh_time = in_time; }

void Engine::deal_event()
{
	/* fetch msg from msg queue ,and distribute to every obj*/

	/*input msg and event*/
	// msgs <= input_queue // need no block.
	// for msg in msgs{
	//	e = event(msg)
	//	e send to objs
	//  put msg to objs's input queue.
	//}

	/*may more msgs and event*/
	//...
	//...
}

void Engine::run() {
	//for (auto & item : objs_)
	//{
	//	//item->pre_run();
	//	item.second->pre_run();
	//}
	const long dt = fresh_time;
	clock_t end_time, start_time;
	clock_t dur;
	while(true){
		start_time = clock();
		deal_event();//event system
		for (auto& item : objs_)
		{
			if (!item.second->ret_is_pre_run())
			{
				item.second->pre_run();
				item.second->set_pre_run();
			}
			item.second->run();
		}
		main_loop();
		render();
		clean_();
		end_time = clock();
		
		dur = end_time - start_time;
		if (dur < dt)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(fresh_time-dur));// render once every second;
		}
		else {
			std::cout << ".";
		}
			//std::this_thread::sleep_for(std::chrono::milliseconds(fresh_time));// render once every second;

	}
}

COORD coord_trans_(COORD in_coord, DrawDirect direct) {
	if (in_coord.X == 0 && in_coord.Y == 0)
		return COORD{ 0,0 };
	else
	{
		switch (direct - DrawDirect::Up)
		{
		case 0: //坐标不变
			return in_coord;
			break;
		case 2://顺时针180度
			return COORD{ -in_coord.X,-in_coord.Y };
			break;
		case 3://逆时针90度
			return COORD{ in_coord.Y,-in_coord.X };
			break;
		case 1://顺时针90度
			return COORD{ -in_coord.Y,in_coord.X };
			break;

		}
	}
}
void Engine::render() {
	//::printf("render here.\n");
		//print the renderDrawables.
//	std::cout << "renderDrawables... obj1,obj2,obj3\n";
	for (auto& item : objs_)
	{
		if (item.second->ret_visualable())
		{
			auto len = item.second->ret_relative_pos()->size();
			auto pos_array = item.second->ret_relative_pos();
			auto char_array = item.second->ret_content();
			auto origin_point_x = item.second->ret_origin_point().X;
			auto origin_point_y = item.second->ret_origin_point().Y;
			auto old_point_x = item.second->ret_old_point().X;
			auto old_point_y = item.second->ret_old_point().Y;
			auto direct = item.second->ret_direct();
			auto direct_old = item.second->ret_old_direct();

			if (!item.second->ret_static())
			{

				for (int index = 0; index < len; index++)
				{

					auto relative_coord_to_erase = coord_trans_(COORD{ (*pos_array).at(index).X, (*pos_array).at(index).Y}, direct_old);
					display_->erase(relative_coord_to_erase.X + old_point_x, old_point_y + relative_coord_to_erase.Y);
				}
			}
			if (!item.second->ret_is_cleaning())//when is cleaning ,not to draw new position.
			{
				for (int index = 0; index < len; index++)
				{
					auto relative_coord_to_draw = coord_trans_(COORD{ (*pos_array).at(index).X, (*pos_array).at(index).Y }, direct);
					display_->draw(relative_coord_to_draw.X + origin_point_x, relative_coord_to_draw.Y + origin_point_y, char_array->at(index));
				}
			}
			item.second->set_no_point_repeat();//important!
			item.second->set_no_direct_repeat();//important * 2!
			//draw(200,200,char_array[0]);
		}
	}
}

void Engine::debug() {
	::printf("this is debug from Engine.\n");
}

//objects' status query service

COORD Engine::get_obj_pos(const std::string & in_name) {
	return objs_[in_name]->ret_origin_point();
}

//get obj's postion accordding obj's name

DrawDirect Engine::get_obj_direct(const std::string & in_name) {
	return objs_[in_name]->ret_direct();
}

//get obj's direct according obj's name

void Engine::delete_obj(const std::string & in_name) {
	//can be call multipy times in a run() loop;
	to_clean_list_.insert(in_name);
	if (objs_.find(in_name) != objs_.end())
	{
		objs_[in_name]->set_cleaning();// wait for render() to clean the memory and remove from display;
	}

}

//input service

std::unique_ptr<InputMsg> Engine::get_key_unblock()
{
	auto msg_ptr = input_queue.recv_unblock();
	//::printf("recieve from queue.");
	if (msg_ptr) {
		std::unique_ptr<InputMsg> ret_msg(new InputMsg(msg_ptr->get_load_data()));//copy here.
		return ret_msg;
	}
	else {
		return std::unique_ptr<InputMsg>(nullptr);
	}
}
