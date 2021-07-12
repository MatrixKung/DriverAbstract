#include <stdio.h>
#include "Communicate\Comm.h"


int main()
{
	Test MyStr = { 0 };

	MyStr.myStr = (PCH)malloc(15);
	memset(MyStr.myStr, 0,15);
	memcpy(MyStr.myStr, "Hello Alice\0", 12);
	DriverComm(TEST, &MyStr, 4, NULL, NULL);
	printf("%s\r\n", MyStr.myStr);
	system("pause");
	return 0;
}
