#!/bin/sh

# Git is a hard dependency and we can't locally install easily.
hash git 2>/dev/null || { echo "Git was not found, please install before trying to build."; exit 1; }

# Check if we have already locally installed gyp
[ -d ./deps/gyp ] && {
	PATH=./deps/gyp:$PATH
} || {
# gyp isn't locally installed, is it globally?
	hash gyp 2>/dev/null || {
# gyp isn't installed anywhere, lets locally install it.
		echo >&2 "gyp was not found! Installing locally"
		git clone --depth 1 https://chromium.googlesource.com/external/gyp.git ./deps/gyp
		PATH=./deps/gyp:$PATH
	}
}

gyp build.gyp --depth=. -f make --generator-output=./build -Dlibrary=static_library

