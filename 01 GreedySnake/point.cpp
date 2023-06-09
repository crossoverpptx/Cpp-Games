#include <iostream>
#include "point.h"
#include "tools.h"

void Point::Print()
{
	SetCursorPosition(x, y);
	std::cout << "■";
}

void Point::Clear()//清除输出
{
	SetCursorPosition(x, y);
	std::cout << "  ";
}

void Point::ChangePosition(const int x, const int y)//改变坐标
{
	this->x = x;
	this->y = y;
}

void Point::PrintCircular()//输出圆形
{
	SetCursorPosition(x, y);
	std::cout << "●";
}
