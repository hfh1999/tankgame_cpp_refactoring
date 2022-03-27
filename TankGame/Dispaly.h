#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
const LPCWSTR Cursor_Up = L"\x1b[A";//光标向上
const LPCWSTR Cursor_Down = L"\x1b[B";
const LPCWSTR Cursor_Left = L"\x1b[D";//光标向左
const LPCWSTR Cursor_Right = L"\x1b[C";
const LPCWSTR Set_Terminal_Width132 = L"\x1b[?3h";//设定终端宽度为132
const LPCWSTR Cursor_To_RightButtom = L"\x1b[999;999H";//光标到达窗口最右下角的位置
void aquire_terminal_size(HANDLE & handle);
enum CharAttr {
	Default,
	Foreground_Bold,//高亮
	Foreground_No_Bold,//取消高亮
	Underline,//下划线
	No_Underline,//取消下划线
	Negative,//交换前后景颜色
	Positive,//取消交换
	Foreground_Black,
	Foreground_Red,
	Foreground_Green,
	Foreground_Yellow,
	Foreground_Blue,
	Foreground_Magenta,
	Foreground_Cyan,
	Foreground_White,
	Foreground_Default,
	Background_Black,
	Background_Red,
	Background_Green,
	Background_Yellow,
	Background_Blue,
	Background_Magenta,
	Background_Cyan,
	Background_White,
	Background_Default,
	Bright_Foreground_Black,
	Bright_Foreground_Red,
	Bright_Foreground_Green,
	Bright_Foreground_Yellow,
	Bright_Foreground_Blue,
	Bright_Foreground_Magenta,
	Bright_Foreground_Cyan,
	Bright_Foreground_White,
	Bright_Background_Black,
	Bright_Background_Red,
	Bright_Background_Green,
	Bright_Background_Yellow,
	Bright_Background_Blue,
	Bright_Background_Magenta,
	Bright_Background_Cyan,
	Bright_Background_White,
	Bright_Background_Default,
};
const LPCWCHAR  GetCharAttrMeta(CharAttr in_attr);
struct TermChar {//表示一个可以画在terminal任意一个几何位置的符号.
	TermChar(char in_content, CharAttr foregroud_attr_in, CharAttr backgroud_attr_in);
	LPCWSTR GetForegroudAttr();
	LPCWSTR GetBackgroudAttr();
	char GetContent();
private:
	char content;
	CharAttr foregroud_attribute;
	CharAttr backgroud_attribute;

};

/*提供最基本的作图功能*/
class Terminal {//这里的Terminal类应当是始终只创建一个
private:
	HANDLE terminal_out;
	COORD cursor_pos;//光标的位置

protected:
	SMALL_RECT display_size;//终端的显示大小
public:
	static enum Direct { Up, Down, Left, Right };

	Terminal();//创建一个terminal类
	virtual ~Terminal();
	void draw(SHORT x, SHORT y, TermChar ch);//写一个符号
	void drawBlank(SHORT x, SHORT y, CharAttr color);
	void erase(SHORT x, SHORT y);//擦除一个符号
	void DrawLine(COORD start_point, Direct direct, SHORT lenth, TermChar ch);//画一条正直线
	void DrawLineBlank(COORD start_point, Direct direct, SHORT lenth, CharAttr color);
	void DrawRect(COORD start_point, SHORT cols, SHORT rows, TermChar ch);//画一个空矩形
	void DrawRectBlank(COORD start_point, SHORT cols, SHORT rows, CharAttr color);
	void CopyRect();//复制一个Rect到指定位置
	void CutRect();//剪切一个Rect到指定位置
	void cls();//清除屏幕
	SMALL_RECT ret_display_size() { return display_size; }
};
