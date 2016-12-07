.PHONY: configure all clean compile

export CC=clang
export CXX=clang++

all: compile

node_modules:
	npm install

configure: node_modules binding.gyp build.gyp Makefile
	node-gyp configure -DCXX=$(CXX) -DCC=$(CC)

test:
	$(MAKE) -C ./test

check: test

compile: configure
	node-gyp build

clean:
	rm -rf ./build

