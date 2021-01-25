#include "draw.hpp"

void Draw::init()
{
	frame_row = 90;
	frame_column = 160;

	cursor_row = 0;
	cursor_column = 0;

	current_row = 0;
	current_column = 0;
}

void Draw::set_frame(int row, int column)
{
	frame_row = row;
	frame_column = column;
}

void Draw::draw_task()
