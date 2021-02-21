#include "message.hpp"
#include <cstring>

Message::Message()
{
	cout << "消息管理器创建成功\n";
}

Message::~Message()
{
	cout << "[success] message distructor running" << endl;
}

bool Message::put(msg *in, char *num, int len)
{
	for(int i = 0; i < msg_set.size(); i ++)
	{
		if(strncmp(num, msg_set[i].account_num, len) == 0)
		{
			msg *tmp = new msg;
			memcpy(tmp, in, sizeof(msg));
			msg_set[i].ur_set.push(tmp);
			msg_set[i].count ++;
		}
	}
	// 这里可能会有bug
	um tmp;
	msg *_tmp = new msg;
	memcpy(_tmp, in, sizeof(msg));
	tmp.ur_set.push(_tmp);
	tmp.count ++;
	msg_set.push_back(tmp);

	return true;
}

bool Message::take(std::queue<msg*> *out, char *num, int len)
{
	for(int i = 0; i < msg_set.size(); i ++)
	{
		// 找到接收者
		if(strncmp(num, msg_set[i].account_num, len) == 0)
		{
			do
			{
				// 将数据拷贝到对象外部
				out->push(msg_set[i].ur_set.front());
				msg_set[i].ur_set.pop();
			}while(msg_set[i].count --);
		}
		return true;
	}
	return false;
}

bool Message::work(int msg_type, const char *msg, int fd)
{
	bool ret = false;
	if(msg_type & LIST_ONLINE)
	{}
	return ret;
}
