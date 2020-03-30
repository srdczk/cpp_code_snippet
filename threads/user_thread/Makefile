all: test1 test2 test3

test1: 
	gcc test1.c -fno-stack-protector -o test1

test2:
	gcc test2.c -o test2

test3:
	gcc test3.c switch.s -fno-stack-protector -o test3

clean:
	rm -rf test1 test2 test3
