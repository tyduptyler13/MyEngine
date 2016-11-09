{
	"targets": [
		{
			"target_name": "MyEngine",
			"product_name": "MyEngine",
			"type": "<(library)",
			"sources": [
 				"src/GLES2Renderer.cpp",
  				"src/Log.cpp",
  				"src/Math.cpp",
  				"src/ShaderUtil.cpp",
  				"src/ShaderGLES2.cpp",
  				"src/Material.cpp"
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
					"-lSOIL",
					"-lGL",
					"<!@(<(pkg-config) --libs-only-l sdl2 capnp)"
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
				"-O2",
				"-fPIC"
			],
			'cflags!': [ '-fno-exceptions' ],
			'cflags_cc!': [ '-fno-exceptions' ],
			"libraries": [
				"<!@(<(pkg-config) --libs-only-l sdl2 capnp)",
				"-lSOIL",
				"-lGL",
				"../deps/glsl-optimizer/libglsl_optimizer.a",
				"../deps/glsl-optimizer/libmesa.a",
				"../deps/glsl-optimizer/libglcpp-library.a"
			],
			"ldflags": [
				"<!@(<(pkg-config) --libs-only-L --libs-only-other sdl2)"
			]
		}
	]
}
