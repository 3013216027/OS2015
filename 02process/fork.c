#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	pid_t val;
	printf("PID before fork(): %d\n", getpid());
	val = fork();
	if (val > 0) {
		printf("Parent PID: %d\n", getpid());
	} else if (val == 0) {
		printf("Child PID: %d\n", (int)getpid());
	} else {
		printf("Fork failed!");
		return 1;
	}
	puts("good bye");
	return 0;
}
