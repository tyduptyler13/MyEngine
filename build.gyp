{
	"targets": [
		{
			"target_name": "MyEngine",
			"type": "<(library)",
			"sources": [
				"src/Material.cpp",
				"src/Log.cpp",
				"src/Math.cpp",
				"src/ShaderUtil.cpp",
				"src/GLES2Shader.cpp",
				"src/GLES2Renderer.cpp",
				"src/GLES2MaterialLib.cpp",
				"src/GLES2RenderTarget.cpp",
				"src/Texture.cpp",
				"src/GeometryImporter.cpp",
				"src/GLFWManager.cpp",
				"src/SoundObject.cpp"
			],
			"include_dirs": [
				"include",
				"local/include"
			],
			"dependencies": [
				"glsl-optimizer"
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
			"variables": {
				"asmjs%": "0"
			},
			"conditions": [
				['OS=="linux"', {
					"conditions": [
						['asmjs==1', {
							"link_settings": {
								"libraries": [
									"../deps/assimp/lib/libassimp.a",
									"-lGLESv2",
									"-s USE_GLFW=3",
									"-lglfw3",
									"../deps/assimp/lib/libzlibstatic.a" #last for a reason
								]
							}
						}, {
							"link_settings": {
								"libraries": [
									"../deps/assimp/lib/libassimp.a",
									"-lGLESv2",
									"<!(pkg-config glfw3 --static --libs-only-l)",
									"-pthread" #We might need pthread for gcc (because their std::threads are broken otherwise)
								]
							}
						}]
					]
				}]
			],
			"ldflags": [ "-L../lib" ],
			"cflags": [
				"-Wall",
				"-Wextra",
				"-pedantic",
				"-fpic"
			],
			"cflags_cc": [
				"-std=c++14"
			],
			'cflags!': [
				'-fno-exceptions'
			],
			'cflags_cc!': [
				'-fno-exceptions',
				'-fno-rtti',
				'-std=gnu++0x'
			]
		}
	]
}
