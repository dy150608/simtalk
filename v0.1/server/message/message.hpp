#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include "../../inc/data_type.hpp"
#include <vector>
#include <iterator>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

class Message
{
public:
	Message();
	~Message();
	bool put(msg *in, char *num, int len);					// 插入数据
	bool take(std::queue<msg*> *out, char *num, int len);	// 取出数据
	bool send(int aim_fd, char *msg, int len);				// 发送消息到目标客户端
	bool work(int msg_type, const char *msg, int fd);		// 处理消息
	
private:
	/*数据集，每个帐号对应一个元素*/
	std::vector<um> msg_set;
};

#endif
