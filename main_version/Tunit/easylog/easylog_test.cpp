#include <part/easylog.hpp>
//#include <gtest/gtest.h>

void funcT_input()
{
	using namespace simtalk::part;
	using namespace simtalk::tools;

	G_LOG.print(PL_INFO, "% % %", __func__, "hello", "world!");
	LOG_ERROR("%s%d\n", "hellowrold!", 2021);
	time_now();
}
int main()
{
	funcT_input();
	return 0;
}
