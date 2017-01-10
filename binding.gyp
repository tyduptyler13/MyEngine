{
	'targets': [
		{
			'target_name': 'nbind',
			'defines': [ 'V8_DEPRECATION_WARNINGS=1' ],
			'sources': [
				'src/node/Vector3.cpp',
				'src/node/Object3D.cpp',
				'src/node/Scene.cpp',
				'src/node/Camera.cpp',
				'src/node/PerspectiveCamera.cpp',
				'src/node/Renderer.cpp',
				'src/node/GLES2Renderer.cpp',
				'src/node/Geometries/BoxGeometry.cpp',
				'src/node/Mesh.cpp',
				'src/node/GeometryImporter.cpp',
				'src/node/GLES2OverlaySystem.cpp',
				'src/node/GLES2MaterialLib.cpp'
			],
			'dependencies': [ 'build.gyp:libMyEngine' ],
			'cflags': [
				'-std=c++14',
				#'-fpic'
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
			]
		}
	]
}
