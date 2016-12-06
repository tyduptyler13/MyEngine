{
	"targets": [
		{
			"target_name": "libMyEngine",
			"type": "shared_library",
			"sources": [
 				"src/GLES2Renderer.cpp",
  				"src/Log.cpp",
  				"src/Math.cpp",
  				"src/ShaderUtil.cpp",
  				"src/ShaderGLES2.cpp",
  				"src/Material.cpp",
  				"src/GLES2MaterialLib.cpp",
  				"src/GLES2RenderTarget.cpp",
  				"src/Texture.cpp",
  				"src/GeometryImporter.cpp"
			],
			"include_dirs": [
				"include",
				"deps/glfw/include",
				"<!(pkg-config assimp --cflags)"
			],
			"direct_dependent_settings": {
				"include_dirs": [
					"include"
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
					"<!(pkg-config --static --libs-only-l glfw3)",
					"<!(pkg-config assimp --libs-only-l assimp)",
					"-pthread", #We might need pthread for gcc (because their std::threads are broken otherwise)
					"-lGLESv2",
					"-lGL",
				],
				"ldflags": [
					"-Wl,-rpath ."
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
				"-fpic",
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
			"cflags": [
				"-O2",
				"-fpic"
			],
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
