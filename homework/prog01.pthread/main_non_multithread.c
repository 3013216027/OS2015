/* **********************************************

  File Name: main_non_multithread.c

  Author: zhengdongjian@tju.edu.cn

  Created Time: Fri 20 Nov 2015 05:10:23 PM CST

*********************************************** */

#include <stdio.h>
#include <stdlib.h>

#define MAX_N 100000000
#define MAX_N_SQR 1e16

int
main(int argc, char* argv[])
{
	double sum = 0.0;
	int i = 0;
	for (; i <= MAX_N; ++i)
	{
		sum += 1.0 / (MAX_N_SQR + (i + 0.5) * (i + 0.5));
	}
	
	sum *= 4.0 * MAX_N;
	printf("PI approximately equals to %.8f\n", sum);

	return 0;
}
