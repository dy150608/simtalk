#ifndef _DRAW_HPP_
#define _DRAW_HPP_

#include <vector>

typedef void (*task)();				// 绘图任务

class Draw
{
public:
	void draw_frame();
	void draw_task(task line);		// 执行一个绘图任务
	void set_frame(int row, int column);
	void init();
	void select_frame();			// 绘图选定框
	
private:
	int frame_row;					// 框架尺寸
	int frame_column;

	int cursor_row;					// 光标坐标
	int cursor_column;

	int current_row;				// 当前坐标
	int current_column;

	std::vector<task> task_list;
};

#endif
