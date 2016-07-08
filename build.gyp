{
	"targets": [
		{
			"target_name": "MyEngine",
			"product_name": "MyEngine",
			"type": "<(library)",
			"sources": [
 # 				"src/GLES2Program.cpp",
 # 				"src/Geometry.cpp",
  				"src/Log.cpp",
  				"src/Math.cpp",
				"src/Texture.cpp"
			],
			"include_dirs": [
				"include",
				"<!@(<(pkg-config) --cflags sdl2)",
				"<!@(<(pkg-config) --cflags SDL2_image)"
			],
			"direct_dependent_settings": {
				"include_dirs": [
					"include/",
					"<!@(<(pkg-config) --cflags sdl2)",
					"<!@(<(pkg-config) --cflags SDL2_image)"
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
				"<!@(<(pkg-config) --libs-only-l SDL2_image)"
			],
			"ldflags": [
				"<!@(<(pkg-config) --libs-only-L --libs-only-other sdl2)",
				"<!@(<(pkg-config) --libs-only-L --libs-only-other SDL2_image)"
			]
		}
	]
}
