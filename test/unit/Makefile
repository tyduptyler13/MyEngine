CFLAGS=-O2 -Wall -Wextra -pedantic -flto -I../include -I../deps/googletest/include -pthread --std=c++11
LFLAGS=-flto -lpthread -L../deps/googletest -lgtest -lgtest_main -L../build/out/Default/obj.target -lMyEngine

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

../../deps/googletest/Makefile: ../../deps/googletest/CMakeLists.txt
	cd ../../deps/googletest; cmake .

../../deps/googletest:
	git submodule init ../../deps/googletest && git submodule update ../../deps/googletest

gtest: ../../deps/googletest ../../deps/googletest/Makefile
	$(MAKE) -C ../../deps/googletest



