/**************************************************
* File name: Input.h
* Author: flyer<2350827470@qq.com>
* Copyright 2021. All right reserved.
* Description:
	引擎的接受输入的模块,为一个独立的线程
	不可复制
	不可作为基类
**************************************************/
#include <Windows.h>
#include <thread>
#include "Msg.h"
#include "MsgQueue.h"
#pragma once
class InputMsg {
	bool is_pressed;
	WORD repeat_count;
	WORD virtual_key_code;
	DWORD control_key_state;
public:
	InputMsg(bool in_is_pressed, WORD in_repeat_count, WORD in_virtual_key_code, DWORD in_control_key_state)
		:is_pressed(in_is_pressed),repeat_count(in_repeat_count),virtual_key_code(in_virtual_key_code),control_key_state(in_control_key_state)
	{
	}
	bool is_down() {
		return is_pressed;
	}
	WORD ret_count() {
		return repeat_count;
	}
	WORD ret_key_code() {
		return virtual_key_code;
	}
	DWORD ret_control_state() {
		return control_key_state;
	}
};
void input_loop_func(HANDLE in_handle, MsgQueue<InputMsg>& in_msgQ, bool& in_loop_flag);
class Input
{
	HANDLE term_in;// input handle.
	MsgQueue<InputMsg>& msgQ;// message queue for threads' communication.
	bool loop_flag;// for terminate the thread.
	std::thread input_loop;// the thread
public:
	Input(MsgQueue<InputMsg>& in_msgQ);
	~Input();
	Input(const Input&) = delete;//copy constructor is unavailable
	Input& operator=(const Input&) = delete;

	void start(); // start input thread to get input.
	void stop();
};

