{
	"targets": [
		{
			"target_name": "MyEngine",
			"product_name": "MyEngine",
			"type": "<(library)",
			"sources": [
				"src/Euler.cpp",
 # 				"src/GLES2Program.cpp",
 # 				"src/Geometry.cpp",
  				"src/Log.cpp",
  				"src/Math.cpp",
 # 				"src/Object3D.cpp",
  				"src/PerspectiveCamera.cpp",
  				"src/Ray.cpp",
  				"src/ShaderChunk.cpp"
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

