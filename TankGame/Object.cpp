#include "Object.h"
#include <fstream>
#include "Engine.h"
using std::ifstream;
Object::Object(const std::string& in_name) :
	name(in_name),
	is_pre_run(false),
	engine_ptr(nullptr),
	is_cleaning(false),
	is_managed(false),
	visualable(false),
	direct(DrawDirect::Up),
	old_direct(DrawDirect::Up),
	origin_point(COORD{ 0,0 }),
	old_point(COORD{0,0}),
	is_point_repeat(false),
	is_direct_repeat(false),
	relative_pos(std::make_shared<std::vector<COORD>>()),
	content(std::make_shared<std::vector<TermChar>>())
{
	/*default constructor to generate a "llll"*/
	TermChar ch(' ', CharAttr::Foreground_Default, CharAttr::Background_Yellow);// yellow block;
	for (SHORT i = 1; i <= 6; i++)//画出四个竖杠
		{
			for (SHORT j = 0; j < 1; j++)
			{
				relative_pos->push_back(COORD{ 1 + 4 * j,i });
				content->push_back(ch);
			}
		}

}
COORD resolve_coord_line_(const std::string & instr)
{
	std::string left, right;
	auto pos = instr.find(',');  //逗号的位置
	for (int count = 0; count < pos; count++)
		if(instr[count] != ' ' && instr[count] != '\t')//去除空白字符
			left.push_back(instr[count]);
	for (int count = pos + 1; count < instr.size(); count++)
		if(instr[count] != ' ' && instr[count] != '\t')
			right.push_back(instr[count]);

	return COORD{ (SHORT)std::stoi(left),(SHORT)std::stoi(right) }; // 强制转换可行,因为总是用SHORT
}
void Object::set_draw(const std::string & filename)
{
	// errors or exceptions are not considerated.
	ifstream draw_file(filename);
	bool status = true; // 表示一个段已经读完，准备好读下一个段

	std::string linebuf;
	std::vector<COORD> points;

	while (std::getline(draw_file, linebuf))
	{
		auto len_of_line = linebuf.size();
		if (status == true)
		{
			if (linebuf[0] == '[' && linebuf[len_of_line - 1] == ']')
			{
				status = false;
				//std::cout << linebuf << std::endl;
			}
			else
			{
				std::cout << "please check your draw file" << std::endl;// 错误,应改为抛出错误
			}
		}
		else {
			if (linebuf[0] == '[' && linebuf[1] == '\\' and linebuf[len_of_line - 1] == ']')
			{
				// 这里返回drawable类
				status = true;
				//std::cout << linebuf << std::endl;
			}
			else
			{
				// 解析坐标行,去除注释
				auto annotation_pos = linebuf.find("//");
				if (annotation_pos != std::string::npos)
				{
					linebuf.erase(annotation_pos, std::string::npos);
				}

				points.push_back(resolve_coord_line_(linebuf));
			}
		}
	}

	TermChar ch(' ', CharAttr::Foreground_Default, CharAttr::Background_Yellow);//黄色空格
	relative_pos->clear();
	content->clear();
	for (auto point : points)
	{
		relative_pos->push_back(point);
		content->push_back(ch);
	}
	origin_point = COORD{ 5,5 }; // 为了观察效果
	old_point = COORD{ 5,5 };
	//drawable_ptr->guid = generate_uuid();
}

std::string Object::ret_name()
{
	return name;
}

void Object::delete_self() { 
	get_engine()->delete_obj(name);
}


void Object::pre_run()
{
}
void Object::run()
{
}

Engine * Object::get_engine()
{
	return engine_ptr;
}

void Object::set_engine(Engine * in_engine_ptr)
{
	engine_ptr = in_engine_ptr;
}

void Object::turn_left()
{
	if (!is_direct_repeat)
	{
		old_direct = direct;
		is_direct_repeat = true;
	}
	direct = DrawDirect::Left;
}

void Object::turn_right()
{
	if (!is_direct_repeat)
	{
		old_direct = direct;
		is_direct_repeat = true;
	}
	direct = DrawDirect::Right;
}

void Object::turn_up()
{
	if (!is_direct_repeat)
	{
		old_direct = direct;
		is_direct_repeat = true;
	}
	direct = DrawDirect::Up;
}

void Object::turn_down()
{
	if (!is_direct_repeat)
	{
		old_direct = direct;
		is_direct_repeat = true;
	}
	direct = DrawDirect::Down;
}

void Object::move_left(SHORT step)
{
	if (!is_point_repeat)
	{
		old_point = origin_point;
		is_point_repeat = true;
	}
	origin_point.X -= step;
}

void Object::move_right(SHORT step)
{
	if (!is_point_repeat)
	{
		old_point = origin_point;
		is_point_repeat = true;
	}
	origin_point.X += step;
}

void Object::move_up(SHORT step)
{
	if (!is_point_repeat)
	{
		old_point = origin_point;
		is_point_repeat = true;
	}
	origin_point.Y -= step;
}

void Object::move_down(SHORT step)
{
	if (!is_point_repeat)
	{
		old_point = origin_point;
		is_point_repeat = true;
	}
	origin_point.Y += step;
}

void Object::move(COORD in_coord)
{
	if (!is_point_repeat)
	{
		old_point = origin_point;
		is_point_repeat = true;
	}
	origin_point.Y = in_coord.Y;
	origin_point.X = in_coord.X;
}

void Object::set_visualable()
{
	visualable = true;
}

void Object::set_unvisualable()
{
	visualable = false;
}

bool Object::ret_visualable()
{
	return visualable;
}



void Object::set_static(bool flag) { is_static = flag; }

void Object::set_no_point_repeat() { is_point_repeat = false; }

void Object::set_no_direct_repeat(){is_direct_repeat = false;}

//same

void Object::set_cleaning()
{
	is_cleaning = true;
}
