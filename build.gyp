{
	"targets": [
		{
			"target_name": "libMyEngine",
			"type": "<(library)",
			"sources": [
 				"src/GLES2Renderer.cpp",
  				"src/Log.cpp",
  				"src/Math.cpp",
  				"src/ShaderUtil.cpp",
  				"src/ShaderGLES2.cpp",
  				"src/Material.cpp",
  				"src/GLES2MaterialLib.cpp",
  				"src/GLES2RenderTarget.cpp",
  				"src/Texture.cpp"
			],
			"include_dirs": [
				"include",
				"deps/sdl2/include",
			],
			"direct_dependent_settings": {
				"include_dirs": [
					"include",
					"deps/sdl2/include"
				]
			},
			"link_dependent_settings": {
				"dependencies": [
					"glsl-optimizer"
				]
			},
			"link_settings": {
				"libraries": [
					"../deps/Simple-OpenGL-Image-Library/libSOIL.a",
					"../deps/sdl2/build/.libs/libSDL2.a",
					"-pthread", #We might need pthread
					"-ldl",
					"-lGLESv2",
					"-lGL",
				]
			},
			"dependencies": [
				"glsl-optimizer"
			],
			"variables": {
				"pkg-config": "pkg-config"
			},
			"cflags": [
				"-Wall",
				"-Wextra",
				"-pedantic",
				"-std=c++14",
				"-g",
				"-O2"
			],
			'cflags!': [ '-fno-exceptions' ],
			'cflags_cc!': [ '-fno-exceptions' ]
		},
		# glsl-optimizer code is a dependency of the engines shader compiler.
		# The settings have been adapted from cmake
		{
			"target_name": "glsl-optimizer",
			"type": "static_library",
			"variables": {
				"root": "deps/glsl-optimizer"
			},
			"include_dirs": [
				"<(root)/include",
				"<(root)/src/mesa",
				"<(root)/src/mapi",
				"<(root)/src/glsl",
				"<(root)/src"
			],
			"sources": [
				"<!@(ls -1 <(root)/src/glsl/glcpp/*.c)",
				"<!@(ls -1 <(root)/src/util/*.c)",
				"<!@(ls -1 <(root)/src/mesa/program/*.c)",
				"<!@(ls -1 <(root)/src/mesa/main/*.c)",
				"<!@(ls -1 <(root)/src/glsl/*.cpp)",
				"<!@(ls -1 <(root)/src/glsl/*.c)"
			],
			"sources!": [
				"<(root)/src/glsl/main.cpp",
				"<(root)/src/glsl/buildin_stubs.cpp"
			],
			"cflags": [ "-O2" ],
			"direct_dependent_settings": {
				"include_dirs": [
					"deps/glsl-optimizer/src"
				]
			}
		},
		# This is our test build to check if the engine can compile properly.
		{
			"target_name": "BasicExample",
			"type": "executable",
			"sources": [ "test/core/BasicExample.cpp" ],
			"dependencies": [ "libMyEngine" ],
			"cflags": [
				"-Wall",
				"-Wextra",
				"-pedantic",
				"-std=c++14",
				"-O3",
				"-g"
			]
		}
	]
}
