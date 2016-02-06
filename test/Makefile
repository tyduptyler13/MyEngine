CFLAGS=-O2 -Wall -Wextra -pedantic -L../deps/googletest -lgtest -lgtest_main
CXX=clang++

default: tests

tests: $(wildcard *.cc:.cc=.test)

%.test: %.cc
	$(CXX) $(CFLAGS) $< -o $@
	./$@

clean:
	$(RM) *.test

