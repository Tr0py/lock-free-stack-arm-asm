all:
	arm-linux-gnueabihf-gcc --static ./main.c ./myadd.s ./stack.s -o main -lpthread
clean:
	rm -f ./main
