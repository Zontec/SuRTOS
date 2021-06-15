
all:
	make clean
	make out

clean:
	rm -f *.o
	rm -rf ./build

out: main.o queue.o stack.o thread.o kernel_spec.o product_specific.o
	cc $^ -o out
	mkdir -p build
	mv *.o ./build
	