#include "/home/lzx/simple-chat/inc/path.hpp"
#include "/home/lzx/simple-chat/src/mfs/local_msg.hpp"

#define TEST_TXT LOCAL_MESSAGE"00001.txt"

int main(int argc, char **argv)
{
	local_msg lm;
	lm.read(TEST_TXT);
	lm.print();
	return 0;
}
