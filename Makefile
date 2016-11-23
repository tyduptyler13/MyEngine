.PHONY: configure all clean compile testapp

#export CC=clang
#export CXX=clang++

all: compile

configure: ./build

test:
	$(MAKE) -C ./test

check: test

compile: ./build
	@echo Entering build directory
	$(MAKE) -C ./build libMyEngine

./build: autogen.sh build.gyp
	./autogen.sh

clean:
	rm -rf ./build

BasicExample: all
	$(MAKE) -C ./build BasicExample