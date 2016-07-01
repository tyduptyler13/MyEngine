.PHONY: configure all clean compile

#export CC=clang
#export CXX=clang++

all: compile

configure: ./build

test:
	$(MAKE) -C ./test

check: test

compile: ./build
	@echo Entering build directory
	$(MAKE) -C ./build

./build: autogen.sh build.gyp
	./autogen.sh
	@echo "Generating ShaderChunk.hpp"

clean:
	rm -rf ./build
