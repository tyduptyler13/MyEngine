CFLAGS=-O2 -Wall -Wextra -pedantic -I../include -I../deps/googletest/include -pthread --std=c++11
LFLAGS=../deps/googletest/libgtest.a ../deps/googletest/libgtest_main.a
CXX=clang++

FILES=$(wildcard *.cc)

test: $(FILES:.cc=.o)
	$(CXX) $(CFLAGS) $(FILES:.cc=.o) $(LFLAGS) -o test

check: test
	./test

%.o: %.cc
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o test

default: check


