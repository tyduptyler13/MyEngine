.PHONY: configure all clean compile

export CC=clang
export CXX=clang++

all: compile

configure: ./build

compile: ./build
	@echo Entering build directory
	$(MAKE) -C ./build

./build: autogen.sh build.gyp
	./autogen.sh

clean:
	rm -rf ./build

