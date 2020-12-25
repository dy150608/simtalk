#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int a = 10;
	char str[10] = {"123"};
	//itoa(a, str, 10);
	a = atoi(str);
	//printf("%s\n", str);
	printf("%d\n", a);
	return 0;
}
