// need to change to windows' call 
#include "Dispaly.h"
#include <time.h>
void aquire_terminal_size(HANDLE & handle)
{
	using std::wcout;
	using std::cout;
	using std::wcin;
	using std::string;
	wcout << Set_Terminal_Width132;
	//	wcout << Cursor_To_RightButtom;
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	auto size = info.srWindow;
	cout << "display width:" << size.Right - size.Left << " display height:" << size.Bottom - size.Top;
	cout << "left = " << size.Left;
	cout << "top = " << size.Top;
}

const LPCWCHAR  GetCharAttrMeta(CharAttr in_attr)
{
	switch (in_attr)
	{
	case Default:
		return  L"\x1b[0m";
		break;
	case Foreground_Bold:
		return  L"\x1b[1m";
		break;
	case Foreground_No_Bold:
		return  L"\x1b[22m";
		break;
	case Underline:
		return  L"\x1b[4m";
		break;
	case No_Underline:
		return  L"\x1b[24m";
		break;
	case Negative:
		return  L"\x1b[7m";
		break;
	case Positive:
		return  L"\x1b[27m";
		break;
	case Foreground_Black:
		return  L"\x1b[30m";
		break;
	case Foreground_Red:
		return  L"\x1b[31m";
		break;
	case Foreground_Green:
		return  L"\x1b[32m";
		break;
	case Foreground_Yellow:
		return  L"\x1b[33m";
		break;
	case Foreground_Blue:
		return  L"\x1b[34m";
		break;
	case Foreground_Magenta:
		return  L"\x1b[35m";
		break;
	case Foreground_Cyan:
		return  L"\x1b[36m";
		break;
	case Foreground_White:
		return  L"\x1b[37m";
		break;
	case Foreground_Default:
		return  L"\x1b[39m";
		break;
	case Background_Black:
		return  L"\x1b[40m";
		break;
	case Background_Red:
		return  L"\x1b[41m";
		break;
	case Background_Green:
		return  L"\x1b[42m";
		break;
	case Background_Yellow:
		return  L"\x1b[43m";
		break;
	case Background_Blue:
		return  L"\x1b[44m";
		break;
	case Background_Magenta:
		return  L"\x1b[45m";
		break;
	case Background_Cyan:
		return  L"\x1b[46m";
		break;
	case Background_White:
		return  L"\x1b[47m";
		break;
	case Background_Default:
		return  L"\x1b[49m";
		break;
	case Bright_Foreground_Black:
		return  L"\x1b[90m";
		break;
	case Bright_Foreground_Red:
		return  L"\x1b[91m";
		break;
	case Bright_Foreground_Green:
		return  L"\x1b[92m";
		break;
	case Bright_Foreground_Yellow:
		return  L"\x1b[93m";
		break;
	case Bright_Foreground_Blue:
		return  L"\x1b[94m";
		break;
	case Bright_Foreground_Magenta:
		return  L"\x1b[95m";
		break;
	case Bright_Foreground_Cyan:
		return  L"\x1b[96m";
		break;
	case Bright_Foreground_White:
		return  L"\x1b[97m";
		break;
	case Bright_Background_Black:
		return  L"\x1b[100m";
		break;
	case Bright_Background_Red:
		return  L"\x1b[101m";
		break;
	case Bright_Background_Green:
		return  L"\x1b[102m";
		break;
	case Bright_Background_Yellow:
		return  L"\x1b[103m";
		break;
	case Bright_Background_Blue:
		return  L"\x1b[104m";
		break;
	case Bright_Background_Magenta:
		return  L"\x1b[105m";
		break;
	case Bright_Background_Cyan:
		return  L"\x1b[106m";
		break;
	case Bright_Background_White:
		return  L"\x1b[107m";
		break;
 	}
}


