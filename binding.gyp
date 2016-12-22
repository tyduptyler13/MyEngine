{
	'targets': [
		{
			'target_name': 'nbind',
			'defines': [ 'V8_DEPRECATION_WARNINGS=1' ],
			'sources': [
				'src/binding.cpp' #Allows include guards to work.
			],
			'dependencies': [ 'build.gyp:libMyEngine' ],
			"conditions": [
				['target=="win"', {
					"cflags_cc!": [ "-rdynamic" ],
					"cflags": [ "-target x86_64-w64-mingw32" ]
				}]
			],
			"configurations": {
				"Debug": {
					"ldflags": [
						"-Wl,-rpath,build/Debug"
					]
				},
				"Release": {
					"ldflags": [
						"-Wl,-rpath,."
					]
				}
			},
			'cflags': [
				'-std=c++14'
			],
			'cflags!': [
				'-fno-exceptions',
				'-std=gnu++0x',
			],
			'cflags_cc!': [
				'-fno-exceptions',
				'-fno-rtti',
				'-std=gnu++0x',
				'-std=c++11'
			],
			'include_dirs': [
				'node_modules/nbind/include',
				'node_modules/nan'
			],
			'includes': [
				'node_modules/nbind/src/nbind.gypi'
			]
		}
	],
	'includes': [
		'node_modules/nbind/src/nbind-common.gypi'
	]
}
