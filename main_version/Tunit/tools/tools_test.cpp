#include <gtest/gtest.h>
#include <tools/buffer.hpp>
#include <tools/cycle.hpp>
#include <tools/file.hpp>
#include <cstring>
#include <cstdlib>

using namespace simtalk::tools;

char str[] = {"hello"};

TEST(buffer, construct) {
	buffer1KB buff;
	buffer1KB buff1(str, strlen(str));
	EXPECT_EQ(0, strncmp("hello", buff1.data(), buff1.size()));
	buffer1KB buff2(buff1);
	EXPECT_EQ(0, strncmp("hello", buff2.data(), buff2.size()));
	buffer1KB buff3(buff1);
	EXPECT_EQ(0, strncmp("hello", buff3.data(), buff3.size()));
}

TEST(buffer, size) {
	buffer1KB buff;
	buff << str;
	EXPECT_EQ(strlen(str), buff.size());
	EXPECT_EQ(buff.data(), buff.current()-buff.size());
	buff.resize();
	EXPECT_EQ(buff.data(), buff.current());
	buff.clear();
	EXPECT_EQ(0, strncmp("", buff.data(), 32));
}

TEST(buffer, append) {
	buffer1KB buff;
	buff.append(str, strlen(str));
	EXPECT_EQ(0, strncmp("hello", buff.data(), buff.size()));
	buff.clear();
	buff << 1 << 'a' << 1.2;
	EXPECT_EQ(0, strncmp("1a1.2", buff.data(), 32));
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
