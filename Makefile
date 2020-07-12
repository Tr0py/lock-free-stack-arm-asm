all:lockfree-stack thread-test
lockfree-stack:main.c 
	arm-linux-gnueabihf-gcc --static $< pure_stack.s  -o $@ -lpthread

thread-test:thread.c
	arm-linux-gnueabihf-gcc --static $< -o $@ -lpthread
clean:
	rm -f lockfree-stack thread-test
