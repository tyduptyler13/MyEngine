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
				"<!@(sdl2-config --cflags 2>/dev/null)",
				"deps/glsl-optimizer/src"
			],
			"direct_dependent_settings": {
				"include_dirs": [
					"include",
					"<!@(sdl2-config --cflags 2>/dev/null)"
				],
				"libraries": [
					"../deps/Simple-OpenGL-Image-Library/libSOIL.a",
					"../deps/glsl-optimizer/libglsl_optimizer.a",
					"../deps/glsl-optimizer/libmesa.a",
					"../deps/glsl-optimizer/libglcpp-library.a",
					"../deps/sdl2/build/.libs/libSDL2.a",
					"-pthread", #We might need pthread
					"-ldl",
					"-lGLESv2",
					"-lGL",
				]
			},
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
