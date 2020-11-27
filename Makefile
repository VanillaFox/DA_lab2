FLAGS = -std=c++14 -Ofast
FILES = lab2.cpp tree.h

all:
	g++ ${FLAGS} ${FILES} -o solution


clean:
	rm -rf *.o
