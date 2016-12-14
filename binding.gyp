{
	'targets': [
		{
			'target_name': 'nbind',
			'defines': [ 'V8_DEPRECATION_WARNINGS=1' ],
			'sources': [
				'src/binding.cpp' #Allows include guards to work.
			],
			'dependencies': [ 'build.gyp:libMyEngine' ],
			'ldflags': [ '-Wl,-rpath .' ],
			'cflags': [
				'-std=c++14'
			],
			'cflags!': [ 
				'-fno-exceptions',
				'-std=gnu++0x',
				'-std=c++11'
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