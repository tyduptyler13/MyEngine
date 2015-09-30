#!/bin/sh

hash git 2>/dev/null || { echo "Git was not found, exiting"; exit 1; }

hash gyp 2>/dev/null || {
	echo >&2 "gyp was not found! Installing locally"
	git clone --depth 1 https://chromium.googlesource.com/external/gyp.git ./deps/gyp
	PATH=./deps/gyp:$PATH
}

gyp build.gyp --depth=. -f make --generator-output=./build -Dlibrary=static_library

