all:
	arm-linux-gnueabihf-gcc --static ./main.c ./stack.s -o main -lpthread
clean:
	rm -f ./main
