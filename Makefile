.PHONY: configure all clean fullclean compile

export CC=clang
export CXX=clang++

ifndef NODEBUILD
	ELECTRONFLAGS=--runtime=electron --target=1.6.1 --arch=x64 --dist-url=https://atom.io/download/electron
endif

ifdef ASMJS
	ASMJS=--asmjs=1
endif

all: compile

node_modules:
	npm install

configure: node_modules binding.gyp build.gyp Makefile
	node-gyp configure $(ELECTRONFLAGS) $(ASMJS)

test:
	$(MAKE) -C ./test

check: test

compile: configure deps/assimp/lib/libassimp.a
	node-gyp build $(ELECTRONFLAGS) $(ASMJS) -- -j8

weakclean:
	node-gyp clean

clean:
	node-gyp clean
	$(MAKE) -C deps/assimp clean

fullclean:
	rm -rf build
	cd deps/assimp/ && git clean -xdf

deps/assimp/Makefile: deps/assimp/CMakeLists.txt
	cd deps/assimp && $(PRECMAKE) cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DCMAKE_CXX_FLAGS="-fpic --std=c++11" -DCMAKE_C_FLAGS=-fpic $(ASSIMPCMAKEFLAGS)

deps/assimp/lib/libassimp.a: deps/assimp/Makefile
	$(MAKE) -C deps/assimp

deps/assimp/CMakeLists.txt:
	git submodule init deps/assimp && git submodule update deps/assimp
