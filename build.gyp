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
  				"src/Math.cpp"
			],
			"include_dirs": [
				"include"
			],
			"direct_dependent_settings": {
				"include_dirs": [ "include/" ]
			},
			"cflags": [
				"-Wall",
				"-Wextra",
				"-pedantic",
				"-std=c++11",
				"-g",
#				"-O",
#				"-flto"
			]
		}
	]
}

