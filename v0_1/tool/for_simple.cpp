#include "for_simple.hpp"

/*void DEBUG(const char *msg)
{
//#ifdef _DEBUG_
	printf("In %s-%d: %s\n", __FILE__, __LINE__, msg);
//#endif
}*/

void ERROR(const char *msg)
{
	printf("In %s-%d: %s\n", __FILE__, __LINE__, msg);
    exit(-1);
}

void DRAW_LINE(int num = 10)
{
//#ifdef _DEBUG_
	for(int i = 0; i < num; i ++)
		printf("-");
	printf("\n");
//#endif
}

char QUERY_SQL[256] = {};
int  QUERY_LEN = 0;

std::string time_to_date(time_t &time)
{
    struct tm *stm = localtime(&time);
    char str[24];
    strftime(str, sizeof(str), "%04Y-%02m-%02d %02H:%02M:%02S", stm);
    return std::string(str);
}
