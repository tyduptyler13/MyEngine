#!/bin/bash
if ! hash x86_64-w64-mingw32-g++-posix 2>/dev/null; then
	echo "You do not have mingw64 installed. Please install and try again."
	exit
fi

if ! hash x86_64-w64-mingw32-pkg-config 2>/dev/null; then
	echo "Currently mingw pkg-config is required for glfw to get the right drivers. Please install and try again."
	exit
fi

CXX=x86_64-w64-mingw32-g++-posix CC=x86_64-w64-mingw32-gcc-posix node-gyp configure -- -Dtarget=win -Dpkg-config=x86_64-w64-mingw32-pkg-config

echo "The environment is now configured. Run 'node-gyp build' to begin the build."
echo "'node-gyp build -- -j8' would run the build on 8 cores."

