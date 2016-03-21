CFLAGS=-O2 -Wall -Wextra -pedantic -I../include -I../deps/googletest/include -pthread --std=c++11
LFLAGS=../deps/googletest/libgtest.a ../deps/googletest/libgtest_main.a
CXX=clang++

default: tests

tests: $(wildcard *.cc:.cc=.test)

%.test: %.cc
	$(CXX) $(CFLAGS) $< -o $@
	./$@

clean:
	$(RM) *.test

