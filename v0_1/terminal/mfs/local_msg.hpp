#ifndef _LOCAL_MSG_HPP_
#define _LOCAL_MSG_HPP_

#include "inc/data_type.hpp"
#include <cstring>
#include <string>

/*
 *这个模块是用于本地消息记录，目前暂时不做这个功能，因此暂时废弃
 */
class local_msg
{
public:
	local_msg();
	~local_msg();
	void init();
    bool read(char *filepath);			/*将消息记录读取到双向链表*/
    bool write(char *filepath);			/*将双向链表的消息内容写入文件*/
	void print();						/*打印所有消息节点*/
private:
    //std::fstream fs;
    char defpath[64];
};

#endif
