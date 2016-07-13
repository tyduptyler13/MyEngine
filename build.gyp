{
	"targets": [
		{
			"target_name": "MyEngine",
			"product_name": "MyEngine",
			"type": "<(library)",
			"sources": [
 				"src/GLES3Renderer.cpp",
  				"src/Log.cpp",
  				"src/Math.cpp"
			],
			"include_dirs": [
				"include",
				"<!@(<(pkg-config) --cflags sdl2)",
				"deps/Simple-OpenGL-Image_Library/src"
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
				"<!@(<(pkg-config) --libs-only-l sdl2)"
			],
			"ldflags": [
				"<!@(<(pkg-config) --libs-only-L --libs-only-other sdl2)"
			]
		}
	]
}
