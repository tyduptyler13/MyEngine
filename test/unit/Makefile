CFLAGS=-O2 -Wall -Wextra -pedantic -flto -I../../include -I../../deps/googletest/googletest/include -pthread --std=c++14
LFLAGS=-flto -lpthread -L../../deps/googletest/googletest -lgtest -lgtest_main -Wl,-rpath ../../ -L../../ -lMyEngine

FILES=$(wildcard *.cc)
HEADERS=$(wildcard ../../include/*.hpp)

test: $(FILES:.cc=.o) gtest
	$(CXX) $(FILES:.cc=.o) $(LFLAGS) -o test

check: test
	./test

%.o: %.cc $(HEADERS)
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o test

default: check gtest

../../deps/googletest/googletest/Makefile: ../../deps/googletest/CMakeLists.txt
	cd ../../deps/googletest/googletest; cmake .

../../deps/googletest:
	git submodule init ../../deps/googletest && git submodule update ../../deps/googletest

gtest: ../../deps/googletest/googletest ../../deps/googletest/googletest/Makefile
	$(MAKE) -C ../../deps/googletest/googletest



