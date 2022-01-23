#include <part/easylog.hpp>
#include <gtest/gtest.h>
#include <cstring>
#include <cstdio>

using namespace simtalk::part;
using namespace simtalk::tools;

void funcT_input()
{

	G_LOG.print(PL_INFO, "% % %", __func__, "hello", "world!");
	LOG_ERROR("%s%d\n", "hellowrold!", 2021);
	time_now();
}

TEST(easylog, message) {
	char str[128] = {};
	snprintf(str, 128, "%s %s %s", __func__, "hello", "world!\n");
	G_LOG.print(PL_INFO, "% % %", __func__, "hello", "world!");
	EXPECT_STREQ(str, G_LOG.msg_data());
}

TEST(easylog, print_name) {
	EXPECT_STREQ("[ASSERT]", plv_name(PL_ASSERT));
	EXPECT_STREQ("[VERBOSE]", plv_name(PL_VERBOSE));
	EXPECT_STREQ("[LV_EXCEPT]", plv_name(PL_UNDEF));
	EXPECT_STREQ("[LV_EXCEPT]", plv_name(PL_EXCEPT));
	EXPECT_STREQ("[LV_EXCEPT]", plv_name(-10));
	EXPECT_STREQ("[LV_EXCEPT]", plv_name(100));
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
