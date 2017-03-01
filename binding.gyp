{
	'targets': [
		{
			'target_name': 'MyEngineBinding',
			'defines': [ 'V8_DEPRECATION_WARNINGS=1' ],
			'sources': [
				'src/nbind/Vector2.cpp',
				'src/nbind/Vector3.cpp',
				'src/nbind/Vector4.cpp',
				'src/nbind/Object3D.cpp',
				'src/nbind/Scene.cpp',
				'src/nbind/Camera.cpp',
				'src/nbind/PerspectiveCamera.cpp',
				'src/nbind/Renderer.cpp',
				'src/nbind/GLES2Renderer.cpp',
				'src/nbind/Geometries/BoxGeometry.cpp',
				'src/nbind/Geometries/SphereGeometry.cpp',
				'src/nbind/Mesh.cpp',
				'src/nbind/GeometryImporter.cpp',
				'src/nbind/GLES2OverlaySystem.cpp',
				'src/nbind/GLES2MaterialLib.cpp',
				'src/nbind/Material.cpp',
				'src/nbind/BufferGeometry.cpp',
				'src/nbind/Geometry.cpp',
				'src/nbind/Materials/GLES2NormalMaterial.cpp'
			],
			'dependencies': [ 'build.gyp:MyEngine' ],
			'cflags': [
				'-std=c++14',
				'-fpic'
			],
			'cflags!': [
				'-fno-exceptions',
				'-fPIC'
			],
			'cflags_cc!': [
				'-fno-exceptions',
				'-fno-rtti',
				'-std=gnu++0x',
				'-std=c++11',
				'-fPIC'
			],
			'include_dirs': [
				'node_modules/nbind/include',
				'node_modules/nan'
			],
			'includes': [
				'node_modules/nbind/src/nbind.gypi'
			],
			"conditions": [
				['OS=="win"', {
					"msvs_settings": {
						"VCCLCompilerTool": {
							"AdditionalOptions": [
								"/GR",
								"/EHsc"
							]
						}
					}
				}]
			]
		}
	]
}
