#include "Input.h"
#include <iostream>
Input::Input(MsgQueue<InputMsg>& in_msgQ) :
	term_in(GetStdHandle(STD_INPUT_HANDLE))
	, msgQ(in_msgQ)
	, loop_flag(false)
	, input_loop(std::thread())
{
	/* Disable quick edit mode to  prevent stucked when mouse click. */
	DWORD prev_mode;
	if (!GetConsoleMode(term_in, &prev_mode))
	{
		std::cout << "Failed to get mode:" << GetLastError() << std::endl;
		getchar();
	}
	prev_mode = (prev_mode & ~ENABLE_QUICK_EDIT_MODE) | ENABLE_EXTENDED_FLAGS;
	if (!SetConsoleMode(term_in, prev_mode))
	{
		std::cout << "Failed to disable quick edit mode:" << GetLastError() << std::endl;
		std::cout << prev_mode;
		getchar();
	}
}

Input::~Input()
{
	stop();
	std::cout << "[TermioApp]input loop terminated.";
}

void Input::start()
{
	loop_flag = true;
	input_loop = std::thread(input_loop_func, term_in, std::ref(msgQ), std::ref(loop_flag));
}

void Input::stop()
{
	if (loop_flag)
	{
		input_loop.join();
		loop_flag = false;
	}
	else {}
}

void input_loop_func(HANDLE in_handle, MsgQueue<InputMsg>& in_msgQ, bool & in_loop_flag) {

	/* reference to:https://docs.microsoft.com/en-us/windows/console/readconsoleinput */

	INPUT_RECORD irInBuf[128];
	DWORD cNumRead;//num of events has been read
	DWORD queueLen;
	while (in_loop_flag)
	{
		/*	get some event */
		//get input 
		//call input
		ReadConsoleInput(in_handle, irInBuf, 128, &cNumRead);
		for (DWORD counter = 0; counter < cNumRead; counter++)
		{
			auto tmp_event = irInBuf[counter];
			auto event = tmp_event.Event.KeyEvent;
			switch (irInBuf[counter].EventType)
			{
			case FOCUS_EVENT:
				break;
			case KEY_EVENT:
				//std::cout << "[eventloop]key event\n";
				//执行一次按键测时函数
				//std::cout << "counter = " << repeat_counter;
				//	std::cout << "counter" << tmp.Event.KeyEvent.wRepeatCount;
				//::printf("send msg!");
				//::printf("count = %d\n", event.wRepeatCount);
				in_msgQ.send(InputMsg(event.bKeyDown,event.wRepeatCount,event.wVirtualKeyCode,event.dwControlKeyState));//send Input Msg.	
				break;
			case MOUSE_EVENT:
				//std::cout << "[eventloop]mouse event\n";
				break;
			case WINDOW_BUFFER_SIZE_EVENT:
				//std::cout << "[eventloop]window buffer size event.\n";
				break;
				//case MENU_EVENT: //内部使用,此处忽略
			default:
				break;
			}
		}
		//std::cout << cNumRead << "events readed" << std::endl;
	}
}
