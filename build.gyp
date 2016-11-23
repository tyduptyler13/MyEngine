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
				"<!@(<(pkg-config) --cflags sdl2)",
				"deps/glsl-optimizer/src"
			],
			"direct_dependent_settings": {
				"include_dirs": [
					"include"
				],
				"libraries": [
					"../deps/Simple-OpenGL-Image-Library/libSOIL.a",
					"-lGL",
					"/usr/local/lib/libSDL2.a",
					"../deps/glsl-optimizer/libglsl_optimizer.a",
					"../deps/glsl-optimizer/libmesa.a",
					"../deps/glsl-optimizer/libglcpp-library.a",
					"-pthread",
					"-ldl"
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
				#"-O2"
			],
			'cflags!': [ '-fno-exceptions' ],
			'cflags_cc!': [ '-fno-exceptions' ],
			"ldflags": [
				"<!@(<(pkg-config) --libs-only-L --libs-only-other sdl2)"
			]
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
				#"-O3",
				"-g"
			]
		}
	]
}
