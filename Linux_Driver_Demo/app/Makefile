CC := /home/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc


all: demo

demo: demo.o
	$(CC) -o $@ $^

clean:
	-rm *.o $(all)
