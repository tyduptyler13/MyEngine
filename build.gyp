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
  				"src/ShaderNode.cpp"
			],
			"include_dirs": [
				"include",
				"<!@(<(pkg-config) --cflags sdl2)"
			],
			"direct_dependent_settings": {
				"include_dirs": [
					"include",
					"<!@(<(pkg-config) --cflags sdl2)"
				]
			},
			"variables": {
				"pkg-config": "pkg-config"
			},
			"cflags": [
				"-Wall",
				"-Wextra",
				"-pedantic",
				"-std=c++11",
				"-g",
				"-O2",
#				"-flto"
			],
			"libraries": [
				"<!@(<(pkg-config) --libs-only-l sdl2)",
				"-lSOIL"
			],
			"ldflags": [
				"<!@(<(pkg-config) --libs-only-L --libs-only-other sdl2)"
			]
		}
	]
}
