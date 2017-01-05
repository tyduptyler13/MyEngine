.PHONY: configure all clean compile

export CC=clang
export CXX=clang++

all: compile

node_modules:
	npm install

configure: node_modules binding.gyp build.gyp Makefile
	node-gyp configure

test:
	$(MAKE) -C ./test

check: test

compile: configure deps/assimp/lib/libassimp.a deps/glfw/src/libglfw3.a deps/Simple-OpenGL-Image-Library/libSOIL.a
	node-gyp build

clean:
	node-gyp clean

deps/assimp/lib/libassimp.a: deps/assimp
	cd deps/assimp && cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_CXX_FLAGS=-fpic -DCMAKE_CC_FLAGS=-fpic
	$(MAKE) -C deps/assimp

deps/assimp:
	git submodule init deps/assimp && git submodule update deps/assimp

deps/glfw/src/libglfw3.a: deps/glfw
	cd deps/glfw && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CC_FLAGS=-fpic -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD-TESTS=OFF
	$(MAKE) -C deps/glfw

deps/glfw:
	git submodule init deps/glfw && git submodule update deps/glfw

deps/Simple-OpenGL-Image-Library/libSOI.a: deps/Simple-OpenGL-Image-Library
	cd deps/Simple-OpenGL-Image-Library && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-fpic -DCMAKE_CC_FLAGS=-fpic
	$(MAKE) -C deps/Simple-OpenGL-Image-Library

deps/Simple-OpenGL-Image-Library:
	git submodule init deps/Simple-OpenGL-Image-Library && git submodule update deps/Simple-OpenGL-Image-Library


