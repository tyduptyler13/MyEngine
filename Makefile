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
	node-gyp build --target=1.4.15 --arch=x64 --dist-url=https://atom.io/download/atom-shell -- -j8

clean:
	node-gyp clean
	$(MAKE) -C deps/assimp clean
	$(MAKE) -C deps/Simple-OpenGL-Image-Library clean

fullclean:
	rm build
	cd deps/assimp/ && git clean -xdf
	cd deps/Simple-OpenGL-Image-Library && git clean -xdf

deps/assimp/lib/libassimp.a: deps/assimp
	cd deps/assimp && cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_CXX_FLAGS=-fpic -DCMAKE_CC_FLAGS=-fpic
	$(MAKE) -C deps/assimp

deps/assimp:
	git submodule init deps/assimp && git submodule update deps/assimp

deps/Simple-OpenGL-Image-Library/libSOIL.a: deps/Simple-OpenGL-Image-Library
	cd deps/Simple-OpenGL-Image-Library && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-fpic -DCMAKE_CC_FLAGS=-fpic
	$(MAKE) -C deps/Simple-OpenGL-Image-Library

deps/Simple-OpenGL-Image-Library:
	git submodule init deps/Simple-OpenGL-Image-Library && git submodule update deps/Simple-OpenGL-Image-Library


