{
	'targets': [
		{
			'target_name': 'nbind',
			'defines': [ 'V8_DEPRECATION_WARNINGS=1' ],
			'sources': [
				'src/binding.cpp'
			],
			'dependencies': [ 'build.gyp:libMyEngine' ],
			'ldflags': [ '-Wl,-rpath .' ],
			'cflags!': [ 
				'-fno-exceptions'
			],
			'cflags_cc!': [
				'-fno-exceptions',
				'fno-rtti'
			],
			'include_dirs': [
				'node_modules/nbind/include',
				'node_modules/nan'
			],
			'includes': [
				'node_modules/nbind/src/nbind.gypi'
			],
			'scripts': {
				'node-gyp': 'node-gyp',
				'autogypi': 'autogypi',
				'build': 'autogypi && node-gyp configure build'
			}
		}
	],
	'includes': [
		'node_modules/nbind/src/nbind-common.gypi'
	]
}