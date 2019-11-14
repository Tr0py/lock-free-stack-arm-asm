all:
	arm-linux-gnueabihf-gcc --static ./main.c ./pure_stack.s -o main -lpthread
clean:
	rm -f ./main
