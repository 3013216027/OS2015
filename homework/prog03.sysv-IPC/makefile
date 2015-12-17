source: ipc init.c putA.c putB.c
	gcc -o init ipc.o init.c
	gcc -o putA ipc.o putA.c
	gcc -o putB ipc.o putB.c
ipc: ipc.h ipc.c
	gcc -c ipc.c
clean:
	rm -vf *.o
