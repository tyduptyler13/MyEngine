CFLAGS=-O2 -Wall -Wextra -pedantic -flto -I../include -I../deps/googletest/include -pthread --std=c++11
LFLAGS=-flto -lpthread -L../deps/googletest -lgtest -lgtest_main -L../build/out/Default/obj.target -lMyEngine

FILES=$(wildcard *.cc)
HEADERS=$(wildcard ../include/*.hpp)

test: $(FILES:.cc=.o)
	$(CXX) $(FILES:.cc=.o) $(LFLAGS) -o test

check: test
	./test

%.o: %.cc $(HEADERS)
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o test

default: check


