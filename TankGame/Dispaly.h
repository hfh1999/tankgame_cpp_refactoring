#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
const LPCWSTR Cursor_Up = L"\x1b[A";//�������
const LPCWSTR Cursor_Down = L"\x1b[B";
const LPCWSTR Cursor_Left = L"\x1b[D";//�������
const LPCWSTR Cursor_Right = L"\x1b[C";
const LPCWSTR Set_Terminal_Width132 = L"\x1b[?3h";//�趨�ն˿��Ϊ132
const LPCWSTR Cursor_To_RightButtom = L"\x1b[999;999H";//��굽�ﴰ�������½ǵ�λ��
void aquire_terminal_size(HANDLE & handle);
enum CharAttr {
	Default,
	Foreground_Bold,//����
	Foreground_No_Bold,//ȡ������
	Underline,//�»���
	No_Underline,//ȡ���»���
	Negative,//����ǰ����ɫ
	Positive,//ȡ������
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
struct TermChar {//��ʾһ�����Ի���terminal����һ������λ�õķ���.
	TermChar(char in_content, CharAttr foregroud_attr_in, CharAttr backgroud_attr_in);
	LPCWSTR GetForegroudAttr();
	LPCWSTR GetBackgroudAttr();
	char GetContent();
private:
	char content;
	CharAttr foregroud_attribute;
	CharAttr backgroud_attribute;

};

/*�ṩ���������ͼ����*/
class Terminal {//�����Terminal��Ӧ����ʼ��ֻ����һ��
private:
	HANDLE terminal_out;
	COORD cursor_pos;//����λ��

protected:
	SMALL_RECT display_size;//�ն˵���ʾ��С
public:
	static enum Direct { Up, Down, Left, Right };

	Terminal();//����һ��terminal��
	virtual ~Terminal();
	void draw(SHORT x, SHORT y, TermChar ch);//дһ������
	void drawBlank(SHORT x, SHORT y, CharAttr color);
	void erase(SHORT x, SHORT y);//����һ������
	void DrawLine(COORD start_point, Direct direct, SHORT lenth, TermChar ch);//��һ����ֱ��
	void DrawLineBlank(COORD start_point, Direct direct, SHORT lenth, CharAttr color);
	void DrawRect(COORD start_point, SHORT cols, SHORT rows, TermChar ch);//��һ���վ���
	void DrawRectBlank(COORD start_point, SHORT cols, SHORT rows, CharAttr color);
	void CopyRect();//����һ��Rect��ָ��λ��
	void CutRect();//����һ��Rect��ָ��λ��
	void cls();//�����Ļ
	SMALL_RECT ret_display_size() { return display_size; }
};
