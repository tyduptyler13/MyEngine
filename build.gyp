{
	"targets": [
		{
			"target_name": "libMyEngine",
			"type": "shared_library",
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
				"src/GeometryImporter.cpp"
			],
			"include_dirs": [
				"include",
				"node_modules/nbind/include",
				"deps/glfw/include",
				"<!(pkg-config assimp --cflags)"
			],
			"direct_dependent_settings": {
				"include_dirs": [
					"include",
					"node_modules/nbind/include"
				]
			},
			"link_dependent_settings": {
				"dependencies": [
					"glsl-optimizer"
				]
			},
			"link_settings": {
				"libraries": [
					"-lSOIL",
					"<!(pkg-config --static --libs-only-l glfw3)",
					"<!(pkg-config assimp --libs-only-l assimp)",
					"-pthread", #We might need pthread for gcc (because their std::threads are broken otherwise)
					"-lGLESv2",
					"-lGL",
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
				"-fpic"
			],
			'cflags!': [ '-fno-exceptions' ],
			'cflags_cc!': [
				'-fno-exceptions',
				'-fno-rtti',
				'-std=gnu++0x'
			]
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
			"cflags!": [
				"-O3",
				"-g"
			],
			"cflags": [
				"-fpic",
				"-O2"
			],
			"direct_dependent_settings": {
				"include_dirs": [
					"deps/glsl-optimizer/src"
				]
			}
		}
	]
}
