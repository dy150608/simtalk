#include <part/easylog.hpp>
#include <gtest/gtest.h>
#include <cstring>
#include <cstdio>

using namespace simtalk::part;
using namespace simtalk::tools;

TEST(easylog, message) {
	char str[128] = {};
	snprintf(str, 128, "%s %s %s", __func__, "hello", "world!\n");
	G_LOG.print(plv::PL_INFO, "% % %", __func__, "hello", "world!");
	EXPECT_STREQ(str, G_LOG.msg_data());
	LOG_ERROR("% %\n", "hellowrold!", 2022);
}

TEST(easylog, print_name) {
	EXPECT_STREQ("[ASSERT]", plv_name(plv::PL_ASSERT));
	EXPECT_STREQ("[VERBOSE]", plv_name(plv::PL_VERBOSE));
	EXPECT_STREQ("[LV_EXCEPT]", plv_name(plv::PL_UNDEF));
	EXPECT_STREQ("[LV_EXCEPT]", plv_name(plv::PL_EXCEPT));
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
