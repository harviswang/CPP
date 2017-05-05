all: hello.o test.o
	$(CXX) test.o hello.o

.c.o:
	$(CC) -c $< -o $@

.cc.o:
	$(CXX) -c $< -o $@

clean:
	rm *.o
	rm a.out

.PHONY: all clean