Terminal::Terminal()
{
	/*需要异常处理*/
	terminal_out = GetStdHandle(STD_OUTPUT_HANDLE);

	//最大化窗口
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
	// TODO:需要让窗口固定大小不变,或是让窗口可以自适应大小
	//Sleep(1000);

	DWORD oldmode;
	GetConsoleMode(terminal_out, &oldmode);
	oldmode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;//使得terminal sequence有效
	if (!SetConsoleMode(terminal_out, oldmode))
	{
		std::cout << "Failed to set Console:" << GetLastError() << std::endl;
	}
	//std::wcout << L"\x1b[?1049h";//启用备用缓冲区
	std::wcout << L"\x1b[?25l";//隐藏光标

	// 设置字体
	CONSOLE_FONT_INFOEX fontInfo;
	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	fontInfo.dwFontSize.X = 8;
	fontInfo.dwFontSize.Y = 8;
	fontInfo.FontFamily = TMPF_DEVICE;
	wcscpy_s(fontInfo.FaceName, L"Terminal");
	if (!SetCurrentConsoleFontEx(terminal_out, FALSE, &fontInfo)) {
		std::cout << "Failed to change the font: " << GetLastError();
	}
	//左上角是0,0
	CONSOLE_SCREEN_BUFFER_INFOEX info;
	info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(terminal_out, &info);
	COORD bufSize;
	auto tmp = GetLargestConsoleWindowSize(terminal_out);
	bufSize.X =tmp.X + 1;
	bufSize.Y =tmp.Y + 1;
	if (!SetConsoleScreenBufferSize(terminal_out, bufSize))
	{
		std::cout << "Failed to change the buffer size: " << GetLastError();
	}
	SMALL_RECT windowSize;
	windowSize.Bottom = tmp.Y - 1;
	windowSize.Left = 0;
	windowSize.Right = tmp.X - 1;
	windowSize.Top = 0;
	//std::cout << "before windows:" << info.srWindow.Right << "," << info.srWindow.Bottom << ";";
	//std::cout << "want set windows:" << windowSize.Right << "," << windowSize.Bottom << ";";
	if (!SetConsoleWindowInfo(terminal_out, true, &windowSize))
	{
		std::cout << "Failed to change the window's size: " << GetLastError();
	}

	GetConsoleScreenBufferInfoEx(terminal_out, &info);
	//std::cout << "buf:" << bufSize.X << "," << bufSize.Y<<";";
	//std::cout << "max windows:" << tmp.X << "," << tmp.Y<<";";
	//std::cout << "after windows:" << info.srWindow.Right << "," << info.srWindow.Bottom << ";";
	display_size.Left = 0;
	display_size.Right = windowSize.Right;
	display_size.Top = 0;
	display_size.Bottom = windowSize.Bottom;

	cursor_pos.X = 0;//左边是0;
	cursor_pos.Y = 0;//上边是0;

	//std::cout <<"buffer-width:::"<<info.dwSize.X << std::endl;
	//std::cout <<"buffer-Height::::"<<info.dwSize.Y<< std::endl;
	//std::cout <<"width:::"<<info.srWindow.Right<<";";
	//std::cout <<"Height::::"<<info.srWindow.Bottom<<";";
	//std::cout << "Max width:" << info.dwMaximumWindowSize.X << ";";
	//std::cout << "Max Height:" << info.dwMaximumWindowSize.Y << ";";
	//std::cout << "Max H" << GetLargestConsoleWindowSize(terminal_out).Y << ";";
	//std::cout << "Max W" << GetLargestConsoleWindowSize(terminal_out).X << ";";
	//std::cout << "left=" << display_size.Left;
	//std::cout << "top=" << display_size.Top;
	//std::cout << "error_code = " << GetLastError();
	//std::cout << "w=" << tmp.X << "H=" << tmp.Y;

}

Terminal::~Terminal()
{
	std::wcout << L"\x1b[?1049l";//转回主缓冲区
}

void Terminal::draw(SHORT x, SHORT y, TermChar ch)
{
	DWORD written;
	//边界判定
	if (x < display_size.Left || y < display_size.Top || x > display_size.Right || y > display_size.Bottom)
		return;
	//移动到位置x,y
	//std::wcout << "\x1b[" << y << ";" << x << "H";
	SetConsoleCursorPosition(terminal_out, COORD{x,y});
	//更新cursor位置
	cursor_pos.X = x;
	cursor_pos.Y = y;
	//转换attribute
	//std::wcout << ch.GetForegroudAttr();
	//std::wcout << ch.GetBackgroudAttr();
	//写入字符
	//std::wcout << ch.GetContent();
	FillConsoleOutputAttribute(terminal_out, BACKGROUND_RED, 1, COORD{ x,y }, &written);
	//回到原来的attribute
	//std::wcout << GetCharAttrMeta(CharAttr::Default);
}

void Terminal::drawBlank(SHORT x, SHORT y, CharAttr color)
{
	TermChar ch(' ', CharAttr::Foreground_Default,color);
	draw(x, y, ch);
}

void Terminal::erase(SHORT x, SHORT y)
{
	//draw(x, y, TermChar(' ', CharAttr::Foreground_Default, CharAttr::Background_Default));
	DWORD written;
	//边界判定
	if (x < display_size.Left || y < display_size.Top || x > display_size.Right || y > display_size.Bottom)
		return;
	//移动到位置x,y
	//std::wcout << "\x1b[" << y << ";" << x << "H";
	SetConsoleCursorPosition(terminal_out, COORD{x,y});
	//更新cursor位置
	cursor_pos.X = x;
	cursor_pos.Y = y;
	//转换attribute
	//std::wcout << ch.GetForegroudAttr();
	//std::wcout << ch.GetBackgroudAttr();
	//写入字符
	//std::wcout << ch.GetContent();
	FillConsoleOutputAttribute(terminal_out, FOREGROUND_RED, 1, COORD{ x,y }, &written);
	//回到原来的attribute
	//std::wcout << GetCharAttrMeta(CharAttr::Default);
}

void Terminal::DrawLine(COORD start_point, Direct direct, SHORT lenth, TermChar ch)
{
	SHORT count = 0;
	switch (direct)
	{
	case Direct::Down:
	case Direct::Up:
		SHORT y_step;
		if (direct == Down)
			y_step = 1;
		else
			y_step = -1;
		count = 0;
		while (count < lenth)
		{
			draw(start_point.X, start_point.Y + count * y_step, ch);
			count++;
		}
		break;
	case Direct::Left:
	case Direct::Right:
		SHORT x_step;
		if (direct == Left)
			x_step = -1;
		else
			x_step = 1;
		 count = 0;
		while (count < lenth)
		{
			draw(start_point.X + count * x_step, start_point.Y, ch);
			count++;
		}
	}
}

void Terminal::DrawLineBlank(COORD start_point, Direct direct, SHORT lenth, CharAttr color)
{
	TermChar ch(' ', CharAttr::Foreground_Default,color);
	DrawLine(start_point, direct, lenth, ch);
}

void Terminal::DrawRect(COORD start_point, SHORT cols, SHORT rows, TermChar ch)
{
	//用直线来构成矩形
	COORD start = start_point;
	DrawLine(start, Direct::Right, cols, ch);
	start.X = start.X + cols - 1;
	DrawLine(start, Direct::Down, rows, ch);
	start.Y = start.Y + rows - 1;
	DrawLine(start, Direct::Left, cols, ch);
	start.X = start.X - cols + 1;
	DrawLine(start, Direct::Up, rows, ch);
}

void Terminal::DrawRectBlank(COORD start_point, SHORT cols, SHORT rows, CharAttr color)
{
	TermChar ch(' ', CharAttr::Foreground_Default,color);
	DrawRect(start_point, cols, rows, ch);
}

void Terminal::CopyRect()
{
}

void Terminal::CutRect()
{
}

void Terminal::cls()
{
	std::wcout << L"\x1b[2J";
}

TermChar::TermChar(char in_content, CharAttr foregroud_attr_in, CharAttr backgroud_attr_in) :
	content(in_content), foregroud_attribute(foregroud_attr_in), backgroud_attribute(backgroud_attr_in)
{
}

LPCWSTR TermChar::GetForegroudAttr()
{
	return GetCharAttrMeta(foregroud_attribute);
}

LPCWSTR TermChar::GetBackgroudAttr()
{
	return GetCharAttrMeta(backgroud_attribute);
}

char TermChar::GetContent()
{
	return content;
}

